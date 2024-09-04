#pragma once

#include <string>
#include <vector>
#include <memory>

#include "glm/glm.hpp"

#include "context.hpp"
#include "texture_holder.hpp"

namespace sr {

class Texture {
public:
	typedef std::shared_ptr<Texture> ptr;

	Texture();
	Texture(bool generatedMipmap);
	~Texture() = default;

	bool isGeneratedMipmap() const { return m_generateMipmap; }
	int getWidth() const { return m_texHolders[0]->getWidth(); }
	int getHeight() const { return m_texHolders[0]->getHeight(); }

	//Sampling options setting
	void setWarpingMode(TextureWarpMode mode);
	void setFilteringMode(TextureFilterMode mode);

	bool loadTextureFromFile(
		const std::string &filepath,
		TextureWarpMode warpMode = TextureWarpMode::REPEAT,
		TextureFilterMode filterMode = TextureFilterMode::LINEAR);

	//Sampling according to the given uv coordinate
	glm::vec4 sample(const glm::vec2 &uv, const float &level = 0.0f) const;

private:
	//Auxiliary functions
	void readPixel(const std::uint16_t &u, const std::uint16_t &v, unsigned char &r, 
		unsigned char &g, unsigned char &b, unsigned char &a, const int level = 0) const;

	void generateMipmap(unsigned char *pixels, int width, int height, int channel);

private:
	bool m_generateMipmap = false;
	std::vector<TextureHolder::ptr> m_texHolders;

	TextureWarpMode m_warpMode;
	TextureFilterMode m_filteringMode;

	friend class TextureSampler;
};

class TextureSampler final
{
public:
	//Sampling algorithm
	static glm::vec4 textureSamplingNearest(TextureHolder::ptr texture, glm::vec2 uv);
	static glm::vec4 textureSamplingBilinear(TextureHolder::ptr texture, glm::vec2 uv);
};

} // namespace sr