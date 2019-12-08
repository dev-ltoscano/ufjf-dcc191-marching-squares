#include "VTKFile.h"

using namespace MarchingSquares;

vector<string> VTKFile::tokenizer(const string& p_pcstStr, char delim)
{
	vector<string> tokens;
	stringstream strStream(p_pcstStr);
	string tmpString;

	while (getline(strStream, tmpString, delim))
	{
		tokens.push_back(tmpString);
	}

	return tokens;
}

VTKFile* VTKFile::loadVTK(string filePath)
{
	ifstream vtkFileStream(filePath.c_str());

	if (!vtkFileStream.is_open())
	{
		LogHelper::log("VTKFileException", "VTK file could not be opened");
	}
	else
	{
		VTKFile *vtkFile = new VTKFile();
		string lineRead, strTrash;

		while (getline(vtkFileStream, lineRead))
		{
			if (!lineRead.empty())
			{
				if (!vtkFile->isValid && ((lineRead.find("STRUCTURED_GRID") != string::npos) || (lineRead.find("STRUCTURED_POINTS") != string::npos)))
				{
					vtkFile->isValid = true;
				}
				else if (vtkFile->isValid)
				{
					if (lineRead.find("DIMENSIONS") != string::npos)
					{
						istringstream strStream(lineRead);
						strStream >> strTrash >> vtkFile->dimension.xDim >> vtkFile->dimension.yDim;
					}

					if (lineRead.find("ORIGIN") != string::npos)
					{
						istringstream strStream(lineRead);
						strStream >> strTrash >> vtkFile->originPoint.x >> vtkFile->originPoint.y;
					}

					if (lineRead.find("SPACING") != string::npos)
					{
						istringstream strStream(lineRead);
						strStream >> strTrash >> vtkFile->spacing.xSpacing >> vtkFile->spacing.ySpacing;
					}

					if (lineRead.find("POINTS") != string::npos)
					{
						istringstream strStream1(lineRead);

						int numPoint;
						strStream1 >> strTrash >> numPoint >> strTrash;

						float xPoint, yPoint;

						for (int i = 0; i < numPoint; i++)
						{
							getline(vtkFileStream, lineRead);

							if (!lineRead.empty())
							{
								istringstream strStream2(lineRead);
								strStream2 >> xPoint >> yPoint;

								vtkFile->pointVector.push_back(Point(i, xPoint, yPoint));
							}
						}
					}

					if (lineRead.find("POINT_DATA") != string::npos)
					{
						istringstream strStream(lineRead);
						strStream >> strTrash >> vtkFile->pointDataCount;
					}

					if (lineRead.find("SCALARS") != string::npos)
					{
						getline(vtkFileStream, strTrash);

						int readCount = 0;
						float scalarValue;

						while (readCount < vtkFile->pointDataCount)
						{
							getline(vtkFileStream, lineRead);

							if (!lineRead.empty())
							{
								vector<string> lineTokens = tokenizer(lineRead, ' ');

								for (int i = 0; i < lineTokens.size(); i++)
								{
									istringstream strStream(lineTokens.at(i));
									strStream >> scalarValue;

									vtkFile->scalarVector.push_back(scalarValue);
									readCount++;
								}
							}
						}
					}
				}
			}
		}

		vtkFileStream.close();

		if (!vtkFile->isValid)
		{
			LogHelper::log("VTKFileException", "VTK file could not be loaded");
			return NULL;
		}

		if (vtkFile->pointVector.size() == 0)
		{
			int pointCount = (vtkFile->dimension.xDim * vtkFile->dimension.yDim);

			int pointIndex = 0;
			int xDisplacement = 0;
			int yDisplacement = 0;
			float xPoint, yPoint;

			for (int j = 0; j < vtkFile->dimension.yDim; j++)
			{
				for (int i = 0; i < vtkFile->dimension.xDim; i++)
				{
					xPoint = (xDisplacement * vtkFile->spacing.xSpacing);
					yPoint = (yDisplacement * vtkFile->spacing.ySpacing);

					vtkFile->pointVector.push_back(Point(pointIndex++, xPoint, yPoint));

					xDisplacement++;
				}

				yDisplacement++;
			}
		}


		return vtkFile;
	}

	return NULL;
}

void VTKFile::saveVTK(string filePath)
{
	ofstream vtkFileStream(filePath.c_str());

	if (!vtkFileStream.is_open())
	{
		LogHelper::log("VTKFileException", "O arquivo VTK não pôde ser criado");
	}
	else
	{
		vtkFileStream << std::fixed << std::setprecision(7);

		vtkFileStream << "# vtk DataFile Version 3.0" << endl;
		vtkFileStream << "vtk output" << endl;
		vtkFileStream << "ASCII" << endl;
		vtkFileStream << "DATASET POLYDATA" << endl;

		if (this->pointVector.size() > 0)
		{
			vtkFileStream << "POINTS  " << this->pointVector.size() << "  float" << endl;

			for (int i = 0; i < this->pointVector.size(); i++)
			{
				vtkFileStream << this->pointVector[i].x << "  " << this->pointVector[i].y << "  " << 0.0f << endl;
			}
		}

		if (this->lineVector.size() > 0)
		{
			vtkFileStream << "LINES  " << this->lineVector.size() << "  " << (3 * this->lineVector.size()) << endl;

			for (int i = 0; i < this->lineVector.size(); i++)
			{
				vtkFileStream << "2  " << this->lineVector[i].startPointIndex << "  " << this->lineVector[i].endPointIndex << endl;
			}
		}

		if (this->scalarVector.size() > 0)
		{
			vtkFileStream << "POINT_DATA  " << this->scalarVector.size() << endl;
			vtkFileStream << "SCALARS scalar_value float" << endl;
			vtkFileStream << "LOOKUP_TABLE default" << endl;

			for (int i = 0; i < this->scalarVector.size(); i++)
			{
				vtkFileStream << this->scalarVector[i] << endl;
			}
		}



		vtkFileStream.close();
	}
}

GridDomain* VTKFile::toGridDomain()
{
	GridDomain *gridDomain = new GridDomain();

	int id0, id1, id2, id3;
	float s0, s1, s2, s3;

	int cellId = 0;
	int xIndexDisp = 0;
	int yIndexDisp = this->dimension.xDim;

	int totalIndexCount = (this->dimension.xDim * this->dimension.yDim) - (this->dimension.xDim + this->dimension.yDim - 1);
	int indexCount = 0;

	while (indexCount < totalIndexCount)
	{
		id0 = xIndexDisp;
		id1 = (id0 + 1);
		id2 = (yIndexDisp + 1);
		id3 = yIndexDisp;

		Point p0(id0, this->pointVector[id0].x, this->pointVector[id0].y);
		Point p1(id1, this->pointVector[id1].x, this->pointVector[id1].y);
		Point p2(id2, this->pointVector[id2].x, this->pointVector[id2].y);
		Point p3(id3, this->pointVector[id3].x, this->pointVector[id3].y);

		s0 = this->scalarVector[id0];
		s1 = this->scalarVector[id1];
		s2 = this->scalarVector[id2];
		s3 = this->scalarVector[id3];

		GridCell cell;

		cell.id = cellId++;

		cell.v0 = p0;
		cell.v1 = p1;
		cell.v2 = p2;
		cell.v3 = p3;

		cell.s0 = s0;
		cell.s1 = s1;
		cell.s2 = s2;
		cell.s3 = s3;

		gridDomain->cellVector.push_back(cell);

		if ((indexCount != 0) && (((indexCount + 1) % (this->dimension.xDim - 1)) == 0))
		{
			xIndexDisp += 2;
			yIndexDisp += 2;
		}
		else
		{
			xIndexDisp++;
			yIndexDisp++;
		}

		indexCount++;
	}

	return gridDomain;
}

void VTKFile::insertPoint(const Point &point)
{
	this->pointVector.push_back(point);
}

void VTKFile::insertLine(const Line &line)
{
	this->lineVector.push_back(line);
}