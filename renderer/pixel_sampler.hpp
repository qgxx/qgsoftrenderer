#pragma once

#include <array>
#include <vector>

#include <glm/glm.hpp>

namespace sr {

template <typename T, size_t N> 
class IPixelSampler {
public:
	std::array<T, N> samplers;

	static size_t getSamplingNum() { return N; }

	T& operator[](const int& index) { return samplers[index]; }
	const T& operator[](const int& index) const { return samplers[index]; }

}; 

//1x Sampling Point
template <typename T>
class PixelSampler1X : public IPixelSampler<T, 1> {
public:
	PixelSampler1X(const T& value) { samplers.fill(value); }

	static const std::array<glm::vec2, 1>& getSamplingOffsets() {
		return { glm::vec2(0.0f, 0.0f) };
	}

};

//2x Sampling Point
template <typename T>
class PixelSampler2X : public IPixelSampler<T, 2> {
public:
	PixelSampler2X(const T& value) { samplers.fill(value); }

	static const std::array<glm::vec2, 2>& getSamplingOffsets() {
		return { glm::vec2(-0.25f, -0.25f), glm::vec2(+0.25f, +0.25f) };
	}

};

//4x Sampling Point
template <typename T>
class PixelSampler4X : public IPixelSampler<T, 4> {
public:
	PixelSampler4X(const T& value) { samplers.fill(value); }

	static const std::array<glm::vec2, 4> getSamplingOffsets() {
		//Sampling points' offset
		//Note:Rotated grid sampling pattern
		//Refs: https://mynameismjp.wordpress.com/2012/10/24/msaa-overview/
		return {
			glm::vec2(+0.125f, +0.375f),
			glm::vec2(+0.375f, -0.125f),
			glm::vec2(-0.125f, -0.375f),
			glm::vec2(-0.375f, +0.125f)
		};
	}
};

//8x Sampling Point
template <typename T>
class PixelSampler8X : public IPixelSampler<T, 8> {
public:
	PixelSampler8X(const T& value) { samplers.fill(value); }

	static const std::array<glm::vec2, 8> getSamplingOffsets() {
		//Sampling points' offset
		//Note:Rotated grid sampling pattern
		//Refs: https://mynameismjp.wordpress.com/2012/10/24/msaa-overview/
		return {
			glm::vec2(-0.375f, +0.375f),
			glm::vec2(+0.125f, +0.375f),
			glm::vec2(-0.125f, +0.125f),
			glm::vec2(+0.375f, +0.125f),
			glm::vec2(-0.375f, -0.125f),
			glm::vec2(+0.125f, -0.125f),
			glm::vec2(-0.125f, -0.375f),
			glm::vec2(+0.375f, -0.375f)
		};
	}
};

#ifdef MSAA4X
template<typename T>
using PixelSampler = PixelSampler4X<T>;
//Note: MSAA 8X is a little time-consuming
//template<typename T>
//using PixelSampler = PixelSampler8X<T>;
#else
template<typename T>
using PixelSampler = PixelSampler1X<T>;
#endif

using PixelRGB = std::array<unsigned char, 3>;
using PixelRGBA = std::array<unsigned char, 4>;
using MaskPixelSampler = PixelSampler<unsigned char>;
using DepthPixelSampler = PixelSampler<float>;
using ColorPixelSampler = PixelSampler<PixelRGBA>;

//Framebuffer attachment
using MaskBuffer = std::vector<MaskPixelSampler>;
using DepthBuffer = std::vector<DepthPixelSampler>;
using ColorBuffer = std::vector<ColorPixelSampler>;

constexpr PixelRGBA k_White = { 255, 255, 255 ,255 };
constexpr PixelRGBA k_Black = { 0, 0, 0, 0 };

} // namespace sr