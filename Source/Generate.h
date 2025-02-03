#pragma once

#include <stdlib.h>
#include <time.h>

#include <algorithm>
#include <set>
#include <string>

#include "Panel.h"
#include "PuzzleSymbols.h"
#include "Random.h"
#include "Randomizer.h"

typedef std::set<Point> Shape;

//The main class for generating puzzles.
class Generate
{
public:
	// Factories under consideration
	// If these are included, make constructors private.
	// // Call this factory from the Randomizer class, so it can be used to hold
	// // the primary seed and make the first call to Random.h to establish the PRNG
	// static Generate make_primary(HWND loadingHandle, int seed, bool colorblind);
	// // Further calls to make ad hoc Generators occur throughout Special for puzzles with multiple solutions,
	// // in Main for the debug commands, and in DesertN for some reason
	// static Generate make_secondary(bool colorblind = false /*, int seed = 0*/);
	// // The main generation thread can deterministically spawn subthreads to speed up processing,
	// // so long as the subthreads are always made in the same order, given a new seed that initializes Random.h
	// // in the new thread, and then works off that seed from then on. Setting this up should be made easy.
	// static Generate spawn_threadsafe();

	// Main call: This should be made from Randomizer
	Generate(HWND loadingHandle, int seed, bool colorblind) 
		: _handle{loadingHandle}
		, _seed{seed}
	{
		if (seed >= 0) this->seed(seed);
		else this->seed(Random::rand());
		resetConfig();
	}

	// Subgenerator call: Everywhere else. They probably shouldn't talk to the GUI.
	Generate(bool colorblind=false) : colorblind{colorblind} {
		this->seed(Random::rand());
		resetConfig();
	}

	enum Config
	{ // See configinfo.txt for explanations of config flags.
		None = 0,
		FullGaps = 0x1,
		StartEdgeOnly = 0x2,
		DisableWrite = 0x4,
		PreserveStructure = 0x8,
		MakeStonesUnsolvable = 0x10,
		SmallShapes = 0x20,
		DisconnectShapes = 0x40,
		ResetColors = 0x80,
		DisableCancelShapes = 0x100,
		RequireCancelShapes = 0x200,
		BigShapes = 0x400,
		SplitShapes = 0x800,
		RequireCombineShapes = 0x1000,
		TreehouseLayout = 0x2000,
		TreehouseColors = 0x4000,
		AlternateColors = 0x8000,
		WriteColors = 0x10000,
		Write2Color = 0x20000,
		FixBackground = 0x40000,
		WriteSpecular = 0x80000,
		LongPath = 0x100000,
		ShortPath = 0x200000,
		EnableFlash = 0x400000,
		DecorationsOnly = 0x800000,
		FalseParity = 0x1000000,
		DisableDotIntersection = 0x2000000,
		WriteDotColor = 0x4000000,
		WriteDotColor2 = 0x8000000,
		LongestPath = 0x10000000,
		WriteInvisible = 0x20000000,
		DisableReset = 0x40000000,
		MountainFloorH = 0x80000000
	};

	void generate(int id)
	{
		PuzzleSymbols symbols({});
		bool done = false;
		while (!done)
		{
			done = generate(id, symbols);
		}
	}
	
	void generate(int id, const std::vector<DecoPair>& symbolVec) {
		PuzzleSymbols symbols(symbolVec);
		while (!generate(id, symbols));
	};

	/// @brief Generate puzzle with multiple solutions.
	/// The actual implementation.
	/// @param id id of the puzzle
	/// @param symbolVec pairs of symbols and amounts to use
	/// @param splitStones flag to indicate the stones should be spread farther
	/// than usual
	void generateMulti(int id,
					   const std::vector<std::shared_ptr<Generate>>& gens,
					   const std::vector<DecoPair>& symbolVec,
					   bool splitStones = false);

	/// Does anyone actually use this version?
	/// @param numSolutions the number of possible solutions
	void generateMulti(int id, int numSolutions,
					   const std::vector<DecoPair>& symbolVec,
					   bool splitStones = false) {
		// Create a vector of numSolutions default-initialized shared Generator
		// pointers. TODO: Investigate changing to unique.
		std::vector<std::shared_ptr<Generate>> gens(
			numSolutions);  // vector ctor (3)
		// Then pass it on to the overload.
		generateMulti(id, gens, symbolVec, splitStones);
	}

	void generateMaze(int id);
	void generateMaze(int id, int numStarts, int numExits);
	void initPanel(int id);
	void setPath(const std::set<Point>& path) {
		// why a Set?
		customPath = path;
		for (Point point : path) {
			setSymbol(Deco::Deco(Deco::Symbol::Path), point);
		}
	}
	void setObstructions(const std::vector<Point>& walls) { _obstructions = { walls }; }
	void setObstructions(const std::vector<std::vector<Point>>& walls) { _obstructions = walls; }
	void setSymbol(const Deco::Deco& symbol, const Point& point);
	void setSymbol(const Deco::Deco& symbol, int x, int y) {
		setSymbol(symbol, {x, y});
	}
	//void setSymbol(IntersectionFlags symbol, int x, int y) { setSymbol(static_cast<Decoration::Shape>(symbol), x, y); }
	// [[deprecated]]
	// void setVal(int val, int x, int y) { _panel->grid[x][y] = val; }
	void setVal(Point point, const Deco::Deco& symbol) {
		_panel->SetGridSymbol(point, symbol);
	}
	void setGridSize(int width, int height);
	void setGridSize(Point point) {
		setGridSize(point.first, point.second);
	};
	void setSymmetry(Panel::Symmetry symmetry);
	void write(int id);
	void setLoadingHandle(HWND handle) {
		_handle = handle;
	}
	void setLoadingData(int totalPuzzles) {
		_totalPuzzles = totalPuzzles;
		_genTotal = 0;
	}
	void setLoadingData(const std::wstring& areaName, int numPuzzles) {
		_areaName = areaName;
		_areaPuzzles = numPuzzles;
		_areaTotal = 0;
	}
	void setFlag(Config option) { _config |= option; };
	void setFlagOnce(Config option) { _config |= option; _oneTimeAdd |= option; };
	bool hasFlag(Config option) const { 
		return _config & option;
	};
	void removeFlag(Config option) {
		_config &= ~option;
	};
	void removeFlagOnce(Config option) {
		_config &= ~option;
		_oneTimeRemove |= option;
	};
	void resetConfig();
	void seed(long seed) {
		Random::seed(seed);
		_seed = Random::rand();
	}
	void incrementProgress();

	float pathWidth; //Controls how thick the line is on the puzzle
	std::vector<Point> hitPoints; //The generated path will be forced to hit these points in order
	std::set<Point> openPos; //Custom set of points that can have symbols placed on
	std::set<Point> blockPos; //Point that must be left open
	std::set<Point> customPath; 

	//For the arrow puzzles
	// TODO: Audit need for these to be Public
	Color arrowColor{0, 0, 0, 0};
	Color backgroundColor{0, 0, 0, 0};
	Color successColor{0, 0, 0, 0};

private:
	// Read the panel's current data, and convert it to a richer object for modification
	const Deco::Deco& get(Point pos) const {
		return Deco::to_deco(_panel->grid[pos.first][pos.second]);
	}
	const Deco::Deco& get(int x, int y) const {
		return Deco::to_deco(_panel->grid[x][y]);
	}

	// Force a specific symbol into a specific position.
	void set(Point pos, const Deco::Deco& val) {
		_panel->SetGridSymbol(pos, val);
	}
	// Force a specific symbol into a specific position.
	void set(int x, int y, const Deco::Deco& val) {
		// x and y are zero in the lower-left corner
		// Uses the Deco's custom cast to int
		_panel->_grid[x][y] = val;
	}
	// Force a specific symbol into a specific position.
	void set(Point pos, int val) {
		set(pos.first, pos.second, val);
	}
	// Force a specific symbol into a specific position.
	void set(int x, int y, int val) {
		if (val == 0) {
			set({x, y}, Deco::kEmpty);
		} else {
			_panel->_grid[x][y] = val;
		}
	}

	[[deprecated("use the .shape property to determine symbol type.")]]
	int get_symbol_type(int flags) const {
		return flags & 0x700;
	}

	void set_path(Point pos);
	Point get_sym_point(Point pos) const {
		return _panel->get_sym_point(pos);
	}

	// TODO: Points should determine their own parity.
	int get_parity(Point pos) const {
		return (pos.first / 2 + pos.second / 2) % 2;
	}

	void clear();
	void resetVars();
	void init_treehouse_layout();

	// Returns true if the point is on the edge of the puzzle grid.
	bool on_edge(Point p) const {
		// Pillars have no horizontal edges
		return (_panel->_pillarWidth == 0 && (p.first == 0 || p.first + 1 == _panel->width) ||
				p.second == 0 || p.second + 1 == _panel->height);
	}

	// Returns true if the point is beyond the extents of the puzzle grid.
	bool off_edge(Point p) const {
		return (p.first < 0 ||
				p.first >= _panel->width ||
				p.second < 0 ||
				p.second >= _panel->height);
	}


	// These are *offsets* from Points, not absolute positions.
	// Keep in mind that the generator grid is doubled, to insersperse paths and symbols.
	// Conceptually, these vectors are a sort of image kernel.
	constexpr const static std::array<Point, 4>& _DIRECTIONS1 {
		Point(0, 1), Point(0, -1), Point(1, 0), Point(-1, 0),
	};
	constexpr const static std::array<Point, 8>& _8DIRECTIONS1 {
		Point(0, 1), Point(0, -1), Point(1, 0), Point(-1, 0),
		Point(1, 1), Point(1, -1), Point(-1, -1), Point(-1, 1),
	};
	constexpr const static std::array<Point, 4>& _DIRECTIONS2 {
		Point(0, 2), Point(0, -2), Point(2, 0), Point(-2, 0),
	};
	constexpr const static std::array<Point, 8>& _8DIRECTIONS2 {
		Point(0, 2), Point(0, -2), Point(2, 0), Point(-2, 0),
		Point(2, 2), Point(2, -2), Point(-2, -2), Point(-2, 2),
	};

	//Used to make the discontiguous shapes
	constexpr const static std::array<Point, 20>& _DISCONNECT {
		Point(0, 2), Point(0, -2), Point(2, 0), Point(-2, 0),
		Point(2, 2), Point(2, -2), Point(-2, -2), Point(-2, 2), 
		Point(0, 2), Point(0, -2), Point(2, 0), Point(-2, 0),
		Point(2, 2), Point(2, -2), Point(-2, -2), Point(-2, 2),
		Point(0, 4), Point(0, -4), Point(4, 0), Point(-4, 0), 
	};

	std::vector<Point> _SHAPEDIRECTIONS = { }; //This will eventually be set to one of the above lists

	bool generate_maze(int id, int numStarts, int numExits);
	bool generate(int id, PuzzleSymbols symbols); //************************************************************
	bool place_all_symbols(PuzzleSymbols& symbols);
	bool generate_path(const PuzzleSymbols& symbols);
	bool generate_path_length(int minLength, int maxLength);
	bool generate_path_length(int minLength) { return generate_path_length(minLength, 10000); };
	bool generate_path_regions(int minRegions);
	bool generate_longest_path();
	bool generate_special_path();
	void erase_path();
	Point adjust_point(Point pos);
	std::set<Point> get_region(Point pos) const;
	std::vector<int> get_symbols_in_region(Point pos) const;
	std::vector<int> get_symbols_in_region(const std::set<Point>& region) const;
	
	bool place_start(int amount);
	bool place_exit(int amount);

	bool can_place_gap(Point pos) const;
	bool place_gaps(int amount);

	bool can_place_dot(Point pos, bool intersectionOnly) const;
	bool place_dots(DecoPair dp); // TODO: Investigate making const&

	bool can_place_stone(const std::set<Point>& region, const Deco::Color color) const;
	bool place_stones(const DecoPair& dp);

	Shape generate_shape(std::set<Point>& region, std::set<Point>& bufferRegion, Point pos, int maxSize);
	Shape generate_shape(std::set<Point>& region, Point pos, int maxSize) { std::set<Point> buffer; return generate_shape(region, buffer, pos, maxSize); }
	int make_shape_symbol(Shape shape, bool rotated, bool negative, int rotation, int depth);
	int make_shape_symbol(const Shape& shape, bool rotated, bool negative) { return make_shape_symbol(shape, rotated, negative, -1, 0); }
	bool place_shapes(const std::vector<Deco::Color>& colors, const std::vector<Deco::Color>& negativeColors, int amount, int numRotated, int numNegative);

	int count_color(const std::set<Point>& region, Deco::Color color) const;

	bool place_stars(DecoPair dp); // TODO: Investigate making const&
	bool has_star(const std::set<Point>& region, Deco::Color color) const;
	bool checkStarZigzag(std::shared_ptr<Panel> panel) const;

	bool place_triangles(DecoPair dp); // TODO: Investigate making const&
	int count_sides(Point pos) const;

	bool place_arrows(DecoPair dp); // TODO: Investigate making const&
	int count_crossings(Point pos, Point dir) const;

	bool place_erasers(
		const std::vector<Deco::Color>& colors,
		const std::vector<Deco::Deco>& eraseSymbols,
	  /*out*/ std::vector<Point>& placedSymbols);
	bool combine_shapes(std::vector<Shape>& shapes);

	std::shared_ptr<Panel> _panel{nullptr}; // TODO: Investigate making this unique or a reference. The Generator Has-A panel every time, right?
	std::vector<std::vector<int>> _custom_grid;
	int _width{0};
	int _height{0};
	Panel::Symmetry _symmetry;
	std::set<Point> _starts, _exits;
	std::set<Point> _gridpos, _openpos;
	std::set<Point> _path, _path1, _path2;
	bool _bisect {false}; // Used only to communicate between place_all_symbols() and place_stones()
	int _stoneTypes{0}; // Used only to communicate between place_all_symbols() and place_stones()
	int _config;
	int _oneTimeAdd, _oneTimeRemove;
	long _seed;
	std::vector<Point> _splitPoints;
	bool _allowNonMatch{false}; //Used for multi-generator
	int _parity{-1}; // Non-full-dots until told otherwise. Could probably become an enum.
	std::vector<std::vector<Point>> _obstructions;
	bool colorblind{false};

// Variables that track the overall process of generation and display on the GUI.
// These should probably be extracted from the class.
	HWND _handle{nullptr};
	unsigned int _areaTotal{0};
	unsigned int _genTotal{0};
	unsigned int _totalPuzzles{0};
	unsigned int _areaPuzzles{0};
	std::wstring _areaName;

	friend class PuzzleList;
	friend class NormalPuzzleList; // Investigate what it takes to remove this.
	friend class HardPuzzleList; // Investigate what it takes to remove this.
	friend class Special;
	friend class MultiGenerate;
};

