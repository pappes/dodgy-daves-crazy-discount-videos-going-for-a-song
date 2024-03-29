// CompressText.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Dictionary.h"
#include "DictionaryFilter.h"
#include <fstream>
#include <filesystem>
#include <sstream>


using namespace std;

void loadData()
{
	string dictfilepath(R"(C:\Users\dj\Documents\GitHub\dodgy-daves-crazy-discount-videos-going-for-a-song\c++\playpen\Compress\CompressText\eula.dict)");
	{//create the file if it does not exisit
		ofstream dictfile(dictfilepath);
	}
	fstream dictfile(dictfilepath);
	Dictionary dictionary(dictfile);

	//istringstream infilel("It’s a ver5y good #” idea of a line. You know it?");
	//ifstream infile(R"(C:\Users\dj\Documents\GitHub\dodgy-daves-crazy-discount-videos-going-for-a-song\c++\playpen\Compress\CompressText\eula.txt)");
	ifstream infile(R"(C:\Users\dj\Documents\GitHub\dodgy-daves-crazy-discount-videos-going-for-a-song\c++\playpen\Compress\CompressText\Dianetics_Today.txt)");

	std::string word;
	// Tell the stream to use our character classifier to treat all non alphabeic chars as whitespace
	infile.imbue(std::locale(std::locale(), new DictionaryFilter));
	while (infile >> word)
	{
		transform(word.begin(), word.end(), word.begin(), ::tolower);
		dictionary.Add(word);
	}
	dictionary.Save();
	std::experimental::filesystem::resize_file(dictfilepath, dictfile.tellp()); // resize to 1 G
	dictfile.close();
	//std::cout << bucket.rdbuf() << "\n";

}


int main()
{


	loadData();

	return 0;
}

