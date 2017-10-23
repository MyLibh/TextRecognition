// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "TextRecognition.hpp"

//#define __DEBUG__

INT main()
{
	//LearnSymbols();

	std::string result;
	if(!FindSymbols(&result)) std::cerr << "ERROR\n";

	system("PAUSE");
	return 0;
}