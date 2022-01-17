#include "gve_image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void GveImage::loadImage(GLFWimage& image, const char* path)
{
	image.pixels = stbi_load(path, &image.width, &image.height, 0, 4);
}

void GveImage::freeImage(const GLFWimage& image)
{
	stbi_image_free(image.pixels);
}
