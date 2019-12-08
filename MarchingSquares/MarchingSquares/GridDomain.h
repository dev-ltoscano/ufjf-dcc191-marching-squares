#pragma once

#include <vector>
#include "Point.h"

using namespace std;

namespace MarchingSquares
{
	struct GridCell
	{
		int id;
		Point v0, v1, v2, v3;
		float s0, s1, s2, s3;
	};

	struct GridDomain
	{
		vector<GridCell> cellVector;
	};
}