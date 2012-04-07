#pragma once

#define GLX_GLXEXT_LEGACY   // Use our local glxext.h rather than the system one.

#include <GL/glx.h>
#include "../glx/glxext.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/xf86vmode.h>
#include <X11/keysym.h>
#include <ctime>

class Game;

class LGLXWindow {
public:
	LGLXWindow(void);
	virtual ~LGLXWindow(void);
	
	bool Create(int width, int hight, int bpp, bool fullscreen);
	void Destroy(void);
	void ProcessEvents(void);
	void AttachGame(Game* game)   { _game = game; }
	
	bool IsRunning(void)          { return _isRunning; }
	void SwapBuffers(void)        { glXSwapBuffers(_display, _xWindow); }
	
	float GetElapsedSeconds(void);
	
private:
	// A pointer to our game thingy..
	Game* _game;
	// Check to see if the window is still running FFS.
	bool _isRunning;
	
	Game* GetAttachedGame(void) { return _game; }
	
	unsigned int _lastTime;
	
	Display*              _display;
	Window                _xWindow;
	GLXContext            _glContext;
	XF86VidModeModeInfo   _XF86DeskMode;
	XSetWindowAttributes  _XSetAttr;
	int                   _screenID;
	
	bool            _isFullscreen;
	unsigned int    _width;
	unsigned int    _height;
	unsigned int    _bpp;
	
	bool _GL3Supported;
	// I think that's about all I need for now.. FOR NOW!!!
};
