
#include "Panel.h"

#include "Memory.h"
#include "Randomizer.h"
#include "Special.h"
#include "Watchdog.h"
// #include "TextureMaker.h"
#include <fstream>
#include <sstream>

std::vector<Panel> Panel::generatedPanels;
std::vector<std::tuple<int, int>> Panel::arrowPuzzles;

template <class T>
int find(const std::vector<T> &data, T search, size_t startIndex = 0) {
	for (size_t i=startIndex ; i<data.size(); i++) {
		if (data[i] == search) return static_cast<int>(i);
	}
	return -1;
}

// Default ctor
 Panel::Panel() {
	// Missing inits:
	// Symmetry _symmetry;
	// float pathWidth;
	// ColorMode colorMode;
	// bool decorationsOnly;
	// bool enableFlash;
	// int _width, _height;
	// std::vector<std::vector<Deco::Deco>> _grid;
	// std::vector<Point> _startpoints;
	// std::vector<Endpoint> _endpoints;
	// float minx, miny, maxx, maxy, unitWidth, unitHeight;
	// int _style;
	// bool _resized;
	// int id;
	// static std::vector<Panel> generatedPanels;
	// static std::vector<std::tuple<int, int>> arrowPuzzles;
}

 Panel::Panel(int id) {
	Read(id);
}

// Initialize the panel based on in-game data for the panel ID

 void Panel::Write() {
	Memory* memory = Memory::get();
	memory->WritePanelData<int>(id, GRID_SIZE_X, { (_width + 1) / 2 });
	memory->WritePanelData<int>(id, GRID_SIZE_Y, { (_height + 1) / 2 });
	if (_resized && memory->ReadPanelData<int>(id, NUM_COLORED_REGIONS) > 0) {
		//Make two triangles that cover the whole panel
		std::vector<int> newRegions = { 0, xy_to_loc(_width - 1, 0), xy_to_loc(0, 0), 0, xy_to_loc(_width - 1, _height - 1), xy_to_loc(_width - 1, 0), 0, 0 };
		memory->WritePanelData<int>(id, NUM_COLORED_REGIONS, { static_cast<int>(newRegions.size()) / 4 });
		memory->WriteArray(id, COLORED_REGIONS, newRegions);
	}
	if (!decorationsOnly) WriteIntersections();
	else {
		std::vector<int> iflags = memory->ReadArray<int>(id, DOT_FLAGS, memory->ReadPanelData<int>(id, NUM_DOTS));
		for (int x = 0; x < _width; x += 2) {
			for (int y = 0; y < _height; y += 2) {
				if (_grid[x][y] & Decoration::Dot) {
					iflags[x / 2 + (y / 2) * (_width / 2 + 1)] = _grid[x][y];
					_style |= Style::HAS_DOTS;
				}
			}
		}
		memory->WriteArray<int>(id, DOT_FLAGS, iflags);
	}
	WriteDecorations();
	if (enableFlash) _style &= ~NO_BLINK;
	memory->WritePanelData<int>(id, STYLE_FLAGS, { _style });
	if (pathWidth != 1) memory->WritePanelData<float>(id, PATH_WIDTH_SCALE, { pathWidth });
	memory->WritePanelData<int>(id, NEEDS_REDRAW, { 1 });
	generatedPanels.push_back(*this);
}

#if 0
 void Panel::SetSymbol(int x, int y, Decoration::Shape symbol, Decoration::Color color)
{
	int gridx = x * 2 + (symbol & IntersectionFlags::COLUMN ? 0 : 1);
	int gridy = y * 2 + (symbol & IntersectionFlags::ROW ? 0 : 1);
	if (symbol & IntersectionFlags::DOT) {
		if (color == Decoration::Color::Blue || color == Decoration::Color::Cyan)
			color = static_cast<Decoration::Color>(IntersectionFlags::DOT_IS_BLUE);
		else if (color == Decoration::Color::Orange || color == Decoration::Color::Yellow)
			color = static_cast<Decoration::Color>(IntersectionFlags::DOT_IS_ORANGE);
		else color = Decoration::Color::Any;
		if (symmetry) {
			Point sp = get_sym_point({gridx, gridy});
			SetGridSymbol(sp, static_cast<Decoration::Shape>(symbol & ~Decoration::Dot), Decoration::Color::Any);
		}
	}
	else if (symbol & IntersectionFlags::ROW || symbol & IntersectionFlags::COLUMN)
		color = Decoration::Color::Any;
	SetGridSymbol(gridx, gridy, symbol, color);
}

 void Panel::SetShape(int x, int y, int shape, bool rotate, bool negative, Decoration::Color color)
{
	if (!shape) return;
	int symbol = Decoration::Shape::Poly;
	while (!(shape & 0xf)) shape >>= 4;
	while (!(shape & 0x1111)) shape >>= 1;
	shape <<= 16;
	if (rotate) shape |= Decoration::Shape::Can_Rotate;
	else shape &= ~Decoration::Shape::Can_Rotate;
	if (negative) shape |= Decoration::Shape::Negative;
	else shape &= ~Decoration::Shape::Negative;
	_grid[x * 2 + 1][y * 2 + 1] = symbol | shape | color;
}

 void Panel::ClearSymbol(int x, int y)
{
	ClearGridSymbol(x * 2 + 1, y * 2 + 1);
}
 void Panel::ClearGridSymbol(int x, int y)
{
	_grid[x][y] = 0;
}
#endif

// When placing an Exit on the grid, determine which direction it should face
 void Panel::PlaceExit(const Point& point) {
	int x = point.first;
	int y = point.second;
	Endpoint::Direction dir;

	// Determine direction of the Exit
	// The Usual Suspects
	if (y == 0) {
		// top edge
		dir = Endpoint::Direction::UP;
	} else if (y == _height - 1) {
		// bottom edge
		dir = Endpoint::Direction::DOWN;
	} else if (x == 0) {
		dir = Endpoint::Direction::LEFT;
	} else {
		dir = Endpoint::Direction::RIGHT;
	}

	// Special Cases
	if (id == 0x09DAF) {
		// I assume this is NOT the place for me to put this?
		dir = Endpoint::Direction::UP_RIGHT;
	} else if (id == 0x033D4 || id == 0x0A3B5) {
		// kOutsideTutorialVault
		// kTutorialBackLeft
		if (x == 0) {
			dir = Endpoint::Direction::LEFT;
		} else {
			dir = Endpoint::Direction::RIGHT;
		}
	} else if (_symmetry == Symmetry::ParallelH || _symmetry == Symmetry::ParallelHFlip) {
		if (x == _width - 1) {
			dir = Endpoint::Direction::RIGHT;
		} else if (x == 0) {
			dir = Endpoint::Direction::LEFT;
		}
	}

	// Use the determined direction in symbol creation.
	_endpoints.emplace_back(
		x, y,
		dir,
		IntersectionFlags::ENDPOINT | 
		(dir == Endpoint::Direction::UP || dir == Endpoint::Direction::DOWN ?
			IntersectionFlags::COLUMN : IntersectionFlags::ROW));
};

// Places a symbol at the spot. A bit safer than direct _grid access.
 void Panel::SetGridSymbol(const Point& point, const Deco::Deco& symbol)
{
	int x = point.first;
	int y = point.second;
	_grid[x][y] = symbol;
}

 void Panel::Resize(int width, int height)
{
	for (Point &s : _startpoints) {
		if (s.first == _width - 1) {
			s.first = width - 1;
		}
		if (s.second == _height - 1) {
			s.second = height - 1;
		}
	}
	for (Endpoint &e : _endpoints) {
		if (e.GetX() == _width - 1) {
			e.SetX(width - 1);
		}
		if (e.GetY() == _height - 1) {
			e.SetY(height - 1);
		}
	}
	if (_width != _height || width != height) {
		float maxDim = max(maxx - minx, maxy - miny);
		float unitSize = maxDim / max(width - 1, height - 1);
		minx = 0.5f - unitSize * (width - 1) / 2;
		maxx = 0.5f + unitSize * (width - 1) / 2;
		miny = 0.5f - unitSize * (height - 1) / 2;
		maxy = 0.5f + unitSize * (height - 1) / 2;
	}
	if (_pillarWidth) _pillarWidth = width;
	_width = width;
	_height = height;
	_grid.resize(width);
	for (auto& row : _grid){
		row.resize(height);
	}
	_resized = true;
}

 void Panel::Read() {
	Memory* memory = Memory::get();

	// Read the panel's current size from the game and convert the coordinate system
	_width = 2 * memory->ReadPanelData<int>(id, GRID_SIZE_X) - 1;
	if (memory->ReadPanelData<int>(id, IS_CYLINDER)) {
		_width++;
		_pillarWidth = _width; // TODO: Refactor static pillarWidth
	} else {
		_pillarWidth = 0; // TODO: Refactor static pillarWidth
	}
	_height = 2 * memory->ReadPanelData<int>(id, GRID_SIZE_Y) - 1;
	if (_width <= 0 || _height <= 0 || _width > 30 || _height > 30) {
		// If the stored GRID_SIZE_ values are nonsense, try this fallback:
		int numIntersections = memory->ReadPanelData<int>(id, NUM_DOTS);
		_width = _height = static_cast<int>(std::round(sqrt(numIntersections))) * 2 - 1;
	}

	// Initialize the instance grid
	_grid.resize(_width);
	for (auto&& column : _grid) {
		column.resize(_height);
	}
	for (int x = 0; x < _width; x++) {
		for (int y = 0; y < _height; y++) {
			_grid[x][y] = Deco::kEmpty;
		}
	}
	_startpoints.clear();
	_endpoints.clear();

	_style = memory->ReadPanelData<int>(id, STYLE_FLAGS);
	ReadAllData();
	ReadIntersections();
	ReadDecorations();
	pathWidth = 1;
	_resized = false;
	colorMode = ColorMode::Default;
	decorationsOnly = false;
	enableFlash = false;
}

 void Panel::ReadAllData() {
	Memory* memory = Memory::get();
	Color pathColor = memory->ReadPanelData<Color>(id, PATH_COLOR);
	Color rpathColor = memory->ReadPanelData<Color>(id, REFLECTION_PATH_COLOR);
	Color successColor = memory->ReadPanelData<Color>(id, SUCCESS_COLOR_A);
	Color strobeColor = memory->ReadPanelData<Color>(id, STROBE_COLOR_A);
	Color errorColor = memory->ReadPanelData<Color>(id, ERROR_COLOR);
	int numDecorations = memory->ReadPanelData<int>(id, NUM_DECORATIONS);
	Color a = memory->ReadPanelData<Color>(id, SYMBOL_A);
	Color b = memory->ReadPanelData<Color>(id, SYMBOL_B);
	Color c = memory->ReadPanelData<Color>(id, SYMBOL_C);
	Color d = memory->ReadPanelData<Color>(id, SYMBOL_D);
	Color e = memory->ReadPanelData<Color>(id, SYMBOL_E);
	Color ppColor = memory->ReadPanelData<Color>(id, PATTERN_POINT_COLOR);
	Color ppColorA = memory->ReadPanelData<Color>(id, PATTERN_POINT_COLOR_A);
	Color ppColorB = memory->ReadPanelData<Color>(id, PATTERN_POINT_COLOR_B);
	int pushSymbolColors = memory->ReadPanelData<int>(id, PUSH_SYMBOL_COLORS);
	int numColored = memory->ReadPanelData<int>(id, NUM_COLORED_REGIONS);
	std::vector<int> colored = memory->ReadArray<int>(id, COLORED_REGIONS, numColored * 4);
	int numConnections = memory->ReadPanelData<int>(id, NUM_CONNECTIONS);
	int numDots = memory->ReadPanelData<int>(id, NUM_DOTS);
	int reflectionData = memory->ReadPanelData<int>(id, REFLECTION_DATA);
	std::vector<int> rdata; if (reflectionData) rdata = memory->ReadArray<int>(id, REFLECTION_DATA, numDots);
	int style = memory->ReadPanelData<int>(id, STYLE_FLAGS);
	std::vector<int> connections_a = memory->ReadArray<int>(id, DOT_CONNECTION_A, numConnections);
	std::vector<int> connections_b = memory->ReadArray<int>(id, DOT_CONNECTION_B, numConnections);
	int numIntersections = memory->ReadPanelData<int>(id, NUM_DOTS);
	std::vector<float> intersections = memory->ReadArray<float>(id, DOT_POSITIONS, numIntersections * 2);
	std::vector<int> intersectionFlags = memory->ReadArray<int>(id, DOT_FLAGS, numIntersections);
	std::vector<int> decorations = memory->ReadArray<int>(id, DECORATIONS, numDecorations);
	std::vector<int> decorationFlags = memory->ReadArray<int>(id, DECORATION_FLAGS, numDecorations);
	float width = memory->ReadPanelData<float>(id, PATH_WIDTH_SCALE);
	int seqLen = memory->ReadPanelData<int>(id, SEQUENCE_LEN);
	std::vector<int> seq = memory->ReadArray<int>(id, SEQUENCE, seqLen);
	std::vector<float> power = memory->ReadPanelData<float>(id, POWER, 2);
	float openRate = memory->ReadPanelData<float>(id, OPEN_RATE);
	int cptr = memory->ReadPanelData<int>(id, DECORATION_COLORS);
	std::vector<Color> colors; if (cptr) colors = memory->ReadArray<Color>(id, DECORATION_COLORS, numDecorations);
	Color outerBackground = memory->ReadPanelData<Color>(id, OUTER_BACKGROUND);
	int outerBackgroundMode = memory->ReadPanelData<int>(id, OUTER_BACKGROUND_MODE);
	Color bgRegionColor = memory->ReadPanelData<Color>(id, BACKGROUND_REGION_COLOR);
	short metadata = memory->ReadPanelData<short>(id, METADATA);
	//void* specularTexture = memory->ReadPanelData<void*>(id, SPECULAR_TEXTURE);
	//std::vector<float> data = memory->ReadPanelData<float>(id, SPECULAR_TEXTURE, 1000);
	int dotSeqLen = memory->ReadPanelData<int>(id, DOT_SEQUENCE_LEN);
	std::vector<int> dotSeq = memory->ReadArray<int>(id, DOT_SEQUENCE, dotSeqLen);
	int dotSeqLenR = memory->ReadPanelData<int>(id, DOT_SEQUENCE_LEN_REFLECTION);
	std::vector<int> dotSeqR = memory->ReadArray<int>(id, DOT_SEQUENCE_REFLECTION, dotSeqLenR);
	void* target = memory->ReadPanelData<void*>(id, TARGET);
	void* panelTarget = memory->ReadPanelData<void*>(id, PANEL_TARGET);
	Color cableTarget = memory->ReadPanelData<Color>(id, CABLE_TARGET_2);
	//std::vector<int> targets = memory->ReadArray<int>(id, PANEL_TARGET, 6);
	int isPillar = memory->ReadPanelData<int>(id, IS_CYLINDER);
	int numTraced = memory->ReadPanelData<int>(id, TRACED_EDGES);
	int numSol = memory->ReadPanelData<int>(id, TRACED_EDGES + 4); //Don't know what this number is for yet
	int tracedptr = memory->ReadPanelData<int>(id, TRACED_EDGE_DATA);
	//float solved = memory->ReadPanelData<float>(id, SOLVED);
	float distance = memory->ReadPanelData<float>(id, MAX_BROADCAST_DISTANCE);
	std::vector<SolutionPoint> traced;
	if (tracedptr) {
		traced = memory->ReadArray<SolutionPoint>(id, TRACED_EDGE_DATA, numTraced);
	}
}

 void Panel::ReadIntersections() {
	Memory* memory = Memory::get();
	int numIntersections = memory->ReadPanelData<int>(id, NUM_DOTS);
	std::vector<float> intersections = memory->ReadArray<float>(id, DOT_POSITIONS, numIntersections * 2);
	int num_grid_points = this->get_num_grid_points();
	if (intersections.size() < num_grid_points * 2 - 2) {
		num_grid_points = intersections.size();
	}
	minx = intersections[0]; miny = intersections[1];
	if (num_grid_points * 2 - 2 >= intersections.size()) {
		minx = miny = 0.1f; maxx = maxy = 0.9f;
		_symmetry = Symmetry::None;
		return; //Not an actual grid
	}
	else {
		maxx = intersections[num_grid_points * 2 - 2]; maxy = intersections[num_grid_points * 2 - 1];
	}
	if (minx > maxx) std::swap(minx, maxx);
	if (miny > maxy) std::swap(miny, maxy);
	unitWidth = (maxx - minx) / (_width - 1);
	if (_pillarWidth) unitWidth = 1.0f / _width;
	unitHeight = (maxy - miny) / (_height - 1);
	std::vector<int> intersectionFlags = memory->ReadArray<int>(id, DOT_FLAGS, numIntersections);
	std::vector<int> symmetryData = memory->ReadPanelData<int>(id, REFLECTION_DATA) ? 
		memory->ReadArray<int>(id, REFLECTION_DATA, numIntersections) : std::vector<int>();
	if (symmetryData.size() == 0) _symmetry = Symmetry::None;
	else if (symmetryData[0] == num_grid_points - 1) _symmetry = Symmetry::Rotational;
	else if (symmetryData[0] == _width / 2 && intersections[1] == intersections[3]) _symmetry = Symmetry::Vertical;
	else _symmetry = Symmetry::Horizontal;

	for (int i = 0; i < num_grid_points; i++) {
		int x = static_cast<int>(std::round((intersections[i * 2] - minx) / unitWidth));
		int y = _height - 1 - static_cast<int>(std::round((intersections[i * 2 + 1] - miny) / unitHeight));
		_grid[x][y] = intersectionFlags[i];
		if (intersectionFlags[i] & IntersectionFlags::STARTPOINT) {
			_startpoints.push_back({x, y});
		}
	}

	for (int y = 0; y < _height; y++) {
		for (int x = 0; x < _width; x++) {
			if (x % 2 == y % 2) continue;
			_grid[x][y] = OPEN;
		}
	}
	int numConnections = memory->ReadPanelData<int>(id, NUM_CONNECTIONS);
	std::vector<int> connections_a = memory->ReadArray<int>(id, DOT_CONNECTION_A, numConnections);
	std::vector<int> connections_b = memory->ReadArray<int>(id, DOT_CONNECTION_B, numConnections);
	//Remove non-existent connections
	std::vector<std::string> out;
	for (int i = 0; i < connections_a.size(); i++) {
		out.push_back("(" + std::to_string(connections_a[i]) + ", " + std::to_string(connections_b[i]) + ")");
		if (connections_a[i] >= num_grid_points || connections_b[i] >= num_grid_points) continue;
		int x = static_cast<int>(std::round((intersections[connections_a[i] * 2] - minx) / unitWidth));
		int y = _height - 1 - static_cast<int>(std::round((intersections[connections_a[i] * 2 + 1] - miny) / unitHeight));
		int x2 = static_cast<int>(std::round((intersections[connections_b[i] * 2] - minx) / unitWidth));
		int y2 = _height - 1 - static_cast<int>(std::round((intersections[connections_b[i] * 2 + 1] - miny) / unitHeight));
		_grid[(x + x2) / 2][(y + y2) / 2] = 0;
	}

	// Iterate the remaining intersections (endpoints, dots, gaps)
	for (int i = num_grid_points; i < numIntersections; i++) {
		float xd = (intersections[i * 2] - minx) / unitWidth;
		float yd = (intersections[i * 2 + 1] - miny) / unitHeight;
		int x = std::clamp((int)std::round(xd), 0, _width - 1);
		int y = _height - 1 - std::clamp((int)std::round(yd), 0, _height - 1);
		if (intersectionFlags[i] & IntersectionFlags::GAP) {
			float xd2 = (intersections[i * 2 + 2] - minx) / unitWidth;
			float yd2 = (intersections[i * 2 + 3] - miny) / unitHeight;
			x = std::clamp((int)std::round((xd + xd2) / 2), 0, _width - 1);
			y = _height - 1 - std::clamp((int)std::round((yd + yd2) / 2), 0, _height - 1);
			bool fake = false;
			for (int j = 0; j < numConnections; j++) {
				if (connections_a[j] == i && connections_b[j] == i + 1 ||
					connections_a[j] == i + 1 && connections_b[j] == i) {
					//Fake symmetry wall
					fake = true;
					_grid[x][y] = 0;
					i++;
					break;
				}
			}
			if (fake) continue;
			i++;
		}
		if (intersectionFlags[i] & IntersectionFlags::ENDPOINT) {
			for (int j = 0; j < numConnections; j++) {
				int location = -1;
				if (connections_a[j] == i) location = connections_b[j];
				if (connections_b[j] == i) location = connections_a[j];
				if (location != -1) {
					Endpoint::Direction dir = Endpoint::Direction::NONE;
					if (intersections[2 * i] < intersections[2 * location]) { // Our (i) x coordinate is less than the target's (location)
						dir = (Endpoint::Direction)(dir | Endpoint::Direction::LEFT);
					}
					if (intersections[2 * i] > intersections[2 * location]) {
						dir = (Endpoint::Direction)(dir | Endpoint::Direction::RIGHT);
					}
					if (intersections[2 * i + 1] < intersections[2 * location + 1]) { // y coordinate is 0 (bottom) 1 (top), so this check is reversed.
						dir = (Endpoint::Direction)(dir | Endpoint::Direction::DOWN);
					}
					if (intersections[2 * i + 1] > intersections[2 * location + 1]) {
						dir = (Endpoint::Direction)(dir | Endpoint::Direction::UP);
					}
					xd = (intersections[location * 2] - minx) / unitWidth;
					yd = (intersections[location * 2 + 1] - miny) / unitHeight;
					x = std::clamp((int)std::round(xd), 0, _width - 1);
					y = _height - 1 - std::clamp((int)std::round(yd), 0, _height - 1);
					_endpoints.emplace_back(Endpoint(x, y, dir, intersectionFlags[i]));
					break;
				}
			}
		}

		else {
			_grid[x][y] = intersectionFlags[i];
			//if (intersectionFlags[i] & IntersectionFlags::STARTPOINT) { //Glitchy right now
			//	_startpoints.push_back({ x, y });
			//}
		}
	}	
}

 void Panel::WriteIntersections() {
	Memory* memory = Memory::get();
	std::vector<float> intersections;
	std::vector<int> intersectionFlags;
	std::vector<int> connections_a;
	std::vector<int> connections_b;
	std::vector<int> symmetryData;
	std::vector<int> polygons;

	unitWidth = (maxx - minx) / (_width - 1);
	if (_pillarWidth) unitWidth = 1.0f / _width;
	unitHeight = (maxy - miny) / (_height - 1);

	for (Point p : _startpoints) {
		_grid[p.first][p.second] |= STARTPOINT;
	}

	_style &= ~HAS_DOTS;

	for (int y = _height - 1; y >= 0; y -= 2) {
		for (int x = 0; x <_width; x += 2) {
			intersections.push_back(static_cast<float>(minx + x * unitWidth));
			intersections.push_back(static_cast<float>(miny + (_height - 1 - y) * unitHeight));
			if (_grid[x][y] & IntersectionFlags::NO_POINT) intersectionFlags.push_back(_grid[x][y]);
			else intersectionFlags.push_back(_grid[x][y] | IntersectionFlags::INTERSECTION);
			if (_grid[x][y] & DOT) {
				_style |= HAS_DOTS;
				if (_grid[x][y] & IntersectionFlags::DOT_IS_BLUE || _grid[x][y] & IntersectionFlags::DOT_IS_ORANGE)
					_style |= IS_2COLOR;
			}

			// Create connections for this intersection -- always write low -> high
			if (y > 0 && _grid[x][y - 1] != OPEN) {
				connections_a.push_back(xy_to_loc(x, y - 2));
				connections_b.push_back(xy_to_loc(x, y));
			}
			if (x > 0 && _grid[x - 1][y] != OPEN) {
				connections_a.push_back(xy_to_loc(x - 2, y));
				connections_b.push_back(xy_to_loc(x, y));
			}
			if (_symmetry) {
				symmetryData.push_back(xy_to_loc(get_sym_point({x, y}).first, get_sym_point({x, y}).second));
			}
		}
		if (_pillarWidth) {
			connections_a.push_back(xy_to_loc(_width - 2, y));
			connections_b.push_back(xy_to_loc(0, y));
		}
	}

	std::vector<std::string> out;
	for (int i = 0; i < connections_a.size(); i++) {
		out.push_back(std::to_string(connections_a[i]) + " -> " + std::to_string(connections_b[i]));
	}

	if (_symmetry) {
		//Rearrange exits to be in symmetric pairs
		for (int i = 0; i < _endpoints.size(); i += 2) {
			Point sp = get_sym_point({_endpoints[i].GetX(), _endpoints[i].GetY()});
			for (int j = i + 1; j < _endpoints.size(); j++) {
				if (_endpoints[j].GetX() == sp.first && _endpoints[j].GetY() == sp.second) {
					std::swap(_endpoints[i + 1], _endpoints[j]);
					break;
				}
			}
		}
	}

	double endDist = _pillarWidth == 0 ? 0.05 : 0.03;

	for (int i = 0; i < _endpoints.size(); i++) {
		Endpoint endpoint = _endpoints[i];
		int x = endpoint.GetX(); int y = endpoint.GetY();
		if (x % 2 || y % 2) {
			break_segment(x, y, connections_a, connections_b, intersections, intersectionFlags);
			connections_a.push_back(static_cast<int>(intersectionFlags.size()) - 1); // Target to connect to
		}
		else {
			connections_a.push_back(xy_to_loc(endpoint.GetX(), endpoint.GetY())); // Target to connect to
		}
		connections_b.push_back(static_cast<int>(intersectionFlags.size()));  // This endpoint
		double xPos = minx + endpoint.GetX() * unitWidth;
		double yPos = miny + (_height - 1 - endpoint.GetY()) * unitHeight;
		if (endpoint.GetDir() & Endpoint::Direction::LEFT) {
			xPos -= endDist;
		}
		if (endpoint.GetDir() & Endpoint::Direction::RIGHT) {
			xPos += endDist;
		}
		if (endpoint.GetDir() & Endpoint::Direction::UP) {
			yPos += endDist;
		}
		if (endpoint.GetDir() & Endpoint::Direction::DOWN) {
			yPos -= endDist;
		}
		intersections.push_back(static_cast<float>(xPos));
		intersections.push_back(static_cast<float>(yPos));
		intersectionFlags.push_back(endpoint.GetFlags());
		if (_symmetry) {
			Point sp = get_sym_point({endpoint.GetX(), endpoint.GetY()});
			for (int j = 0; j < _endpoints.size(); j++) {
				if (_endpoints[j].GetX() == sp.first && _endpoints[j].GetY() == sp.second) {
					symmetryData.push_back(get_num_grid_points() + j);
					break;
				}
				if (j == _endpoints.size() - 1) symmetryData.push_back(get_num_grid_points() + i); //No exit matches up with it symmetrically
			}
		}
	}

	// Dots/Gaps
	for (int y = _height - 1; y >= 0; y--) {
		for (int x = 0; x < _width; x++) {
			if (x % 2 == y % 2) continue;
			if (_grid[x][y] == 0 || _grid[x][y] == OPEN) continue;
			if (_grid[x][y] & IntersectionFlags::DOT) {
				_style |= HAS_DOTS;
				if (_grid[x][y] & IntersectionFlags::DOT_IS_BLUE || _grid[x][y] & IntersectionFlags::DOT_IS_ORANGE)
					_style |= IS_2COLOR;
			}
			if (locate_segment(x, y, connections_a, connections_b) == -1)
				continue;
			if (_grid[x][y] & IntersectionFlags::GAP) {
				if (!break_segment_gap(x, y, connections_a, connections_b, intersections, intersectionFlags))
					continue;
				if (_symmetry) {
					auto[sx, sy] = get_sym_point({x, y});
					break_segment_gap(sx, sy, connections_a, connections_b, intersections, intersectionFlags);
					symmetryData.push_back(static_cast<int>(intersectionFlags.size()) - 1);
					symmetryData.push_back(static_cast<int>(intersectionFlags.size()) - 2);
					symmetryData.push_back(static_cast<int>(intersectionFlags.size()) - 3);
					symmetryData.push_back(static_cast<int>(intersectionFlags.size()) - 4);
					if (x % 2 == 0 && get_sym_dir(Endpoint::Direction::UP, _symmetry) == Endpoint::Direction::UP ||
						y % 2 == 0 && get_sym_dir(Endpoint::Direction::LEFT, _symmetry) == Endpoint::Direction::LEFT || _symmetry == Symmetry::FlipXY) {
						std::swap(symmetryData[symmetryData.size() - 1], symmetryData[symmetryData.size() - 2]);
						std::swap(symmetryData[symmetryData.size() - 3], symmetryData[symmetryData.size() - 4]);
					}
				}
			}
			else {
				if (_grid[x][y] == IntersectionFlags::COLUMN || _grid[x][y] == IntersectionFlags::ROW)
					continue;
				if (!break_segment(x, y, connections_a, connections_b, intersections, intersectionFlags))
					continue;
				if (_symmetry) {
					auto[sx, sy] = get_sym_point({x, y});
					if (!break_segment(sx, sy, connections_a, connections_b, intersections, intersectionFlags))
						continue;
					symmetryData.push_back(static_cast<int>(intersectionFlags.size()) - 1);
					symmetryData.push_back(static_cast<int>(intersectionFlags.size()) - 2);
				}
			}
		}
	}

	//Arrows (if applicable)
	for (int y = 1; y < _height; y += 2) {
		for (int x = 1; x < _width; x += 2) {
			if ((_grid[x][y] & 0x700) == Decoration::Arrow)
				render_arrow(x, y, (_grid[x][y] & 0xf000) >> 12, (_grid[x][y] & 0xf0000) >> 16, intersections, intersectionFlags, polygons);
		}
	}

	//Symmetry Data
	if (id == 0x01D3F && _symmetry == Symmetry::None || id == 0x00076 && _symmetry == Symmetry::None) {
		_style &= ~Style::SYMMETRICAL;
		memory->WritePanelData<long long>(id, REFLECTION_DATA, { 0 });
	}
	else if (symmetryData.size() > 0) {
		_style |= Style::SYMMETRICAL;
		memory->WriteArray<int>(id, REFLECTION_DATA, symmetryData);
	}
	else {
		_style &= ~Style::SYMMETRICAL;
		memory->WritePanelData<long long>(id, REFLECTION_DATA, { 0 });
	}

	memory->WritePanelData<int>(id, NUM_DOTS, { static_cast<int>(intersectionFlags.size()) });
	memory->WriteArray<float>(id, DOT_POSITIONS, intersections);
	memory->WriteArray<int>(id, DOT_FLAGS, intersectionFlags);
	memory->WritePanelData<int>(id, NUM_CONNECTIONS, { static_cast<int>(connections_a.size()) });
	memory->WriteArray<int>(id, DOT_CONNECTION_A, connections_a);
	memory->WriteArray<int>(id, DOT_CONNECTION_B, connections_b);
	
	if (polygons.size() > 0) {
		memory->WritePanelData<int>(id, NUM_COLORED_REGIONS, { static_cast<int>(polygons.size()) / 4 });
		memory->WriteArray<int>(id, COLORED_REGIONS, polygons);
	}
}

 void Panel::ReadDecorations() {
	Memory* memory = Memory::get();
	int numDecorations = memory->ReadPanelData<int>(id, NUM_DECORATIONS);
	std::vector<int> decorations = memory->ReadArray<int>(id, DECORATIONS, numDecorations);
	std::vector<int> decorationFlags = memory->ReadArray<int>(id, DECORATION_FLAGS, numDecorations);

	for (int i=0; i<numDecorations; i++) {
		auto [x, y] = dloc_to_xy(i);
		_grid[x][y] = decorations[i];
	}
}

 void Panel::WriteDecorations() {
	Memory* memory = Memory::get();
	std::vector<int> decorations;
	std::vector<Color> decorationColors;
	std::vector<Color> decorationSpecular;
	bool any = false;
	bool arrows = false;
	_style &= ~0x3fc0; //Remove all element flags
	for (int y=_height-2; y>0; y-=2) {
		for (int x=1; x<_width; x+=2) {
			if (colorMode == ColorMode::Treehouse || colorMode == ColorMode::TreehouseAlternate) {
				if ((_grid[x][y] & 0xf) == Decoration::Color::Green) {
					_grid[x][y] &= ~0xf; _grid[x][y] |= 6;
				}
				if ((_grid[x][y] & 0xf) == Decoration::Color::Orange) {
					_grid[x][y] &= ~0xf; _grid[x][y] |= 5;
				}
				if ((_grid[x][y] & 0xf) == Decoration::Color::Magenta) {
					_grid[x][y] &= ~0xf; _grid[x][y] |= 4;
				}
			}
			else if (colorMode == ColorMode::Specular) {
				Color color = get_color_rgb(_grid[x][y] & 0xf);
				if ((_grid[x][y] & 0xf) == Decoration::Color::Red) {
					decorationSpecular.push_back({ 0, 0, 0, 1 });
					decorationColors.push_back(memory->ReadPanelData<Color>(id, BACKGROUND_REGION_COLOR));
					_grid[x][y] &= ~0xf; _grid[x][y] |= 6;
				}
				else {
					decorationSpecular.push_back({ 0, 0, 0, color.g });
					decorationColors.push_back({ color.r, color.r, color.b, 1 });
				}
			}
			else decorationColors.push_back(get_color_rgb(_grid[x][y] & 0xf));
			decorations.push_back(_grid[x][y]);
			if (_grid[x][y])
				any = true;
			if ((_grid[x][y] & 0x700) == Decoration::Shape::Stone) _style |= HAS_STONES;
			if ((_grid[x][y] & 0x700) == Decoration::Shape::Star) _style |= HAS_STARS;
			if ((_grid[x][y] & 0x700) == Decoration::Shape::Poly) _style |= HAS_SHAPERS;
			if ((_grid[x][y] & 0x700) == Decoration::Shape::Eraser) _style |= HAS_ERASERS;
			if ((_grid[x][y] & 0x700) == Decoration::Shape::Triangle) _style |= HAS_TRIANGLES;
			if ((_grid[x][y] & 0x700) == Decoration::Shape::Arrow) {
				_style |= HAS_TRIANGLES | HAS_STONES;
				arrows = true;
			}
		}
	}
	if (arrows) {
		for (int i = 0; i < decorations.size(); i++) {
			if (decorations[i] == 0) decorations[i] = Decoration::Triangle; //To force it to be unsolvable
		}
		memory->WritePanelData<int>(id, OUTER_BACKGROUND_MODE, { 1 });
	}
	if (!any) {
		memory->WritePanelData<int>(id, NUM_DECORATIONS, { 0 });
	}
	else {
		memory->WritePanelData<int>(id, NUM_DECORATIONS, { static_cast<int>(decorations.size()) });
		if (colorMode == ColorMode::WriteColors || colorMode == ColorMode::Treehouse || colorMode == ColorMode::TreehouseAlternate || colorMode == ColorMode::Specular || memory->ReadPanelData<int>(id, DECORATION_COLORS))
			memory->WriteArray<Color>(id, DECORATION_COLORS, decorationColors);
		else if (colorMode == ColorMode::Reset || colorMode == ColorMode::Alternate) {
			memory->WritePanelData<int>(id, PUSH_SYMBOL_COLORS, { colorMode == ColorMode::Reset ? 0 : 1 });
		}
		if (colorMode == ColorMode::Treehouse) {
			memory->WritePanelData<int>(id, PUSH_SYMBOL_COLORS, { 1 });
			memory->WritePanelData<Color>(id, SYMBOL_A, { { 0, 0, 0, 1 } }); //Black
			memory->WritePanelData<Color>(id, SYMBOL_B, { { 1, 1, 1, 1 } }); //White
			memory->WritePanelData<Color>(id, SYMBOL_C, { { 1, 0.5, 0, 1 } }); //Orange
			memory->WritePanelData<Color>(id, SYMBOL_D, { { 1, 0, 1, 1 } }); //Magenta
			memory->WritePanelData<Color>(id, SYMBOL_E, { { 0, 1, 0, 1 } }); //Green
		}
		else if (colorMode == ColorMode::TreehouseAlternate) {
			memory->WritePanelData<int>(id, PUSH_SYMBOL_COLORS, { 1 });
			memory->WritePanelData<Color>(id, SYMBOL_A, { { 0, 0, 0, 1 } }); //Black
			memory->WritePanelData<Color>(id, SYMBOL_B, { { 0, 0, 1, 1 } }); //White->Blue
			memory->WritePanelData<Color>(id, SYMBOL_C, { { 1, 0.5, 0, 1 } }); //Orange
			memory->WritePanelData<Color>(id, SYMBOL_D, { { 1, 0, 1, 1 } }); //Magenta
			memory->WritePanelData<Color>(id, SYMBOL_E, { { 1, 1, 1, 1 } }); //Green->White
		}
		if (colorMode == ColorMode::Specular) {
			//TextureMaker tm(1024, 1024);
			//auto wtxBuffer = tm.generate_color_panel_grid(_grid, id, decorationSpecular, true);
			//memory->LoadTexture(memory->ReadPanelData<uint64_t>(id, SPECULAR_TEXTURE), wtxBuffer);
			memory->WritePanelData<int>(id, SEQUENCE_LEN, 0);
			memory->WritePanelData<void*>(id, SEQUENCE, 0);
			_style |= NO_BLINK;
			memory->WritePanelData<int>(id, PUSH_SYMBOL_COLORS, { 1 });
			memory->WritePanelData<Color>(id, SYMBOL_A, { { 0, 0, 0, 1 } });
			memory->WritePanelData<Color>(id, SYMBOL_C, { { 0, 0, 0, 1 } });
			memory->WritePanelData<Color>(id, SYMBOL_E, { memory->ReadPanelData<Color>(id, BACKGROUND_REGION_COLOR) });
		}
	}
	if (any || memory->ReadPanelData<int>(id, DECORATIONS)) {
		memory->WriteArray<int>(id, DECORATIONS, decorations);
		for (int i = 0; i < decorations.size(); i++) decorations[i] = 0;
		memory->WriteArray<int>(id, DECORATION_FLAGS, decorations);
	}
	if (arrows) {
		arrowPuzzles.emplace_back(id, _pillarWidth);
	}
}

 Endpoint::Direction Panel::get_sym_dir(Endpoint::Direction direction, Symmetry symmetry) const {
		int dirIndex = -1;
		if (direction == Endpoint::Direction::LEFT) dirIndex = 0;
		if (direction == Endpoint::Direction::RIGHT) dirIndex = 1;
		if (direction == Endpoint::Direction::UP) dirIndex = 2;
		if (direction == Endpoint::Direction::DOWN) dirIndex = 3;
		std::vector<Endpoint::Direction> mapping;
		switch (symmetry) {
		case Symmetry::Horizontal: mapping = { Endpoint::Direction::LEFT, Endpoint::Direction::RIGHT, Endpoint::Direction::DOWN, Endpoint::Direction::UP }; break;
		case Symmetry::Vertical: mapping = { Endpoint::Direction::RIGHT, Endpoint::Direction::LEFT, Endpoint::Direction::UP, Endpoint::Direction::DOWN }; break;
		case Symmetry::Rotational: mapping = { Endpoint::Direction::RIGHT, Endpoint::Direction::LEFT, Endpoint::Direction::DOWN, Endpoint::Direction::UP }; break;
		case Symmetry::RotateLeft: mapping = { Endpoint::Direction::DOWN, Endpoint::Direction::UP, Endpoint::Direction::LEFT, Endpoint::Direction::RIGHT }; break;
		case Symmetry::RotateRight: mapping = { Endpoint::Direction::UP, Endpoint::Direction::DOWN, Endpoint::Direction::RIGHT, Endpoint::Direction::LEFT }; break;
		case Symmetry::FlipXY: mapping = { Endpoint::Direction::UP, Endpoint::Direction::DOWN, Endpoint::Direction::LEFT, Endpoint::Direction::RIGHT }; break;
		case Symmetry::FlipNegXY: mapping = { Endpoint::Direction::DOWN, Endpoint::Direction::UP, Endpoint::Direction::RIGHT, Endpoint::Direction::LEFT }; break;
		case Symmetry::ParallelH: mapping = { Endpoint::Direction::LEFT, Endpoint::Direction::RIGHT, Endpoint::Direction::UP, Endpoint::Direction::DOWN }; break;
		case Symmetry::ParallelV: mapping = { Endpoint::Direction::LEFT, Endpoint::Direction::RIGHT, Endpoint::Direction::UP, Endpoint::Direction::DOWN }; break;
		case Symmetry::ParallelHFlip: mapping = { Endpoint::Direction::RIGHT, Endpoint::Direction::LEFT, Endpoint::Direction::UP, Endpoint::Direction::DOWN }; break;
		case Symmetry::ParallelVFlip: mapping = { Endpoint::Direction::LEFT, Endpoint::Direction::RIGHT, Endpoint::Direction::DOWN, Endpoint::Direction::UP }; break;
		case Symmetry::PillarParallel: [[fallthrough]];
		case Symmetry::PillarHorizontal: [[fallthrough]];
		case Symmetry::PillarVertical: [[fallthrough]];
		case Symmetry::PillarRotational: [[fallthrough]];
		default: mapping = { Endpoint::Direction::LEFT, Endpoint::Direction::RIGHT, Endpoint::Direction::UP, Endpoint::Direction::DOWN }; break;
		}
		return mapping[dirIndex];
	}

 Color Panel::get_color_rgb(int color) const {
		if (colorMode == ColorMode::Treehouse) {
			switch (color) {
			case 1: return { 0, 0, 0, 1 }; //Black
			case 2: return { 1, 1, 1, 1 }; //White
			case 3: return { 1, 0, 0, 1 }; //Red (Not used)
			case 4: return { 1, 0, 1, 1 }; //Magenta
			case 5: return { 1, 0.5, 0, 1 }; //Orange
			case 6: return { 0, 1, 0, 1 }; //Green
			default: return { 0, 0, 0, 0 };
			}
		}
		else if (colorMode == ColorMode::TreehouseAlternate) {
			switch (color) {
			case 1: return { 0, 0, 0, 1 }; //Black
			case 2: return { 0, 0, 1, 1 }; //White->Blue
			case 3: return { 1, 0, 0, 1 }; //Red (Not used)
			case 4: return { 1, 0, 1, 1 }; //Magenta
			case 5: return { 1, 0.5, 0, 1 }; //Orange
			case 6: return { 1, 1, 1, 1 }; //Green->White
			default: return { 0, 0, 0, 0 };
			}
		}
		switch (color) {
		case Decoration::Color::Black: return { 0, 0, 0, 1 };
		case Decoration::Color::White: return { 1, 1, 1, 1 };
		case Decoration::Color::Red: return { 1, 0, 0, 1 };
		case Decoration::Color::Green: return { 0, 1, 0, 1 };
		case Decoration::Color::Blue: return { 0, 0, 1, 1 };
		case Decoration::Color::Cyan: return { 0, 1, 1, 1 };
		case Decoration::Color::Magenta: return { 1, 0, 1, 1 };
		case Decoration::Color::Yellow: return { 1, 1, 0, 1 };
		case Decoration::Color::Orange: return { 1, 0.5, 0, 1 };
		case Decoration::Color::Purple: return { 0.5, 0, 1, 1 };
		case Decoration::Color::X: {
			Color xColor = GetBackgroundColor();
			xColor.a = 1;
			return xColor;
		}
		default: return { 0, 0, 0, 0 };
		}
	}

 void Panel::StartArrowWatchdogs(const std::map<int, int>& shuffleMappings) {
	std::map<int, int> invertedMappings;
	for (const auto& [from, to] : shuffleMappings) {
		invertedMappings[to] = from;
	}
	for (const auto& [id, pillarWidth] : arrowPuzzles) {
		int realId = id;
		if (invertedMappings.count(realId)) realId = invertedMappings.at(realId);

		ArrowWatchdog* watchdog = new ArrowWatchdog(realId, pillarWidth);
		watchdog->start();
	}
}


// Helper function for WriteIntersections
 bool Panel::break_segment_gap(int x, int y, std::vector<int>& connections_a, std::vector<int>& connections_b, std::vector<float>& intersections, std::vector<int>& intersectionFlags) {
		int i = locate_segment(x, y, connections_a, connections_b);
		if (i == -1) {
			return false;
		}
		int other_connection = connections_b[i];
		connections_b[i] = static_cast<int>(intersectionFlags.size() + 1);
		connections_a.push_back(other_connection);
		connections_b.push_back(static_cast<int>(intersectionFlags.size()));
		if (_grid[x][y] | static_cast<int>(Deco::Symbol::Gap)) { //kludge
			// If there isn't already a Gap at the location, place one there.
			_grid[x][y] = (x % 2 == 0 ? Deco::Gap(Deco::Location::Column) : Deco::Gap(Deco::Location::Row));
			connections_a.push_back(static_cast<int>(intersectionFlags.size()));
			connections_b.push_back(static_cast<int>(intersectionFlags.size() + 1));
		}
		double xOffset = _grid[x][y] & IntersectionFlags::ROW ? 0.5 : 0;
		double yOffset = _grid[x][y] & IntersectionFlags::COLUMN ? 0.5 : 0;
		intersections.push_back(static_cast<float>(minx + (x + xOffset) * unitWidth));
		intersections.push_back(static_cast<float>(miny + (_height - 1 - y - yOffset) * unitHeight));
		intersections.push_back(static_cast<float>(minx + (x - xOffset) * unitWidth));
		intersections.push_back(static_cast<float>(miny + (_height - 1 - y + yOffset) * unitHeight));
		intersectionFlags.push_back(_grid[x][y]);
		intersectionFlags.push_back(_grid[x][y]);
		return true;
	}
