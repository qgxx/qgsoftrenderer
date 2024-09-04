#pragma once

#include "mesh.hpp"

namespace sr {

class Model {
public:
	typedef std::shared_ptr<Model> ptr;

	Model(const std::string &path, bool generatedMipmap);

	void clear();

	void setAmbientCoff(const glm::vec3 &cof) { m_drawingMaterial.m_kA = cof; }
	void setDiffuseCoff(const glm::vec3 &cof) { m_drawingMaterial.m_kD = cof; }
	void setSpecularCoff(const glm::vec3 &cof) { m_drawingMaterial.m_kS = cof; }
	void setEmissionCoff(const glm::vec3 &cof) { m_drawingMaterial.m_kE = cof; }
	void setSpecularExponent(const float &cof) { m_drawingMaterial.m_shininess = cof; }
	void setTransparency(const float &alpha) { m_drawingMaterial.m_transparency = alpha; }

	const glm::vec3& getAmbientCoff() const { return m_drawingMaterial.m_kA; }
	const glm::vec3& getDiffuseCoff() const { return m_drawingMaterial.m_kD; }
	const glm::vec3& getSpecularCoff() const { return m_drawingMaterial.m_kS; }
	const glm::vec3& getEmissionCoff() const { return m_drawingMaterial.m_kE; }
	const float& getSpecularExponent() const { return m_drawingMaterial.m_shininess; }
	const float& getTransparency() const { return m_drawingMaterial.m_transparency; }

	void setCullfaceMode(CullFaceMode mode) { m_drawing_config.m_cullfaceMode = mode; }
	void setDepthtestMode(DepthTestMode mode) { m_drawing_config.m_depthtestMode = mode; }
	void setDepthwriteMode(DepthWriteMode mode) { m_drawing_config.m_depthwriteMode = mode; }
	void setAlphablendMode(AlphaBlendingMode mode) { m_drawing_config.m_alphaBlendMode = mode; }
	void setModelMatrix(const glm::mat4& mat) { m_drawing_config.m_modelMatrix = mat; }
	void setLightingMode(LightingMode mode) { m_drawing_config.m_lightingMode = mode; }

	CullFaceMode getCullfaceMode() const { return m_drawing_config.m_cullfaceMode; }
	DepthTestMode getDepthtestMode() const { return m_drawing_config.m_depthtestMode; }
	DepthWriteMode getDepthwriteMode() const { return m_drawing_config.m_depthwriteMode; }
	AlphaBlendingMode getAlphablendMode() const { return m_drawing_config.m_alphaBlendMode; }
	const glm::mat4& getModelMatrix() const { return m_drawing_config.m_modelMatrix; }
	LightingMode getLightingMode() const { return m_drawing_config.m_lightingMode; }

	unsigned int getDrawableMaxFaceNums() const;
	MeshBuffer& getDrawableSubMeshes() { return m_meshes; }

protected:
	void importMeshFromFile(const std::string &path, bool generatedMipmap = true);

protected:
	MeshBuffer m_meshes;

	struct DrawableConfig {
		CullFaceMode m_cullfaceMode = CullFaceMode::CULL_BACK;
		DepthTestMode m_depthtestMode = DepthTestMode::DEPTH_TEST_ENABLE;
		DepthWriteMode m_depthwriteMode = DepthWriteMode::DEPTH_WRITE_ENABLE;
		AlphaBlendingMode m_alphaBlendMode = AlphaBlendingMode::ALPHA_DISABLE;
		LightingMode m_lightingMode = LightingMode::LIGHTING_ENABLE;
		glm::mat4 m_modelMatrix = glm::mat4(1.0f);
	};
	DrawableConfig m_drawing_config;

	struct DrawableMaterialCof {
		glm::vec3 m_kA = glm::vec3(0.0f);//Ambient coefficient
		glm::vec3 m_kD = glm::vec3(1.0f);//Diffuse coefficient
		glm::vec3 m_kS = glm::vec3(0.0f);//Specular coefficient
		glm::vec3 m_kE = glm::vec3(0.0f);//Emission
		float m_shininess = 1.0f;		   //Specular highlight exponment
		float m_transparency = 1.0f;	   //Transparency
	};
	DrawableMaterialCof m_drawingMaterial;

};

} // namespace sr