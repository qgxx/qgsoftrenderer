#pragma once

#include <SDL2/SDL.h>

namespace sr {

class Timer {
public:
    Timer() {
        m_startTicks = 0;
		m_pausedTicks = 0;

		m_paused = false;
		m_started = false;
    }

    void start() {
        m_started = true;
		m_paused = false;

		m_startTicks = SDL_GetTicks();
		m_pausedTicks = 0;
    }

    void stop() {
        m_started = false;
		m_paused = false;

		m_startTicks = 0;
		m_pausedTicks = 0;
    }

    void pause() {
        if (m_started && !m_paused) {
			m_paused = true;

			m_pausedTicks = SDL_GetTicks() - m_startTicks;
			m_startTicks = 0;
		}
    }

    void unpause() {
        if (m_started && m_paused) {
			m_paused = false;

			m_startTicks = SDL_GetTicks() - m_pausedTicks;
			m_pausedTicks = 0;
		}
    }

    Uint32 getTicks() {
        Uint32 time = 0;
		if (m_started) {
			if (m_paused) time = m_pausedTicks;
			else time = SDL_GetTicks() - m_startTicks;
		}
		return time;
    }

    bool isStarted() { return m_started; }
    bool isPaused() { return m_paused && m_started; }

private:
    Uint32 m_startTicks;
    Uint32 m_pausedTicks;

    bool m_paused;
    bool m_started;

};

} // namespace sr