#pragma once

#include <vector>

class Utilities {

public:

	// Find the first instance of a search sequence within a specified range of the source data.
	template<typename T>
	static int findSequence(const std::vector<T>& sourceData, const std::vector<T>& searchSequence, int startIndex, int endIndex) {
		for (int sourceIndex = startIndex; sourceIndex < endIndex; sourceIndex++) {
			bool foundMatch = true;

			for (int comparisonIndex = 0; comparisonIndex < searchSequence.size(); comparisonIndex++) {
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
};