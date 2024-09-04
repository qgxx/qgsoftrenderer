#pragma once

#include "pipeline.hpp"

namespace sr {

class Pipeline3D : public Pipeline {
public:
	typedef std::shared_ptr<Pipeline3D> ptr;

	virtual ~Pipeline3D() = default;

	virtual void vertexShader(VertexData &vertex) const override;
	virtual void fragmentShader(const FragmentData &data, glm::vec4 &fragColor,
		const glm::vec2 &dUVdx, const glm::vec2 &dUVdy) const override;

};

class DoNothingShading : public Pipeline {
public:
	typedef std::shared_ptr<DoNothingShading> ptr;
	virtual ~DoNothingShading() = default;

	virtual void vertexShader(VertexData &vertex) const override;
	virtual void fragmentShader(const FragmentData &data, glm::vec4 &fragColor,
		const glm::vec2 &dUVdx, const glm::vec2 &dUVdy) const override;
};

class TextureShading final : public Pipeline3D {
public:

	typedef std::shared_ptr<TextureShading> ptr;

	virtual ~TextureShading() = default;

	virtual void fragmentShader(const FragmentData &data, glm::vec4 &fragColor,
		const glm::vec2 &dUVdx, const glm::vec2 &dUVdy) const override;
};

class LODVisualize final : public Pipeline3D {
public:
	typedef std::shared_ptr<LODVisualize> ptr;

	virtual ~LODVisualize() = default;

	virtual void fragmentShader(const FragmentData &data, glm::vec4 &fragColor,
		const glm::vec2 &dUVdx, const glm::vec2 &dUVdy) const override;
};

class PhongShading final : public Pipeline3D {
public:
	typedef std::shared_ptr<PhongShading> ptr;

	virtual ~PhongShading() = default;

	virtual void fragmentShader(const FragmentData &data, glm::vec4 &fragColor,
		const glm::vec2 &dUVdx, const glm::vec2 &dUVdy) const override;
};

class BlinnPhongShading final : public Pipeline3D {
public:
	typedef std::shared_ptr<BlinnPhongShading> ptr;

	virtual ~BlinnPhongShading() = default;

	virtual void fragmentShader(const FragmentData &data, glm::vec4 &fragColor,
		const glm::vec2 &dUVdx, const glm::vec2 &dUVdy) const override;
};

class BlinnPhongNormalMapShading final : public Pipeline3D
{
public:
	typedef std::shared_ptr<BlinnPhongNormalMapShading> ptr;

	virtual ~BlinnPhongNormalMapShading() = default;

	virtual void vertexShader(VertexData &vertex) const override;
	virtual void fragmentShader(const FragmentData &data, glm::vec4 &fragColor,
		const glm::vec2 &dUVdx, const glm::vec2 &dUVdy) const override;
};

class AlphaBlendingShading final : public Pipeline3D {
public:
	typedef std::shared_ptr<AlphaBlendingShading> ptr;

	virtual ~AlphaBlendingShading() = default;

	virtual void fragmentShader(const FragmentData &data, glm::vec4 &fragColor,
		const glm::vec2 &dUVdx, const glm::vec2 &dUVdy) const override;
};

// class SkyboxShading final : public Pipeline3D {
// public:
// 	typedef std::shared_ptr<SkyboxShading> ptr;

// 	virtual ~SkyboxShading() = default;

// 	virtual void vertexShader(VertexData &vertex) const override;
// 	virtual void fragmentShader(const FragmentData &data, glm::vec4 &fragColor,
// 		const glm::vec2 &dUVdx, const glm::vec2 &dUVdy) const override;

// };

// class ShadowMapping final : public Pipeline3D {
// public:

// };

// class PRTShading final : public Pipeline3D {
// public:

// };

} // namespace sr