#include "stdafx.h"
#include "Dictionary.h"
#include "DictionaryFilter.h"


Dictionary::Dictionary(iostream &store) :_store(store)
{
}


Dictionary::~Dictionary()
{
}


void Dictionary::Add(string word)
{
	_cache[word]++;
}

void Dictionary::Save()
{
	multimap<unsigned long, string> sorted;
	sortCache(_cache, sorted);
	for (auto it = sorted.begin(); it != sorted.end(); ++it)
	{
		_store << it->first << " " << it->second << endl;
	}
}

void Dictionary::Load()
{
	std::string word;
	while (_store >> word)
	{
		Add(word);
	}
}


void Dictionary::sortCache(const map<string, unsigned long> &cache,
	                       multimap<unsigned long, string> &sorted) {

	for (auto it = cache.begin(); it != cache.end(); ++it)
	{
		sorted.insert(pair<unsigned long, string>(it->second, it->first));
	}
}