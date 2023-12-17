#include <map>
#include <vector>
#include <set>

inline const std::vector<float> horizontalLine = { 0.9f, 0.5f, 0.1f, 0.5f };
inline const std::vector<float> verticalLine = { 0.5f, 0.9f, 0.5f, 0.1f };
inline const std::vector<float> pillarLine = { 0.9f, 0.5f, 0.7f, 0.5f, 0.5f, 0.5f, 0.3f, 0.5f, 0.1f, 0.5f };
inline const std::vector<float> offsetline = { 0.2f, 0.9f, 0.2f, 0.1f };
inline const std::vector<float> shortLineTop = { 0.5f, 0.9f, 0.5f, 0.7f };

inline const std::map<int, std::vector<float>> skip_specialLine = {
	{0x18076, verticalLine},
	{0x28B39, verticalLine},
	{0x193A7, verticalLine},
	{0x193AA, verticalLine},
	{0x193AB, verticalLine},
	{0x193A6, verticalLine},
	{0x0A15C, horizontalLine},
	{0x09FFF, horizontalLine},
	{0x0A15F, horizontalLine},
	{0x33Af5, offsetline},
	{0x33AF7, offsetline},
	{0x09F6E, shortLineTop},
	{0x0383A, pillarLine},
	{0x09E56, pillarLine},
	{0x09E5A, pillarLine},
	{0x33961, pillarLine},
	{0x0383D, pillarLine},
	{0x0383F, pillarLine},
	{0x03859, pillarLine},
	{0x339BB, pillarLine},
	{0x09DD5, pillarLine},
	{0x1C31A, pillarLine},
	{0x1C319, pillarLine},
};

inline const std::set<int> dont_power = {
	0x01983,
	0x01987, // Peekaboo

	// Challenge Puzzles
	0x0A332, 0x0088E, 0x00BAF, 0x00BF3, 0x00CB9, 0x00CA1, 0x00C80, 0x00C68, 0x00C59, 0x00C22, 0x034F4, 0x034EC, 0x1C31A, 0x1C319,

	0x17D8F, // Treehouse Yellow 2
	0x17D74, // Treehouse Yellow 3
	0x17DAC, // Treehouse Yellow 4
	0x17D9E, // Treehouse Yellow 5
	0x17DB9, // Treehouse Yellow 6
	0x17D9C, // Treehouse Yellow 7
	0x17DC2, // Treehouse Yellow 8
	0x17DC4, // Treehouse Yellow 9
	0x17DC7, // Treehouse First Purple 2
	0x17CE4, // Treehouse First Purple 3
	0x17D2D, // Treehouse First Purple 4
	0x17D6C, // Treehouse First Purple 5
	0x17D99, // Treehouse Second Purple 2
	0x17DAA, // Treehouse Second Purple 3
	0x17D97, // Treehouse Second Purple 4
	0x17BDF, // Treehouse Second Purple 5
	0x17D91, // Treehouse Second Purple 6
	0x17DC6, // Treehouse Second Purple 7
	0x17DB5, // Treehouse Left Orange 2
	0x17DB6, // Treehouse Left Orange 3
	0x17DC0, // Treehouse Left Orange 4
	0x17DD7, // Treehouse Left Orange 5
	0x17DD9, // Treehouse Left Orange 6
	0x17DB8, // Treehouse Left Orange 7
	0x17DDC, // Treehouse Left Orange 8
	0x17DDE, // Treehouse Left Orange 10
	0x17DE3, // Treehouse Left Orange 11
	0x17DEC, // Treehouse Left Orange 12
	0x17DAE, // Treehouse Left Orange 13
	0x17DB0, // Treehouse Left Orange 14
	0x17DDB, // Treehouse Left Orange 15
	0x17DB4, // Treehouse Right Orange 2
	0x17D8C, // Treehouse Right Orange 3
	0x17DCD, // Treehouse Right Orange 5
	0x17DB2, // Treehouse Right Orange 6
	0x17DCC, // Treehouse Right Orange 7
	0x17DCA, // Treehouse Right Orange 8
	0x17D8E, // Treehouse Right Orange 9
	0x17DB1, // Treehouse Right Orange 11
	0x17DA2, // Treehouse Right Orange 12
	0x17E4D, // Treehouse Green 2
	0x17E4F, // Treehouse Green 3
	0x17E5B, // Treehouse Green 5
	0x17E5F, // Treehouse Green 6
	0x17E61, // Treehouse Green 7
	0x17DD1, // Treehouse Left Orange 9
	0x17CE3, // Treehouse Right Orange 4
	0x17DB7, // Treehouse Right Orange 10
	0x17E52, // Treehouse Green 4
};

inline const std::set<int> skip_noLine = {
	0x03629, // Tutorial Gate Open

	0x0C373, // Patio Floor
	0x17C31, // Desert Final Room Transparent Reflection
	0x28A69, // Town Lattice

	0x15ADD, // River Vault

	0x00290, 0x00038, 0x00037, //Monastery Lattice

	0x033EA, 0x01BE9, 0x01CD3, 0x01D3F, //Keep PPs

	0x09F7D, 0x09FDC, 0x09FF7, 0x09F82, 0x09FF8, 0x09D9F, 0x09DA1, 0x09DA2, 0x09DAF, 0x0A010, 0x0A01B, 0x0A01F, 0x17E63, 0x17E67,

	0x09FCC, 0x09FCE, 0x09FCF, 0x09FD0, 0x09FD1, 0x09FD2, // Same solution set

	0x17E52, 0x17DD1, 0x17CE3, 0x17DB7, // Treehouse Directional Bridge Panels

	0x17C34, // Mountaintop Perspective 3-way

	0x09E39, 0x09E86, 0x09ED8, // Light Bridge Controls

	0x17CAA, // River to Monastery shortcut lattice
};

inline const std::set<int> cutoutPanels = {
	0x28a69, 0x15ADD, 0x00290, 0x00038, 0x00037, 0x17caa, 0x09F7D, 0x09FDC, 0x09FF7, 0x09F82, 0x09D9F, 0x09FF8, 0x09DA1, 0x09DA2, 0x09DAF, 0x0A010, 0x0A01B, 0x0A01F, 0x17E63, 0x17E67,
};

inline const std::set<int> dont_touch_panel_at_all = {
	0x00064, 0x00182, // Entry Panels

	0x03505, // Gate Close

	0x00C09, // Challenge Maze
	0x00CDB, 0x0051F, 0x00524, 0x00CD4, // Challenge reallocating
};

// Unskippable panels it would be nice to be able to skip
inline const std::set<int> apology_panels = {
	0x00CDB, 0x0051F, 0x00524, 0x00CD4, // Challenge reallocating
};

inline const std::set<int> skip_completelyExclude = {
	0x0c373, // Patio Floor (EP)

	0x00064, 0x00182, // Entry Panels
	0x03481, 0x0339E, 0x03535, 0x03702, 0x03542, 0x0356B, 0x2FAF6, //Vault Boxes
	0x17CC8, 0x17CA6, 0x0A054, 0x17CDF, 0x09DB8, 0x17C95, //Boat Spawns
	0x34D96, 0x34C7F, //Boat Map & Speed
	0x0360D, 0x03608, 0x032F5, 0x17CA4, 0x03616, 0x09DE0, 0x03615, 0x03613, //Lasers except Shadows & Keep

	0x0042D, // Mountaintop River Shape

	//Controls, e.g. Door Handles

	0x09FA0, 0x09F86, 0x0C339, 0x09FAA, 0x0A249, 0x1C2DF, 0x1831E, 0x1C260, 0x1831C, 0x1C2F3, 0x1831D, 0x1C2B1, 0x1831B, 0x0A015,  
	0x17CC4, 0x275ED, 0x03678, 0x03679, 0x03675, 0x03676, 0x17CAC, 0x03852, 0x03858, 0x38663, 0x275FA, 0x334DB, 0x334DC, 0x09E49,
	0x0361B, 0x334D8, 0x2896A, 0x17D02, 0x03713, 0x00B10, 0x00C92, 0x09D9B, 0x17CAB, 0x337FA, 0x0A099, 0x34BC5, 0x34BC6, 0x17CBC,
	0x2700B, 0x09EEB, 0x334E1, 0x27732, 0x2773D, 0x3D9A6, 0x3D9A7, 0x3C113, 0x3C114, 0x3D9AA, 0x3D9A8, 0x3D9A9,

	//Video Inputs
	0x00815, 0x03553, 0x03552, 0x0354E, 0x03549, 0x0354F, 0x03545,

	//Needs to stay the same!

	0x03505, // Tutorial Gate Close
	0x09F98, // Town Laser Redirect
	0x0A079, // Bunker Elevator
	0x00609, 0x17E2B, 0x18488, // Swamp Bridge Controls
	0x037FF, // Treehouse Bridge to Keep
	0x09F7F, // Mountaintop Box Open
	0x335AB, 0x335AC, 0x3369D, // Elevators in Caves 

	//Complex issues arise, but could be changed
	0x034E4, 0x034E3, //Sound room
	0x0A3B5, // Tutorial Multi-Solution


	// Doesn't seem to work at all
	0x0C335, // Tutorial Pillar

	0x00C09, // Challenge Maze Panel
	0x00CDB, 0x0051F, 0x00524, 0x00CD4, // Challenge reallocating
};

// Puzzles that don't make sense to skip on Normal but are useful to skip on Hard.
inline const std::set<int> skip_excludeOnNormal = {
	0x181F5, // Swamp Rotating Bridge
	0x334D8, // Town RGB Control
};

// Puzzles that don't make sense to skip on Hard but are useful to skip on Normal.
inline const std::set<int> skip_excludeOnHard = {
};

// Non-trivial puzzles that have multiple gameplay-relevant solutions and should continue to be skippable even after the puzzle has been solved.
//   Note that there are puzzles in here that are currently excluded or special-cased in some manner; they're here for completeness's sake so
//   that if we ever decide to un-exclude them we'll already have them represented here and (hopefully) not run into any issues there.
inline const std::set<int> skip_multisolvePuzzles = {
	// Bunker
	0x0A079, // Bunker Elevator

	// Caves
	0x17FA2, // Entry "discard" triangle puzzle
	0x335AB, 0x335AC, 0x3369D, // Elevators

	// Keep
	0x0A3A8, 0x0A3B9, 0x0A3BB, 0x0A3AD, // Pressure plate resets (needed for EPs)

	// Mountain
	0x09FC1, 0x09F8E, 0x09F01, 0x09EFF, // Metapuzzle small panels <-- TODO: These are fully "solved" once the big puzzle has been solved.
	0x09E39, 0x09E86, 0x09ED8, // Light Bridge Controls
	0x09FCC, 0x09FCE, 0x09FCF, 0x09FD0, 0x09FD1, 0x09FD2, // Same solution set

	// Quarry
	0x03678, 0x03675, 0x03679, 0x03676, // Mill ramp/lift controls
	0x03852, 0x03858, // Boathouse ramp controls
	0x275FA, // Hook

	// Swamp
	0x00609, 0x17E2B, 0x18488, // Swamp Bridge Controls
	0x181F5, // Swamp Rotating Bridge

	// Theater
	0x00815, // Video input

	// Town
	0x334D8, // RGB Control
	0x079DF, // Shadow/Perspective/Reflection Triple Solve <-- TODO: This is fully solved once all three of its solutions have been entered.
	0x2896A, // Rooftop bridge

	// Treehouse
	0x17E52, 0x17DD1, 0x17CE3, 0x17DB7, // Treehouse Directional Bridge Panels

	// Tutorial
	0x0A3B5, // Tutorial back left
	0x03629, // Tutorial gate
};

inline const std::set<int> skip_specialCase = {
	0x09E86, 0x09ED8,

	0x00A52, 0x00A57, 0x00A5B, 0x00A61, 0x00A64, 0x00A68, //Symmetry Laser Panels

	0x0A3A8, 0x0A3B9, 0x0A3BB, 0x0A3AD, // Pressure plate reset lines

	0x09FC1, 0x09F8E, 0x09F01, 0x09EFF, // Metapuzzle

	0x181F5, // Swamp Rotating Bridge
	0x334D8, // Town RGB Control

	// Challenge Puzzles
	0x0A332, 0x0088E, 0x00BAF, 0x00BF3, 0x00CB9, 0x00CA1, 0x00C80, 0x00C68, 0x00C59, 0x00C22, 0x034F4, 0x034EC, 0x1C31A, 0x1C319,
	0x01983, 0x01987, // Peekaboo
};

inline const std::set<int> skip_dontRandomize = {
	// Challenge Puzzles
	0x0A332, 0x0088E, 0x00BAF, 0x00BF3, 0x00CB9, 0x00CA1, 0x00C80, 0x00C68, 0x00C59, 0x00C22, 0x034F4, 0x034EC, 0x1C31A, 0x1C319,
	0x01983, 0x01987, // Peekaboo
};

inline const std::set<int> very_thin_panels = {
	0x09FA0, 0x09F86, 0x0C339, 0x0A249, 0x1C2DF, 0x1831E, 0x1C260, 0x1831C, 0x1C2F3, 0x1831D, 0x1C2B1, 0x1831B, 0x0A015,
	0x275ED, 0x17CAC, 0x38663, 0x334DB, 0x334DC, 0x09E49,
	0x0361B, 0x03713, 0x17CAB, 0x337FA, 0x0A099, 0x34BC5, 0x34BC6, 0x17CBC,
	0x2700B, 0x334E1, 0x27732, 0x2773D, 0x3D9A6, 0x3D9A7, 0x3C113, 0x3C114, 0x3D9AA, 0x3D9A8, 0x3D9A9,

	0x0360D, 0x03608, 0x032F5, 0x17CA4, 0x03616, 0x09DE0, 0x03615, 0x03613,
};

inline const std::set<int> fairly_thin_panels = {
	0x335AB,0x335AC,0x3369D, // Mountain & Caves Elevators
};

inline const std::set<int> wide_panels = {
	0x17CC8, 0x17CA6, 0x0A054, 0x17CDF, 0x09DB8, 0x17C95, //Boat Spawns
	0x03858,
	0x09E39, 0x09E86, 0x09ED8 // Light Bridges
};

// Canonized "panel sets" that get collected out together.
// The key is the final panel in the panel set, that is a check.
// The value is all the panels in the corresponding "panel set", IN ORDER (this is very important!!!)
inline const std::map<int, std::vector<int>> collectTogether = {
	{ 0x002C2, { 0x00293, 0x00295, }}, // Tutorial Front Left
	{ 0x0C373, { 0x0C335, }}, // Tutorial Patio Floor
	{ 0x03481, { 0x033D4, }}, // Tutorial Vault
	{ 0x00061, { 0x0005D, 0x0005E, 0x0005F, 0x00060, }}, // Outside Tutorial Shed Row 5
	{ 0x00021, { 0x018AF, 0x0001B, 0x012C9, 0x0001C, 0x0001D, 0x0001E, 0x0001F, 0x00020, }}, // Outside Tutorial Tree Row 9
	{ 0x0005C, { 0x00086, 0x00087, 0x00059, 0x00062, }}, // Glass Factory Back Wall 5
	{ 0x00083, { 0x0008D, 0x00081, }}, // Glass Factory Front 3
	{ 0x0343A, { 0x00084, 0x00082, }}, //Glass Factory Melting 3
	{ 0x00026, { 0x00022, 0x00023, 0x00024, 0x00025, }}, // Symmetry Island Right 5
	{ 0x00079, { 0x0007C, 0x0007E, 0x00075, 0x00073, 0x00077, }}, // Symmetry Island Back 6
	{ 0x00076, { 0x00065, 0x0006D, 0x00072, 0x0006F, 0x00070, 0x00071, }}, // Symmetry Island Left 7
	{ 0x00B8D, { 0x009B8, 0x003E8, 0x00A15, 0x00B53, }}, // Symmetry Island Scenery Outlines 5
	{ 0x00A5B, { 0x00A52, 0x00A57, }}, // Symmetry Island Laser Yellow 3
	{ 0x00A68, { 0x00A61, 0x00A64, }}, // Symmetry Island Laser Blue 3
	{ 0x00055, { 0x00143, 0x0003B, }}, // Orchard Apple Tree 3 (Doors)
	{ 0x032FF, { 0x00143, 0x0003B, 0x00055, 0x032F7, }}, // Orchard Apple Tree 5
	{ 0x09F92, { 0x00698, 0x0048F, }}, // Desert Surface 3 (Extra)
	{ 0x09F94, { 0x00698, 0x0048F, 0x09F92, 0x0A036, 0x09DA6, 0x0A049, 0x0A053, }}, // Desert Surface 8
	{ 0x0339E, { 0x0CC7B }}, // Desert Vault
	{ 0x0A02D, { 0x00422, 0x006E3, }}, // Desert Light Room 3
	{ 0x18313, { 0x00C72, 0x0129D, 0x008BB, 0x0078D, }}, // Desert Pond Room 5
	{ 0x18076, { 0x04D18, 0x01205, 0x181AB, 0x0117A, 0x17ECA, }}, // Desert Flood Room 6
	{ 0x012D7, { 0x17C31, }}, // Desert Final Hexagonal
	{ 0x0A15F, { 0x0A15C, 0x09FFF, }}, // Desert Final Bent 3
	{ 0x014E8, { 0x00E0C, 0x01489, 0x0148A, 0x014D9, 0x014E7, }}, // Quarry Stoneworks Lower Row 6
	{ 0x014E9, { 0x00557, 0x005F1, 0x00620, 0x009F5, 0x0146C, 0x3C12D, 0x03686, }}, // Quarry Stoneworks Upper Row 8
	{ 0x021AE, { 0x021B3, 0x021B4, 0x021B0, 0x021AF, }}, // Quarry Boathouse Front Row 5
	{ 0x09DB4, { 0x021B5, 0x021B6, 0x021B7, 0x021BB, 0x09DB5, 0x09DB1, 0x3C124, 0x09DB3, }}, // Quarry Boathouse Back First Row 8
	{ 0x0A3D0, { 0x0A3CB, 0x0A3CC, }}, // Quarry Boathouse Back Second Row 3
	{ 0x198BF, { 0x198B5, 0x198BD, }}, // Shadows Intro 3 (Doors)
	{ 0x0A8DC, { 0x198B5, 0x198BD, 0x198BF, 0x19771, }}, // Shadows Intro 5 (Doors)
	{ 0x0A8E0, { 0x198B5, 0x198BD, 0x198BF, 0x19771, 0x0A8DC, 0x0AC74, 0x0AC7A, }}, // Shadows Intro 8
	{ 0x1972F, { 0x386FA, 0x1C33F, 0x196E2, 0x1972A, 0x19809, 0x19806, 0x196F8, }}, // Shadows Far 8
	{ 0x197E5, { 0x19797, 0x1979A, 0x197E0, 0x197E8, }}, // Shadows Near 5
	{ 0x03535, { 0x00AFB, }}, // Shipwreck Vault
	{ 0x00037, { 0x00290, 0x00038, }}, // Monastery Outside 3
	{ 0x193A6, { 0x193A7, 0x193AA, 0x193AB, }}, // Monastery Inside 4
	{ 0x28AC1, { 0x2899C, 0x28A33, 0x28ABF, 0x28AC0, }}, // Town Wooden Roof Lower 5
	{ 0x28ACC, { 0x28AC7, 0x28AC8, 0x28ACA, 0x28ACB, }}, // Town Red Rooftop 5
	{ 0x034E3, { 0x034E4, }}, // Sound Room Right
	{ 0x002C6, { 0x002C4, 0x00767, }}, // Jungle First Row 3
	{ 0x002C7, { 0x0070E, 0x0070F, 0x0087D, }}, // Jungle Second Row 4
	{ 0x014B2, { 0x0026D, 0x0026E, 0x0026F, 0x00C3F, 0x00C41 }}, // Jungle Popup Wall 6
	{ 0x03702, { 0x15ADD, }}, // Jungle Vault
	{ 0x09FF8, { 0x09F7D, 0x09FDC, 0x09FF7, 0x09F82, }}, // Bunker Intro Left 5
	{ 0x09DAF, { 0x09D9F, 0x09DA1, 0x09DA2, }}, // Bunker Intro Back 4
	{ 0x0A01F, { 0x0A010, 0x0A01B, }}, // Bunker Glass Room 3
	{ 0x17E67, { 0x17E63, }}, // Bunker UV Room 2
	{ 0x0056F, { 0x00469, 0x00472, 0x00262, 0x00474, 0x00553, }}, // Swamp Intro Front 5
	{ 0x181A9, { 0x00390, 0x010CA, 0x00983, 0x00984, 0x00986, 0x00985, 0x00987, }}, // Swamp Intro Back 8
	{ 0x00990, { 0x00982, 0x0097F, 0x0098F, }}, // Swamp Platform Row 4
	{ 0x17C0E, { 0x17C0D, }}, // Swamp Platform Shortcut Right
	{ 0x00596, { 0x00002, 0x00004, 0x00005, 0x013E6, }}, // Swamp Cyan Underwater 5
	{ 0x009A1, { 0x00999, 0x0099D, 0x009A0, }}, // Swamp Between Bridges Near Row 4
	{ 0x0000A, { 0x00007, 0x00008, 0x00009, }}, // Swamp Between Bridges Far Row 4
	{ 0x014D1, { 0x00001, 0x014D2, 0x014D4, }}, // Swamp Red Underwater 4
	{ 0x00E3A, { 0x003B2, 0x00A1E, 0x00C2E, }}, // Swamp Beyond Rotating Bridge 4
	{ 0x00006, { 0x009AB, 0x009AD, 0x009AE, 0x009AF , }}, // Swamp Blue Underwater 5
	{ 0x17C02, { 0x17C05, }}, // Swamp Laser Shortcut Right
	{ 0x17DC4, { 0x17D72, 0x17D8F, 0x17D74, 0x17DAC, 0x17D9E, 0x17DB9, 0x17D9C, 0x17DC2, }}, // Yellow Bridge
	{ 0x17D6C, { 0x17DC8, 0x17DC7, 0x17CE4, 0x17D2D, }}, // First Purple Bridge
	{ 0x17DA2, { 0x17D88, 0x17DB4, 0x17D8C, 0x17DCD, 0x17DB2, 0x17DCC, 0x17DCA, 0x17D8E, 0x17DB1, 0x17CE3, 0x17DB7, }}, // Right Orange Bridge
	{ 0x17DC6, { 0x17D9B, 0x17D99, 0x17DAA, 0x17D97, 0x17BDF, 0x17D91, }}, // Second Purple Bridge
	{ 0x17DDB, { 0x17DB3, 0x17DB5, 0x17DB6, 0x17DC0, 0x17DD7, 0x17DD9, 0x17DB8, 0x17DD1, 0x17DDC, 0x17DDE, 0x17DE3, 0x17DEC, 0x17DAE, 0x17DB0, }}, // Left Orange Bridge
	{ 0x17E61, { 0x17E3C, 0x17E4D, 0x17E4F, 0x17E5B, 0x17E5F, 0x17E52, }}, // Green Bridge
	{ 0x03542, { 0x002A6, }}, // Mountainside Vault
	{ 0x09E7B, { 0x09E7A, 0x09E71, 0x09E72, 0x09E69, }}, // Mountain Floor 1 Right Row 5
	{ 0x09E6B, { 0x09E73, 0x09E75, 0x09E78, 0x09E79, 0x09E6C, 0x09E6F, }}, // Mountain Floor 1 Left Row 7
	{ 0x09F6E, { 0x33AF5, 0x33AF7, }}, // Mountain Floor 1 Back Row 3
	{ 0x09EAF, { 0x09EAD, }}, // Mountain Floor 1 Trash Pillar 2
	{ 0x09FD8, { 0x09FD3, 0x09FD4, 0x09FD6, 0x09FD7, }}, // Mountain Floor 2 Near Row 5
	{ 0x09FD2, { 0x09FCC, 0x09FCE, 0x09FCF, 0x09FD0, 0x09FD1, }}, // Mountain Floor 2 Far Row 6
	{ 0x09FDA, { 0x09FC1, 0x09F8E, 0x09F01, 0x09EFF, }}, // Mountain Bottom Floor Giant Puzzle
	{ 0x33961, { 0x0383A, 0x09E56, 0x09E5A, }}, // Mountain Final Room Right Pillar 4
	{ 0x339BB, { 0x0383D, 0x0383F, 0x03859, }}, // Mountain Final Room Left Pillar 4
	{ 0x006FE, { 0x00190, 0x00558, 0x00567, }}, // Caves Blue Tunnel Right First 4
	{ 0x0097E, { 0x008B8, 0x00973, 0x0097B, 0x0097D, }}, // Caves Blue Tunnel Left Second 5
	{ 0x00998, { 0x00994, 0x334D5, 0x00995, 0x00996, }}, // Caves Blue Tunnel Right Second 5
	{ 0x0A2EA, { 0x0A16B, 0x0A2CE, 0x0A2D7, 0x0A2DD, }}, // Caves Upstairs Left Row 5
	{ 0x0006C, { 0x0008F, 0x0006B, 0x0008B, 0x0008C, 0x0008A, 0x00089, 0x0006A, }}, // Caves Right Upstairs Left Row 8
	{ 0x00029, { 0x00027, 0x00028, }}, // Caves Right Upstairs Right Row 3
	{ 0x0356B, { 0x0A332, }}, // Challenge Vault -> Timer
};