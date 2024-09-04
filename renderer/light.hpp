#pragma once

#include <memory>

#include <glm/glm.hpp>

namespace sr {

class Light {
public:
	typedef std::shared_ptr<Light> ptr;

	Light() : m_intensity(glm::vec3(1.0f)) {}
	Light(const glm::vec3& intensity) : m_intensity(intensity) {}
	virtual ~Light() = default;

	const glm::vec3& intensity() const { return m_intensity; }
	virtual float attenuation(const glm::vec3 &fragPos) const = 0;
	virtual float cutoff(const glm::vec3 &lightDir) const = 0;
	virtual glm::vec3 direction(const glm::vec3 &fragPos) const = 0;

protected:
	glm::vec3 m_intensity;

};

class PointLight : public Light {
public:
	typedef std::shared_ptr<PointLight> ptr;

	PointLight(const glm::vec3 &intensity, const glm::vec3 &lightPos, const glm::vec3 &atten)
		: Light(intensity), m_lightPos(lightPos), m_attenuation(atten) { }

	virtual float attenuation(const glm::vec3 &fragPos) const override {
		float distance = glm::length(m_lightPos - fragPos);
		return 1.0 / (m_attenuation.x + m_attenuation.y * distance + m_attenuation.z * (distance * distance));
	}

	virtual glm::vec3 direction(const glm::vec3 &fragPos) const override {
		return glm::normalize(m_lightPos - fragPos);
	}

	virtual float cutoff(const glm::vec3 &lightDir) const override { return 1.0f; }

	glm::vec3 &getLightPos() { return m_lightPos; }

private:
	glm::vec3 m_lightPos; // world space
	glm::vec3 m_attenuation;

};

class SpotLight final : public PointLight {
public:
	typedef std::shared_ptr<SpotLight> ptr;

	SpotLight(const glm::vec3 &intensity, const glm::vec3 &lightPos, const glm::vec3 &atten, const glm::vec3 &dir,
		const float &innerCutoff, const float &outerCutoff) : PointLight(intensity, lightPos, atten),
		m_spotDir(glm::normalize(dir)), m_innerCutoff(innerCutoff), m_outerCutoff(outerCutoff) { }

	virtual float cutoff(const glm::vec3 &lightDir) const override {
		float theta = glm::dot(lightDir, -m_spotDir);
		static const float epsilon = m_innerCutoff - m_outerCutoff;
		return glm::clamp((theta - m_outerCutoff) / epsilon, 0.0f, 1.0f);
	}

	glm::vec3 &getSpotDirection() { return m_spotDir; }

private:
	glm::vec3 m_spotDir;
	float m_innerCutoff;
	float m_outerCutoff;

};

class DirectionalLight final : public Light
{
public:
	typedef std::shared_ptr<DirectionalLight> ptr;

	DirectionalLight(const glm::vec3 &intensity, const glm::vec3 &dir)
		: Light(intensity), m_lightDir(glm::normalize(dir)) { }

	virtual float attenuation(const glm::vec3 &fragPos) const override { return 1.0f; }
	virtual glm::vec3 direction(const glm::vec3 &fragPos) const override { return m_lightDir; }
	virtual float cutoff(const glm::vec3 &lightDir) const override { return 1.0f; }

private:
	glm::vec3 m_lightDir;
};

} // namespace sr