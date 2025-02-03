#ifndef SIGMA_RANDO_DECORATION_H
#define SIGMA_RANDO_DECORATION_H

#include <stdint.h>

#include <stdexcept>
#include <utility>  // Defines pair

namespace Deco {
enum class Symbol {
	// Basic symbol shapes
	Exit = 0x600001,  // Directionality will be handled by Panel
	Start = 0x600002,
	Open = 0x3,  // Puzzle loader flag - not to be written out
	Path = 0x4,  // Generator use only
	Stone = 0x100,
	Star = 0x200,
	Poly = 0x400,
	Eraser = 0x500,
	Triangle = 0x600,
	Arrow = 0x700,
	Gap = 0x100000,
	Dot = 0x20,
	Empty = 0xA00,
};
// Open vs. Gap: Logically equivalent, but stylistically distinct
// Gap:     Open:
// ╔═╦═╦═╗  ╔═╦═╦═╗
// ╠═╬ ╬═╣  ╠═╣ ╠═╣
// ╚═╩═╩═╝  ╚═╩═╩═╝

enum class Color {
	Any = 0,
	Black = 0x1,
	White = 0x2,
	Red = 0x3,  // Doesn't work sadly
	Purple = 0x4,
	Green = 0x5,
	Cyan = 0x6,
	Magenta = 0x7,
	Yellow = 0x8,
	Blue = 0x9,
	Orange = 0xA,
	X = 0xF,
};
enum class Location {
	// Used for Gaps and Dots
	Any,
	Row = 0x200000,
	Column = 0x400000,
	Intersection = 0x600000,
};
enum class Parity {
	// Just for symmetry dots. Which path must go through the dot.
	Any = 0,
	Blue = 0x100,
	Cyan,  // alias for convenience
	Orange = 0x200,
	Yellow,  // alias for convenience
};
enum class Size {
	// Just for sound dots
	Undef = 0,
	Small = 0x2000,
	Medium = 0x4000,
	Large = 0x8000,
};

// Compositional Decoration class to cover most cases
struct Deco {
	// Constraints upon how this symbol can be generated:
	// These may not change over the lifetime of the symbol, but may be read
	// externally.
	const Symbol symbol;  // Basic type of symbol
	Color color;          // Used in Stone, Star, Poly, Eraser, Triangle, Arrow

	// explicit for the Symbol-only case
	constexpr explicit Deco(Symbol symbol, Color color = Color::Any)
		// Initialize ALL the things!
		: symbol{symbol}, color{color} {};
	virtual ~Deco() = default;

	// Making these explicit apparenlty broke DecoPair initializer lists,
	// but this smells of slicing, and I suspect we'll lose data.
	constexpr Deco(const Deco& other)
		: symbol{other.symbol}, color{other.color} {};
	constexpr Deco(Deco&& other)
		: symbol{other.symbol}, color{other.color} {};

	// copy assignment
	virtual Deco& operator=(const Deco& other) {
		*this = other;
		return *this;
	};
	// move assignment?
	virtual Deco& operator=(Deco&& other) {
		*this = other;
		return *this;
	};

	virtual bool operator==(const Deco& other) const {
		return symbol == other.symbol && color == other.color;
	}

	// Canonical conversion of the decorator to the in-game int repr
	// Used for placement into the Panel::_grid
	virtual operator int() const {
		int out = static_cast<int>(symbol) | static_cast<int>(color);
		return out;
	};
};

//////////////////////////////////////////////
// convenience classes and const prototypes //
//////////////////////////////////////////////

// Constant prototypes. No need to construct more.
constinit const auto kEmpty = Deco{Symbol::Empty, Color::Any};
constinit const auto kStart = Deco{Symbol::Start, Color::Any};
constinit const auto kExit =
	Deco{Symbol::Exit, Color::Any};  // This may need a special function in the
									 // generator to force a direction
constinit const auto kPath = Deco{Symbol::Path, Color::Any};
constinit const auto kOpen = Deco{Symbol::Open, Color::Any};

// Convenience constructor
struct Stone : public Deco {
	constexpr explicit Stone(Color color) : Deco{Symbol::Stone, color} {}
};

// Convenience constructor
struct Star : public Deco {
	constexpr explicit Star(Color color) : Deco{Symbol::Star, color} {}
};

// Convenience constructor
struct Eraser : public Deco {
	constexpr explicit Eraser(Color color) : Deco{Symbol::Eraser, color} {}
};

//////////////////////////////////
// extensions to the base class //
//////////////////////////////////
// adds Location:
// Are there constraints on where this may be placed?
struct Gap : public Deco {
	Location location;
	constexpr explicit Gap(Location location = Location::Any)
		: Deco{Symbol::Gap, Color::Any}, location{location} {}

	virtual Gap& operator=(const Gap& other) {
		*this = other;
		return *this;
	};
	virtual Gap& operator=(const Deco& other) override {
		if (const auto& aOther = dynamic_cast<const Gap&>(other)) {
			*this = aOther;
		}
		return *this;
	};

	virtual operator int() const override {
		int out = static_cast<int>(symbol) | static_cast<int>(location);
		return out;
	};
};

struct Poly : public Deco {
	bool can_rotate;
	bool negative;

	constexpr Poly(Color color = Color::Any, bool can_rotate = false,
				   bool negative = false)
		: Deco{Symbol::Poly, color},
		  can_rotate{can_rotate},
		  negative{negative} {}

	virtual Poly& operator=(const Poly& other) {
		*this = other;
		return *this;
	};
	virtual Poly& operator=(const Deco& other) override {
		if (const auto& aOther = dynamic_cast<const Poly&>(other)) {
			*this = aOther;
		}
		return *this;
	};

	virtual operator int() const override {
		int out = static_cast<int>(symbol) | static_cast<int>(color) |
				  (can_rotate ? 0x1000u : 0u ) | (negative ? 0x2000u : 0u);
		return out;
	};
};

// Convenience constructors so you don't have to list both bools each time
// The members are still mutable, so if they have to be modified, it's ok.
struct SpinPoly : public Poly {
	constexpr explicit SpinPoly(Color color = Color::Any, bool negative = false)
		: Poly{color, true, negative} {}
};
struct NegaPoly : public Poly {
	constexpr explicit NegaPoly(Color color = Color::Any,
								bool can_rotate = false)
		: Poly{color, can_rotate, true} {}
};

struct Triangle : public Deco {
	int quantity;  // 0 means Any

	// Constructor:
	// Even though most Triangles are orange, let's be clear in the generators.
	// Currently, only 12/236 Triangle placements specify the number of sides
	// ahead of time, so quantity is defaulted to 0
	constexpr explicit Triangle(Color color, int quantity = 0)
		: Deco(Symbol::Triangle), quantity{quantity} {
		if (quantity > kMax) {
			throw std::exception(
				"Error: quantity of plural symbol set higher than any "
				"implementation!");
		}
	}

	virtual Triangle& operator=(const Triangle& other) {
		*this = other;
		return *this;
	};
	virtual Triangle& operator=(const Deco& other) override {
		if (const auto& aOther = dynamic_cast<const Triangle&>(other)) {
			*this = aOther;
		}
		return *this;
	};
	virtual operator int() const override {
		int out = static_cast<int>(symbol) | static_cast<int>(color) |
				  (quantity << kShift);
		return out;
	};

   private:
	static const int kMax = 4;
	static const int kShift = 16;
};

struct Arrow : public Deco {
	enum class Direction {
		// This must be kept in LOCKSTEP with Generate.cpp's _8DIRECTIONS2
		// vector
		Up = 0,
		Down,
		Right,
		Left,
		UpRight = 4,
		UpLeft,
		DownLeft,
		DownRight,
		TBD = 8,
	};
	int quantity;
	Direction direction;

	constexpr explicit Arrow(Color color = Color::Any, int quantity = 0,
							 Direction direction = Direction::TBD)
		: Deco(Symbol::Arrow), quantity{quantity}, direction{direction} {
		if (quantity > kMax) {
			throw std::exception(
				"Error: quantity of plural symbol set higher than any "
				"implementation!");
		}
	}

	virtual Arrow& operator=(const Arrow& other) {
		*this = other;
		return *this;
	};
	virtual Arrow& operator=(const Deco& other) override {
		if (const auto& aOther = dynamic_cast<const Arrow&>(other)) {
			*this = aOther;
		}
		return *this;
	}
	virtual operator int() const override {
		int out = static_cast<int>(symbol) | static_cast<int>(color) |
				  (quantity << kShift) | (static_cast<int>(direction) << 16);
		return out;
	};

   private:
	static const int kMax = 3;
	static const int kShift = 12;
};

//
struct Dot : public Deco {
	Location location = Location::Intersection;
	Parity parity = Parity::Any;
	Size size = Size::Undef;
	bool invisible = false;

	constexpr Dot()
		: Deco{Symbol::Dot, Color::Any} {
			  // Standard dot is plain, on an intersection.
		  };

	// Allow construction like Deco::Dot(Deco::Parity::Cyan) for legibility.
	constexpr explicit Dot(Parity parity)
		: Deco{Symbol::Dot, Color::Any}, parity{parity} {
		standardize_color();
	};

	constexpr explicit Dot(Location location, Parity parity = Parity::Any,
						   bool invisible = false, Size size = Size::Undef)
		: Deco{Symbol::Dot, Color::Any},
		  location{location},
		  parity{parity},
		  invisible{invisible},
		  size{size} {
		standardize_color();
	}

	// short-term kludge
	Dot& operator|(int modifier) {
		if (modifier == 0x8) {
			// Decoration::Color::Yellow
			parity = Parity::Orange;
			color = Color::Yellow;
		} else if (modifier == 0x9) {
			// Decoration::Color::Blue
			parity = Parity::Blue;
			color = Color::Blue;
		}
		return *this;
	};

	virtual Dot& operator=(const Dot& other) {
		*this = other;
		return *this;
	};
	virtual Dot& operator=(const Deco& other) override {
		if (const auto& aOther = dynamic_cast<const Dot&>(other)) {
			*this = aOther;
		}
		return *this;
	};

	virtual operator int() const override {
		int out = static_cast<int>(symbol) | static_cast<int>(color) |
				  static_cast<int>(location) | static_cast<int>(parity) |
				  (invisible * 0x1000) | static_cast<int>(size);
		return out;
	};

   private:
	constexpr void standardize_color() {
		switch (parity) {
			case Parity::Any:
				color = Color::Any;
				break;
			case Parity::Blue:
				color = Color::Blue;
				break;
			case Parity::Cyan:
				color = Color::Cyan;
				parity = Parity::Blue;
				break;
			case Parity::Orange:
				color = Color::Orange;
				break;
			case Parity::Yellow:
				color = Color::Yellow;
				parity = Parity::Orange;
				break;
			default:
				throw std::range_error(
					"Unsupported parity designator supplied.");
		}
	};
};

inline const Deco& to_deco(int rawSymbol) {
	if (rawSymbol == 0x600001) {
		return kExit;
	} else if (rawSymbol == 0x600002) {
		return kStart;
	}
};

};  // namespace Deco

// A lot of places need a defined symbol and a quantity of it.
using DecoPair = std::pair<Deco::Deco, int>;

#endif // SIGMA_RANDO_DECORATION_H