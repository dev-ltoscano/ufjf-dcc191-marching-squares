#pragma once

namespace MarchingSquares
{
	struct Point
	{
		int id;
		float x, y;

		Point()
		{
			this->id = -1;
			this->x = 0.0f;
			this->y = 0.0f;
		}

		Point(int id, float x, float y)
		{
			this->id = id;
			this->x = x;
			this->y = y;
		}
	};
}