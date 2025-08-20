// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "Generate.h"

#include "MultiGenerate.h"
#include "Randomizer.h"
#include "Special.h"
#include "Utilities.h"

/// @brief Generate puzzle with multiple solutions.
/// The actual implementation.
/// @param id id of the puzzle
/// @param symbolVec pairs of symbols and amounts to use
/// @param splitStones flag to indicate the stones should be spread farther
/// than usual
void Generate::generateMulti(int id,
					const std::vector<std::shared_ptr<Generate>>& gens,
					const std::vector<DecoPair>& symbolVec,
					bool splitStones) {
	// Simple, but not inlined because of the header dependency.
	MultiGenerate gen(splitStones);
	gen.generate(id, gens, symbolVec);
	incrementProgress();
}

//Make a maze puzzle. The maze will have one solution. id - id of the puzzle
void Generate::generateMaze(int id) {
	while (!generate_maze(id, 0, 0));
}

//Make a maze puzzle. The maze will have one solution. id - id of the puzzle. numStarts - how many starts to add (only one will be valid). numExits - how many exits to add. All will work
//Setting numStarts or numExits to 0 will keep the starts/exits where they originally were, otherwise the starts/exits originally there will be removed and new ones randomly placed.
void Generate::generateMaze(int id, int numStarts, int numExits)
{
	while (!generate_maze(id, numStarts, numExits));
}

//Read in default panel data, such as dimensions, symmetry, starts/exits, etc. id - id of the puzzle
void Generate::initPanel(int id) {
	if (_panel == nullptr) {
		_panel = std::make_shared<Panel>(id);
	}
	if (_width > 0 && _height > 0 && (_width != _panel->width || _height != _panel->height)) {
		_panel->Resize(_panel->_pillarWidth ? _width - 1 : _width, _height); // Bad: Polling some uncontrolled static
	}
	if (hasFlag(Config::FixBackground)) {
		_panel->Resize(_panel->width, _panel->height); //This will force the panel to have to redraw the background
	}
	if (hasFlag(Config::TreehouseLayout)) {
		init_treehouse_layout();
	}
	if (_custom_grid.size() > 0) { //If we want to start with a certain default grid when generating
		if (_custom_grid.size() < _panel->_width) {
			_custom_grid.resize(_panel->_width, std::vector<int>());
		}
		if (_custom_grid[_custom_grid.size() - 1].size() < _panel->height) {
			for (auto& row : _custom_grid) {
				row.resize(_panel->height, Deco::kEmpty);
			}
		}
		if (hasFlag(Config::PreserveStructure)) {
			for (unsigned int x = 0; x < _panel->width; ++x) {
				for (unsigned int y = 0; y < _panel->height; ++y) {
					if (_panel->grid[x][y] == Deco::Deco(Deco::Symbol::Open) ||
						(_panel->grid[x][y] & 0x60000f) == NO_POINT ||
						 _panel->grid[x][y] == Deco::kEmpty)
					{
						_custom_grid[x][y] = _panel->grid[x][y];
					}
				}
			}
		}
		_panel->_grid = _custom_grid;
	}
	//Sync up start/exit points between panel and generator. If both are different, the generator's start/exit point list will be used
	if (_starts.empty()) {
		_starts = std::set<Point>(_panel->_startpoints.begin(), _panel->_startpoints.end());
	} else {
		_panel->_startpoints = std::vector<Point>(_starts.begin(), _starts.end()); // Why does the Generate use a set, but Panel uses a vector?
	}
	if (_exits.empty()) {
		for (const Endpoint& endpoint : _panel->_endpoints) {
			_exits.emplace(endpoint.x, endpoint.y);
		}
	} else {
		_panel->_endpoints.clear();
		for (const Point& exit : _exits) {
			_panel->PlaceExit(exit);
		}
	}
	//Fill gridpos with every available grid block
	_gridpos.clear();
	for (int x = 1; x < _panel->width; x += 2) {
		for (int y = 1; y < _panel->height; y += 2) {
			if (!(hasFlag(Config::PreserveStructure) && get(x, y) == Deco::kEmpty))
				_gridpos.emplace(Point(x, y));
		}
	}
	//Init the open positions available for symbols. Defaults to every grid block unless a custom openpos has been specified
	if (openPos.size() > 0) _openpos = openPos;
	else _openpos = _gridpos;
	for (Point p : blockPos) _openpos.erase(p); //Remove the points which the user has defined to not place symbols on
	for (Point p : _splitPoints) _openpos.erase(p); //The split points will have erasers and cannot have any other symbols placed on them
	// _fullGaps = hasFlag(Config::FullGaps); // Just poll the config directly.
	if (_symmetry || id == 0x00076 || id == 0x01D3F) _panel->_symmetry = _symmetry; //Init user-defined puzzle symmetry if not "None".
	//0x00076 (Symmetry Island Fading Lines 7) and 0x01D3F (Keep Blue Pressure Plates) are exceptions because they need to have symmetry removed
	if (pathWidth != 1) _panel->pathWidth = pathWidth; //Init path scale. "1" is considered the default, and therefore means no change.
}

//Place a specific symbol into the puzzle at the specified location. The generator will add other symbols, but will leave the set ones where they are.
//symbol - the symbol to place. //x, y - the coordinates to put it at. (0, 0) is at top left. Lines are at even coordinates and grid blocks at odd coordinates
void Generate::setSymbol(const Deco::Deco& symbol, const Point& point) {
	int x = point.first;
	int y = point.second;
	if (_custom_grid.size() < x + 1) {
		_custom_grid.resize(x + 1, std::vector<int>());
		for (auto& row : _custom_grid) {
			row.resize(_custom_grid[0].size(), Deco::kEmpty);
		}
	}
	for (auto& row : _custom_grid) {
		if (row.size() < y + 1) {
			row.resize(y + 1, Deco::kEmpty);
		}
	}

	//Starts and exits are not set into the grid
	if (symbol == Deco::kStart) {
		_starts.emplace(point);
	} else if (symbol.symbol == Deco::Symbol::Exit) {
		_exits.emplace(point);
	} else {
		_custom_grid[x][y] = symbol;
	}
}

//Set the dimensions of the puzzles. This setting will persist between puzzle generation calls. (0, 0) will have the generator use the same dimensions as the orignal puzzle.
//width, height - the dimensions to use, measured in grid blocks.
void Generate::setGridSize(int width, int height) {
	if (width <= 0 || height <= 0) {
		_width = 0; _height = 0;
	} else {
		_width = width * 2 + 1; _height = height * 2 + 1;
	}
}

// Set the type of symmetry to use. This setting will persist between puzzle generation calls.
// Using "None" will make the generator use the existing puzzle symmetry.
void Generate::setSymmetry(Panel::Symmetry symmetry) {
	using enum Panel::Symmetry;
	_symmetry = symmetry;
	if (_symmetry == ParallelV || _symmetry == ParallelVFlip) {
		std::vector<Point> points;
		for (int y = 0; y < _height; y += 2) {
			points.emplace_back(_width / 2, y);
		}
		setObstructions(points); //This prevents the generator from invalidly passing through the center line
	}
	if (_symmetry == ParallelH || _symmetry == ParallelHFlip) {
		std::vector<Point> points;
		for (int x = 0; x < _width; x += 2) {
			points.emplace_back(x, _height / 2);
		}
		setObstructions(points); //This prevents the generator from invalidly passing through the center line
	}
}

//Write out panel data to the puzzle with the given id
void Generate::write(int id)
{
	std::vector<std::vector<int>> backupGrid;
	if (hasFlag(Config::DisableReset)) {
		//Allows panel data to be preserved after writing. Normally writing erases the panel data.
		backupGrid = _panel->grid; // TODO: encapsulate _grid better as a private member
	}

	erase_path();

	incrementProgress();

	Memory* memory = Memory::get();

	if (hasFlag(Config::ResetColors)) {
		_panel->colorMode = Panel::ColorMode::Reset;
	} else if (hasFlag(Config::AlternateColors)) {
		_panel->colorMode = Panel::ColorMode::Alternate;
	} else if (hasFlag(Config::WriteColors)) {
		_panel->colorMode = Panel::ColorMode::WriteColors;
	} else if (hasFlag(Config::TreehouseColors)) {
		_panel->colorMode = colorblind ? Panel::ColorMode::TreehouseAlternate : Panel::ColorMode::Treehouse;
	} else if (hasFlag(Config::WriteSpecular)) {
		_panel->colorMode = Panel::ColorMode::Specular;
	}

	if (_panel->_symmetry != Panel::Symmetry::None) {
		memory->WritePanelData(id, SUCCESS_COLOR_B, memory->ReadPanelData<Color>(id, SUCCESS_COLOR_A));
	}

	if (hasFlag(Config::Write2Color)) {
		memory->WritePanelData(id, PATTERN_POINT_COLOR_A, memory->ReadPanelData<Color>(0x0007C, PATTERN_POINT_COLOR_A));
		memory->WritePanelData(id, PATTERN_POINT_COLOR_B, memory->ReadPanelData<Color>(0x0007C, PATTERN_POINT_COLOR_B));
		memory->WritePanelData(id, REFLECTION_PATH_COLOR, memory->ReadPanelData<Color>(0x0007C, PATTERN_POINT_COLOR_B));
		memory->WritePanelData(id, ACTIVE_COLOR, memory->ReadPanelData<Color>(0x0007C, PATTERN_POINT_COLOR_A));
	}
	if (hasFlag(Config::WriteInvisible)) {
		memory->WritePanelData(id, REFLECTION_PATH_COLOR, memory->ReadPanelData<Color>(0x00076, REFLECTION_PATH_COLOR));
	}
	if (hasFlag(Config::WriteDotColor))
		memory->WritePanelData<float>(id, PATTERN_POINT_COLOR, { 0.1f, 0.1f, 0.1f, 1 });
	if (hasFlag(Config::WriteDotColor2)) {
		Color color = memory->ReadPanelData<Color>(id, SUCCESS_COLOR_A);
		memory->WritePanelData(id, PATTERN_POINT_COLOR, color);
	}
	if (arrowColor.a > 0 || backgroundColor.a > 0 || successColor.a > 0) {
		memory->WritePanelData<Color>(id, OUTER_BACKGROUND, { backgroundColor });
		if (arrowColor.a == 0)
			memory->WritePanelData<Color>(id, BACKGROUND_REGION_COLOR, { memory->ReadPanelData<Color>(id, SUCCESS_COLOR_A) });
		memory->WritePanelData<Color>(id, BACKGROUND_REGION_COLOR, { arrowColor });
		memory->WritePanelData(id, OUTER_BACKGROUND_MODE, 1);
		if (successColor.a == 0) memory->WritePanelData(id, SUCCESS_COLOR_A, memory->ReadPanelData<Color>(id, BACKGROUND_REGION_COLOR));
		else memory->WritePanelData(id, SUCCESS_COLOR_A, successColor);
		memory->WritePanelData<Color>(id, SUCCESS_COLOR_B, memory->ReadPanelData<Color>(id, SUCCESS_COLOR_A));
		memory->WritePanelData<float>(id, ACTIVE_COLOR, { 1, 1, 1, 1 });
		memory->WritePanelData<float>(id, REFLECTION_PATH_COLOR, { 1, 1, 1, 1 });
	}
	if (hasFlag(Config::TreehouseLayout)) {
		memory->WritePanelData(id, SPECULAR_ADD, 0.001f);
	}

	_panel->decorationsOnly = hasFlag(Config::DecorationsOnly);
	_panel->enableFlash = hasFlag(Config::EnableFlash);
	_panel->Write(id);
	
	if (hasFlag(Config::DisableReset)) {
		_panel->_grid = backupGrid;
	} else {
		//Resets the generator data such as openpos, custom grids, etc. that doesn't persist across puzzles
		resetVars();
	}
	//Undo any one-time config changes
	if (_oneTimeAdd) {
		_config &= ~_oneTimeAdd;
		_oneTimeAdd = 0;
	}
	if (_oneTimeRemove) {
		_config |= _oneTimeRemove;
		_oneTimeRemove = 0;
	}
	//Manually advance seed by 1 each generation to prevent seeds "funneling" from repeated fails
	Random::seed(_seed);
	_seed = Random::rand();
}

//Reset all config flags and persistent settings, including width/height and symmetry.
void Generate::resetConfig()
{
	setGridSize(0, 0);
	_symmetry = Panel::Symmetry::None;
	pathWidth = 1;
	if (hasFlag(Config::DisableReset)) {
		resetVars();
	}
	_config = 0;
	_oneTimeAdd = Config::None;
	_oneTimeRemove = Config::None;
	arrowColor = backgroundColor = successColor = { 0, 0, 0, 0 };
}

//Increment the counter on the progress indicator. This is called each time a puzzle is written, but may be called manually in other situations
void Generate::incrementProgress()
{
	++_areaTotal;
	++_genTotal;
	if (_handle)
	{
		int total = (_totalPuzzles == 0 ? _areaPuzzles : _totalPuzzles);
		if (total == 0)
		{
			return;
		}
		std::wstring text = _areaName + L": " + std::to_wstring(_areaTotal) + L"/" + std::to_wstring(_areaPuzzles) + L" (" + std::to_wstring(_genTotal * 100 / total) + L"%)";
		SetWindowText(_handle, text.c_str()); // Windows API call to the progress bar
	}
}

//----------------------Private--------------------------

//Add the point (pos) to the intended solution path, using symmetry if applicable.
void Generate::set_path(Point pos)
{
	_panel->_grid[pos.first][pos.second] = PATH;
	_path.insert(pos);
	if (_panel->_symmetry) {
		_path1.insert(pos);
		Point sp = get_sym_point(pos);
		_panel->_grid[sp.first][sp.second] = PATH;
		_path.insert(sp);
		_path2.insert(sp);
	}
}

//Remove the path and all symbols from the grid. This does not affect starts/exits. If PreserveStructure is active, open gaps will be kept. If a custom grid is set, this will reset it back to the custom grid state.
void Generate::clear()
{
	if (_custom_grid.size() > 0) {
		_panel->_grid = _custom_grid;
	}
	else for (int x = 0; x < _panel->width; x++) {
		for (int y = 0; y < _panel->height; y++) {
			if (hasFlag(Config::PreserveStructure) &&
				(_panel->_grid[x][y] == OPEN ||
				 (_panel->_grid[x][y] & 0x60000f) == NO_POINT ||
				 (_panel->_grid[x][y] == 0)))
			{
				continue;
			}
			_panel->_grid[x][y] = 0;
		}
	}
	_panel->_style &= ~0x2ff8; //Remove all element flags
	_path.clear(); _path1.clear(); _path2.clear();
}

//Reset generator variables and lists used when generating puzzles. (not config settings)
void Generate::resetVars() {
	_panel = nullptr; //This is needed for the generator to read in the next panel
	_starts.clear();
	_exits.clear();
	_custom_grid.clear();
	hitPoints.clear();
	_obstructions.clear();
	openPos.clear();
	blockPos.clear();
	_splitPoints.clear();
}

//Place start and exits in central positions like in the treehouse
// ╔═╦╩╦═╗
// ╠═╬═╬═╣
// ╠═╬═╬═╣
// ╚═╩╦╩═╝
void Generate::init_treehouse_layout()
{
	bool pivot = _panel->_endpoints.size() > 2;
	setSymbol(Deco::kStart, _panel->width / 2, _panel->height - 1);
	setSymbol(Deco::kExit, _panel->width / 2, 0);
	if (pivot) {
		setSymbol(Deco::kExit, _panel->width - 1, _panel->height / 2 );
		setSymbol(Deco::kExit, 0, _panel->height / 2);
	}
}

//Private version of generateMaze. Should be called again if false is returned.
//The algorithm works by generating a correct path, then extending lines off of it until the maze is filled.
bool Generate::generate_maze(int id, int numStarts, int numExits)
{
	initPanel(id);

	if (numStarts > 0) {
	place_start(numStarts);
	}
	if (numExits > 0) {
	place_exit(numExits);
	}

	//Prevent start and exit from overlapping, except in one one particular puzzle (0x00083).
	if (id == 0x00083 && _width == 15 && _height == 15) {
		clear();
		_panel->_endpoints.clear();
		_exits.clear();
		Point start = Utilities::pick_random(_starts);
		_panel->PlaceExit(start);
		Point sp = get_sym_point(start);
		_panel->PlaceExit(sp);
		set_path(start); set_path(sp);
	}
	else {
		for (Point p : _starts)
			if (_exits.contains(p))
				return false;

		clear();
		if (hasFlag(Generate::Config::ShortPath)) {
			while (!generate_path_length((_panel->width + _panel->height),
				min((_panel->width + _panel->height) * 2, (_panel->width / 2 + 1) * (_panel->height / 2 + 1) * 1 / 2))) clear();
		}
		while (!generate_path_length((_panel->width + _panel->height),
			min((_panel->width + _panel->height) * 2, (_panel->width / 2 + 1) * (_panel->height / 2 + 1) * 4 / 5))) clear();
	}
	
	std::set<Point> path = _path; //Backup

	//Extra false starts are tracked in a separate list so that the generator can make sure to extend each of them by a higher amount than usual.
	std::set<Point> extraStarts;
	for (Point pos : _starts) {
		if (!_path.contains(pos)) {
			extraStarts.insert(pos);
		}
		set_path(pos);
	}
	//Check to see if the correct path runs over any of the false start points. If so, start over
	if (extraStarts.size() != (_panel->_symmetry ? _starts.size() / 2 - 1 : _starts.size() - 1))
		return false;

	std::set<Point> check;
	std::vector<Point> deadEndH, deadEndV;
	for (Point p : _path) {
		if (p.first % 2 == 0 && p.second % 2 == 0)
			check.insert(p); //Only extend off of the points at grid intersections.
	}
	while (check.size() > 0) {
		//Pick a random extendable point and extend it for some randomly chosen amount of units.
		Point randomPos = (extraStarts.size() > 0 ? Utilities::pick_random(extraStarts) : Utilities::pick_random(check));
		Point pos = randomPos;
		for (int i = (extraStarts.size() > 0 ? 7 : 1); i >= 0; --i) { //False starts are extended by up to 7 units. Other points are extended 1 unit at a time
			std::vector<Point> validDir;
			for (Point dir : _DIRECTIONS2) {
				if (!off_edge(pos + dir) && get(pos + dir) == 0) {
					validDir.push_back(dir);
				}
			}
			if (validDir.size() < 2) check.erase(pos); //If there are 0 or 1 open directions, the point cannot be extended again.
			if (validDir.size() == 0) {
				if (extraStarts.size() > 0) {
					return false; //Not all the starts were extended successfully.
				}
				//If full gaps mode is enabled, detect dead ends, so that square tips can be put on them
				if (hasFlag(Config::FullGaps) && !_exits.contains(pos) && !_starts.contains(pos)) {
					int countOpenRow = 0, countOpenColumn = 0;
					for (Point dir2 : _DIRECTIONS1) {
						if (!off_edge(pos + dir2) && get(pos + dir2) == Deco::kPath) {
							if (dir2.first == 0) ++countOpenColumn;
							else ++countOpenRow;
						}
					}
					if (countOpenRow + countOpenColumn == 1) {
						if (countOpenRow) deadEndH.push_back(pos);
						else deadEndV.push_back(pos);
					}
				}
				break; //A dead end has been reached, extend a different point
			}
			Point dir = Utilities::pick_random(validDir);
			Point newPos = pos + dir;
			set_path(newPos);
			set_path(pos + dir / 2);
			check.insert(newPos);
			pos = newPos;
		}
		if (extraStarts.size() > 0) extraStarts.erase(randomPos);
	}
	//Put openings or gaps in any unused row or column segment
	for (int y = 0; y < _panel->height; ++y) {
		for (int x = (y + 1) % 2; x < _panel->width; x += 2) {
			if (get(x, y) != Deco::kPath) {
				set(x, y, hasFlag(Config::FullGaps) ? OPEN : x % 2 == 0 ? Decoration::Gap_Column : Decoration::Gap_Row);
				if (_panel->_symmetry) {
					Point sp = get_sym_point({x, y});
					if (sp.first == x && sp.second == y || sp.first == x && x % 2 == 0 && abs(sp.second - y) <= 2 ||
						sp.second == y && y % 2 == 0 && abs(sp.first - x) <= 2 || abs(sp.first - x) == 1) {
						set(x, y, Deco::kPath);
					}
					else if (Random::rand() % 2 == 0) {
						set(sp, Deco::kPath);
					}
					else {
						set(x, y, Deco::kPath);
						set(sp, hasFlag(Config::FullGaps) ? OPEN : x % 2 == 0 ? Decoration::Gap_Column : Decoration::Gap_Row);
					}
				}
			}
		}
	}
	//Put square ends on any dead ends
	for (Point p : deadEndH) {
		set(p, Decoration::Gap_Row);
	}
	for (Point p : deadEndV) {
		set(p, Decoration::Gap_Column);
	}
	_path = path; //Restore backup of the correct solution for testing purposes
	std::vector<std::string> solution; //For debugging only
	for (int y = 0; y < _panel->height; ++y) {
		std::string row;
		for (int x = 0; x < _panel->width; ++x) {
			if (_path.contains(Point(x, y))) {
				row += "xx";
			}
			else row += "    ";
		}
		solution.push_back(row);
	}
	if (!hasFlag(Config::DisableWrite)) write(id);
	return true;
}

//The primary generation function. id - id of the puzzle. symbols - a structure representing the amount and types of each symbol to add to the puzzle
//The algorithm works by making a random path and then adding the chosen symbols to the grid in such a way that they will be satisfied by the path.
//if at some point the generator fails to add a symbol while still making the solution correct, the function returns false and must be called again.
bool Generate::generate(int id, PuzzleSymbols symbols)
// This may be correct to pass by value instead of reference, if the function can fail to reset the state after a failed run
{
	initPanel(id);

	//Multiple erasers are forced to be separate by default. This is because combining them causes unpredictable and inconsistent behavior. 
	if (symbols.getNum(Deco::Symbol::Eraser) == 2) {
		setSymbol(Deco::Gap(Deco::Location::Row), 1, 0);
		setSymbol(Deco::Gap(Deco::Location::Row), _panel->width - 2, _panel->height - 1);
		_splitPoints = { Point(1, 1), Point(_panel->width - 2, _panel->height - 2) };
		initPanel(id); //Re-initing to account for the newly added information
	}

	//Init parity for full dot puzzles
	if (symbols.getNum(Deco::Symbol::Dot) >= _panel->get_num_grid_points() - 2)
		_parity = (_panel->get_parity() + (
			!symbols.symbols.contains(Deco::Symbol::Start) ? get_parity(Utilities::pick_random(_starts)) :
			!symbols.symbols.contains(Deco::Symbol::Exit) ? get_parity(Utilities::pick_random(_exits)) : Random::rand() % 2)) % 2;
	else _parity = -1; //-1 indicates a non-full dot puzzle

	place_start(symbols.getNum(Deco::Symbol::Start));
	place_exit(symbols.getNum(Deco::Symbol::Exit));

	//Make a random path unless a fixed one has been defined
	if (customPath.size() == 0) {
		int fails = 0;
		while (!generate_path(symbols)) {
			if (fails++ > 20) return false; //It gets several chances to make a path so that the whole init process doesn't have to be repeated so many times
		}
	}
	else _path = customPath;

	std::vector<std::string> solution; //For debugging only
	for (int y = 0; y < _panel->height; ++y) {
		std::string row;
		for (int x = 0; x < _panel->width; ++x) {
			if (get(x, y) == Deco::kPath) {
				row += "xx";
			}
			else row += "    ";
		}
		solution.push_back(row);
	}

	//Attempt to add the symbols
	if (!place_all_symbols(symbols))
		return false;

	if (!hasFlag(Config::DisableWrite)) write(id);
	return true;
}

//Place the provided symbols onto the puzzle. symbols - a structure describing types and amounts of symbols to add.
bool Generate::place_all_symbols(PuzzleSymbols& symbols)
{
	std::vector<Deco::Deco> eraseSymbols;
	std::vector<Deco::Color> eraserColors;
	//If erasers are present, choose symbols to be erased and remove them pre-emptively
	for (const auto& [symbol, quantity] : symbols[Deco::Symbol::Eraser]) {
		for (unsigned int i = 0; i < quantity; ++i) {
			eraserColors.push_back(symbol.color);
			if (hasFlag(Config::FalseParity)) {
				eraseSymbols.push_back(Deco::Dot());
			} else {
				eraseSymbols.push_back(symbols.popRandomSymbol());
			}
		}
	}

	//Symbols are placed in stages according to their type
	if (hasFlag(Config::DisconnectShapes)) {
		_SHAPEDIRECTIONS.assign(_DISCONNECT.begin(), _DISCONNECT.end());
	} else {
		_SHAPEDIRECTIONS.assign(_DIRECTIONS2.begin(), _DIRECTIONS2.end());
	}
	int numShapes = 0;
	int numRotate = 0;
	int numNegative = 0;
	std::vector<Deco::Color> colors;
	std::vector<Deco::Color> negativeColors;
	for (const auto& [symbol, quantity] : symbols[Deco::Symbol::Poly]) {
		try {
			const auto& poly = dynamic_cast<const Deco::Poly&>(symbol);
			for (int i = 0; i < quantity; ++i) {
				if (poly.can_rotate) {
					++numRotate;
				}

				if (poly.negative) {
					++numNegative;
					negativeColors.push_back(symbol.color);
				} else {
					++numShapes;
					colors.push_back(symbol.color);
				}
			}
		}
		catch (...) {
			throw std::exception("Somehow, something other than a Poly got in the Poly bucket.");
		}
	}
	if (numShapes == 0 && numNegative > 0 ||
		numShapes > 0 &&
			!place_shapes(
				colors, negativeColors, numShapes, numRotate, numNegative))
		return false;

	_stoneTypes = static_cast<int>(symbols[Deco::Symbol::Stone].size());
	_bisect = true; //This flag helps the generator prevent making two adjacent regions of stones the same color
	for (const auto& dp : symbols[Deco::Symbol::Stone]) {
		if (!place_stones(dp)) {
			return false;
		}
	}
	for (const auto& dp : symbols[Deco::Symbol::Triangle]) {
		if (!place_triangles(dp)) {
			return false;
		}
	}
	for (const auto& dp : symbols[Deco::Symbol::Arrow]) {
	// TODO: Sort this first by number of arrowheads, then by color, from most to least constrained
		if (!place_arrows(dp)) {
			return false;
		}
	}
	for (const auto& dp : symbols[Deco::Symbol::Star]) {
		if (!place_stars(dp)) {
			return false;
		}
	}
	if (symbols.style == Panel::Style::HAS_STARS && hasFlag(Generate::Config::TreehouseLayout) && !checkStarZigzag(_panel)) {
		return false;
	}

	std::vector<Point> placedErasers; // It's more useful in Special, trust me.
	if (eraserColors.size() > 0 && !place_erasers(eraserColors, eraseSymbols, placedErasers)) {
		return false;
	}
	for (const auto& dp : symbols[Deco::Symbol::Dot]) {
		// TODO: Sort this vector somehow so that the intersections dots are first, as they are more constrained
			if (!place_dots(dp)) {
				return false;
			}
	}
	for (const auto [symbol, quantity] : symbols[Deco::Symbol::Gap]) {
		if (!place_gaps(quantity)) {
			return false;
		}
	}
	return true;
}

//Generate a random path for a puzzle with the provided symbols.
//The path starts at a random start and will not cross through walls or symbols.
//Puzzle symbols are provided because they can influence how long the path should be.
bool Generate::generate_path(const PuzzleSymbols & symbols)
{
	clear();

	if (_obstructions.size() > 0) {
		std::vector<Point> walls = Utilities::pick_random(_obstructions);
		for (Point p : walls) if (get(p) == 0) set(p, p.first % 2 == 0 ? Decoration::Gap_Column : Decoration::Gap_Row);
		bool result = (hasFlag(Config::ShortPath) ? generate_path_length(1) : _parity != -1 ? generate_longest_path() :
			hitPoints.size() > 0 ? generate_special_path() : generate_path_length(_panel->get_num_grid_points() * 3 / 4));
		for (Point p : walls) if (get(p) & Decoration::Gap) set(p, 0);
		return result;
	}

	if (hitPoints.size() > 0) {
		return generate_special_path();
	}

	if (_parity != -1 || hasFlag(Generate::LongestPath)) {
		return generate_longest_path();
	}

	if (hasFlag(Config::ShortPath))
		return generate_path_length(1);

	//The diagonal symmetry puzzles have a lot of points that can't be hit, so I have to reduce the path length
	if (_panel->_symmetry == Panel::Symmetry::FlipXY || _panel->_symmetry == Panel::Symmetry::FlipNegXY) {
		return generate_path_length(_panel->get_num_grid_points() * 3 / 4 - _panel->width / 2);
	}

	//Dot puzzles have a longer path by default. Vertical/horizontal symmetry puzzles are also longer because they tend to be too simple otherwise
	if (hasFlag(Config::LongPath) || symbols.style == Panel::Style::HAS_DOTS && !hasFlag(Config::PreserveStructure) &&
		!(_panel->_symmetry == Panel::Symmetry::Vertical && (_panel->width / 2) % 2 == 0 ||
			_panel->_symmetry == Panel::Symmetry::Horizontal && (_panel->height / 2) % 2 == 0)) {
		return generate_path_length(_panel->get_num_grid_points() * 7 / 8);
	}

	//For stone puzzles, the path must have a certain number of regions
	if (symbols.style == Panel::Style::HAS_STONES && _splitPoints.empty())
		return generate_path_regions(min(symbols.getNum(Deco::Symbol::Stone), (_panel->width / 2 + _panel->height / 2) / 2 + 1));

	if (symbols.style == Panel::Style::HAS_SHAPERS) {
		if (hasFlag(Config::SplitShapes)) {
			return generate_path_regions(symbols.getNum(Deco::Symbol::Poly) + 1);
		}
		return generate_path_length(_panel->get_num_grid_points() / 2);
	}

	return generate_path_length(_panel->get_num_grid_points() * 3 / 4);
}

//Generate a random path with the provided minimum length.
bool Generate::generate_path_length(int minLength, int maxLength)
{
	int fails = 0;
	Point pos = adjust_point(Utilities::pick_random(_starts));
	Point exit = adjust_point(Utilities::pick_random(_exits));
	if (off_edge(pos) || off_edge(exit))
		return false;
	set_path(pos);
	while (pos != exit) {
		if (fails++ > 20) // post-increment matters, so refactor
			return false;
		Point dir = Utilities::pick_random(_DIRECTIONS2);
		Point newPos = pos + dir;
		if (off_edge(newPos) || get(newPos) != 0 || get(pos + dir / 2) != 0
			|| newPos == exit && _path.size() / 2 + 2 < minLength) continue;
		if (_panel->_symmetry && (off_edge(get_sym_point(newPos)) || newPos == get_sym_point(newPos)))
			continue;
		set_path(newPos);
		set_path(pos + dir / 2);
		pos = newPos;
		fails = 0;
	}
	return _path.size() / 2 + 1 >= minLength && _path.size() / 2 + 1 <= maxLength;
}

//Generate a path with the provided number of regions.
bool Generate::generate_path_regions(int minRegions)
{
	int fails = 0;
	int regions = 1;
	Point pos = adjust_point(Utilities::pick_random(_starts));
	Point exit = adjust_point(Utilities::pick_random(_exits));
	if (off_edge(pos) || off_edge(exit)) return false;
	set_path(pos);
	while (pos != exit) {
		if (fails++ > 20) // dangerous post-increment. consider with care
			return false;
		Point dir = Utilities::pick_random(_DIRECTIONS2);
		Point newPos = pos + dir;
		if (off_edge(newPos) || get(newPos) != 0 || get(pos + dir / 2) != 0
			|| newPos == exit && regions < minRegions)
			continue;
		if (_panel->_symmetry && (off_edge(get_sym_point(newPos)) || newPos == get_sym_point(newPos))) continue;
		set_path(newPos);
		set_path(pos + dir / 2);
		if (!on_edge(newPos) && on_edge(pos)) {
			++regions;
			if (_panel->_symmetry) ++regions;
		}
		pos = newPos;
		fails = 0;
	}
	return regions >= minRegions;
}

//Generate a path that covers the maximum number of points.
bool Generate::generate_longest_path()
{
	Point pos = adjust_point(Utilities::pick_random(_starts));
	Point exit = adjust_point(Utilities::pick_random(_exits));
	if (off_edge(pos) || off_edge(exit)) return false;
	Point block(-10, -10);
	if (hasFlag(Config::FalseParity)) { //If false parity, one dot must be left uncovered
		if (get_parity(pos + exit) == _panel->get_parity())
			return false;
		block = Point(Random::rand() % (_panel->width / 2 + 1) * 2, Random::rand() % (_panel->height / 2 + 1) * 2);
		while (pos == block || exit == block) {
			block = Point(Random::rand() % (_panel->width / 2 + 1) * 2, Random::rand() % (_panel->height / 2 + 1) * 2);
		}
		set_path(block);
	}
	else if (get_parity(pos + exit) != _panel->get_parity())
		return false;
	int fails = 0;
	int reqLength = _panel->get_num_grid_points() + static_cast<int>(_path.size()) / 2;
	bool centerFlag = !on_edge(pos);
	set_path(pos);
	while (pos != exit && !(_panel->_symmetry && get_sym_point(pos) == exit)) {
		std::vector<std::string> solution; //For debugging only
		for (int y = 0; y < _panel->height; ++y) {
			std::string row;
			for (int x = 0; x < _panel->width; ++x) {
				if (get(x, y) == Deco::kPath) {
					row += "xx";
				}
				else row += "    ";
			}
			solution.push_back(row);
		}
		if (fails++ > 20)  // dangerous post-increment. consider with care
			return false;
		Point dir = Utilities::pick_random(_DIRECTIONS2);
		for (Point checkDir : _DIRECTIONS2) {
			Point check = pos + checkDir;
			if (off_edge(check) || get(check) != 0)
				continue;
			if (check == exit) continue;
			int open = 0;
			for (Point checkDir2 : _DIRECTIONS2) {
				if (!off_edge(check + checkDir2) && get(check + checkDir2) == 0) {
					if (++open >= 2) break;
				}
			}
			if (open < 2) {
				dir = checkDir;
				break;
			}
		}
		Point newPos = pos + dir;
		//Various checks to see if going this direction will lead to any issues 
		if (off_edge(newPos) || get(newPos) != 0 || get(pos + dir / 2) != 0
			|| newPos == exit && _path.size() / 2 + 3 < reqLength ||
			_panel->_symmetry && get_sym_point(newPos) == exit && _path.size() / 2 + 3 < reqLength) continue;
		if (_panel->_symmetry && (off_edge(get_sym_point(newPos)) || newPos == get_sym_point(newPos))) continue;
		if (on_edge(newPos) && _panel->_pillarWidth == 0 && _panel->_symmetry != Panel::Symmetry::Horizontal && newPos + dir != block && (off_edge(newPos + dir) || get(newPos + dir) != 0)) {
			if (centerFlag && off_edge(newPos + dir)) {
				centerFlag = false;
			}
			else {
				int open = 0;
				for (Point checkDir : _DIRECTIONS2) {
					if (!off_edge(newPos + checkDir) && get(newPos + checkDir) == 0) {
						if (++open >= 2) break;
					}
				}
				if (open >= 2) continue;
			}
		}
		set_path(newPos);
		set_path(pos + dir / 2);
		pos = newPos;
		fails = 0;
	}
	if (!off_edge(block)) //Uncover the one dot for false parity
		set(block, 0);
	return _path.size() / 2 + 1 == reqLength;
}

//Generate path that passes through all of the hitPoints in order
bool Generate::generate_special_path()
{
	Point pos = adjust_point(Utilities::pick_random(_starts));
	Point exit = adjust_point(Utilities::pick_random(_exits));
	if (off_edge(pos) || off_edge(exit))
		return false;
	set_path(pos);
	for (Point p : hitPoints) {
		set(p, Deco::kPath);
	}
	int hitIndex = 0;
	int minLength = _panel->get_num_grid_points() * 3 / 4;
	while (pos != exit) {
		std::vector<Point> validDir;
		for (Point dir : _DIRECTIONS2) {
			Point newPos = pos + dir;
			if (off_edge(newPos)) continue;
			Point connectPos = pos + dir / 2;
			//Go through the hit point if passing next to it
			if (get(connectPos) == Deco::kPath && hitIndex < hitPoints.size() && connectPos == hitPoints[hitIndex]) {
				validDir = { dir };
				++hitIndex;
				break;
			}
			if (get(newPos) != 0 || get(connectPos) != 0 || newPos == exit && (hitIndex != hitPoints.size() || _path.size() / 2 + 2 < minLength)) continue;
			if (_panel->_symmetry && newPos == get_sym_point(newPos)) continue;
			bool fail = false;
			for (Point dir : _DIRECTIONS1) {
				if (!off_edge(newPos + dir) && get(newPos + dir) == Deco::kPath && newPos + dir != hitPoints[hitIndex]) {
					fail = true;
					break;
				}
			}
			if (fail) continue;
			validDir.push_back(dir);
		}
		if (validDir.size() == 0)
			return false;
		Point dir = Utilities::pick_random(validDir);
		set_path(pos + dir);
		set_path(pos + dir / 2);
		pos = pos + dir;
	}
	return hitIndex == hitPoints.size() && _path.size() >= minLength;
}

//Erase the path from the puzzle grid
void Generate::erase_path()
{
	for (int y = 0; y < _panel->height; ++y) {
		for (int x = 0; x < _panel->width; ++x) {
			if (get(x, y) == Deco::kPath) {
				set(x, y, 0);
			}
		}
	}
}

//If a point is on an edge, bump it randomly to an adjacent vertex. Otherwise, the point is untouched
Point Generate::adjust_point(Point pos) {
	if (pos.first % 2 != 0) {
		if (get(pos) != 0) return { -10, -10 };
		set_path(pos);
		return Point(pos.first - 1 + Random::rand() % 2 * 2, pos.second);
	}
	if (pos.second % 2 != 0) {
		if (get(pos) != 0) return { -10, -10 };
		set_path(pos);
		return Point(pos.first, pos.second - 1 + Random::rand() % 2 * 2);
	}
	if (_panel->_symmetry && _exits.contains(pos) && !_exits.contains(get_sym_point(pos))) return { -10, -10 };
	return pos;
}

//Get the set of points in region containing the point (pos)
std::set<Point> Generate::get_region(Point pos) const {
	std::set<Point> region;
	std::vector<Point> check;
	check.push_back(pos);
	region.insert(pos);
	while (check.size() > 0) {
		Point p = check[check.size() - 1];
		check.pop_back();
		for (Point dir : _DIRECTIONS1) {
			Point p1 = p + dir;
			if (on_edge(p1)) continue;
			if (get(p1) == Deco::kPath || get(p1) == Deco::kOpen) continue;
			Point p2 = p + dir * 2;
			if (get(p2) == Deco::kEmpty) continue;
			if (region.insert(p2).second) {
				check.push_back(p2);
			}
		}
	}
	return region;
}

//Get all the symbols in the region containing including the point (pos)
std::vector<int> Generate::get_symbols_in_region(Point pos) const {
	return get_symbols_in_region(get_region(pos));
}

//Get all the symbols in the given region
std::vector<int> Generate::get_symbols_in_region(const std::set<Point>& region) const {
	std::vector<int> symbols;
	for (Point p : region) {
		if (get(p)) symbols.push_back(get(p));
	}
	return symbols;
}

//Place start points in random locations
bool Generate::place_start(int amount) {
	if (amount <= 0) {
		return false; // not used
	}
	_starts.clear();
	_panel->_startpoints.clear();
	while (amount > 0) {
		Point pos = Point(Random::rand() % (_panel->width / 2 + 1) * 2, Random::rand() % (_panel->height / 2 + 1) * 2);
		if (hasFlag(Config::StartEdgeOnly))
		switch (Random::rand() % 4) {
		case 0: pos.first = 0; break;
		case 1: pos.second = 0; break;
		case 2: pos.first = _panel->width - 1; break;
		case 3: pos.second = _panel->height - 1; break;
		}
		if (_parity != -1 && get_parity(pos) != (amount == 1 ? _parity : !_parity)) continue;
		if (_starts.contains(pos) || _exits.contains(pos)) continue;
		if (_panel->_symmetry && pos == get_sym_point(pos)) continue;
		//Highly discourage putting start points adjacent
		bool adjacent = false;
		for (Point dir : _DIRECTIONS2) {
			if (!off_edge(pos + dir) && get(pos + dir) == Deco::kStart) {
				adjacent = true;
				break;
			}
		}
		if (adjacent && Random::rand() % 10 > 0) continue;
		_starts.insert(pos);
		_panel->PlaceStart(pos);
		--amount;
		if (_panel->_symmetry) {
			Point sp = get_sym_point(pos);
			_starts.insert(sp);
			_panel->PlaceStart(sp);
		}
	}
	return true; // not used
}

//Place an exit point in a random location on the edge of the grid
bool Generate::place_exit(int amount)
{
	_exits.clear();
	_panel->_endpoints.clear();
	while (amount > 0) {
		Point pos = Point(Random::rand() % (_panel->width / 2 + 1) * 2, Random::rand() % (_panel->height / 2 + 1) * 2);
		switch (Random::rand() % 4) {
		case 0: pos.first = 0; break;
		case 1: pos.second = 0; break;
		case 2: pos.first = _panel->width - 1; break;
		case 3: pos.second = _panel->height - 1; break;
		}
		if (_parity != -1 && (get_parity(pos) + _panel->get_parity()) % 2 != (amount == 1 ? _parity : !_parity)) continue;
		if (_starts.contains(pos) || _exits.contains(pos)) continue;
		if (_panel->_symmetry && pos == get_sym_point(pos)) continue;
		if (_panel->_symmetry && get_sym_point(pos).first != 0 && get_sym_point(pos).second != 0) continue;
		//Prevent putting exit points adjacent
		bool adjacent = false;
		for (Point dir : _8DIRECTIONS2) {
			if (!off_edge(pos + dir) && get(pos + dir) == Deco::kExit) {
				adjacent = true;
				break;
			}
		}
		if (adjacent) continue;
		_exits.insert(pos);
		_panel->PlaceExit(pos);
		--amount;
		if (_panel->_symmetry) {
			Point sp = get_sym_point(pos);
			_exits.insert(sp);
			_panel->PlaceExit(sp);
		}
	}
	return true;
}

//Check if a gap can be placed at pos.
bool Generate::can_place_gap(Point pos) const {
	//Prevent putting open gaps at edges of the puzzle
	if (pos.first == 0 || pos.second == 0) {
		if (hasFlag(Config::FullGaps)) return false;
	}
	else if (Random::rand() % 2 == 0) return false; //Encourages gaps on outside border
	//Prevent putting a gap on top of a start/end point
	if (_starts.contains(pos) || _exits.contains(pos))
		return false;
	//For symmetry puzzles, prevent putting two gaps symmetrically opposite 
	if (_panel->_symmetry && (get_sym_point(pos) == pos) || (get(get_sym_point(pos)) & Decoration::Gap)) return false;
	if ((_panel->_symmetry == Panel::Symmetry::ParallelH || _panel->_symmetry == Panel::Symmetry::ParallelHFlip) && pos.second == _panel->height / 2) return false;
	if ((_panel->_symmetry == Panel::Symmetry::ParallelV || _panel->_symmetry == Panel::Symmetry::ParallelVFlip) && pos.first == _panel->width / 2) return false;
	if (_panel->_symmetry == Panel::Symmetry::FlipNegXY && (pos.first + pos.second == _width - 1 || pos.first + pos.second == _width + 1)) return false;
	if (_panel->_symmetry == Panel::Symmetry::FlipXY && (pos.first - pos.second == 1 || pos.first - pos.second == -1)) return false;
	if (hasFlag(Config::FullGaps)) { //Prevent forming dead ends with open gaps
		std::vector<Point> checkPoints = (pos.first % 2 == 0 ? std::vector<Point>({ Point(pos.first, pos.second - 1), Point(pos.first, pos.second + 1) })
			: std::vector<Point>({ Point(pos.first - 1, pos.second), Point(pos.first + 1, pos.second) }));
		for (Point check : checkPoints) {
			int valid = 4;
			for (Point dir : _DIRECTIONS1) {
				Point p = check + dir;
				if (off_edge(p) || get(p) & GAP || get(p) == OPEN) {
					if (--valid <= 2) {
						return false;
					}
				}
			}
		}
	}
	return true;
}

//Place the given amount of gaps radomly around the puzzle
bool Generate::place_gaps(int amount) {
	std::set<Point> open;
	for (int y = 0; y < _panel->height; ++y) {
		for (int x = (y + 1) % 2; x < _panel->width; x += 2) {
			if (get(x, y) == 0 && (!hasFlag(Config::FullGaps) || !on_edge(Point(x, y)))) {
				open.emplace(Point(x, y));
			}
		}
	}

	while (amount > 0) {
		if (open.size() == 0)
			return false;
		Point pos = Utilities::pick_random(open);
		if (can_place_gap(pos)) {
			set(pos, hasFlag(Config::FullGaps) ? OPEN : pos.first % 2 == 0 ? Decoration::Gap_Column : Decoration::Gap_Row);
			--amount;
		}
		open.erase(pos);
	}
	return true;
}

//Check if a dot can be placed at pos.
bool Generate::can_place_dot(Point pos, bool intersectionOnly) const {
	if (get(pos) & DOT)
		return false;
	if (_panel->_symmetry) {
		//For symmetry puzzles, make sure the current pos and symmetric pos are both valid
		Point symPos = get_sym_point(pos);
		if (symPos == pos) return false;
		Panel::Symmetry backupSym = _panel->_symmetry;
		_panel->_symmetry = Panel::Symmetry::None; //To prevent endless recursion
		//if (!can_place_dot(get_sym_point(pos))) {
		if (!can_place_dot(symPos, intersectionOnly)) {
			_panel->_symmetry = backupSym;
			return false;
		}
		_panel->_symmetry = backupSym;
	}
	if (_panel->_symmetry == Panel::Symmetry::RotateLeft && _path1.contains(pos) && _path2.contains(pos))
		return false; //Prevent sharing of dots between symmetry lines
	if (hasFlag(Config::DisableDotIntersection)) return true;
	for (Point dir : _8DIRECTIONS1) {
		Point p = pos + dir;
		if (!off_edge(p) && (get(p) & DOT)) {
			//Don't allow adjacent dots
			if (dir.first == 0 || dir.second == 0)
				return false;
			//Allow diagonally adjacent placement some of the time
			if (Random::rand() % 2 > 0)
				return false;
		}
	}
	//Allow 2-space horizontal/vertical placement some of the time
	if (Random::rand() % (intersectionOnly ? 10 : 5) > 0) {
		for (Point dir : _DIRECTIONS2) {
			Point p = pos + dir;
			if (!off_edge(p) && (get(p).symbol == Deco::Symbol::Dot)) {
				return false;
			}
		}
	}
	return true;
}

//Place the given amount of dots at random points on the path
bool Generate::place_dots(DecoPair dp) {
	int amount = dp.second;
	bool intersectionOnly = false; // TBD by reading dp.first
	try {
		auto& dotInfo = dynamic_cast<Deco::Dot&>(dp.first);
		
		if (_parity != -1) { //For full dot puzzles, don't put dots on the starts and exits unless there are multiple
			for (int x = 0; x < _panel->width; x += 2) {
				for (int y = 0; y < _panel->height; y += 2) {
					if (_starts.size() == 1 && _starts.contains(Point(x, y))) continue;
					if (_exits.size() == 1 && _exits.contains(Point(x, y))) continue;
					if (get(x, y) == 0) continue;
					set(x, y, Deco::Dot());
				}
			}
			amount -= _panel->get_num_grid_points();
			if (amount <= 0) return true;
			dotInfo.location = Deco::Location::Any;
			setFlagOnce(Config::DisableDotIntersection);
		}

		std::set<Point> open;
		switch (dotInfo.parity) {
		case Deco::Parity::Blue:
			open = _path1; // Place blue dots on the blue/cyan path.
			break;
		case Deco::Parity::Orange:
			open = _path2; // Place orange/yellow dots on the orange path.
			break;
		case Deco::Parity::Any: [[fallthrough]];
		default: // Any other dots may be placed on any path.
			open = _path;
		}

		// Remove starts, exits, and known blockers from the pool of available spots.
		// This could be done with vectors
		for (Point p : _starts) open.erase(p);
		for (Point p : _exits) open.erase(p);
		for (Point p : blockPos) open.erase(p);
		if (dotInfo.location == Deco::Location::Intersection) {
			intersectionOnly = true;
			std::set<Point> intersections;
			for (Point p : open) {
				if (p.first % 2 == 0 && p.second % 2 == 0)
					intersections.insert(p);
			}
			open = intersections;
		}
		if (hasFlag(Config::DisableDotIntersection)) {
			std::set<Point> intersections;
			for (Point p : open) {
				if (p.first % 2 != 0 || p.second % 2 != 0)
					intersections.insert(p);
			}
			open = intersections;
		}

		while (amount > 0) {
			if (open.size() == 0) {
				// If we run out of open spots before we run out of dots, the function fails.
				return false;
			}
			Point pos = Utilities::pop_random(open);

			if (!can_place_dot(pos, intersectionOnly)) continue;
			Deco::Dot symbol = (pos.first & 1) == 1	   ? Deco::Dot(Deco::Location::Row, dotInfo.parity)
							   : (pos.second & 1) == 1 ? Deco::Dot(Deco::Location::Column, dotInfo.parity)
													   : Deco::Dot(Deco::Location::Intersection, dotInfo.parity);
			set(pos, symbol);
			// Remove adjacent points from the path set, if any
			for (Point dir : _DIRECTIONS1) {
				open.erase(pos + dir);
			}
			
			//If symmetry, set a flag to break the point symmetric to the dot
			if (_panel->_symmetry) {
				Point sp = get_sym_point(pos);
				symbol = (sp.first & 1) == 1	? Deco::Dot(Deco::Location::Row)
						 : (sp.second & 1) == 1 ? Deco::Dot(Deco::Location::Column)
												: Deco::Dot(Deco::Location::Intersection);
				if (symbol != Decoration::Dot_Intersection) {
					set(sp, symbol & ~Decoration::Dot);
				}
				// The generator is not allowed to place same-colored Dots
				// in positions symmetrical to one that was just placed.
				// If the dot placed is blue/orange, then the symmetrical position
				// must be the opposite parity.
				// If the dot placed is black, then the symmetrical position is trivial.
				open.erase(sp);
				for (Point dir : _DIRECTIONS1) {
					open.erase(sp + dir);
				}
			}
			--amount;
		}
		return true;
	}
	catch (...) {
		throw std::exception("Somehow, something other than a Dot got in the Dot bucket.");
	}
}

//Check if a stone can be placed at a position within the region.
bool Generate::can_place_stone(const std::set<Point>& region, const Deco::Color color) const
{
	for (Point point : region) {
		const Deco::Deco& sym = get(point);
		if (sym.symbol == Deco::Symbol::Stone) {
			return (sym.color == color);
		}
	}
	return true;
}

//Place the given amount of stones with the given color
bool Generate::place_stones(const DecoPair& dp) {
	// const Deco::Stone& stone, int amount
	int amount = dp.second;
	auto& stone = dp.first;
	std::set<Point> open = _openpos;
	std::set<Point> open2; //Used to store open points removed from the first pass, to make sure a stone is put in every non-adjacent region
	int passCount = 0;
	int originalAmount = amount;
	while (amount > 0) {
		if (open.size() == 0) {
			//Make sure there is room for the remaining stones and enough partitions have been made (based on the grid size)
			if (open2.size() < amount || 
				_bisect &&
				passCount < min(originalAmount,
								(_panel->width / 2 + _panel->height / 2 + 2) / 4)
				)
				return false;
			//Put remaining stones wherever they will fit
			Point pos = Utilities::pick_random(open2);
			set(pos, stone);
			_openpos.erase(pos);
			open2.erase(pos);
			--amount;
			continue;
		}
		Point pos = Utilities::pick_random(open);
		std::set<Point> region = get_region(pos);
		if (!can_place_stone(region, stone.color)) {
			for (Point p : region) {
				open.erase(p);
			}
			continue;
		}
		if (_stoneTypes > 2) { //If more than two colors, group stones together, otherwise it takes too long to generate.
			open.clear();
			for (Point p : region) {
				if (_openpos.contains(p))
					open.insert(p);
			}
		}
		open.erase(pos);
		if (_panel->_symmetry) {
			open.erase(get_sym_point(pos));
		}
		if (_stoneTypes == 2) {
			for (Point p : region) {
				if (open.erase(p)) open2.insert(p);
			} //Remove adjacent regions from the open list
			for (Point p : region) {
				for (Point dir : _8DIRECTIONS2) {
					Point pos2 = p + dir;
					if (open.contains(pos2) && !region.contains(pos2)) {
						for (Point P : get_region(pos2)) {
							open.erase(P);
						}
					}
				}
			}
		}
		set(pos, stone);
		_openpos.erase(pos);
		--amount;
		++passCount;
	}
	_bisect = false; //After placing one color, adjacent regions are allowed
	--_stoneTypes;
	return true;
}

//Generate a random shape. region - the region of points to choose from; points chosen will be removed.
//bufferRegion - points that may be chosen twice due to overlapping shapes; points will be removed from here before points in region.
//maxSize - the maximum size of the generated shape. Whether the points can be contiguous or not is determined by local variable _SHAPEDIRECTIONS
Shape Generate::generate_shape(std::set<Point>& region, std::set<Point>& bufferRegion, Point pos, int maxSize)
{
	Shape shape;
	shape.insert(pos);
	if (!bufferRegion.erase(pos))
		region.erase(pos);
	while (shape.size() < maxSize && region.size() > 0) {
		pos = Utilities::pick_random(shape);
		int i = 0;
		for (; i < 10; ++i) {
			Point dir = Utilities::pick_random(_SHAPEDIRECTIONS);
			Point p = pos + dir;
			if (region.contains(p) && !shape.contains(p)) {
				shape.insert(p);
				if (!bufferRegion.erase(p))
					region.erase(p);
				break;
			}
		}
		if (i == 10)
			return shape;
	}
	return shape;
}

//Get the integer representing the shape, accounting for whether it is rotated or negative. -1 rotation means a random rotation, depth is for controlling recursion and should be set to 0
int Generate::make_shape_symbol(Shape shape, bool rotated, bool negative, int rotation, int depth)
{
	int symbol = static_cast<int>(Decoration::Poly);
	if (rotated) {
		if (rotation == -1) {
			if (make_shape_symbol(shape, rotated, negative, 0, depth + 1) == make_shape_symbol(shape, rotated, negative, 1, depth + 1))
				return 0; //Check to make sure the shape is not the same when rotated
			rotation = Random::rand() % 4;
		}
		symbol |= Decoration::Can_Rotate;
		Shape newShape; //Rotate shape points according to rotation
		for (Point p : shape) {
			switch (rotation) {
			case 0: newShape.insert(p); break;
			case 1: newShape.emplace(Point(p.second, -p.first)); break;
			case 2: newShape.emplace(Point(-p.second, p.first)); break;
			case 3: newShape.emplace(Point(-p.first, -p.second)); break;
			}
		}
		shape = newShape;
	}
	if (negative) symbol |= Decoration::Negative;
	int xmin = INT_MAX, xmax = INT_MIN, ymin = INT_MAX, ymax = INT_MIN;
	for (Point p : shape) {
		if (p.first < xmin) xmin = p.first;
		if (p.first > xmax) xmax = p.first;
		if (p.second < ymin) ymin = p.second;
		if (p.second > ymax) ymax = p.second;
	}
	if (xmax - xmin > 6 || ymax - ymin > 6) { //Shapes cannot be more than 4 in width and height
		if (_panel->_pillarWidth == 0 || ymax - ymin > 6 || depth > _panel->_pillarWidth / 2) return 0;
		Shape newShape;
		for (Point p : shape) newShape.insert({ (p.first - xmax + _panel->_pillarWidth) % _panel->_pillarWidth, p.second });
		return make_shape_symbol(newShape, rotated, negative, rotation, depth + 1);
	}
	//Translate to the corner and set bit flags (16 bits, 1 where a shape block is present)
	for (Point p : shape) {
		symbol |= (1 << ((p.first - xmin) / 2 + (ymax  - p.second) * 2)) << 16;
	}
	if (Random::rand() % 4 > 0) { //The generator makes a certain type of symbol way too often (2x2 square with another square attached), this makes it much less frequent
		int type = symbol >> 16;
		if (type == 0x0331 || type == 0x0332 || type == 0x0037 || type == 0x0067 || type == 0x0133 || type == 0x0233 || type == 0x0073 || type == 0x0076)
			return 0;
	}
	return symbol;
}

//Place the given amount of shapes with random colors selected from the color vectors.
//colors - colors for regular shapes, negativeColors - colors for negative shapes, amount - how many normal shapes
//numRotated - how many rotated shapes, numNegative - how many negative shapes
bool Generate::place_shapes(const std::vector<Deco::Color>& colors, const std::vector<Deco::Color>& negativeColors, int amount, int numRotated, int numNegative)
{
	std::set<Point> open = _openpos;
	int shapeSize = hasFlag(Config::SmallShapes) ? 2 : hasFlag(Config::BigShapes) ? amount == 1 ? 8 : 6 : 4;
	int targetArea = amount * shapeSize * 7 / 8; //Average size must be at least 7/8 of the target size
	if (amount * shapeSize > _panel->get_num_grid_blocks()) targetArea = _panel->get_num_grid_blocks();
	int originalAmount = amount;
	if (hasFlag(Generate::Config::MountainFloorH) && _panel->width == 9) { //The 4 small puzzles shape size may vary depending on the path
		targetArea = 0;
		removeFlag(Generate::Config::MountainFloorH);
	}
	int totalArea = 0;
	int minx = _panel->width, miny = _panel->height, maxx = 0, maxy = 0;
	int colorIndex = Random::rand() % colors.size();
	int colorIndexN = Random::rand() % (negativeColors.size() + 1);
	bool shapesCanceled = false, shapesCombined = false, flatShapes = true;
	if (amount == 1) shapesCombined = true;
	while (amount > 0) {
		if (open.size() == 0)
			return false;
		Point pos = Utilities::pick_random(open);
		std::set<Point> region = get_region(pos);
		std::set<Point> bufferRegion;
		std::set<Point> open2; //Open points for just that region
		for (Point p : region) {
			if (open.erase(p)) open2.insert(p);
		}
		if (region.size() + totalArea == _panel->get_num_grid_blocks() &&
			targetArea != _panel->get_num_grid_blocks()) continue; //To prevent shapes from filling every grid point
		std::vector<Shape> shapes;
		std::vector<Shape> shapesN;
		int numShapesN = min(Random::rand() % (numNegative + 1), static_cast<int>(region.size()) / 3); //Negative blocks may be at max 1/3 of the regular blocks
		if (amount == 1) numShapesN = numNegative;
		if (numShapesN) {
			std::set<Point> regionN = _gridpos;
			int maxSize = static_cast<int>(region.size()) - numShapesN * 3; //Max size of negative shapes
			if (maxSize == 0) maxSize = 1;
			for (int i = 0; i < numShapesN; ++i) {
				pos = Utilities::pick_random(region);
				//Try to pick a random point adjacent to a shape
				for (int i = 0; i < 10; ++i) {
					Point p = pos + Utilities::pick_random(_SHAPEDIRECTIONS);
					if (regionN.contains(p) && !region.contains(p)) {
						pos = p;
						break;
					}
				}
				if (!regionN.contains(pos)) return false;
				Shape shape = generate_shape(regionN, pos, min(Random::rand() % 3 + 1, maxSize));
				shapesN.push_back(shape);
				for (Point p : shape) {
					if (region.contains(p)) {
						// Buffer region stores overlap between shapes
						bufferRegion.insert(p);
					} else {
						region.insert(p);
					}
				}
			}
		}
		int numShapes = static_cast<int>(region.size() + bufferRegion.size()) / (shapeSize + 1) + 1; //Pick a number of shapes to make. I tried different ones until I found something that made a good variety of shapes
		if (numShapes == 1 && bufferRegion.size() > 0) ++numShapes; //If there is any overlap, we need at least two shapes
		if (numShapes < amount && region.size() > shapeSize && Random::rand() % 2 == 1) ++numShapes; //Adds more variation to the shape sizes
		if (region.size() <= shapeSize + 1 && bufferRegion.size() == 0 && Random::rand() % 2 == 1) numShapes = 1; //For more variation, sometimes make a bigger shape than the target if the size is close
		if (hasFlag(Config::MountainFloorH)) {
			if (region.size() < 19) continue;
			numShapes = 6; //The big mountain floor puzzle on hard mode needs additional shapes since some combine
			targetArea = 19;
		}
		if (hasFlag(Config::SplitShapes) && numShapes != 1) continue;
		if (hasFlag(Config::RequireCombineShapes) && numShapes == 1) continue;
		bool balance = false;
		if (numShapes > amount //The region is too big for the number of shapes chosen
			|| numNegative > 0 && _panel->id == 0x288AA //Expert UTM Perspective 4
			|| numNegative > 0 && _panel->id == 0x00089) { //Expert UTM Invisible 6
			if (numNegative < 2 || hasFlag(Config::DisableCancelShapes)) continue;
			//Make balancing shapes - Positive and negative will be switched so that code can be reused
			balance = true;
			std::set<Point> regionN = _gridpos;
			numShapes = max(2, Random::rand() % numNegative + 1);			//Actually the negative shapes
			numShapesN = min(amount, 1);		//Actually the positive shapes
			if (numShapesN >= numShapes * 3 || numShapesN * 5 <= numShapes) continue;
			shapes.clear();
			shapesN.clear();
			region.clear();
			bufferRegion.clear();
			for (int i = 0; i < numShapesN; ++i) {
				Shape shape = generate_shape(regionN, Utilities::pick_random(regionN), min(shapeSize + 1, numShapes * 2 / numShapesN + Random::rand() % 3 - 1));
				shapesN.push_back(shape);
				for (Point p : shape) {
					region.insert(p);
				}
			}
			shapesCanceled = true;
			//Let the rest of the algorithm create the cancelling shapes
		}
		if (_panel->_symmetry && numShapes == originalAmount && numShapes >= 3 && _panel->_pillarWidth == 0 && !region.contains(Point((_panel->width / 4) * 2 + 1, (_panel->height / 4) * 2 + 1)))
			continue; //Prevent it from shoving all shapes to one side of symmetry
		if ((_panel->_symmetry == Panel::Symmetry::ParallelH || _panel->_symmetry == Panel::Symmetry::ParallelV ||
			_panel->_symmetry == Panel::Symmetry::ParallelHFlip || _panel->_symmetry == Panel::Symmetry::ParallelVFlip)
			&& region.contains(Point((_panel->width / 4) * 2 + 1, (_panel->height / 4) * 2 + 1)))
			continue; //Prevent parallel symmetry from making regions through the center line (this tends to make the puzzles way too hard)
		if (!balance && numShapesN && (numShapesN > 1 && numRotated > 0 || numShapesN > 2 || numShapes + numShapesN > 6))
			continue; //Trying to prevent the game's shape calculator from lagging too much
		if (!(hasFlag(Config::MountainFloorH) && _panel->width == 11) && open2.size() < numShapes + numShapesN) continue; //Not enough space to put the symbols
		if (numShapes == 1) {
			shapes.push_back(region);
			region.clear();
		}
		else for (; numShapes > 0; --numShapes) {
			if (region.size() == 0) break;
			Shape shape = generate_shape(region, bufferRegion, Utilities::pick_random(region), balance ? Random::rand() % 3 + 1 : shapeSize);
			if (!balance && numShapesN) for (Shape s : shapesN) if (std::equal(shape.begin(), shape.end(), s.begin(), s.end())) return false; //Prevent unintentional in-group canceling
			shapes.push_back(shape);
		}
		//Take remaining area and try to stick it to existing shapes
		multibreak:
		while (region.size() > 0) {
			pos = Utilities::pick_random(region);
			for (Shape& shape : shapes) {
				if (shape.size() > shapeSize || shape.contains(pos)) {continue;}
				for (Point p : shape) {
					for (Point dir : _DIRECTIONS2) {
						if (pos + dir == p) {
							shape.insert(pos);
							if (!bufferRegion.erase(pos))
								region.erase(pos);
							goto multibreak;
						}
					}
				}
			}
			//Failed to cover entire region, need to pick a different region
			break;
		}
		if (region.size() > 0) continue;
		if (balance) { //Undo swap for balancing shapes
			std::swap(shapes, shapesN);
		}
		numShapes = static_cast<int>(shapes.size());
		for (Shape& shape : shapesN) {
			shapes.push_back(shape);
		}
		if (hasFlag(Config::DisconnectShapes)) {
			//Make sure at least one shape is disconnected
			bool disconnect = false;
			for (Shape& shape : shapes) {
				if (shape.size() == 1) continue;
				disconnect = true;
				for (Point p : shape) {
					for (Point dir : _DIRECTIONS2) {
						if (shape.contains(p + dir)) {
							disconnect = false;
							break;
						}
					}
					if (!disconnect) break;
				}
				if (disconnect) break;
			}
			if (!disconnect) continue;
		}
		if (numShapes > 1) shapesCombined = true;
		numNegative -= static_cast<int>(shapesN.size());
		if (hasFlag(Generate::Config::MountainFloorH) && amount == 6) { //For mountain floor, combine some of the shapes together
			if (!combine_shapes(shapes) || !combine_shapes(shapes)) //Must call this twice b/c there are two combined areas
				return false;
			amount -= 2;
		}
		for (Shape& shape : shapes) {
			int symbol = make_shape_symbol(shape, (numRotated-- > 0), (numShapes-- <= 0));  // dangerous post-increment. consider with care
			if (symbol == 0)
				return false;
			if (!((symbol >> 16) == 0x000F || (symbol >> 16) == 0x1111))
				flatShapes = false;
			//Attempt not to put shape symbols adjacent
			Point pos;
			for (int i = 0; i < 10; ++i) {
				if (open2.size() == 0) return false;
				pos = Utilities::pick_random(open2);
				bool pass = true;
				for (Point dir : _8DIRECTIONS2) {
					Point p = pos + dir;
					if (!off_edge(p) && get(p) & Decoration::Poly) {
						pass = false;
						break;
					}
				}
				if (pass) break;
			}
			if (symbol & Decoration::Negative) { // TODO: make this not rely on the int-cast.
				// TODO: Shuffle the vectors rather than just starting at one point and cycling forward in order.
				// (It's modular, so it doesn't overflow, at least)
				set(pos, symbol | static_cast<int>(negativeColors[++colorIndexN % negativeColors.size()])); // kludge_cast
			} else {
				set(pos, symbol | static_cast<int>(colors[++colorIndex % colors.size()])); // kludge_cast
				totalArea += static_cast<int>(shape.size());
				--amount;
			}
			open2.erase(pos);
			_openpos.erase(pos);
			if (_panel->_symmetry && _panel->_pillarWidth == 0 && originalAmount >= 3) {
				for (const Point& p : shape) {
					if (p.first < minx) minx = p.first;
					if (p.second < miny) miny = p.second;
					if (p.first > maxx) maxx = p.first;
					if (p.second > maxy) maxy = p.second;
				}
			}
		}
	} //Do some final checks - make sure targetArea has been reached, all shapes have been placed, and that config requirements have been met
	if (totalArea < targetArea || numNegative > 0 ||
		hasFlag(Config::RequireCancelShapes) && !shapesCanceled ||
		hasFlag(Config::RequireCombineShapes) && !shapesCombined ||
		originalAmount > 1 && flatShapes)
		return false;
	//If symmetry, make sure it didn't shove all the shapes to one side
	if (_panel->_symmetry && _panel->_pillarWidth == 0 && originalAmount >= 3 &&
		(minx >= _panel->width / 2 || maxx <= _panel->width / 2 || miny >= _panel->height / 2 || maxy <= _panel->height / 2))
		return false;
	return true;
}

//Count the occurrence of the given symbol color in the given region (for the stars)
int Generate::count_color(const std::set<Point>& region, Deco::Color color) const {
	int count = 0;
	for (Point p : region) {
		auto sym = get(p);
		if (sym && sym.color == color) {
			if (count++ == 2) {  // dangerous post-increment. consider with care
				return count;
			}
		}
	}
	return count;
}

//Place the given amount of stars with the given color
bool Generate::place_stars(DecoPair dp) {
	auto amount = dp.second;
	auto color = dp.first.color;
	std::set<Point> open = _openpos;
	while (amount > 0) {
		if (open.size() == 0)
			return false;
		Point pos = Utilities::pick_random(open);
		std::set<Point> region = get_region(pos);
		std::set<Point> open2; //All of the open points in that region
		for (Point p : region) {
			if (open.erase(p)) open2.insert(p);
		}
		int count = count_color(region, color);
		if (count >= 2) continue; //Too many of that color
		if (open2.size() + count < 2) continue; //Not enough space to get 2 of that color
		if (count == 0 && amount == 1) continue; //If one star is left, it needs a pair
		set(pos, Deco::Star(color));
		_openpos.erase(pos);
		--amount;
		if (count == 0) { //Add a second star of the same color
			open2.erase(pos);
			if (open2.size() == 0)
				return false;
			pos = Utilities::pick_random(open2);
			set(pos, Deco::Star(color));
			_openpos.erase(pos);
			--amount;
		}
	}
	return true;
}

//Check if there is a star in the given region
bool Generate::has_star(const std::set<Point>& region, Deco::Color color) const {
	for (Point p : region) {
		if (get(p) == (Deco::Star(color))) {
			return true;
		}
	}
	return false;
}

// If any given row contains any color with an even number of symbols, return true
bool Generate::checkStarZigzag(std::shared_ptr<Panel> panel) const {
	if (panel->_width <= 5 || panel->_height <= 5) {return true;}
	for (int y = 1; y < panel->_height; y += 2) {
		std::map<Deco::Color, int> colorCount;
		for (int x = 1; x < panel->_width; x += 2) {
			Deco::Color color = Deco::Color{panel->_grid[x][y] & 0xF}; // kludge
			if (color == Deco::Color::Any) {
				continue;
			}
			colorCount.try_emplace(color, 0); // initialize a new key's value
			colorCount[color] += 1;
		}
		
		for (const auto& count : colorCount) {
			if (count.second % 2 != 0) {
				return true;
			}
		}
	}
	return false;
}

//Place the given amount of triangles with the given color. targetCount is how many triangles are in the symbol, or 0 for random
bool Generate::place_triangles(DecoPair dp)
//Deco::Color color, int amount, int targetCount
{
	int amount = dp.second;
	auto color = dp.first.color;
	int targetCount = dynamic_cast<Deco::Triangle&>(dp.first).quantity; 
	if (_panel->id == 0x033EA) { //Keep Yellow Pressure Plate
		int count = count_sides({ 1, 3 });
		set({ 1, 3 }, Deco::Triangle(color, count));
		_openpos.erase({ 1, 3 });
	}
	std::set<Point> open = _openpos;
	int count1 = 0, count2 = 0, count3 = 0;
	while (amount > 0) {
		if (open.size() == 0)
			return false;
		Point pos = Utilities::pick_random(open);
		int count = count_sides(pos);
		open.erase(pos);
		if (_panel->_symmetry) {
			open.erase(get_sym_point(pos));
		}
		if (count == 0 || targetCount && count != targetCount) continue;
		if (hasFlag(Config::TreehouseLayout) || _panel->id == 0x289E7) {
			// If the block is adjacent to a start or exit, don't place a triangle there
			// Triangles are poorly defined for half-lines, after all.
			bool found = false;
			for (Point dir : _DIRECTIONS1) {
				if (_starts.contains(pos + dir) || _exits.contains(pos + dir)) {
					found = true;
					break;
				}
			}
			if (found) continue;
		}
		if (count == 1) {
			if (!targetCount && count1 * 2 > count2 + count3 && Random::rand() % 2 == 0) continue;
			++count1;
		}
		if (count == 2) {
			if (!targetCount && count2 * 2 > count1 + count3 && Random::rand() % 2 == 0) continue;
			++count2;
		}
		if (count == 3) {
			if (!targetCount && count3 * 2 > count1 + count2 && Random::rand() % 2 == 0) continue;
			++count3;
		}
		set(pos, Deco::Triangle(color, count));
		_openpos.erase(pos);
		--amount;
	}
	return true;
}

//Count how many sides are touched by the line (for the triangles)
int Generate::count_sides(Point pos) const
{
	int count = 0;
	for (Point dir : _DIRECTIONS1) {
		Point p = pos + dir;
		if (!off_edge(p) && get(p) == Deco::kPath) {
			++count;
		}
	}
	return count;
}

//Place the given amount of arrows with the given color. targetCount is how many ticks on the arrows, or 0 for random
//The color won't actually be reflected, ArrowRecolor must be used instead
bool Generate::place_arrows(DecoPair dp)
//Deco::Color color, int amount, int targetCount
{
	int amount = dp.second;
	int targetCount = dynamic_cast<Deco::Arrow&>(dp.first).quantity;
	std::set<Point> open = _openpos;
	while (amount > 0) {
		if (open.size() == 0)
			return false;
		Point pos = Utilities::pick_random(open);
		open.erase(pos);
		if (pos.first == _panel->width / 2 || _panel->_pillarWidth > 0 && pos.first == _panel->width / 2 - 1)
			continue; //Because of a glitch where arrows in the center column won't draw right
		int fails = 0;
		while (fails++ < 20) { //Keep picking random directions until one works // dangerous post-increment. consider with care
			int choice = (_parity == -1 ? Random::rand() % 8 : Random::rand() % 4);
			Point dir = _8DIRECTIONS2[choice];
			if (_panel->_pillarWidth > 0 && dir.second == 0) continue; //Sideways arrows on a pillar would wrap forever
			int count = count_crossings(pos, dir);
			if (count == 0 || count > 3 || targetCount && count != targetCount) continue;
			if (dir.first < 0 && count == (pos.first + 1) / 2 || dir.first > 0 && count == (_panel->width - pos.first) / 2 ||
				dir.second < 0 && count == (pos.second + 1) / 2 || dir.second > 0 && count == (_panel->height - pos.second) / 2 && Random::rand() % 10 > 0)
				continue; //Make it so that there will be some possible edges that aren't passed, in the vast majority of cases
			set(pos, Deco::Arrow(dp.first.color, count) | (choice << 16));
			_openpos.erase(pos);
			--amount;
			break;
		}
	}
	return true;
}

//Count the number of times the given vector is passed through (for the arrows)
int Generate::count_crossings(Point pos, Point dir) const
{
	pos = pos + dir / 2;
	int count = 0;
	while (!off_edge(pos)) {
		if (get(pos) == Deco::kPath) ++count;
		pos = pos + dir;
	}
	return count;
}

//Place the given amount of erasers with the given colors. eraseSymbols are the symbols that were erased
// preemptively earlier in the procedure
bool Generate::place_erasers(
	const std::vector<Deco::Color>& colors,
	const std::vector<Deco::Deco>& eraseSymbols,
  /*out*/ std::vector<Point>& placedSymbols)
{
	placedSymbols.clear();
	std::set<Point> open = _openpos;
	if (_panel->id == 0x288FC && hasFlag(Generate::Config::DisableWrite)) {
		//For the puzzle in the cave with a pillar in middle
		open.erase({ 5, 5 });
	}
	int amount = static_cast<int>(colors.size());
	while (amount > 0) {
		if (open.empty()) {
			// nowhere left to place the erased symbol back on the board
			return false;
		}
		const auto& toErase = eraseSymbols[amount - 1];
		Deco::Color color = colors[amount - 1];

		// A point is picked randomly from the remaining set of open points.
		Point pos = Utilities::pick_random(open);
		std::set<Point> region = get_region(pos);
		std::set<Point> open2;
		for (Point p : region) {
			if (open.erase(p) > 0) {
				// Move each open point in the region to a new set.
				// This region will not be considered for future Erasers
				open2.insert(p);
			}
		}
		if (!_splitPoints.empty()) {
			//Make sure this is one of the split point regions
			bool found = false;
			for (Point p : _splitPoints) {
				if (region.contains(p)) {
					found = true;
					break;
				}
			}
			if (!found) {
				// Start a new loop, but with the just-checked region disqualified
				continue; // while (amount > 0)
			} 
		}
		// At this point, the region where the Eraser and extra symbol should go has been ID'd.

		if (_panel->id == 0x288FC && hasFlag(Generate::Config::DisableWrite) &&
			!region.contains({5, 5})) {
			// For the puzzle in the cave with a pillar in middle
			// Why not just pick that region from the get-go if we know it's hard-coded?
			continue;
		}
		if (hasFlag(Config::MakeStonesUnsolvable)) {
			std::set<Point> valid;
			for (Point p : open2) {
				//Try to make a checkerboard pattern with the stones
				if ((!off_edge(p + Point(2, 2)) && 
						  get(p + Point(2, 2)) == toErase &&
						  get(p + Point(0, 2)) != Deco::kEmpty &&
						  get(p + Point(0, 2)) != toErase&&
						  get(p + Point(2, 0)) != Deco::kEmpty &&
						  get(p + Point(2, 0)) != toErase) ||
					(!off_edge(p + Point(-2, 2)) &&
						  get(p + Point(-2, 2)) == toErase &&
						  get(p + Point(0, 2)) != Deco::kEmpty &&
						  get(p + Point(0, 2)) != toErase &&
						  get(p + Point(-2, 0)) != Deco::kEmpty &&
						  get(p + Point(-2, 0)) != toErase) ||
					(!off_edge(p + Point(2, -2)) &&
						  get(p + Point(2, -2)) == toErase &&
						  get(p + Point(0, -2)) != Deco::kEmpty &&
						  get(p + Point(0, -2)) != toErase &&
						  get(p + Point(2, 0)) != Deco::kEmpty &&
						  get(p + Point(2, 0)) != toErase) ||
					(!off_edge(p + Point(-2, -2)) &&
						  get(p + Point(-2, -2)) == toErase &&
						  get(p + Point(0, -2)) != Deco::kEmpty &&
						  get(p + Point(0, -2)) != toErase &&
						  get(p + Point(-2, 0)) != Deco::kEmpty &&
						  get(p + Point(-2, 0)) != toErase))
					valid.insert(p);
			}
			// Update open2 to only the subset that passed the filter.
			open2 = valid;
		}
		if ((open2.empty() ||
			 _splitPoints.empty() && open2.size() == 1) &&
			!(toErase.symbol == Deco::Symbol::Dot)) {
			// If there isn't *still* room for both symbols (dots take less space)
			// then try a different region.
			continue;
		}

		bool canPlace = false;
		if (toErase.symbol == Deco::Symbol::Stone) {
			// Normally, placing a stone of this color in this region would break the puzzle. Good.
			canPlace = !can_place_stone(region, (toErase.color));
		} else if (toErase.symbol == Deco::Symbol::Star) {
			canPlace = (count_color(region, (toErase.color)) + (color == (toErase.color) ? 1 : 0) != 1);
		} else {canPlace = true;}
		if (!canPlace) {
			// if not can place when can not place stone... These double/triple negatives are a bit extra.
			continue;
		} 

		if (toErase.symbol == Deco::Symbol::Stone || toErase.symbol == Deco::Symbol::Star) {
			set(pos, toErase);
		}

		else if (toErase.symbol == Deco::Symbol::Dot) { //Find an open edge to put the dot on
			auto eraseDot = dynamic_cast<const Deco::Dot&>(toErase);
			std::set<Point> openEdge;
			for (Point p : region) {
				for (Point dir : _8DIRECTIONS1) {
					if (eraseDot.location == Deco::Location::Intersection &&
						(dir.first == 0 || dir.second == 0)) {
						continue;
					}
					Point p2 = p + dir;
					if (get(p2) == 0 && (hasFlag(Config::FalseParity) || can_place_dot(p2, false))) {
						openEdge.insert(p2);
					}
				}
			}
			if (openEdge.empty()){
				continue;
			}
			pos = Utilities::pick_random(openEdge);

			switch (pos.spot()) {
			case Point::Row:
				set(pos, Deco::Dot(Deco::Location::Row, eraseDot.parity));
				break;
			case Point::Column:
				set(pos, Deco::Dot(Deco::Location::Column, eraseDot.parity));
				break;
			case Point::Intersection:
				set(pos, Deco::Dot(Deco::Location::Intersection, eraseDot.parity));
				break;
			default:
				throw std::exception("Not sure how you got here, in the deep dark of place_erasers() without a valid position for your dot.");
			}
		}

		else if (toErase.symbol == Deco::Symbol::Poly) {
			int symbol = 0; //Make a random shape to cancel
			while (symbol == 0) {
				std::set<Point> area = _gridpos;
				int shapeSize;
				if ((toErase & Decoration::Negative) || hasFlag(Config::SmallShapes)) shapeSize = Random::rand() % 3 + 1;
				else {
					shapeSize = Random::rand() % 5 + 1;
					if (shapeSize < 3)
						shapeSize += Random::rand() % 3;
				}
				Shape shape = generate_shape(area, Utilities::pick_random(area), shapeSize);
				if (shape.size() == region.size()) continue; //Don't allow the shape to match the region, to guarantee it will be wrong
				symbol = make_shape_symbol(shape, toErase & Decoration::Can_Rotate, toErase & Decoration::Negative);
			}
			set(pos, symbol | (toErase & 0xf));
		}

		else if (toErase.symbol == Deco::Symbol::Triangle) {
			if (hasFlag(Config::TreehouseLayout) || _panel->id == 0x289E7) { //If the block is adjacent to a start or exit, don't place a triangle there
				bool found = false;
				for (Point dir : _DIRECTIONS1) {
					if (_starts.contains(pos + dir) || _exits.contains(pos + dir)) {
						found = true;
						break;
					}
				}
				if (found) continue;
			}
			int count = count_sides(pos);
			if (count == 0) count = Random::rand() % 3 + 1;
			else count = (count + (Random::rand() & 1)) % 3 + 1;
			set(pos, toErase | (count << 16));
		}

		if (toErase.symbol != Deco::Symbol::Dot) {
			_openpos.erase(pos);
			open2.erase(pos);
		}

		//Place the eraser at a random open point
		if (_splitPoints.empty()) {
			pos = Utilities::pick_random(open2);
		} else {
			// Try to keep erasers separated.
			for (Point p : _splitPoints) {
				if (region.contains(p)) {
					pos = p;
					break;
				}
			}
		}
		if (_panel->id == 0x288FC && hasFlag(Generate::Config::DisableWrite)) {
			if (get(5, 5) != 0) return false;
			pos = { 5, 5 }; //For the puzzle in the cave with a pillar in middle
		}
		set(pos, Deco::Eraser(color));
		placedSymbols.push_back(pos);
		_openpos.erase(pos);
		--amount;
	}
	return true;
}

//For the mountain floor puzzle on hard mode. Combine two tetris shapes into one
bool Generate::combine_shapes(std::vector<Shape>& shapes)
{
	for (int i = 0; i < shapes.size(); ++i) {
		for (int j = 0; j < shapes.size(); ++j) {
			if (i == j) continue;
			if (shapes[i].size() + shapes[j].size() <= 5) continue;
			if (shapes[i].size() > 5 || shapes[j].size() > 5) continue;
			//Look for adjacent points
			for (Point p1 : shapes[i]) {
				for (Point p2 : shapes[j]) {
					for (Point dir : _DIRECTIONS2) {
						if (p1 + dir == p2) {
							//Combine shapes
							for (Point p : shapes[i]) shapes[j].insert(p);
							//Make sure there are no holes
							std::set<Point> area = _gridpos;
							for (Point p : shapes[j]) area.erase(p);
							while (area.size() > 0) {
								std::set<Point> region;
								std::vector<Point> check;
								check.push_back(*area.begin());
								region.insert(*area.begin());
								while (check.size() > 0) {
									Point p = check[check.size() - 1];
									check.pop_back();
									for (Point dir : _DIRECTIONS1) {
										Point p2 = p + dir * 2;
										if (area.contains(p2) && region.insert(p2).second) {
											check.push_back(p2);
										}
									}
								}
								bool connected = false;
								for (Point p : region) {
									if (p.first == 1 || p.second == 1 || p.first == _panel->width - 2 || p.second == _panel->height - 2) {
										connected = true;
										break;
									}
								}
								if (!connected) return false;
								for (Point p : region) area.erase(p);
							}
							shapes.erase(shapes.begin() + i);
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}
