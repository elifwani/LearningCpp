#include <windows.h>

bool running = true;

struct Render_State {
	int width, height;
	void* memory;

	BITMAPINFO bitmap_info;
};

Render_State render_state; // to clean up and get rid of buffer

#include "renderer.cpp"

LRESULT CALLBACK window_callback (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	LRESULT result = 0;

	switch (uMsg) {
		case WM_CLOSE:
		case WM_DESTROY: { // case if user wants to close the window
			running = false;
		} break;
		case WM_SIZE: { // case if user decides to resize the window
			RECT rect;
			GetClientRect(hwnd, &rect);
			render_state.width = rect.right - rect.left;
			render_state.height = rect.bottom - rect.top;
			int size = render_state.width * render_state.height * sizeof(unsigned int); // 32 bits, 8 for red, blue, green, padding to get to 32

			if (render_state.memory) VirtualFree(render_state.memory, 0, MEM_RELEASE); // if there is already memory, we should free first with VirtualFree
			render_state.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE); // we have the memory, now we have to ask to use it

			render_state.bitmap_info.bmiHeader.biSize = sizeof(render_state.bitmap_info.bmiHeader);
			render_state.bitmap_info.bmiHeader.biWidth = render_state.width;
			render_state.bitmap_info.bmiHeader.biHeight = render_state.height;
			render_state.bitmap_info.bmiHeader.biPlanes = 1; // needs to be 1
			render_state.bitmap_info.bmiHeader.biBitCount = 32; // because we're using 32 bits
			render_state.bitmap_info.bmiHeader.biCompression = BI_RGB;

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
	
	// Create the window
	HWND window = CreateWindow(window_class.lpszClassName, "My First Cpp Game!", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
	HDC hdc = GetDC(window); // device context

	while (running) {
		// Gather input
		MSG message;
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) { // it gathers all the messages that we haven't read so far and gives s a chance to process it; PM_REMOVE to read the message only once
			TranslateMessage(&message);
			DispatchMessage(&message); // we need to dispatch the message to callback
		}
		// Simulate
		// now to draw we only need to change the color of our pixels

		render_background();
		clear_screen(0x000000);
		draw_rect(50, 50, 1000, 1000, 0xffffff);

		// Render // we have render_state_memory, so now we need to ask Windows to use it
		StretchDIBits(hdc, 0, 0, render_state.width, render_state.height,0,0, render_state.width, render_state.height, render_state.memory, &render_state.bitmap_info, DIB_RGB_COLORS, SRCCOPY); // bitmapinfo holds a lot of information, like what pixels look like, compression, etc. for Windows to know how to copy that to the Video Card
																																			    // operation is source copy

	}
}