/*The MIT License (MIT)

Copyright (c) 2021-Present, Wencong Yang (yangwc3@mail2.sysu.edu.cn).

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.*/

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "win_app.hpp"
#include "renderer.hpp"
#include "math_utils.hpp"
#include "shader.hpp"
#include "scene.hpp"

#include <iostream>

using namespace sr;

int main(int argc, char* args[]) {
	constexpr int width =  800;
	constexpr int height = 600;
	const char* title = "Point Scene";

	WindowsApp::ptr winApp = WindowsApp::getInstance(width, height, title);

	if (winApp == nullptr)
	{
		return -1;
	}

	bool generatedMipmap = true;
	Renderer::ptr renderer = std::make_shared<Renderer>(width, height);

	//Load scene
	SceneParser parser;
	parser.parse("assets/scenes/pointlight.scene", renderer, generatedMipmap);

	renderer->setViewMatrix(calcViewMatrix(parser.m_scene.m_cameraPos,
		parser.m_scene.m_cameraFocus, parser.m_scene.m_cameraUp));
	renderer->setProjectMatrix(calcPerspProjectMatrix(parser.m_scene.m_frustumFovy,
		static_cast<float>(width) / height, parser.m_scene.m_frustumNear, parser.m_scene.m_frustumFar),
		parser.m_scene.m_frustumNear, parser.m_scene.m_frustumFar);

	winApp->readyToStart();

	//Blinn-Phong lighting
	renderer->setShaderPipeline(std::make_shared<BlinnPhongShading>());

	PointLight::ptr redLight = std::dynamic_pointer_cast<PointLight>(renderer->getLightSource(parser.getLight("readLight")));
	PointLight::ptr greenLight = std::dynamic_pointer_cast<PointLight>(renderer->getLightSource(parser.getLight("greenLight")));
	PointLight::ptr blueLight = std::dynamic_pointer_cast<PointLight>(renderer->getLightSource(parser.getLight("blueLight")));

	glm::mat4 redLightModelMat(1.0f);
	glm::mat4 greenLightModelMat(1.0f);
	glm::mat4 blueLightModelMat(1.0f);
	glm::vec3 &redLightPos = redLight->getLightPos();
	glm::vec3 &greenLightPos = greenLight->getLightPos();
	glm::vec3 &blueLightPos = blueLight->getLightPos();

	glm::vec3 cameraPos = parser.m_scene.m_cameraPos;
	glm::vec3 lookAtTarget = parser.m_scene.m_cameraFocus;

	Model::ptr redLightMesh = parser.getEntity("RedLight");
	Model::ptr greenLightMesh = parser.getEntity("GreenLight");
	Model::ptr blueLightMesh = parser.getEntity("BlueLight");

	const auto originMat = redLightMesh->getModelMatrix();

	//Rendering loop
	while (!winApp->shouldWindowClose())
	{
		//Process event
		winApp->processEvent();

		//Clear frame buffer (both color buffer and depth buffer)
		renderer->clearColorAndDepth(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f);

		//Draw call
		renderer->setViewerPos(cameraPos);
		auto numTriangles = renderer->renderAllModels();

		//Display to screen
		double deltaTime = winApp->updateScreenSurface(
			renderer->commitRenderedColorBuffer(),
			width, 
			height,
			3,//RGB
			numTriangles);

		//Model transformation
		{
			redLightModelMat = glm::rotate(glm::mat4(1.0f), (float)deltaTime * 0.0008f, glm::vec3(1, 1, 0));
			redLightPos = glm::vec3(redLightModelMat * glm::vec4(redLightPos, 1.0f));
			redLightMesh->setModelMatrix(glm::translate(glm::mat4(1.0f), redLightPos));

			greenLightModelMat = glm::rotate(glm::mat4(1.0f), (float)deltaTime * 0.0008f, glm::vec3(1, 1, 1));
			greenLightPos = glm::vec3(greenLightModelMat * glm::vec4(greenLightPos, 1.0f));
			greenLightMesh->setModelMatrix(glm::translate(glm::mat4(1.0f), greenLightPos));

			blueLightModelMat = glm::rotate(glm::mat4(1.0f), (float)deltaTime * 0.0008f, glm::vec3(-1, 1, 1));
			blueLightPos = glm::vec3(blueLightModelMat * glm::vec4(blueLightPos, 1.0f));
			blueLightMesh->setModelMatrix(glm::translate(glm::mat4(1.0f), blueLightPos));
		}

		//Camera operation
		{
			//Camera rotation
			if (winApp->getIsMouseLeftButtonPressed())
			{
				int deltaX = winApp->getMouseMotionDeltaX();
				int deltaY = winApp->getMouseMotionDeltaY();
				glm::mat4 cameraRotMat(1.0f);
				if(std::abs(deltaX) > std::abs(deltaY))
					cameraRotMat = glm::rotate(glm::mat4(1.0f), -deltaX * 0.001f, glm::vec3(0, 1, 0));
				else 
					cameraRotMat = glm::rotate(glm::mat4(1.0f), -deltaY * 0.001f, glm::vec3(1, 0, 0));

				cameraPos = glm::vec3(cameraRotMat * glm::vec4(cameraPos, 1.0f));
				renderer->setViewMatrix(calcViewMatrix(cameraPos, lookAtTarget, glm::vec3(0.0, 1.0, 0.0f)));
			}

			//Camera zoom in and zoom out
			if (winApp->getMouseWheelDelta() != 0)
			{
				glm::vec3 dir = glm::normalize(cameraPos - lookAtTarget);
				float dist = glm::length(cameraPos - lookAtTarget);
				glm::vec3 newPos = cameraPos + (winApp->getMouseWheelDelta() * 0.1f) * dir;
				if (glm::length(newPos - lookAtTarget) > 1.0f)
				{
					cameraPos = newPos;
					renderer->setViewMatrix(calcViewMatrix(cameraPos, lookAtTarget, glm::vec3(0.0, 1.0, 0.0f)));
				}
			}
		}
	}

	renderer->unloadDrawableMesh();

	return 0;
}
