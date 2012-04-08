#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY		//  Use our local glxext.h rather than the system one.

#ifdef _WIN32
#include <windows.h>
#include "GLWindow.h"
#else
#include "LGLXWindow.h"
#endif

#include "Game.h"
#include "../System/Debug.h"

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, int cmdShow) {
#else
int main(int argc, char** argv) {
#endif
  // Start by opening a debug log.
  Debug::openLog(true);
  Debug::logger->message("\n ----- Engine Loading ------");
	// Get our window settings.
	const int windowWidth         = 800;
	const int windowHeight        = 600;
	const int windowBPP           = 16;
	const int windowFullscreen    = false;
	
#ifdef _WIN32
	// This is our window.
	GLWindow programWindow(hInstance);
#else
	LGLXWindow programWindow;
#endif
	// Our game code.
	Game game;
	
	// Attach our game to the window.
	programWindow.AttachGame(&game);
	
	// Attempt to create the window.
	if(!programWindow.Create(windowWidth, windowHeight, windowBPP, windowFullscreen)) {
		// If it fails.
#ifdef _WIN32
		MessageBox(NULL, TEXT("Unable to create the OpenGL Window"), TEXT("An error occured"), MB_ICONERROR | MB_OK);
#endif
		programWindow.Destroy();    // Reset the display and exit.
		return 1;
	}
	if(!game.Init()) {            // Initialize our game.
#ifdef _WIN32
		MessageBox(NULL, TEXT("Could not initialize the application"), TEXT("An error occured"), MB_ICONERROR | MB_OK);
#endif
		programWindow.Destroy();    // Reset the display and exit.
		return 1;
	}
	// This is the main loop, we render frames until IsRunning returns false.
	while(programWindow.IsRunning()) {
		programWindow.ProcessEvents();    // Process any window events.
		// We get the time that passed since the last frame.
		float elapsedTime = programWindow.GetElapsedSeconds();
		
		game.Prepare(elapsedTime);    // Do any pre-rendering logic.
		game.Render();                // Render the scene.
		
		programWindow.SwapBuffers();
	}
	game.Shutdown();            // Free any resouces.
	Debug::closeLog();
	programWindow.Destroy();    // Destroy the program window.
	
	return 0;
}
