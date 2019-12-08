#pragma once

#include <iostream>
#include <string>

using namespace std;

namespace MarchingSquares
{
	class LogHelper
	{
		public:
			static void log(string errType, string errMsg)
			{
				std::cout << "[ " << errType << "]: " << errMsg << endl;
			}
	};
}