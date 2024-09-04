#pragma once

#include <glm/glm.hpp>

namespace sr {

	enum class TextureWarpMode {
		REPEAT,
		MIRRORED_REPEAT,
		CLAMP_TO_EDGE	
	};

	enum class TextureFilterMode {
		NEAREST,
		LINEAR
	};


	enum class CullFaceMode {
		CULL_DISABLE,
		CULL_FRONT,
		CULL_BACK
	};

	enum class DepthTestMode
	{
		DEPTH_TEST_DISABLE,
		DEPTH_TEST_ENABLE
	};

	enum class DepthWriteMode
	{
		DEPTH_WRITE_DISABLE,
		DEPTH_WRITE_ENABLE
	};

	enum class AlphaBlendingMode
	{
		ALPHA_DISABLE,
		ALPHA_BLENDING,
		ALPHA_TO_COVERAGE
	};


	enum class LightingMode
	{
		LIGHTING_DISABLE,
		LIGHTING_ENABLE
	};


	struct Context {
		CullFaceMode m_CullFaceMode = CullFaceMode::CULL_BACK;
		DepthTestMode m_DepthTestMode = DepthTestMode::DEPTH_TEST_ENABLE;
		DepthWriteMode m_DepthWriteMode	= DepthWriteMode::DEPTH_WRITE_ENABLE;
		AlphaBlendingMode m_AlphaBlendMode = AlphaBlendingMode::ALPHA_DISABLE;
	};

} // namespace sr