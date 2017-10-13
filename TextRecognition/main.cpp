#include "TextRecognition.hpp"

INT main()
{
	//LearnSymbols();

	if(!FindSymbols()) std::cerr << "ERROR\n";

	system("PAUSE");
	return 0;
}