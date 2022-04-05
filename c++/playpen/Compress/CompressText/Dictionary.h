#pragma once
#include "stdafx.h"
#include "word.h"
#include <iostream>
#include <map>


using namespace std;

class Dictionary
{
private:
	iostream &_store;
	map<string, unsigned long> _cache;
	void sortCache(const map<string, unsigned long> &cache,
		multimap<unsigned long, string> &sorted);
public:
	Dictionary(iostream &store);
	~Dictionary();
	void Add(string word);
	void Save();
	void Load();
};