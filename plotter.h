#pragma once
#include <vector>
#include <cstdint>

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

class plotter
{
public:
	plotter(int w, int h, std::uint32_t c) : w(w), h(h)
	{
		canvas.assign(w * h, c | 0xff000000);
	}

	void set(int x, int y, std::uint32_t c)
	{
		if (x >=0 && x < w && y >=0 && y < h)
			canvas[x + (h - y - 1) * w] = c | 0xff000000;
	}

	int write(const char* filename)
	{
		return stbi_write_png(filename, w, h, 4, canvas.data(), w << 2);
	}

private:
	int w, h;
	std::vector<std::uint32_t> canvas;
};
