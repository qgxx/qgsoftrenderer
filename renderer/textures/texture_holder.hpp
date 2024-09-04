#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "parallel_wrapper.hpp"

namespace sr {

class TextureHolder {
public:
	typedef std::shared_ptr<TextureHolder> ptr;

	TextureHolder(std::uint16_t width, std::uint16_t height) : m_width { width}, m_height { height }, 
		m_data { nullptr } {}
	virtual ~TextureHolder() {
		freeTextureHolder();
	}

	std::uint16_t getWidth() const { return m_width; }
	std::uint16_t getHeight() const { return m_height; }

	virtual std::uint32_t read(const std::uint16_t& x, const std::uint16_t& y) const {
		return m_data[to_index(x, y)];
	}

	virtual void read(const std::uint16_t& x, const std::uint16_t& y, unsigned char& r, unsigned char& g,
		unsigned char& b, unsigned char& a) const {
		std::uint32_t texel = read(x, y);
		r = (texel >> 24) & 0xFF;
		g = (texel >> 16) & 0xFF;
		b = (texel >>  8) & 0xFF;
		a = (texel >>  0) & 0xFF;
	}
	
protected:
	std::uint16_t m_width, m_height;
	std::uint32_t *m_data; 

	void loadTextureHolder(const unsigned int& nElements, unsigned char* data, const std::uint16_t& width, 
		const std::uint16_t& height, const int& channel) {
		m_data = new std::uint32_t[nElements];
		parallelFor((int)0, (int)(height * width), [&](const int &index) -> void {
			int y = index / width, x = index % width;
			unsigned char r, g, b, a;
			int addres = index * channel;
			switch (channel) {
				case 1:
					r = g = b = data[addres], a = 255;
					break;
				case 3:
					r = data[addres + 0], g = data[addres + 1], b = data[addres + 2], a = 255;
					break;
				case 4:
					r = data[addres + 0], g = data[addres + 1], b = data[addres + 2], a = data[addres + 3];
					break;
				default:
					r = g = b = data[addres], a = 255;
					break;
			}
			m_data[to_index(x, y)] = (r << 24) | (g << 16) | (b << 8) | (a << 0);
		});
	}

	void freeTextureHolder() {
		if (m_data != nullptr) {
			delete[] m_data;
			m_data = nullptr;
		}
	}

	virtual unsigned int to_index(const std::uint16_t& x, const std::uint16_t& y) const = 0;
};


class LinearTextureHolder : public TextureHolder {
public:
	typedef std::shared_ptr<LinearTextureHolder> ptr;

	LinearTextureHolder(unsigned char *data, std::uint16_t width, std::uint16_t height, int channel) 
	: TextureHolder(width, height) {
		TextureHolder::loadTextureHolder(width * height, data, width, height, channel);
	}
	virtual ~LinearTextureHolder() = default;

private:
	virtual unsigned int to_index(const std::uint16_t &x, const std::uint16_t &y) const override {
		return y * m_width + x;
	}

};


class TilingTextureHolder : public TextureHolder {
public:
	typedef std::shared_ptr<TilingTextureHolder> ptr;

	TilingTextureHolder(unsigned char* data, std::uint16_t width, std::uint16_t height, int channel) 
	: TextureHolder(width, height) {
		m_widthInTiles = (width + k_blockSize - 1) / k_blockSize;
		m_heightInTiles = (height + k_blockSize - 1) / k_blockSize;
		unsigned int nElements = m_widthInTiles * m_heightInTiles * k_blockSize2;
		TextureHolder::loadTextureHolder(nElements, data, width, height, channel);
	}
	virtual ~TilingTextureHolder() = default;

private:
	static constexpr int k_blockSize = 4;
	static constexpr int k_blockSize2 = 16;

	int m_widthInTiles = 0;
	int m_heightInTiles = 0;

	virtual unsigned int to_index(const std::uint16_t& x, const std::uint16_t& y) const override {
		// Tiling address mapping
		// Note: this is naive version
		// return ((int)(y / k_blockSize) * m_widthInTiles + (int)(x / k_blockSize)) * k_blockSize2 + (y % k_blockSize) * k_blockSize + x % k_blockSize;
		// Note: this is optimized version
		return (((int)(y >> 2) * m_widthInTiles + (int)(x >> 2)) << 4) + ((y & 3) << 2) + (x & 3);
	}

};


//Tiling and morton order layout
//Refs: https://fgiesen.wordpress.com/2011/01/17/TextureHolder-tiling-and-swizzling/
class ZCurveTilingTextureHolder : public TextureHolder {
public:
	typedef std::shared_ptr<ZCurveTilingTextureHolder> ptr;

	ZCurveTilingTextureHolder(unsigned char* data, std::uint16_t width, std::uint16_t height, int channel) 
	: TextureHolder(width, height) {
		m_widthInTiles = (width + k_blockSize - 1) / k_blockSize;
		m_heightInTiles = (height + k_blockSize - 1) / k_blockSize;
		unsigned int nElements = m_widthInTiles * m_heightInTiles * k_blockSize2;
		TextureHolder::loadTextureHolder(nElements, data, width, height, channel);
	}
	virtual ~ZCurveTilingTextureHolder() = default;

private:
	//Block size for tiling
	static constexpr int k_blockSize = 32; //Note: block size should not exceed 256
	static constexpr int k_blockSize2 = 1024;
	static constexpr int bits = 5;

	int m_widthInTiles = 0;
	int m_heightInTiles = 0;

	virtual unsigned int to_index(const std::uint16_t &x, const std::uint16_t & y) const override {
		std::uint8_t rx = x & (k_blockSize - 1), ry = y & (k_blockSize - 1);
		std::uint16_t ri = 0;
		ZCurveTilingTextureHolder::encodeMortonCurve(rx, ry, ri);
		return ((y >> bits) * m_widthInTiles + (x  >> bits)) * k_blockSize2 + ri;
	}

	static void decodeMortonCurve(const std::uint16_t &index, std::uint8_t &x, std::uint8_t &y) {
		//Morton curve decoding
		//Refs: https://en.wikipedia.org/wiki/Z-order_curve
		x = 0, y = 0;
		for (int i = 0; i < bits; ++i) {
			x |= (index & (1 << (2 * i))) >> i;
			y |= (index & (1 << (2 * i + 1))) >> (i + 1);
		}
	}

	inline static void encodeMortonCurve(const std::uint8_t &x, const std::uint8_t &y, std::uint16_t &index) {
		//Morton curve encoding
		//Refs: https://en.wikipedia.org/wiki/Z-order_curve
		index = 0;
		index |= ((x & (1 << 0)) << (0)) | ((y & (1 << 0)) << (1));
		index |= ((x & (1 << 1)) << (1)) | ((y & (1 << 1)) << (2));
		index |= ((x & (1 << 2)) << (2)) | ((y & (1 << 2)) << (3));
		index |= ((x & (1 << 3)) << (3)) | ((y & (1 << 3)) << (4));
		index |= ((x & (1 << 4)) << (4)) | ((y & (1 << 4)) << (5));
		index |= ((x & (1 << 5)) << (5)) | ((y & (1 << 5)) << (6));
		index |= ((x & (1 << 6)) << (6)) | ((y & (1 << 6)) << (7));
		index |= ((x & (1 << 7)) << (7)) | ((y & (1 << 7)) << (8));
		index |= ((x & (1 << 8)) << (8)) | ((y & (1 << 8)) << (9));
		index |= ((x & (1 << 9)) << (9)) | ((y & (1 << 9)) << (10));
	}
};

} // namespace sr