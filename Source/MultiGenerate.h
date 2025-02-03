#pragma once

#include "Decoration.h"
#include "Generate.h"
#include "Random.h"
#include "Utilities.h"

//Class for generating puzzles with multiple solutions.
class MultiGenerate {
public:

	MultiGenerate(bool splitStones = false) :
		splitStones{splitStones}
	{}
	~MultiGenerate() { }

	std::vector<std::shared_ptr<Generate>> generators;

	void generate(
		int id,
		const std::vector<std::shared_ptr<Generate>>& gens,
		const std::vector<DecoPair>& symbolVec);


private:
	bool splitStones;

	// Main lifter
	bool generate(int id, PuzzleSymbols symbols);

	// Functionality after preprocessing
	bool place_all_symbols(PuzzleSymbols symbols);

	// Detailed procedures
	bool place_stars(Deco::Color color, int amount);
	bool place_stones(Deco::Color color, int amount);
	bool place_triangles(Deco::Color color, int amount);

	// Const ability checks
	bool can_place_gap(Point pos) const;
	bool can_place_triangle(Point pos) const;

	friend class Special;
};

