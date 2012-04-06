#include <iostream>
#include <string>

#ifdef __unix__
#include <sys/time.h>
#endif

#include "LGLXWindow.h"
#include "Game.h"

using std::string;

//typedef GLXContext (*PFNGLXCREATECONTEXTATTRIBSARBPROC)(Display* dpy, GLXFBConfig config,
//																		GLXContext share_context, bool direct, const int* attrib_list);

unsigned int GetTickCount(void) {
  struct timeval t;
  gettimeofday(&t, NULL);
  
  unsigned long secs = t.tv_sec * 1000;
  	secs += (t.tv_usec / 1000);
  	return secs;
}

LGLXWindow::LGLXWindow(void) : 
		_game(NULL),
		_isRunning(true),
		_lastTime(0),
		_display(NULL),
		_xWindow(0),
		_glContext(0),
		_screenID(0),
		_isFullscreen(false),
		_width(0),
		_height(0),
		_bpp(0),
		_GL3Supported(false)  {}
		
LGLXWindow::~LGLXWindow(void) {

}

bool LGLXWindow::Create(int width, int height, int bpp, bool fullscreen) {
	// Open the default display.
	_display = XOpenDisplay(0);
	if(!_display) {
		std::cerr << "Could not open the display." << std::endl;
		return false;
	}
	
	// Get the default screen ID.
	_screenID = DefaultScreen(_display);
	
	int n = 0, modeNum = 0;
	// Get a frambuffer config useing the default attributes.
	GLXFBConfig framebufferConfig = (*glXChooseFBConfig(_display, DefaultScreen(_display), 0, &n));
	
	XF86VidModeModeInfo **modes;
	if(!XF86VidModeGetAllModeLines(_display, _screenID, &modeNum, &modes)) {
		std::cerr << "Could not query the video modes." << std::endl;
		return false;
	}
	
	_XF86DeskMode = *modes[0];
	
	int bestMode = -1;
	for(int i = 0; i < modeNum; i++) {
		if((modes[i]->hdisplay == width) && (modes[i]->vdisplay == height)) {
			bestMode = i;
		}
	}
	
	if(bestMode == -1) {
		std::cerr << "Could not find a suitable graphics mode." << std::endl;
		return false;
	}
	
	int doubleBufferedAttribList[] = {
		GLX_RGBA,				GLX_DOUBLEBUFFER,
		GLX_RED_SIZE,		4,
		GLX_GREEN_SIZE,	4,
		GLX_BLUE_SIZE, 	4,
		GLX_DEPTH_SIZE, 16,
		None
	};
	
	XVisualInfo* vi = NULL;
	// Attempt to create a double buffered window.
	vi = glXChooseVisual(_display, _screenID, doubleBufferedAttribList);
	if(!vi) {
		std::cerr << "Could not create a double buffere window.. Sux.." <<std::endl;
	}
	
	// Time to create a GL 2.1 context.
	GLXContext gl2Context = glXCreateContext(_display, vi, 0, GL_TRUE);
	if(!gl2Context) {
		std::cerr << "Could Not create a GL 2.1 context, check your darn graphics drivers" << std::endl;
		return false;
	}
	
	// Get a pointer to the GL 3.0 context creation.
	PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribs 
				= (PFNGLXCREATECONTEXTATTRIBSARBPROC) glXGetProcAddress((GLubyte*)"glXCreateContextAttribsARB");
	if(glXCreateContextAttribs == NULL) {
		std::cerr << "OpenGL 3.0 is not supported, falling back to 2.1" << std::endl;
		_glContext = gl2Context;
		_GL3Supported = false;
	} else {
		// We create a GL 3.0 context..
		
		int attribs[] = {
			GLX_CONTEXT_MAJOR_VERSION_ARB, 3, // We want a 3.0 context.
			GLX_CONTEXT_MINOR_VERSION_ARB, 0, 
			0 // Zero indicates the end of the array.		
		};
		
		_glContext = glXCreateContextAttribs(_display, framebufferConfig, 0, true, &attribs[0]);
		// We can destroy thr GL 2.0 context once the 3.0 one has been checked.
		glXDestroyContext(_display, gl2Context);
		_GL3Supported = true;
	}
	
	Colormap cmap = XCreateColormap(_display, RootWindow(_display, vi->screen), vi->visual, AllocNone);
	_XSetAttr.colormap = cmap;
	_XSetAttr.border_pixel = 0;
	_XSetAttr.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | 
																											StructureNotifyMask;
	_XSetAttr.override_redirect = False;
	
	unsigned long windowAttributes = CWBorderPixel | CWColormap | CWEventMask;
	
	if(fullscreen) {
		windowAttributes = CWBorderPixel | CWColormap | CWEventMask | CWOverrideRedirect;
		
		XF86VidModeSwitchToMode(_display, _screenID, modes[bestMode]);
		XF86VidModeSetViewPort(_display, _screenID, 0, 0);
		_XSetAttr.override_redirect = True;
	}																		
	
	_xWindow = XCreateWindow(_display, RootWindow(_display, vi->screen),
													0, 0, width, height, 0, vi->depth, InputOutput, vi->visual,
													CWBorderPixel | CWColormap | CWEventMask, &_XSetAttr);
	string title = "LibD";
	
	if(fullscreen) {
	  XWarpPointer(_display, None, _xWindow, 0, 0, 0, 0, 0, 0);
	  XMapRaised(_display, _xWindow);
	  XGrabKeyboard(_display, _xWindow, True, GrabModeAsync, GrabModeAsync, CurrentTime);
	  XGrabPointer(_display, _xWindow, True, ButtonPressMask, GrabModeAsync, GrabModeAsync,
	  								_xWindow, None, CurrentTime);
	} else {
		Atom wmDelete = XInternAtom(_display, "WM_DELETE_WINDOW", True);
		XSetWMProtocols(_display, _xWindow, &wmDelete, 1);
		XSetStandardProperties(_display, _xWindow, title.c_str(), None, NULL, NULL, 0, NULL);
		XMapRaised(_display, _xWindow);
	}
	
	XFree(modes);
	
	glXMakeCurrent(_display, _xWindow, _glContext);
	
	int posX = 0;
	int posY = 0;
	Window winDummy;
	unsigned int borderDummy;	
	
	_width 	= (unsigned) width;
	_height = (unsigned) height;
	_bpp		= (unsigned) bpp;
	
	XGetGeometry(_display, _xWindow, &winDummy, &posX, &posY, 
								&_width, &_height, &borderDummy, &_bpp);

	// Init the time.
	_lastTime = GetTickCount();
	return true;
}
							
void LGLXWindow::Destroy(void) {
	if(_glContext) {
		glXMakeCurrent(_display, None, NULL);
		glXDestroyContext(_display, _glContext);
		_glContext = NULL;
	}
	if(_isFullscreen) {
		XF86VidModeSwitchToMode(_display, _screenID, &_XF86DeskMode);
		XF86VidModeSetViewPort(_display, _screenID, 0, 0);
	}
	
	XCloseDisplay(_display);
}													

void LGLXWindow::ProcessEvents(void) {
	XEvent event;
	
	while(XPending(_display) > 0) {
		XNextEvent(_display, &event);
		switch(event.type) {
		case Expose:
			if(event.xexpose.count != 0)
				break;
			break;
		case ConfigureNotify:
		{
			int width  = event.xconfigure.width;
			int height = event.xconfigure.height;
			GetAttachedGame()->OnResize(width, height);
		}
		break;
		case KeyPress:
		{
			if(XLookupKeysym(&event.xkey, 0) == XK_Escape) {
				_isRunning = false;
			}
			// Register the key press with keyboard interface.
		}
		break;
		case KeyRelease:
		{
			// Code here NAW!
		}
		break;
		case ClientMessage:
			if(string(XGetAtomName(_display, event.xclient.message_type)) == string("WM_PROTOCOLA")) {
				_isRunning = true;
			}
			break;
		default:
			break;
		}
	}
}

float LGLXWindow::GetElapsedSeconds(void) {
	unsigned int currentTime = GetTickCount();
	unsigned int diff = currentTime - _lastTime;
	_lastTime = currentTime;
	return float(diff) / 1000.0f;
}				
		