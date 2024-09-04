#pragma once

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "frame_buffer.hpp"
#include "model.hpp"
#include "context.hpp"
#include "pipeline.hpp"

namespace sr {
class Renderer final {
public:
	typedef std::shared_ptr<Renderer> ptr;

	Renderer(int width, int height);
	~Renderer() = default;

	//Drawable objects load/unload
	void addModel(Model::ptr model);
	void addModel(const std::vector<Model::ptr> &models);
	void unloadDrawableMesh();

	void clearColor(const glm::vec4 &color) { m_backBuffer->clearColor(color); }
	void clearDepth(const float &depth) { m_backBuffer->clearDepth(depth); }
	void clearColorAndDepth(const glm::vec4 &color, const float &depth) { m_backBuffer->clearColorAndDepth(color, depth); }

	//Setting
	void setViewMatrix(const glm::mat4 &view) { m_viewMatrix = view; }
	void setModelMatrix(const glm::mat4 &model) { m_modelMatrix = model; }
	void setProjectMatrix(const glm::mat4 &project, float near, float far) { m_projectMatrix = project;m_frustumNearFar = glm::vec2(near, far); }
	void setShaderPipeline(Pipeline::ptr shader) { m_pipelineHandler = shader; }
	void setViewerPos(const glm::vec3 &viewer);

	int addLightSource(Light::ptr lightSource);
	Light::ptr getLightSource(const int &index);
	void setExposure(const float &exposure);

	//Draw call
	unsigned int renderAllModels();

	unsigned int renderModel(const size_t &index);

	//Commit rendered result
	unsigned char* commitRenderedColorBuffer();

	//Homogeneous space clipping - Sutherland Hodgeman algorithm
	static std::vector<Pipeline::VertexData> clipingSutherlandHodgeman(
		const Pipeline::VertexData &v0,
		const Pipeline::VertexData &v1,
		const Pipeline::VertexData &v2,
		const float &near, 
		const float &far);

private:

	//Cliping auxiliary functions
	static std::vector<Pipeline::VertexData> clipingSutherlandHodgemanAux(
		const std::vector<Pipeline::VertexData> &polygon,
		const int &axis, 
		const int &side);

private:
	//Drawable mesh array
	std::vector<Model::ptr> m_models;

	//MVP transformation matrices
	glm::mat4 m_modelMatrix = glm::mat4(1.0f);				//From local space  -> world space
	glm::mat4 m_viewMatrix = glm::mat4(1.0f);				//From world space  -> camera space
	glm::mat4 m_projectMatrix = glm::mat4(1.0f);			//From camera space -> homogeneous clip space
	glm::mat4 m_viewportMatrix = glm::mat4(1.0f);			//From ndc space    -> screen space

	Context m_context;

	//Near plane & far plane
	glm::vec2 m_frustumNearFar;

	//Shader pipeline handler
	Pipeline::ptr m_pipelineHandler = nullptr;

	//Double buffers
	FrameBuffer::ptr m_backBuffer;                      // The frame buffer that's goint to be written.
	FrameBuffer::ptr m_frontBuffer;                     // The frame buffer that's goint to be displayed.
	std::vector<unsigned char> m_renderedImg;			// The rendered image.
};

} // namespace sr