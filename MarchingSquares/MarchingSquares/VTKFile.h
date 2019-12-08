#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "Point.h"
#include "GridDomain.h"
#include "LogHelper.h"

using namespace std;

namespace MarchingSquares
{
	struct Line
	{
		int startPointIndex, endPointIndex;

		Line()
		{
			this->startPointIndex = -1;
			this->endPointIndex = -1;
		}

		Line(int startPointIndex, int endPointIndex)
		{
			this->startPointIndex = startPointIndex;
			this->endPointIndex = endPointIndex;
		}
	};

	struct Dimension
	{
		int xDim, yDim;

		Dimension()
		{
			this->xDim = -1;
			this->yDim = -1;
		}

		Dimension(int xDim, int yDim)
		{
			this->xDim = xDim;
			this->yDim = yDim;
		}
	};

	struct Spacing
	{
		float xSpacing, ySpacing;

		Spacing()
		{
			this->xSpacing = -1.0f;
			this->ySpacing = -1.0f;
		}

		Spacing(float xSpacing, float ySpacing)
		{
			this->xSpacing = xSpacing;
			this->ySpacing = ySpacing;
		}
	};

	class VTKFile
	{
		private:
			bool isValid = false;
			Dimension dimension;
			Point originPoint;
			Spacing spacing;
			int pointDataCount;
			vector<Point> pointVector;
			vector<Line> lineVector;
			vector<float> scalarVector;

			static vector<string> tokenizer(const string& p_pcstStr, char delim);
		public:
			static VTKFile* loadVTK(string filePath);
			void saveVTK(string filePath);

			GridDomain* toGridDomain();
			
			void insertPoint(const Point &point);
			void insertLine(const Line &line);
	};
}