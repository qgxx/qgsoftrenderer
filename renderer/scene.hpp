#pragma once

#include <map>
#include <string>

#include "renderer.hpp"
#include "model.hpp"

namespace sr {
class SceneParser final
{
public:

	struct Config
	{
		glm::vec3 m_cameraPos;
		glm::vec3 m_cameraFocus;
		glm::vec3 m_cameraUp;

		float m_frustumFovy;
		float m_frustumNear;
		float m_frustumFar;

		std::map<std::string, int> m_lights;
		std::map<std::string, Model::ptr> m_entities;
	} m_scene;

	
	Model::ptr getEntity(const std::string &name);
	int getLight(const std::string &name);

	void parse(const std::string &path, Renderer::ptr renderer, bool generatedMipmap);

private:
	float parseFloat(std::string str) const;
	glm::vec3 parseVec3(std::string str) const;
	bool parseBool(std::string str) const;
	std::string parseStr(std::string str) const;

};

} // namespace sr