void render_background() {
	unsigned int* pixel = (unsigned int*)render_state.memory;
	for (int x = 0; x < render_state.width; x++) {
		for (int y = 0; y < render_state.height; y++) {
		*pixel++ = 0xff00ff * x + 0x00ff00 * y;
		}
	}
}

void clear_screen(unsigned int color) {
	unsigned int* pixel = (unsigned int*)render_state.memory;
	for (int x = 0; x < render_state.width; x++) {
		for (int y = 0; y < render_state.height; y++) {
			*pixel++ = color;
		}
	}
}

inline int
clamp(int min, int val, int max) {
	if (val < min) return min;
	if (val > max) return max;
	return val;
}

void draw_rect(int x0, int y0, int x1, int y1, unsigned int color) {

	x0 = clamp(0, x0, render_state.width);
	x1 = clamp(0, x1, render_state.width);
	y0 = clamp(0, y0, render_state.height);
	y1 = clamp(0, y1, render_state.height);

	for (int x = x0; x < x1; x++) {
		unsigned int* pixel = (unsigned int*)render_state.memory + y0 + x*render_state.width;
		for (int y = y0; y < y1; y++) {
			*pixel++ = color;
		}
	}
}