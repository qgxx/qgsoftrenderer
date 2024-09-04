#pragma once

#include "texture.hpp" 

namespace sr {

class CubeMap {
public:

private:
    bool m_generateMipmap = false;
	std::vector<Texture::ptr> m_textures;

	TextureWarpMode m_warpMode;
	TextureFilterMode m_filteringMode;

};

} // namespace sr