#include <iostream>
#include <iomanip>

#include "win_app.hpp"
#include "parallel_wrapper.hpp"

namespace sr {
	
WindowsApp::ptr WindowsApp::m_instance = nullptr;

bool WindowsApp::setup(int width, int height, const char* title) {
	m_screenWidth = width;
	m_screenHeight = height;
	m_windowTitle = title;

	m_lastMouseX = 0;
	m_lastMouseY = 0;
	m_mouseDeltaX = 0;
	m_mouseDeltaY = 0;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	m_windowHandle = SDL_CreateWindow(
		m_windowTitle,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		m_screenWidth,
		m_screenHeight,
		SDL_WINDOW_SHOWN);

	if (m_windowHandle == nullptr) {
		std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	m_screenSurface = SDL_GetWindowSurface(m_windowHandle);

	return true;
}

WindowsApp::~WindowsApp() {
	SDL_DestroyWindow(m_windowHandle);
	m_windowHandle = nullptr;
	SDL_Quit();
}

void WindowsApp::readyToStart() {
	m_timer.start();
	m_lastTimePoint = m_timer.getTicks();

	m_fps = 0;
	m_fpsCounter = 0.0f;
	m_fpsTimeRecorder = 0.0f;
}

void WindowsApp::processEvent() {
	m_wheelDelta = 0;
	while (SDL_PollEvent(&m_events) != 0)
	{
		if (m_events.type == SDL_QUIT || (
			m_events.type == SDL_KEYDOWN && m_events.key.keysym.sym == SDLK_ESCAPE))
		{
			m_quit = true;
		}
		if (m_events.type == SDL_MOUSEMOTION)
		{
			static bool firstEvent = true;
			if (firstEvent)
			{
				firstEvent = false;
				m_lastMouseX = m_events.motion.x;
				m_lastMouseY = m_events.motion.y;
				m_mouseDeltaX = 0;
				m_mouseDeltaY = 0;
			}
			else
			{
				m_mouseDeltaX = m_events.motion.x - m_lastMouseX;
				m_mouseDeltaY = m_events.motion.y - m_lastMouseY;
				m_lastMouseX = m_events.motion.x;
				m_lastMouseY = m_events.motion.y;
			}
		}
		if (m_events.type == SDL_MOUSEBUTTONDOWN && m_events.button.button == SDL_BUTTON_LEFT)
		{
			m_mouseLeftButtonPressed = true;
			m_lastMouseX = m_events.motion.x;
			m_lastMouseY = m_events.motion.y;
			m_mouseDeltaX = 0;
			m_mouseDeltaY = 0;
		}
		if (m_events.type == SDL_MOUSEBUTTONUP && m_events.button.button == SDL_BUTTON_LEFT)
		{
			m_mouseLeftButtonPressed = false;
		}
		if (m_events.type == SDL_MOUSEWHEEL)
		{
			m_wheelDelta = m_events.wheel.y;
		}
	}
}

double WindowsApp::updateScreenSurface(
	unsigned char *pixels,
	int width,
	int height,
	int channel,
	unsigned int num_triangles) {
	SDL_LockSurface(m_screenSurface);
	{
		Uint32* destPixels = (Uint32*)m_screenSurface->pixels;
		parallelFor((size_t)0, (size_t)width *height, [&](const size_t &index) {
			Uint32 color = SDL_MapRGB(
				m_screenSurface->format,
				static_cast<uint8_t>(pixels[index * channel + 0]),
				static_cast<uint8_t>(pixels[index * channel + 1]),
				static_cast<uint8_t>(pixels[index * channel + 2]));
			destPixels[index] = color;
		});
	}
	SDL_UnlockSurface(m_screenSurface);
	SDL_UpdateWindowSurface(m_windowHandle);

	m_deltaTime = m_timer.getTicks() - m_lastTimePoint;
	m_lastTimePoint = m_timer.getTicks();

	{
		m_fpsTimeRecorder += m_deltaTime;
		++m_fpsCounter;
		if (m_fpsTimeRecorder > 1000.0)
		{
			m_fps = static_cast<unsigned int>(m_fpsCounter);
			m_fpsCounter = 0.0f;
			m_fpsTimeRecorder = 0.0f;

			std::stringstream ss;
			ss << " FPS:" << std::setiosflags(std::ios::left) << std::setw(3) << m_fps;
			ss << " #Triangles:" << std::setiosflags(std::ios::left) << std::setw(5) << num_triangles;
			SDL_SetWindowTitle(m_windowHandle, (m_windowTitle + ss.str()).c_str());
		}
	}

	return m_deltaTime;
}

WindowsApp::ptr WindowsApp::getInstance(int width, int height, const char* title) {
	if (m_instance == nullptr) {
		m_instance = std::shared_ptr<WindowsApp>(new WindowsApp);
		if (!m_instance->setup(width, height, title)) return nullptr;
	}
	return m_instance;
}

} // namespace sr