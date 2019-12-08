#include <iostream>
#include <string>
#include <cmath>

#include "VTKFile.h"
#include "GridDomain.h"

#define BIT_0			0
#define BIT_1			1

using namespace std;
using namespace MarchingSquares;

int calculateCaseIndex(bool *binaryCode)
{
	int caseIndex = 0;

	caseIndex += binaryCode[0] * pow(2, 0);
	caseIndex += binaryCode[1] * pow(2, 1);
	caseIndex += binaryCode[2] * pow(2, 2);
	caseIndex += binaryCode[3] * pow(2, 3);

	return caseIndex;
}

int main(int argc, char **argv)
{
	if (argc < 4)
	{
		std::cout << "Parameter: <vtkFilePath> <scalarValue> <outFilePath>" << endl;
		return 0;
	}

	string vtkFilePath = argv[1];
	float scalarValue = atof(argv[2]);
	string outFilePath = argv[3];

	cout << "Loading VTK file..." << endl;
	VTKFile *vtkFile = VTKFile::loadVTK(vtkFilePath);

	if (vtkFile == NULL)
	{
		return 0;
	}

	cout << "Building cell grid..." << endl;
	GridDomain *gridDomain = vtkFile->toGridDomain();
	
	VTKFile *vtkCountour = new VTKFile();
	bool *binaryCode = new bool[4];
	GridCell *gridCell;
	int caseKey;
	
	int pointIndex = 0;
	int startPointIndex, endPointIndex;
	float p0_xValue, p0_yValue;
	float p1_xValue, p1_yValue;

	cout << "Calculating the contour... [ Total cells: " << gridDomain->cellVector.size() << "]" << endl;

	for (int i = 0; i < gridDomain->cellVector.size(); i++)
	{
		gridCell = &gridDomain->cellVector[i];

		binaryCode[0] = (gridCell->s0 > scalarValue) ? BIT_1 : BIT_0;
		binaryCode[1] = (gridCell->s1 > scalarValue) ? BIT_1 : BIT_0;
		binaryCode[2] = (gridCell->s2 > scalarValue) ? BIT_1 : BIT_0;
		binaryCode[3] = (gridCell->s3 > scalarValue) ? BIT_1 : BIT_0;

		caseKey = calculateCaseIndex(binaryCode);

		switch (caseKey)
		{
		case 1:
		case 14:
			p0_xValue = (((gridCell->v0.x * (gridCell->s1 - scalarValue)) + (gridCell->v1.x * (scalarValue - gridCell->s0))) / (gridCell->s1 - gridCell->s0));
			p1_yValue = (((gridCell->v3.y * (gridCell->s0 - scalarValue)) + (gridCell->v0.y * (scalarValue - gridCell->s3))) / (gridCell->s0 - gridCell->s3));

			vtkCountour->insertPoint(Point(pointIndex, p0_xValue, gridCell->v0.y));
			startPointIndex = pointIndex++;

			vtkCountour->insertPoint(Point(pointIndex, gridCell->v3.x, p1_yValue));
			endPointIndex = pointIndex++;

			vtkCountour->insertLine(Line(startPointIndex, endPointIndex));
			break;
		case 2:
		case 13:
			p0_xValue = (((gridCell->v0.x * (gridCell->s1 - scalarValue)) + (gridCell->v1.x * (scalarValue - gridCell->s0))) / (gridCell->s1 - gridCell->s0));
			p1_yValue = (((gridCell->v1.y * (gridCell->s2 - scalarValue)) + (gridCell->v2.y * (scalarValue - gridCell->s1))) / (gridCell->s2 - gridCell->s1));

			vtkCountour->insertPoint(Point(pointIndex, p0_xValue, gridCell->v0.y));
			startPointIndex = pointIndex++;

			vtkCountour->insertPoint(Point(pointIndex, gridCell->v1.x, p1_yValue));
			endPointIndex = pointIndex++;

			vtkCountour->insertLine(Line(startPointIndex, endPointIndex));
			break;
		case 3:
		case 12:
			p0_yValue = (((gridCell->v1.y * (gridCell->s2 - scalarValue)) + (gridCell->v2.y * (scalarValue - gridCell->s1))) / (gridCell->s2 - gridCell->s1));
			p1_yValue = (((gridCell->v3.y * (gridCell->s0 - scalarValue)) + (gridCell->v0.y * (scalarValue - gridCell->s3))) / (gridCell->s0 - gridCell->s3));

			vtkCountour->insertPoint(Point(pointIndex, gridCell->v1.x, p0_yValue));
			startPointIndex = pointIndex++;

			vtkCountour->insertPoint(Point(pointIndex, gridCell->v3.x, p1_yValue));
			endPointIndex = pointIndex++;

			vtkCountour->insertLine(Line(startPointIndex, endPointIndex));
			break;
		case 4:
		case 11:
			p0_yValue = (((gridCell->v1.y * (gridCell->s2 - scalarValue)) + (gridCell->v2.y * (scalarValue - gridCell->s1))) / (gridCell->s2 - gridCell->s1));
			p1_xValue = (((gridCell->v2.x * (gridCell->s3 - scalarValue)) + (gridCell->v3.x * (scalarValue - gridCell->s2))) / (gridCell->s3 - gridCell->s2));

			vtkCountour->insertPoint(Point(pointIndex, gridCell->v1.x, p0_yValue));
			startPointIndex = pointIndex++;

			vtkCountour->insertPoint(Point(pointIndex, p1_xValue, gridCell->v2.y));
			endPointIndex = pointIndex++;

			vtkCountour->insertLine(Line(startPointIndex, endPointIndex));
			break;
		case 5:
		case 10:
			p0_xValue = (((gridCell->v0.x * (gridCell->s1 - scalarValue)) + (gridCell->v1.x * (scalarValue - gridCell->s0))) / (gridCell->s1 - gridCell->s0));
			p1_yValue = (((gridCell->v3.y * (gridCell->s0 - scalarValue)) + (gridCell->v0.y * (scalarValue - gridCell->s3))) / (gridCell->s0 - gridCell->s3));

			vtkCountour->insertPoint(Point(pointIndex, p0_xValue, gridCell->v0.y));
			startPointIndex = pointIndex++;

			vtkCountour->insertPoint(Point(pointIndex, gridCell->v3.x, p1_yValue));
			endPointIndex = pointIndex++;

			vtkCountour->insertLine(Line(startPointIndex, endPointIndex));

			p0_yValue = (((gridCell->v1.y * (gridCell->s2 - scalarValue)) + (gridCell->v2.y * (scalarValue - gridCell->s1))) / (gridCell->s2 - gridCell->s1));
			p1_xValue = (((gridCell->v2.x * (gridCell->s3 - scalarValue)) + (gridCell->v3.x * (scalarValue - gridCell->s2))) / (gridCell->s3 - gridCell->s2));

			vtkCountour->insertPoint(Point(pointIndex, gridCell->v1.x, p0_yValue));
			startPointIndex = pointIndex++;

			vtkCountour->insertPoint(Point(pointIndex, p1_xValue, gridCell->v2.y));
			endPointIndex = pointIndex++;

			vtkCountour->insertLine(Line(startPointIndex, endPointIndex));
			break;
		case 6:
		case 9:
			p0_xValue = (((gridCell->v0.x * (gridCell->s1 - scalarValue)) + (gridCell->v1.x * (scalarValue - gridCell->s0))) / (gridCell->s1 - gridCell->s0));
			p1_xValue = (((gridCell->v2.x * (gridCell->s3 - scalarValue)) + (gridCell->v3.x * (scalarValue - gridCell->s2))) / (gridCell->s3 - gridCell->s2));

			vtkCountour->insertPoint(Point(pointIndex, p0_xValue, gridCell->v0.y));
			startPointIndex = pointIndex++;

			vtkCountour->insertPoint(Point(pointIndex, p1_xValue, gridCell->v2.y));
			endPointIndex = pointIndex++;

			vtkCountour->insertLine(Line(startPointIndex, endPointIndex));
			break;
		case 7:
		case 8:
			p0_xValue = (((gridCell->v2.x * (gridCell->s3 - scalarValue)) + (gridCell->v3.x * (scalarValue - gridCell->s2))) / (gridCell->s3 - gridCell->s2));
			p1_yValue = (((gridCell->v3.y * (gridCell->s0 - scalarValue)) + (gridCell->v0.y * (scalarValue - gridCell->s3))) / (gridCell->s0 - gridCell->s3));

			vtkCountour->insertPoint(Point(pointIndex, p0_xValue, gridCell->v2.y));
			startPointIndex = pointIndex++;

			vtkCountour->insertPoint(Point(pointIndex, gridCell->v3.x, p1_yValue));
			endPointIndex = pointIndex++;

			vtkCountour->insertLine(Line(startPointIndex, endPointIndex));
			break;
		default:
			break;
		}

		if ((i % 100) == 0)
		{
			cout << "[" << i << " / " << gridDomain->cellVector.size() << "]" << endl;
		}
	}

	cout << "Saving contour VTK file..." << endl;
	vtkCountour->saveVTK(outFilePath);
	cout << "Terminated!" << endl;

	return 0;
}