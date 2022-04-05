#pragma once
#include "stdafx.h"
#include <locale>
#include <vector>

using namespace std;

// To tell a stream to use our character classifier:
// istreamname.imbue(std::locale(std::locale(), new DictionaryFilter));
struct DictionaryFilter : ctype<char>
{
// from https://stackoverflow.com/questions/43833675/reading-alphabetical-characters-only-from-file-c
	DictionaryFilter() : ctype<char>(get_table()) {}

	static ctype_base::mask const* get_table() {
		// A ctype facet that classifies everything other than a letter as white space.

		// Mark all possible characters as whitespace.
		static vector<ctype_base::mask> table(ctype<char>::table_size, ctype_base::space);

		for (int i = 0; i < numeric_limits<char>::max(); i++)
		{
			if (islower(i))
				table[i] = std::ctype_base::lower;
			else if (isupper(i))
				table[i] = std::ctype_base::upper;
			else
				table[i] = std::ctype_base::space;
		}
		return &table[0];
	}
};

