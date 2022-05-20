#pragma once

#include <memory>
#include <set>
#include <map>
#include "nlohmann\json.hpp"
#include "..\Randomizer.h"
#include "..\Special.h"
#include "APState.h"
#include "PanelLocker.h"
#include "PuzzleData.h"
#include "APWatchdog.h"
#include "..\Converty.h"
#include "..\DateTime.h"

class APRandomizer {
	public:
		APRandomizer() {
			_memory = std::make_shared<Memory>("witness64_d3d11.exe");
			panelLocker = new PanelLocker(_memory);
		};

		int Seed = 0;
		int FinalPanel = 0;

		bool Hard = false;
		bool UnlockSymbols = false;
		bool DisableNonRandomizedPuzzles = false;

		bool connected = false;

		bool Connect(HWND& messageBoxHandle, std::string& server, std::string& user, std::string& password);
		void PostGeneration(HWND loadingHandle);

		void GenerateNormal();
		void GenerateHard();

	private:
		std::map<int, int> panelIdToLocationId;
		std::shared_ptr<Memory> _memory;

		APClient* ap;
		APWatchdog* async;
		PanelLocker* panelLocker;

		APState state = APState();
	
		void PreventSnipes();

		void setPuzzleLocks(HWND loadingHandle);

		std::string buildUri(std::string& server);
};

const int AllPuzzles[]{
		0x0A171, 0x04CA4, // Tutorial Secret back area
		0x0005D, 0x0005E, 0x0005F, 0x00060, 0x00061, // Tutorial Dots Tutorial
		0x018AF, 0x0001B, 0x012C9, 0x0001C, 0x0001D, 0x0001E, 0x0001F, 0x00020, 0x00021,	// Tutorial Stones Tutorial
		0x00086, 0x00087, 0x00059, 0x00062, 0x0005C, // Symmetry Vertical Symmetry Mazes
		0x0008D, 0x00081, 0x00083, 0x00084, 0x00082, 0x0343A, // Symmetry Rotational Symmetry Mazes
		0x00022, 0x00023, 0x00024, 0x00025, 0x00026, // Symmetry Black Dots
		0x0007C, 0x0007E, 0x00075, 0x00073, 0x00077, 0x00079, // Symmetry Colored Dots
		0x00065, 0x0006D, 0x00072, 0x0006F, 0x00070, 0x00071, 0x00076, // Symmetry Fading Lines
		0x00A52, 0x00A61, 0x00A57, 0x00A64, 0x00A5B, 0x00A68, // Symmetry Dot Reflection Dual Panels (before laser)
		0x17C09, // Quarry Entry Gates
		0x01E59, // Quarry Mill Entry Door
		0x00E0C, 0x01489, 0x0148A, 0x014D9, 0x014E7, 0x014E8, // Quarry Dots
		0x00557, 0x005F1, 0x00620, 0x009F5, 0x0146C, 0x3C12D,0x03686, 0x014E9, // Quarry Stones
		0x0367C, 0x3C125, // Quarry Dots + Stones
		0x034D4, 0x021D5, // Quarry Boathouse Ramp Activation
		0x021B3, 0x021B4, 0x021B0, 0x021AF, 0x021AE, // Quarry Eraser + Shapes
		0x021B5, 0x021B6, 0x021B7, 0x021BB, 0x09DB5, 0x09DB1, 0x3C124, // Quarry + Stars
		0x09DB3, 0x09DB4, 0x0A3CB, 0x0A3CC, 0x0A3D0, // Quarry Eraser + Stars + Shapes
		0x00469, 0x00472, 0x00262, 0x00474, 0x00553, 0x0056F, // Swamp First row
		0x00390, 0x010CA, 0x00983, 0x00984, 0x00986, 0x00985, 0x00987, 0x181A9, // Swamp Second Row
		0x00609, // Swamp Bridge controll
		0x00982, 0x0097F, 0x0098F, 0x00990, 0x17C0D, 0x17C0E, // Swamp Red Panels
		0x00999, 0x0099D, 0x009A0, 0x009A1, // Swamp Disconnected Shapes
		0x00007, 0x00008, 0x00009, 0x0000A, 0x003B2, 0x00A1E, 0x00C2E, 0x00E3A, // Swamp Rotating Shapes
		0x009A6, // Swamp Optional Tetris
		0x009AB, 0x009AD, 0x009AE, 0x009AF, 0x00006, // Swamp Negative Shapes 1
		0x00002, 0x00004, 0x00005, 0x013E6, 0x00596, // Swamp Negative Shapes 2
		0x00001, 0x014D2, 0x014D4, 0x014D1, // Swamp Negative Shapes 3
		0x17C05, 0x17C02, // Swamp Exit Shortcut
		0x02886, // Treehouse Entry door
		0x17D72, 0x17D8F, 0x17D74, 0x17DAC, 0x17D9E, 0x17DB9, 0x17D9C, 0x17DC2, 0x17DC4, // Treehouse Yellow Bridge
		0x17DC8, 0x17DC7, 0x17CE4, 0x17D2D, 0x17D6C, // Treehouse Pink Bridge 1
		0x17D9B, 0x17D99, 0x17DAA, 0x17D97, 0x17BDF, 0x17D91, 0x17DC6, // Treehouse Pink Bridge 2
		0x17DB3, 0x17DB5, 0x17DB6, 0x17DC0, 0x17DD7, 0x17DD9, 0x17DB8, 0x17DDC, 0x17DD1, 0x17DDE, 0x17DE3, 0x17DEC, 0x17DAE, 0x17DB0, 0x17DDB, // Treehouse Orange Bridge 1
		0x17D88, 0x17DB4, 0x17D8C, 0x17CE3, 0x17DCD, 0x17DB2, 0x17DCC, 0x17DCA, 0x17D8E, 0x17DB7, 0x17DB1, 0x17DA2, // Treehouse Orange Bridge 2
		0x17E3C, 0x17E4D, 0x17E4F, 0x17E52, 0x17E5B, 0x17E5F, 0x17E61, // Treehouse Green Bridge
		0x2899C, 0x28A33, 0x28ABF, 0x28AC0, 0x28AC1, 0x28AD9, // Town Full Dots + Shapes
		0x28AC7, 0x28AC8, 0x28ACA, 0x28ACB, 0x28ACC, // Town Blue Symmetry
		0x18590, // Town Symmetry start
		0x28A69, // Town Church Star Door
		0x034E3, // Town Soundproof Room
		0x03C0C, 0x03C08, // Town 3-color Room
		0x0A0C8, // Town Orange
		0x17F89, 0x0A168, 0x33AB2, //Windmill Puzzles
		0x033D4, 0x0CC7B, 0x002A6, 0x00AFB, 0x15ADD, // Vaults
		0x17C34, // Mountain
		0x09E73, 0x09E75, 0x09E78, 0x09E79, 0x09E6C, 0x09E6F, 0x09E6B, // Mountain Orange Row
		0x09E7A, 0x09E71, 0x09E72, 0x09E69, 0x09E7B, // Mountain Green Row
		0x09EAD, 0x09EAF, 0x33AF5, 0x33AF7, 0x09F6E, // Mountain Purple Panels
		0x09FD3, 0x09FD4, 0x09FD6, 0x09FD7, 0x09FD8, // Mountain Rainbow Row
		0x09FCC, 0x09FCE, 0x09FCF, 0x09FD0, 0x09FD1, 0x09FD2, // Mountain multi puzzle
		0x09E86, 0x09E39, // Mountain double bridge //might need to be disable if we cant fix the scaling
		0x09EFF, 0x09F01, 0x09FC1, 0x09F8E, 0x09FDA, // Mountain floor
		0x0383D, 0x0383A, 0x0383F, 0x09E56, 0x03859, 0x09E5A, 0x339BB, 0x33961, // Mountain pillar puzzles

		0x17FA2, 0x00FF8, // Caves
		0x01A0D, 0x018A0, 0x009A4, 0x00A72, // Caves Blue Symmetry/Tetris
		0x00190, 0x00558, 0x00567, 0x006FE, 0x008B8, 0x00973, 0x0097B, 0x0097D, 0x0097E, 0x00994, 0x334D5, 0x00995, 0x00996, 0x00998, // Caves riangle Puzzles
		0x32962, 0x32966, 0x01A31, 0x00B71, // Caves First alcove
		0x288EA, 0x288FC, 0x289E7, 0x288AA, // Caves Perspective
		0x0A16B, 0x0A2CE, 0x0A2D7, 0x0A2DD, 0x0A2EA, 0x17FB9, // Caves Full Dots
		0x0008F, 0x0006B, 0x0008B, 0x0008C, 0x0008A, 0x00089, 0x0006A, 0x0006C, 0x00027, 0x00028, 0x00029,	// Caves Invisible Dots
		0x17CF2, 0x021D7, // Caves Exit
		0x09DD5, 0x0A16E, 0x039B4, 0x09E85, // Caves Deep challenge caves

		//0x032FF, //Why a single apple tree?
		/*
		0x00698,	0x0048F, 0x09F92,	0x0A036,	0x09DA6,	0x0A049, 0x0A053, 0x09F94, // Desert Surface
		0x00422, 0x006E3, 0x0A02D, // Desert Light
		0x00C72, 0x0129D,	0x008BB, 0x0078D, 0x18313, // Desert Pond
		0x04D18,	0x01205, 0x181AB, 0x0117A, 0x17ECA, // Desert Flood
		0x18076, 0x0A15C, 0x09FFF, 0x0A15F, 0x012D7 // Desert Final and exit
		*/
		0x033EA, 0x01BE9, 0x01CD3, 0x01D3F, 0x03317, 0x0360E, // Keep
		//0x17F9B,	0x002C4,	0x00767,	0x002C6,	0x0070E,	0x0070F,	0x0087D,	0x002C7, // Jungle Sound waves //Does not contain dots so isnt locked
		0x0026D,	0x0026E,	0x0026F,	0x00C3F,	0x00C41,	0x014B2, // Jungle Dots
		//Doors
		0x01A54, // Glass Factory Entry
		0x000B0, // Symmetry Island Door 1
		0x1C349, // Symmetry Island Door 2
		0x01E5A, // Mill Entry Door Left
		0x09E57, // Quarry Entry Gate 1
		0x0288C, // Treehouse Door 1&2
		0x0A182, // Treehouse Door 3
		0x2700B, // Treehouse Exterior Door Control
		0x334DB, // Shadows Outer Door Control
		0x00B10, // Monastery Left Door
		0x00C92, // Monastery Right Door
		0x0056E, // Swamp Entry
		0x17C2E, // Bunker Entry Door
		0x17CAB, // Jungle Pop-up Wall
		0x0C339, // Desert Surface Door
		0x0A249, // Desert Pond Exit Door
		0x28998, // Town Yellow Door
		0x28A0D, // Town Church Stars
		0x17F5F, // Windmill door
		//Discards
		0x17F93, // Mountain 2 Discard
		0x17CFB, // Outside Tutorial Discard
		0x3C12B, // Glass Factory Discard
		0x17CE7, // Desert Discard
		0x17CF0, // Mill Discard
		0x17FA9, // Treehouse Green Bridge Discard
		0x17FA0, // Treehouse Laser Discard
		0x17D27, // Keep Discard
		0x17D28, // Shipwreck Discard
		0x17D01, // Town Orange Crate Discard
		0x17F9B, // Jungle Discard
		0x17C42, // Mountainside Discard
		0x386FA,	0x1C33F,	0x196E2,	0x1972A,	0x19809,	0x19806,	0x196F8,	0x1972F, //shadows avoid
		0x19797,	0x1979A,	0x197E0,	0x197E8,	0x197E5, //Shadow follow
		0x09F7F, // Quary lazer panel
		// 0x181F5, 0x18488, // Swamp bridge controlls //no work
};


