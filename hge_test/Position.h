#pragma once

struct Position
{
	float x;
	float y;
	Position()
	{
		x = 0;
		y = 0;
	}
	Position(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
	Position(const Position & pos)
	{
		x = pos.x;
		y = pos.y;
	}
};
