#pragma once

#include <stdint.h>

#include <map>
#include <memory>
#include <tuple>
#include <vector>

#include "Decoration.h"
#include "Memory.h"

struct Point {
	// Most functions are constexpr because they satisfy the requirements.
	// i.e. if a Point is defined at compile time, it can be used at compile time.
	// This can help with static_assert checks, for example.
	// Symmetric binary operator overloads are friends to follow the principle of least surprise.
	enum Alignment {
		Row,
		Column,
		Intersection,
		Space,
	};

	int first{0};
	int second{0};

	constexpr Point() {};
	constexpr Point(int x, int y) : first{x}, second{y} {};

	// Depends on the coordinate system. Investigate more later.
	// Points have been observed for use in the puzzle grid as row/column and x/y
	// Points have also been observed inside Poly shape descriptions, and those don't have intersections at all.
	constexpr Alignment spot() {
		if (first % 2) {
			if (second % 2) {
				return Space;
			}
			return Row;
		} else if (second % 2) {
			return Column;
		}
		return Intersection;
	}

	constexpr Point operator+(const Point& right) {
		return {first + right.first, second + right.second};
	};

	constexpr Point operator/(int d) {
		return {first / d, second / d};
	};

	constexpr friend bool operator==(const Point& left, const Point& right) {
		return left.first == right.first && left.second == right.second;
	};

	constexpr friend bool operator!=(const Point& left, const Point& right) {
		return left.first != right.first || left.second != right.second;
	};

	constexpr friend Point operator*(const int& left, const Point& right) {
		return {right.first * left, right.second * left};
	}
	constexpr friend Point operator*(const Point& left, const int& right) {
		// Let scalar multiplication of Points be commutative.
		return right*left;
	}

	// Comparison operator is required for placement into std::set
	constexpr friend bool operator<(const Point& p1, const Point& p2) {
		if (p1.first == p2.first) {
			return p1.second < p2.second;
		}
		return p1.first < p2.first;
	};
};
static_assert(2 * ::Point(2, 2) * 2 == ::Point(16, 16) / 2);

class Decoration
{
public:
	enum Shape : int {
		Exit =		0x600001,
		Start =		0x600002,
		Stone =		0x100,
		Star =		0x200,
		Poly =		0x400,
		Eraser =	0x500,
		Triangle =	0x600,
		Triangle1 = 0x10600,
		Triangle2 = 0x20600,
		Triangle3 = 0x30600,
		Triangle4 = 0x40600,
		Arrow =		0x700,
		Arrow1 = 0x1700,
		Arrow2 = 0x2700,
		Arrow3 = 0x3700,
		Can_Rotate = 0x1000,
		Negative = 0x2000,
		Gap = 0x100000,
		Gap_Row = 0x300000,
		Gap_Column = 0x500000,
		Dot = 0x20,
		Dot_Row = 0x240020,
		Dot_Column = 0x440020,
		Dot_Intersection = 0x600020,
		Empty = 0xA00,
	};
	enum Color : int {
		None = 0,
		Black = 0x1,
		White = 0x2,
		Red =	0x3, //Doesn't work sadly
		Purple = 0x4,
		Green = 0x5,
		Cyan = 0x6,
		Magenta = 0x7,
		Yellow = 0x8,
		Blue = 0x9,
		Orange = 0xA,
		X = 0xF,
	};
};

enum IntersectionFlags : int {
	ROW = 0x200000,
	COLUMN = 0x400000,
	INTERSECTION = 0x600000,
	ENDPOINT = 0x1,
	STARTPOINT = 0x2,
	OPEN = 0x3, //Puzzle loader flag - not to be written out
	PATH = 0x4, //Generator use only
	NO_POINT = 0x8, //Points that nothing connects to
	GAP = 0x100000,
	DOT = 0x20,
	DOT_IS_BLUE = 0x100,
	DOT_IS_ORANGE = 0x200,
	DOT_IS_INVISIBLE = 0x1000,
	DOT_SMALL = 0x2000,
	DOT_MEDIUM = 0x4000,
	DOT_LARGE = 0x8000,
};

class Endpoint {
public:
	enum Direction {
		NONE = 0,
		LEFT = 1,
		RIGHT = 2,
		UP = 4,
		DOWN = 8,
		UP_LEFT = 5,
		UP_RIGHT = 6,
		DOWN_LEFT = 9,
		DOWN_RIGHT = 10
	};

	Endpoint(int x, int y, Direction dir, int flags)
		: x{x}
		, y{y}
		, dir{dir}
		, _flags{flags}
		{}

// If there's public access for both get and set, why even make them private?
	[[deprecated("Prefer public member access when both get & set are public.")]]
	int GetX() const {return x;}
	[[deprecated("Prefer public member access when both get & set are public.")]]
	void SetX(int x) {this->x = x;}
	[[deprecated("Prefer public member access when both get & set are public.")]]
	int GetY() const {return y;}
	[[deprecated("Prefer public member access when both get & set are public.")]]
	void SetY(int y) {this->y = y;}
	[[deprecated("Prefer public member access when both get & set are public.")]]
	Direction GetDir() const {return dir;}
	[[deprecated("Prefer public member access when both get & set are public.")]]
	void SetDir(Direction dir) {this->dir = dir;}
	
	int GetFlags() const { return _flags; }

	int x;
	int y; 
	Direction dir;
private:
	int _flags;
};

struct Color {
	// Four-float Red-Green-Blue-Alpha color struct
	// Assumption: Valid component values are [0..1]
	// Limitations: No knowledge of color space or gamma power
	//    It's fine for our limited less-than-artistic puposes.
	float r;
	float g;
	float b;
	float a;

	// Used by std::less() as the Compare functor for std::set and std::map
	constexpr friend bool operator<(const Color& lhs, const Color& rhs) {
		return lhs.r * 8 + lhs.g * 4 + lhs.b * 2 + lhs.a < rhs.r * 8 + rhs.g * 4 + rhs.b * 2 + rhs.a;
	};

	// Will likely fail on arithmetically mutated floats
	// Define an allowable precision if using more than literal value assignments.
	constexpr friend bool operator==(const Color& lhs, const Color& rhs) {
		return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b && lhs.a == rhs.a;
	};
};

namespace { // anonymous for testing
	// Consider tossing these into a unit test module.
	static_assert(Color{0,0,0,0} < Color{1,1,1,1});
	//static_assert(std::hash(Color{0,0,0,0}) == std::hash(Color{0,0,0,0}));
	//static_assert(std::hash(Color{0,0,0,0}) != std::hash(Color{1,1,1,1}));
	static_assert(Color{0,0,0,0} == Color{0,0,0,0});
}

struct SolutionPoint {
	int pointA;
	int pointB;
	[[maybe_unused]] int pointC;
	[[maybe_unused]] int pointD;
	[[maybe_unused]] float f1x;
	[[maybe_unused]] float f1y;
	[[maybe_unused]] float f2x;
	[[maybe_unused]] float f2y;
	[[maybe_unused]] float f3x;
	[[maybe_unused]] float f3y;
	[[maybe_unused]] float f4x;
	[[maybe_unused]] float f4y;
	[[maybe_unused]] int endnum;
};

class Panel
{
public:
	enum Style {
		SYMMETRICAL = 0x2, //Not on the town symmetry puzzles? IDK why.
		NO_BLINK = 0x4,
		HAS_DOTS = 0x8,
		IS_2COLOR = 0x10,
		HAS_STARS = 0x40,
		HAS_TRIANGLES = 0x80,
		HAS_STONES = 0x100,
		HAS_ERASERS = 0x1000,
		HAS_SHAPERS = 0x2000,
		IS_PIVOTABLE = 0x8000,
	};

	enum Symmetry { //NOTE - Not all of these are valid symmetries for certain puzzles
		None, Horizontal, Vertical, Rotational,
		RotateLeft, RotateRight, FlipXY, FlipNegXY, ParallelH, ParallelV, ParallelHFlip, ParallelVFlip,
		PillarParallel, PillarHorizontal, PillarVertical, PillarRotational
	};

	enum ColorMode {
		Default,
		Reset,
		Alternate,
		WriteColors,
		Treehouse,
		TreehouseAlternate,
		Specular
	};

	// Constructors //
	Panel();
	Panel(int id);

	// Destructor //
	virtual ~Panel() = default;

	// Initialize the panel based on in-game data for the panel ID
	void Read(int id) {
		this->id = id;
		Read();
	}

	// Write the instance data to the game's RAM
	void Write();
	void Write(int id) {
		this->id = id;
		Write();
	}

	void SetSymbol(int x, int y, Decoration::Shape symbol, Decoration::Color color);
	void SetShape(int x, int y, int shape, bool rotate, bool negative, Decoration::Color color);
	void ClearSymbol(int x, int y);
	void ClearGridSymbol(int x, int y);

	// Place a Start node at the given point.
	void PlaceStart(const Point& point) {
		_startpoints.push_back(point);
	};

	// Place an Exit stub at the given point.
	// Direction will be handled in the implementation.
	void PlaceExit(const Point& point);
	void SetGridSymbol(const Point& point, const Deco::Deco& symbol);
	void Resize(int width, int height);

	[[deprecated("TODO: GetBackgroundColor needs magic numbers removed")]]
	Color GetBackgroundColor() const {
		return Memory::get()->ReadPanelData<Color>(0x0008F, 0x110); // BACKGROUND_REGION_COLOR from Randomizer.h
	}
	// Given a Point and a Symmetry type, return the Point symmetrical to the input.
	Point get_sym_point(const Point& point, const Symmetry symmetry) const
	{
		int x = point.first;
		int y = point.second;
		switch (symmetry) {
		case None: return point;
		case Symmetry::Horizontal: return Point(x, _height - 1 - y);
		case Symmetry::Vertical: return Point(_width - 1 - x, y);
		case Symmetry::Rotational: return Point(_width - 1 - x, _height - 1 - y);
		case Symmetry::RotateLeft: return Point(y, _width - 1 - x);
		case Symmetry::RotateRight: return Point(_height - 1 - y, x);
		case Symmetry::FlipXY: return Point(y, x);
		case Symmetry::FlipNegXY: return Point(_height - 1 - y, _width - 1 - x);
		case Symmetry::ParallelH: return Point(x, y == _height / 2 ? _height / 2 : (y + (_height + 1) / 2) % (_height + 1));
		case Symmetry::ParallelV: return Point(x == _width / 2 ? _width / 2 : (x + (_width + 1) / 2) % (_width + 1), y);
		case Symmetry::ParallelHFlip: return Point(_width - 1 - x, y == _height / 2 ? _height / 2 : (y + (_height + 1) / 2) % (_height + 1));
		case Symmetry::ParallelVFlip: return Point(x == _width / 2 ? _width / 2 : (x + (_width + 1) / 2) % (_width + 1), _height - 1 - y);
		case Symmetry::PillarParallel: return Point(x + _width / 2, y);
		case Symmetry::PillarHorizontal: return Point(x + _width / 2, _height - 1 - y);
		case Symmetry::PillarVertical: return Point( _width / 2 - x, y);
		case Symmetry::PillarRotational: return Point(_width / 2 - x, _height - 1 - y);
		default:
			throw std::exception("Call to get_sym_point with unimplemented Symmetry case");
		}
	}

	// If not explicit, use the Panel instance's saved symmetry setting.
	Point get_sym_point(const Point& point) const {
		return get_sym_point(point, _symmetry);
	}

	static void StartArrowWatchdogs(const std::map<int, int>& shuffleMappings = {});


	Symmetry _symmetry;
	float pathWidth;
	ColorMode colorMode;
	bool decorationsOnly;
	bool enableFlash;

	// public read-only references to the private properties
	// Try to refactor these in to minimize friend use.
	const std::vector<std::vector<int>>& grid = _grid;
	const int& width = _width;
	const int& height = _height;
	const std::vector<Point>& startpoints = _startpoints;
	const std::vector<Endpoint>& endpoints = _endpoints;


private:
	void Read();

	void ReadAllData();
	void ReadIntersections();
	void WriteIntersections();
	void ReadDecorations();
	void WriteDecorations();

	Endpoint::Direction get_sym_dir(Endpoint::Direction direction, Symmetry symmetry) const;

	int get_num_grid_points() const {
		return ((_width + 1) / 2) * ((_height + 1) / 2);
	}
	int get_num_grid_blocks() const {
		return (_width / 2) * (_height / 2);
	}
	int get_parity() const {
		return (get_num_grid_points() + 1) % 2;
	}


	Color get_color_rgb(int color) const;

	Point loc_to_xy(int location) const {
		int height2 = (_height - 1) / 2;
		int width2 = (_width + 1) / 2;

		int x = 2 * (location % width2);
		int y = 2 * (height2 - location / width2);
		return {x, y};
	}

	// ArrowWatchdog could be a non-friend if it weren't for this.
	// 
	int xy_to_loc(int x, int y) const {
		int height2 = (_height - 1) / 2;
		int width2 = (_width + 1) / 2;

		int rowsFromBottom = height2 - y/2;
		return rowsFromBottom * width2 + x/2;
	}

	// Converts a location from the in-game decoration array index
	// into a Point struct for use in the randomizer's grid
	Point dloc_to_xy(int location) const {
		int height2 = (_height - 3) / 2;
		int width2 = _width / 2;

		int x = 2 * (location % width2) + 1;
		int y = 2 * (height2 - location / width2) + 1;
		return {x, y};
	}

	int xy_to_dloc(int x, int y) const {
		int height2 = (_height - 3) / 2;
		int width2 = _width / 2;

		int rowsFromBottom = height2 - (y - 1)/2;
		return rowsFromBottom * width2 + (x - 1)/2;
	}

	// Point-based override of locate_segment
	int locate_segment(Point point, std::vector<int>& connections_a, std::vector<int>& connections_b) const {
		locate_segment(point.first, point.second, connections_a, connections_b);
	};
	//
	int locate_segment(int x, int y, std::vector<int>& connections_a, std::vector<int>& connections_b) const {
		for (int i = 0; i < connections_a.size(); i++) {
			Point coord1 = loc_to_xy(connections_a[i]);
			Point coord2 = loc_to_xy(connections_b[i]);
			int x1 = coord1.first, y1 = coord1.second, x2 = coord2.first, y2 = coord2.second;
			if (_pillarWidth > 0) {
				if ((x1 == (x - 1 + _pillarWidth) % _pillarWidth && // force positive modulus
				     x2 == (x + 1) % _pillarWidth &&
					 y1 == y &&
					 y2 == y) ||
					(y1 == y - 1 &&
					 y2 == y + 1 &&
					 x1 == x &&
					 x2 == x)) {
					return i;
				}
			}
			else if ((x1 == x - 1 &&
					  x2 == x + 1 &&
					  y1 == y &&
					  y2 == y) ||
					 (y1 == y - 1 &&
					  y2 == y + 1 &&
					  x1 == x &&
					  x2 == x)) {
				return i;
			}
		}
		return -1;
	}

	bool break_segment(int x, int y, std::vector<int>& connections_a, std::vector<int>& connections_b, std::vector<float>& intersections, std::vector<int>& intersectionFlags) {
		int i = locate_segment(x, y, connections_a, connections_b);
		if (i == -1) {
			return false;
		}
		int other_connection = connections_b[i];
		connections_b[i] = static_cast<int>(intersectionFlags.size());
		connections_a.push_back(static_cast<int>(intersectionFlags.size()));
		connections_b.push_back(other_connection);
		intersections.push_back(static_cast<float>(minx + x * unitWidth));
		intersections.push_back(static_cast<float>(miny + (_height - 1 - y) * unitHeight));
		intersectionFlags.push_back(_grid[x][y]);
		return true;
	}

	// Helper function for WriteIntersections
	bool break_segment_gap(
		int x,
		int y,
		std::vector<int>& connections_a,
		std::vector<int>& connections_b,
		std::vector<float>& intersections,
		std::vector<int>& intersectionFlags);

	void render_arrow(int x, int y, int ticks, int dir, std::vector<float>& intersections, std::vector<int>& intersectionFlags, std::vector<int>& polygons) {
		std::vector<float> positions = { 0.1f, 0.45f, 0.1f, 0.55f, 0.85f, 0.45f, 0.85f, 0.55f,
			0.9f, 0.5f, 0.75f, 0.5f, 0.45f, 0.2f, 0.6f, 0.2f, 0.45f, 0.8f, 0.6f, 0.8f,
			0.7f, 0.5f, 0.55f, 0.5f, 0.25f, 0.2f, 0.4f, 0.2f, 0.25f, 0.8f, 0.4f, 0.8f,
			0.5f, 0.5f, 0.35f, 0.5f, 0.05f, 0.2f, 0.2f, 0.2f, 0.05f, 0.8f, 0.2f, 0.8f, };
		std::vector<int> polys = { 0, 1, 2, 0, 1, 2, 3, 0,
			4, 5, 7, 0, 5, 6, 7, 0, 4, 5, 9, 0, 5, 8, 9, 0,
			10, 11, 13, 0, 11, 12, 13, 0, 10, 11, 15, 0, 11, 14, 15, 0,
			16, 17, 19, 0, 17, 18, 19, 0, 16, 17, 21, 0, 17, 20, 21, 0,
		};
		std::vector<int> angles = { -90, 90, 0, 180, -45, 45, 135, -135 };
		float angle = angles[dir] * 3.141592653589793238f / 180;
		for (int i = 0; i < positions.size(); i += 2) {
			//Translate to center
			positions[i] -= 0.5f;
			positions[i + 1] -= 0.5f;
			if (ticks == 3 && dir > 3) positions[i] += 0.1f;
			//Scale
			positions[i] *= unitHeight * 1.5f;
			positions[i + 1] *= unitHeight * 1.5f;
			//Rotate
			float newx = positions[i] * cos(angle) - positions[i + 1] * sin(angle);
			float newy = positions[i] * sin(angle) + positions[i + 1] * cos(angle);
			positions[i] = newx; positions[i + 1] = newy;
			//Translate to correct position
			positions[i] += intersections[xy_to_loc(x, y) * 2] + unitWidth;
			positions[i + 1] += intersections[xy_to_loc(x, y) * 2 + 1] - unitWidth;
		}
		int posIndex = 0, polyIndex = 0;
		if (ticks == 1) {
			posIndex = 20; polyIndex = 24;
		}
		if (ticks == 2) {
			posIndex = 32; polyIndex = 40;
		}
		if (ticks == 3) {
			posIndex = 44; polyIndex = 56;
		}
		int baseIndex = static_cast<int>(intersectionFlags.size());
		for (int i = 0; i < posIndex; i++) {
			intersections.push_back(positions[i]);
			if (i % 2 == 0) intersectionFlags.push_back(IntersectionFlags::NO_POINT);
		}
		for (int i = 0; i < polyIndex; i++) {
			polygons.push_back(polys[i] + baseIndex);
		}
	}

	int _width;
	int _height;
	int _pillarWidth{0};

	// Because the Panel class integrates so closely with the memory,
	// its grid can be in the game's format, so long as it provides conversions.
	std::vector<std::vector<int>> _grid;
	std::vector<Point> _startpoints;
	std::vector<Endpoint> _endpoints;
	float minx, miny, maxx, maxy, unitWidth, unitHeight;
	int _style;
	bool _resized;
	int id;

	static std::vector<Panel> generatedPanels;
	static std::vector<std::tuple<int, int>> arrowPuzzles;

	friend class PanelExtractionTests;
	friend class Generate;
	friend class PuzzleList;
	friend class AbstractPuzzleList;
	friend class Special;
	friend class MultiGenerate;
	friend class ArrowWatchdog;
	friend class TextureLoader;
};