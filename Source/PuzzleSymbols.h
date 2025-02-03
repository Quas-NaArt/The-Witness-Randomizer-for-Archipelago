#pragma once
#include <vector>
#include <map>
#include "Panel.h"
#include "Random.h"

struct PuzzleSymbols {
	using SymbolMap = std::multimap<Deco::Symbol, DecoPair>;
	SymbolMap symbols;

	 //copy out a view of the symbols that match the type query
	auto operator[](Deco::Symbol symbolType) {
		std::vector<DecoPair> out;
		for (auto [iter, end] = symbols.equal_range(symbolType); iter != end; ++iter) {
			out.push_back(iter->second);
		}
		return out;
	}
	int style;
	int getNum(Deco::Symbol symbolType) const {
		int total = 0;
		auto [iter, end] = symbols.equal_range(symbolType);
		for (; iter!=end; ++iter) {
			total += iter->second.second;
		}
		return total;
	}

	// Remove a random allowable symbol from the map of symbols.
	// Used for generating puzzles with Erasers in them.
	// Returns the class of symbol that was removed
	// Attempts not to remove a dot from a Full Dots board.
	// Limits random runtime by making a flattened copy of the symbols map once
	// O(symbols.size()) as each symbol type is checked for validity
	// LIMITATIONS:
	//     Assumes that Full Dots boards always have at least 25 dots on them.
	//     Random selection is unweighted linear in specific types of symbol.
	//     E.g. if there are ten black Dots, one single Triangle, and one double Triangle,
	//     both types of Triangle are just as likely to be picked as each other, and just
	//     as likely as all the Dots combined.
	const Deco::Deco& popRandomSymbol() {
		std::vector<SymbolMap::iterator> allowedForRemoval(symbols.size()); // initialize empty
		// Make one scan through symbols and make a vector of valid iterators.
		for (auto iter = symbols.begin(); iter != symbols.end(); ++iter) {
			switch (iter->first) {
			case Deco::Symbol::Start:
			case Deco::Symbol::Exit:
			case Deco::Symbol::Gap:
			case Deco::Symbol::Eraser:
				// The above symbols are not allowed to be removed.
				break;
			case Deco::Symbol::Dot:
				if (iter->second.second >= 25) {
					// Try not to remove dots from a Full Dots grid.
					break;
				} else {
					[[fallthrough]];
				}
			default:
				// Non-Full Dots and all symbols not listed above
				if (iter->second.second == 0) {
					// Cannot have fewer than zero of a symbol.
					break;
				}
				// Put the iterator to the specific allowed element into the vector
				allowedForRemoval.emplace_back(iter);
				break;
			}; // end switch
		}
		// Roll one random number to pick one of the valid iterators.
		int randIndex = Random::rand() % allowedForRemoval.size();
		auto randSymbolIter = allowedForRemoval[randIndex];
		// Decrement the count for the chosen
		--(randSymbolIter->second.second);
		return randSymbolIter->second.first;
	}


	// Constructor from vector of DecoPairs
	PuzzleSymbols(const std::vector<DecoPair>& symbolVec) {
		for (const auto& pair : symbolVec) {
			// Multimap will accept multiple instances of the same key
			symbols.emplace(pair.first.symbol, pair);
		}
		style = 0;
		if (symbols.contains(Deco::Symbol::Dot)) {
			style |= Panel::Style::HAS_DOTS;
		}
		if (symbols.contains(Deco::Symbol::Stone)) {
			style |= Panel::Style::HAS_STONES;
		}
		if (symbols.contains(Deco::Symbol::Star)) {
			style |= Panel::Style::HAS_STARS;
		}
		if (symbols.contains(Deco::Symbol::Poly)) {
			style |= Panel::Style::HAS_SHAPERS;
		}
		if (symbols.contains(Deco::Symbol::Triangle)) {
			style |= Panel::Style::HAS_TRIANGLES;
		}
		if (symbols.contains(Deco::Symbol::Arrow)) {
			style |= Panel::Style::HAS_TRIANGLES;
		}
	}
};

