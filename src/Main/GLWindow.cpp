#ifdef _WIN32_
#include <ctime>
#include <iostream>
#include <Windows.h>
#include <GL/gl.h>

#include "../glx/wglext.h"
#include "GLWindow.h"
#include "Game.h"

typedef HGLRC(APIENTRYP PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC, HGLRC, const int*);
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;

GLWindow::GLWindow(HINSTANCE hInstance) :
  _isRunning(true),
  _game(NULL),
  _hinstance(hInstance),
  _lastTime(0)
  {}

GLWindow::~GLWindow() {

}

	
bool GLWindow::Create(int width, int height, int bpp, bool fullscreen) {
  DWORD   dwExStyle;		// Window extended style.
  DWORD   dwStyle;			// Window style.
	
  _isFullScreen       = fullscreen;     // Store the fullscreen flag.
	
  _windowRect.left    = (long)0;        // Set left value to zero.
  _windowRect.right   = (long)width;    // Set right value to the requested width.
  _windowRect.top     = (long)0;        // Set top value to zero.
  _windowRect.bottom  = (long)height;   // Set bottom value to the requested height.
	
  // Fill out the class structure.
  ZeroMemory(&_windowClass, sizeof(_windowClass));
  _windowClass.cbSize         = sizeof(WNDCLASSEX);
  _windowClass.style          = CS_HREDRAW | CS_VREDRAW;
  _windowClass.lpfnWndProc    = GLWindow::StaticWndProc;    // Set out static method as the next event.
  _windowClass.cbClsExtra	    = 0;
  _windowClass.cbWndExtra	    = 0;
  _windowClass.hInstance      =	_hinstance;
  _windowClass.hIcon          =	LoadIcon(NULL, IDI_APPLICATION);  // Default icon.
  _windowClass.hCursor        = LoadCursor(NULL, IDC_ARROW);      // Default arrow.
  _windowClass.hbrBackground  = NULL;                             // Don't need a background.
  _windowClass.lpszMenuName   = NULL;                             // No menu.
  _windowClass.lpszClassName  = TEXT("GLClass");
  _windowClass.hIconSm        =	LoadIcon(NULL, IDI_WINLOGO);      // Windows logo, small icon.
	
  // Register the window class.
  if(!RegisterClassEx(&_windowClass)) {
  	return false;
  }
  if(_isFullScreen) {
    // Then we need to change the display mode.
    DEVMODE dmScreenSettings; // Device mode.
		
	  memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
	  dmScreenSettings.dmSize = sizeof(dmScreenSettings);
	
	  dmScreenSettings.dmPelsWidth    = width;    // Screen width.
	  dmScreenSettings.dmPelsHeight   = height;   // Screen height.
	  dmScreenSettings.dmBitsPerPel   =	bpp;      // Bits per pixel.
	  dmScreenSettings.dmFields       = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
	
  	if(ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
      // Setting the display mode failed, so switch to windowed mode.
      MessageBox(NULL, TEXT("Display mode failed"), NULL, MB_OK);
      _isFullScreen = false;
	  }
	}
	// Are we still in fullscreen mode?
	if(_isFullScreen) {
		dwExStyle   = WS_EX_APPWINDOW;    // Window extended style.
		dwStyle     = WS_POPUP;           // Windows style.
		ShowCursor(false);                // Might as well hide the mouse cursor.
	} else {
		dwExStyle   = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;   // Window extended style.
		dwStyle     = WS_OVERLAPPEDWINDOW;                  // Windows style.
	}
	
	AdjustWindowRectEx(&_windowRect, dwStyle, false, dwExStyle);    // Adjust window to true requested size.
	
	// The class is registered, so now we can create our window.
	_hwnd = CreateWindowEx(dwExStyle, TEXT("GLClass"), TEXT("LibD"), dwStyle,
                            0, 0, _windowRect.right - _windowRect.left,
                            _windowRect.bottom - _windowRect.top, NULL, NULL, _hinstance, this);
														
	// Check if the window creation failed - hwnd would be NULL.
	if(!_hwnd)
		return NULL;
		
	_hdc = GetDC(_hwnd);
	
	ShowWindow(_hwnd, SW_SHOW);             // Display the window.
	UpdateWindow(_hwnd);                    // Update the window.
	
	_lastTime = GetTickCount() / 1000.0f;   // Initialize the time.
	return true;
}

void GLWindow::Destroy(void) {
	if(_isFullScreen) {
		// Change back to the desktop.
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(true);     // Show us the mouse cursor again.
	}
}

void GLWindow::ProcessEvents(void) {
	MSG msg;
	
	// While there are messages in the queue, store them in msg.
	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		// Process the message one by one.
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void GLWindow::SetupPixelFormat(void) {
	int pixelFormat;
	
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),    // Size.
			1,                              // Version.
			PFD_SUPPORT_OPENGL |            // OpenGL window.
			PFD_DRAW_TO_WINDOW |            // Render to window.
			PFD_DOUBLEBUFFER,	              // Support double-buffering.
			PFD_TYPE_RGBA,                  // Color type.
			32,                             // Prefered color depth.
			0, 0, 0, 0, 0, 0,               // Color bits (ignored).
			0,                              // No alpha buffer.
			0,                              // Alpha bits (ignored).
			0,                              // No accumulation buffer.
			0, 0, 0, 0,                     // Accumulation bits (ignored).
			16,                             // Depth buffer.
			0,                              // No stencil buffer.
			0,                              // No auxiliary buffers.
			PFD_MAIN_PLANE,                 // Main layer.
			0,                              // Reserverd.
			0, 0, 0,                        // No layer, visibility, damage masks.
	};
	
	pixelFormat = ChoosePixelFormat(_hdc, &pfd);
	SetPixelFormat(_hdc, pixelFormat, &pfd);
}

LRESULT GLWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_CREATE:	{         // Window creation.
		_hdc = GetDC(hWnd);
		SetupPixelFormat();
		
		// Set the version that we want. in this case, 3.0.
		int attribs[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 0,
			0	};		// Zero indicates the end of the array.
			
			// Create temporary context so we can get a pointer to the function.
			HGLRC tmpContext = wglCreateContext(_hdc);
			// Make it current.
			wglMakeCurrent(_hdc, tmpContext);
			
			// Get the function pointer.
			wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress("wglCreateContextAttribsARB");
			
			// If this is NULL then OpenGL 3.0 is not supported.
			if(!wglCreateContextAttribsARB) {
				std::cerr << "OpenGL 3.0 is not supported, falling back to GL 2.1" << std::endl;
				_hglrc = tmpContext;
			} else {
				// Create an OpenGL 3.0 context using the new function.
				_hglrc = wglCreateContextAttribsARB(_hdc, 0, attribs);
				// Delete them temp context.
				wglDeleteContext(tmpContext);
			}
			// Make the GL3 context current.
			wglMakeCurrent(_hdc, _hglrc);
			_isRunning = true;    // Mark our window as running now.
		}
	break;
	case WM_DESTROY:    // Destroy window.
	case WM_CLOSE:      // Windows is closing.
		wglMakeCurrent(_hdc, NULL);
		wglDeleteContext(_hglrc);
		_isRunning = false;   // Stop the main loop.
		PostQuitMessage(0);   // Send a WM_QUIT message.
		return 0;
	break;
	case WM_SIZE: {
		int height		= HIWORD(lParam);   // Retrieve width and height.
		int width			= LOWORD(lParam);
		GetAttachedGame()->OnResize(width, height);   // Call the games resize method.
	}
	break;
	case WM_KEYDOWN:
		if(wParam == VK_ESCAPE) {
			// Then send a WM_DESTROY request.
			DestroyWindow(_hwnd);
		}
	break;
	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK GLWindow::StaticWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	GLWindow* window = NULL;
	
	// If this is the create message:
	if(uMsg == WM_CREATE) {
		// Get the pointer we stored during create.
		window = (GLWindow*)((LPCREATESTRUCT)lParam)->lpCreateParams;
		
		// Associate the window pointer with the hwnd for the events to acess.
		SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR)window);
	} else {
		// If this is not a creation event, then we should have stored a pointer to the window.
		window = (GLWindow*) GetWindowLongPtr(hWnd, GWL_USERDATA);
	}
	// Call our window's member WndProc - Allows us to create access member variables)
	return window->WndProc(hWnd, uMsg, wParam, lParam);
}

float GLWindow::GetElapsedSeconds(void) {
	float currentTime = float(GetTickCount()) / 1000.0f;
	float seconds			= float(currentTime - _lastTime);
	_lastTime = currentTime;
	return seconds;
}

#endif
