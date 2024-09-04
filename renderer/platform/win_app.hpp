#pragma once

#include <string>
#include <sstream>
#include <memory>

#include <SDL2/SDL.h>

#include "timer.hpp"

namespace sr {

class WindowsApp {
public:
	typedef std::shared_ptr<WindowsApp> ptr;

	~WindowsApp();

	void readyToStart();

	void processEvent();
	bool shouldWindowClose() const { return m_quit; }
	double getTimeFromStart() { return m_timer.getTicks(); }
	int getMouseMotionDeltaX() const { return m_mouseDeltaX; }
	int getMouseMotionDeltaY() const { return m_mouseDeltaY; }
	int getMouseWheelDelta() const { return m_wheelDelta; }
	bool getIsMouseLeftButtonPressed() const { return m_mouseLeftButtonPressed; }

	double updateScreenSurface(
		unsigned char *pixels,
		int width, 
		int height, 
		int channel,
		unsigned int num_triangles);

	static WindowsApp::ptr getInstance(int width = 800, int height = 600, const char* title = "winApp");

private:
	WindowsApp() = default;

	WindowsApp(WindowsApp&) = delete;
	WindowsApp& operator=(const WindowsApp&) = delete;

	bool setup(int width, int height, const char* title);

private:
	SDL_Event m_events;
	int m_lastMouseX, m_lastMouseY;
	int m_mouseDeltaX, m_mouseDeltaY;
	bool m_mouseLeftButtonPressed = false;
	int m_lastWheelPos;
	int m_wheelDelta;
	bool m_quit = false;

	Timer m_timer;
	double m_lastTimePoint;
	double m_deltaTime;
	double m_fpsCounter;
	double m_fpsTimeRecorder;
	unsigned int m_fps;

	int m_screenWidth;
	int m_screenHeight;
	const char* m_windowTitle;

	SDL_Window* m_windowHandle = nullptr;
	SDL_Surface* m_screenSurface = nullptr;

	static WindowsApp::ptr m_instance;

};

} // namespace sr