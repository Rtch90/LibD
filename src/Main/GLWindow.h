#pragma once
#include <windows.h>
#include <ctime>

class Game;

class GLWindow {
public:
	GLWindow(HINSTANCE hInstance);
	~GLWindow(void);
	
	bool Create(int width, int height, int bpp, bool fullscreen);
	void Destroy(bool);
	void ProcessEvents(void);
	void AttachGame(Game* game) 	{ _game = game; }
	
	bool IsRunning(void) 					{ return _isRunning; }
	void SwapBuffers(void)				{ SwapBuffers(_hdc); }
	
	static LRESULT CALLBACK StaticWndProc(HWND wnd, UINT msg, WPARAP wParam, LPARAM lParam);
	LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	float GetElapsedSeconds(void);

private:
	void SetupPixelFormat(void);
	Game* GetAttachedGame(void) { return _game; }

	// A pointer to the application.
	Game*		_game;
	bool		_isRunning;
	bool		_isFullScreen;
	float		_lastTime;
	
	HWND 				_hwnd;					// Window handle.
	HGLRC				_hglrc;					// Rendering context.
	HDC					_hdc;						// Device context.
	RECT				_windowRect;		// Window bound.
	HINSTANCE		_hinstance;			// Application instance.
	WNDCLASSEX	_windowClass;
}