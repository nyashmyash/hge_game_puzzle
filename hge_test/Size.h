#pragma once
struct Size
{
	float width;
	float height;
	Size()
	{
		width = 0;
		height = 0;
	}

	Size(float width, float height)
	{
		this->width = width;
		this->height = height;
	}
	Size(const Size &size)
	{
		width = size.width;
		height = size.height;
	}
};