
#include "MultiGenerate.h"
#include "PuzzleSymbols.h"


// inline Point operator+(const Point& l, const Point& r) { return { l.first + r.first, l.second + r.second }; }

void MultiGenerate::generate(
	int id,
	const std::vector<std::shared_ptr<Generate>>& gens,
	const std::vector<DecoPair>& symbolVec)
{
	generators = gens;
	PuzzleSymbols symbols(symbolVec);
	while (!generate(id, symbols));
}

bool MultiGenerate::generate(int id, PuzzleSymbols symbols)
{
	for (std::shared_ptr<Generate> gen : generators) {
		gen->initPanel(id);
		int fails = 0;
		while (!gen->generate_path(symbols)) {
			if (fails++ > 20)  // dangerous post-increment. consider with care
				return false;
		}
	}

	std::vector<std::string> solution1; //For debugging only
	for (int y = 0; y < generators[0]->_panel->height; ++y) {
		std::string row;
		for (int x = 0; x < generators[0]->_panel->width; ++x) {
			if (generators[0]->get(x, y) == Deco::kPath) {
				row += "xx";
			}
			else row += "    ";
		}
		solution1.push_back(row);
	}
	std::vector<std::string> solution2; //For debugging only
	for (int y = 0; y < generators[1]->_panel->height; ++y) {
		std::string row;
		for (int x = 0; x < generators[1]->_panel->width; ++x) {
			if (generators[1]->get(x, y) == Deco::kPath) {
				row += "xx";
			}
			else row += "    ";
		}
		solution2.push_back(row);
	}
	std::vector<std::string> solution3; //For debugging only
	for (int y = 0; y < generators[2]->_panel->height; ++y) {
		std::string row;
		for (int x = 0; x < generators[2]->_panel->width; ++x) {
			if (generators[2]->get(x, y) == Deco::kPath) {
				row += "xx";
			}
			else row += "    ";
		}
		solution3.push_back(row);
	}

	if (!place_all_symbols(symbols)){
		return false;
	}

	if (!generators[0]->hasFlag(Generate::Config::DisableWrite)) {
		generators[0]->write(id);
	}
	return true;
}

bool MultiGenerate::place_all_symbols(PuzzleSymbols symbols) {
	for (const auto& [symbol, quantity] : symbols[Deco::Symbol::Stone]) {
		if (!place_stones(symbol.color, quantity)) {
			return false;
		}
	}
	for (const auto& [symbol, quantity] : symbols[Deco::Symbol::Triangle]) {
		if (!place_triangles(symbol.color, quantity)) {
			return false;
		}
	}
	for (const auto& [symbol, quantity] : symbols[Deco::Symbol::Star]) {
		if (!place_stars(symbol.color, quantity)) {
			return false;
		}
	}
	return true;
}

bool MultiGenerate::can_place_gap(Point pos) const
{
	for (auto&& gen : generators) {
		if (gen->get(pos) != 0)
			return false;
	}
	return true;
}

bool MultiGenerate::place_stones(Deco::Color color, int amount)
{
	std::set<Point> open = generators[0]->_openpos;
	while (amount > 0) {
		if (open.size() < amount)
			return false;
		Point pos = Utilities::pick_random(open);
		bool valid = true;
		for (auto&& gen : generators) {
			std::set<Point> region = gen->get_region(pos);
			if (!gen->can_place_stone(region, color)) {
				for (Point p : region) {
					open.erase(p);
				}
				valid = false;
			}
			else if (splitStones) {
				for (Point p : region) {
					open.erase(p);
				}
			}
		}
		if (!valid) continue;
		for (auto&& gen : generators) {
			gen->set(pos, Deco::Stone(color));
			gen->_openpos.erase(pos);
			open.erase(pos);
		}
		--amount;
	}
	return true;
}

bool MultiGenerate::place_stars(Deco::Color color, int amount) {
	std::set<Point> open = generators[0]->_openpos;
	int halfPoint = amount / 2;
	for (auto&& gen : generators) {
		gen->_allowNonMatch = amount % 2;
	}
	while (amount > 0) {
		if (open.size() < amount) {
			return false;
		}
		Point pos = Utilities::pick_random(open);
		std::vector<std::set<Point>> regions;
		std::vector<std::shared_ptr<Generate>> nonMatch;
		for (auto&& gen : generators) {
			std::set<Point> region = gen->get_region(pos);
			if (region.size() == 1) {
				for (Point p : region) {
					open.erase(p);
				}
				continue;
			}
			int count = gen->count_color(region, color);
			if (count == 0) {
				if (amount <= halfPoint || amount == halfPoint + 1 && gen->_allowNonMatch)
					for (Point p : region) {
						open.erase(p);
					}
				else regions.push_back(region);
			}
			else if (count == 1) {
				if (amount <= halfPoint && !gen->has_star(region, color) || amount > halfPoint && (!gen->_allowNonMatch || gen->has_star(region, color)))
					for (Point p : region) {
						open.erase(p);
					}
				else {
					regions.push_back(region);
					if (amount > halfPoint) nonMatch.push_back(gen);
				}
			}
			else {
				for (Point p : region) {
					open.erase(p);
				}
			}
		}
		if (regions.size() < generators.size()) continue;
		for (auto&& gen : nonMatch) {
			gen->_allowNonMatch = false;
		}
		for (std::set<Point>& region : regions) for (Point p : region) {
			open.erase(p);
		}
		for (auto&& gen : generators) {
			gen->set(pos, Deco::Star(color));
			gen->_openpos.erase(pos);
		}
		--amount;
		if (amount == halfPoint) open = generators[0]->_openpos;
	}
	return true;
}

bool MultiGenerate::can_place_triangle(Point pos) const
{
	int count = generators[0]->count_sides(pos);
	if (count == 0) {
		return false;
	}
	for (int i = 1; i < generators.size(); ++i) {
		if (generators[i]->count_sides(pos) != count) {
			return false;
		}
	}
	return true;
}

bool MultiGenerate::place_triangles(Deco::Color color, int amount)
{
	std::set<Point> open;
	for (Point p : generators[0]->_openpos) {
		if (can_place_triangle(p)) {
			open.insert(p);
		}
	}
	if (generators[0]->get(1, 1) == 0xA05) { //Mountain Hatch Perspective puzzle
		open.erase({ 1, 5 }); open.erase({ 9, 5 });
	}
	while (amount > 0) {
		if (open.size() < amount)
			return false;
		Point pos = Utilities::pick_random(open);
		open.erase(pos);
		int count = generators[0]->count_sides(pos);
		for (auto&& gen : generators) {
			gen->set(pos, Deco::Triangle(color, count));
			gen->_openpos.erase(pos);
		}
		--amount;
	}
	return true;
}
