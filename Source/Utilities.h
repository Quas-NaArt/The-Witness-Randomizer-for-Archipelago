#pragma once

#include <set>
#include <vector>

#include "Random.h"
namespace Utilities {

///////////////////////////////////////////////////////////////////////////////////////////////
// pick_random(container)
// For an input container, pick one element linearly from that container's first order
// (e.g picking from std::map<char, std::map<short, int>> would return a map<...>, not an int)
// Implemented for std::array, std::set, std::vector
///////////////////////////////////////////////////////////////////////////////////////////////
template <class T, size_t S> T pick_random(const std::array<T, S>& arr) {
	return arr[Random::rand() % S];
}

template <class T> T pick_random(const std::vector<T>& vec) {
	return vec[Random::rand() % vec.size()];
}

template <class T> T pick_random(const std::set<T>& set) {
	auto it = set.begin();
	std::advance(it, Random::rand() % set.size());
	return *it;
	}

///////////////////////////////////////////////////////////////////////////////////////////////
// pop_random(container)
// For an input container, pick one element linearly from that container's first order,
// remove it from the container, and return it.
// Implemented for std::set, std::vector
// Not implemented for std::array because if you wanted to remove elements, you'd use a vector
///////////////////////////////////////////////////////////////////////////////////////////////
template <class T> T pop_random(std::vector<T>& vec) {
	int i = Random::rand() % vec.size();
	T item = vec[i];
	vec.erase(vec.begin() + i);
	return item; }

template <class T> T pop_random(std::set<T>& set) {
	T item = pick_random(set); set.erase(item);
	return item; }

// Find the first instance of a search sequence within a specified range of the source data.
template<typename T>
static int findSequence(const std::vector<T>& sourceData, const std::vector<T>& searchSequence, int startIndex, int endIndex) {
	for (int sourceIndex = startIndex; sourceIndex < endIndex; ++sourceIndex) {
		bool foundMatch = true;

		for (int comparisonIndex = 0; comparisonIndex < searchSequence.size(); ++comparisonIndex) {
			if (sourceData[sourceIndex + comparisonIndex] != searchSequence[comparisonIndex])
			{
				foundMatch = false;
				break;
			}
		}

		if (foundMatch) {
			return sourceIndex;
		}
	}

	return -1;
}

// Find the first instance of a search sequence within the entirety of the source data.
template<typename T>
static int findSequence(const std::vector<T>& sourceData, const std::vector<T>& searchSequence) {
	return findSequence(sourceData, searchSequence, 0, sourceData.size() - searchSequence.size());
}

// Find all instances of a search sequence within a specified range in the source data.
template<typename T>
static std::vector<int> findAllSequences(const std::vector<T>& sourceData, const std::vector<T>& searchSequence, int startIndex, int endIndex) {
	std::vector<int> foundIndices;

	int searchIndex = startIndex;
	for (searchIndex = startIndex; searchIndex < endIndex;) {
		int foundIndex = findSequence(sourceData, searchSequence, searchIndex, endIndex);
		if (foundIndex != -1) {
			foundIndices.push_back(foundIndex);
			searchIndex = foundIndex + searchSequence.size();
		}
		else {
			break;
		}
	}

	return foundIndices;
}

// Find all instances of a search sequence within the entirety of the source data.
template<typename T>
static std::vector<int> findAllSequences(const std::vector<T>& sourceData, const std::vector<T>& searchSequence) {
	return findAllSequences(sourceData, searchSequence, 0, sourceData.size() - searchSequence.size());
}
}; // namespace Utilities