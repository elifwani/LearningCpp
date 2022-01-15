#include <windows.h>

bool running = true;

LRESULT CALLBACK window_callback (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	LRESULT result = 0;

	switch (uMsg) {
		case WM_CLOSE:
		case WM_DESTROY: {
			running = false;
		} break;

		default: {
		result = DefWindowProc(hwnd, uMsg, wParam, lParam);
		}

	}

	return result; // whenever we receive a message, we're gonna do the default behaviour
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	// Create a Window class
	WNDCLASS window_class = {0}; // create a window class, WNDCLASSA (for Ansi) gives problems when RegisterClass
	window_class.style = CS_HREDRAW | CS_VREDRAW; // the style of the class
	window_class.lpszClassName = "Game window class";  // to identify the window class, to define a Unicode string we put an L
	window_class.lpfnWndProc = window_callback; // lpfnWndProc is a callback function used to pass messages to us, for example if the users decides to be inactive, resize, close etc...

	// Register the class
	RegisterClass(&window_class); // pointer to the locker with window_class

	HWND window = CreateWindow(window_class.lpszClassName, "My First Cpp Game!", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0); {
	
	}

	// Create the window
	while (running) {
		// Gather input
		MSG message;
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) { // it gathers all the messages that we haven't read so far and gives s a chance to process it; PM_REMOVE to read the message only once
			TranslateMessage(&message);
			DispatchMessage(&message); // we need to dispatch the message to callback
		}
		// Simulate

		// Render
	}
}