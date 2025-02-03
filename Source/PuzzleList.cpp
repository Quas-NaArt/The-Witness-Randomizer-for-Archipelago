// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include "PuzzleList.h"
#include "Utilities.h"
#include "Watchdog.h"

using enum Deco::Color;
using Deco::Arrow,
	  Deco::Dot,
	  Deco::Eraser,
	  Deco::kExit,
	  Deco::Gap,
	  Deco::kStart,
	  Deco::NegaPoly,
	  Deco::Poly,
	  Deco::SpinPoly,
	  Deco::Star,
	  Deco::Stone,
	  Deco::Triangle;

void PuzzleList::GenerateAllN()
{
	generator->setLoadingHandle(_handle);
	generator->setLoadingData(336);
	CopyTargets();
	GenerateTutorialN();
	GenerateSymmetryN();
	GenerateQuarryN();
	// GenerateBunkerN();
	GenerateSwampN();
	GenerateTreehouseN();
	GenerateTownN();
	GenerateVaultsN();
	GenerateTrianglePanelsN();
	GenerateOrchardN();
	GenerateDesertN();
	GenerateKeepN();
	GenerateJungleN();
	GenerateMountainN();
	GenerateCavesN();
	SetWindowText(_handle, L"Done!");
	(new ArrowWatchdog(0x0056E))->start(); // Easy way to close the randomizer when the game is done
										   // GenerateShadowsN(); //Can't randomize
										   // GenerateMonasteryN(); //Can't randomize
}

void PuzzleList::GenerateAllH()
{
	// generator->setLoadingHandle(_handle); // When????????????
	generator->setLoadingData(349);
	CopyTargets();
	GenerateTutorialH();
	GenerateSymmetryH();
	GenerateQuarryH();
	// GenerateBunkerH(); //Can't randomize because panels refuse to render the symbols
	GenerateSwampH();
	GenerateTreehouseH();
	GenerateTownH();
	GenerateVaultsH();
	GenerateTrianglePanelsH();
	GenerateOrchardH();
	GenerateDesertH();
	GenerateKeepH();
	GenerateJungleH();
	GenerateMountainH();
	GenerateCavesH();
	SetWindowText(_handle, L"Done!");
	// GenerateShadowsH(); //Can't randomize
	// GenerateMonasteryH(); //Can't randomize
}

// Provide alternate power sources for certain nonrandomized targets
// See: https://github.com/sigma144/witness-randomizer/wiki/Activation-Triggers
void PuzzleList::CopyTargets()
{
	Special::copyTarget(0x00021, 0x19650);
	Special::copyTarget(0x00061, 0x09DE0);
	Special::copyTarget(0x17CFB, 0x28B39);
	Special::copyTarget(0x3C12B, 0x28B39);
	Special::copyTarget(0x17CE7, 0x17CA4); // Desert Discard targets Monastery Laser
	Special::copyTarget(0x00B8D, 0x28B39);
	Special::copyTarget(0x17FA9, 0x17CA4);
	Special::copyTarget(0x17FA0, 0x17CAB);
	Special::copyTarget(0x17D27, 0x17CAB);
	Special::copyTarget(0x17D28, 0x19650);
	Special::copyTarget(0x17D01, 0x09DE0);
	Special::copyTarget(0x17C71, 0x19650);
	Special::copyTarget(0x17CF7, 0x28B39);
	Special::copyTarget(0x17D01, 0x09DE0);
	Special::copyTarget(0x17F9B, 0x17CAB);
	Special::copyTarget(0x17C42, 0x09DE0);
	Special::copyTarget(0x00A5B, 0x17CA4);

	Special::setPower(0x17CA4, true);
	Special::setPower(0x17CAB, true);
	Special::setPower(0x28B39, true);
}

void PuzzleList::GenerateTutorialN()
{
	generator->setLoadingData(L"Tutorial", 21);
	generator->resetConfig();
	Special::drawSeedAndDifficulty(0x00064, seed, false, !seedIsRNG, false);
	Special::drawGoodLuckPanel(0x00182);
	// Mazes
	generator->setFlag(Generate::Config::FullGaps);
	generator->setGridSize(6, 6);
	generator->generateMaze(0x00293);
	generator->setGridSize(9, 9);
	generator->generateMaze(0x00295, 1, 1);
	generator->setGridSize(12, 12);
	generator->generateMaze(0x002C2);
	generator->resetConfig();
	// 2 starts maze
	generator->generateMaze(0x0A3B2);
	// 2 exits maze
	generator->setSymbol(kExit, 12, 12);
	generator->setFlagOnce(Generate::Config::DisableWrite);
	generator->generateMaze(0x0A3B5);
	generator->_panel->_endpoints.emplace_back(Endpoint(12, 0, Endpoint::Direction::RIGHT, kExit));
	generator->write(0x0A3B5);
	generator->resetConfig();
	// Secret back area
	generator->generate(
		0x0A171, {{Dot(), 25},
				  {Gap(), 4}});
	generator->generate(
		0x04CA4, {{Dot(), 25},
				  {Stone(Black), 2},
				  {Stone(White), 2}});
	// Dots Tutorial
	generator->setFlag(Generate::Config::FullGaps);
	generator->setGridSize(3, 3);
	generator->generate(
		0x0005D, {{kStart, 1},
				  {kExit, 1},
				  {Dot(), 7},
				  {Gap(), 4}});
	generator->generate(
		0x0005E, {{kStart, 2},
				  {kExit, 1},
				  {Dot(), 7},
				  {Gap(), 4}});
	generator->setGridSize(4, 4);
	generator->generate(
		0x0005F, {{kStart, 2},
				  {kExit, 1},
				  {Dot(), 10},
				  {Gap(), 8}});
	generator->generate(
		0x00060, {{kStart, 2},
				  {kExit, 1},
				  {Dot(), 10},
				  {Gap(), 8}});
	generator->generate(
		0x00061, {{kStart, 3},
				  {kExit, 1},
				  {Dot(), 10},
				  {Gap(), 8}});
	generator->resetConfig();
	// Stones Tutorial
	generator->setGridSize(4, 4);
	generator->generate(
		0x018AF, {{kExit, 1},
				  {Stone(Black), 7},
				  {Stone(White), 5}});
	generator->generate(
		0x0001B, {{kExit, 1},
				  {Stone(Black), 7},
				  {Stone(White), 5}});
	generator->generate(
		0x012C9, {{kExit, 1},
				  {Stone(Black), 7},
				  {Stone(White), 5},
				  {kStart, 3}});
	generator->setGridSize(5, 5);
	generator->generate(
		0x0001C, {{kExit, 1},
				  {Stone(Black), 11},
				  {Stone(White), 8}});
	generator->generate(
		0x0001D, {{kExit, 1},
				  {Stone(Black), 11},
				  {Stone(White), 8}});
	generator->generate(
		0x0001E, {{kExit, 1},
				  {Stone(Black), 11},
				  {Stone(White), 8},
				  {kStart, 3}});
	generator->generate(
		0x0001F, {{kExit, 1},
				  {Stone(Black), 7},
				  {Stone(White), 5},
				  {Gap(), 10}});
	generator->generate(
		0x00020, {{kExit, 1},
				  {Stone(Black), 7},
				  {Stone(White), 5},
				  {Gap(), 10}});
	generator->generate(
		0x00021, {{kExit, 1},
				  {Stone(Black), 7},
				  {Stone(White), 5},
				  {Gap(), 10},
				  {kStart, 3}});
}

void PuzzleList::GenerateSymmetryN()
{
	generator->setLoadingData(L"Symmetry", 33);
	generator->resetConfig();
	generator->setFlag(Generate::Config::StartEdgeOnly);
	// Vertical Symmetry Mazes
	generator->setSymmetry(Panel::Symmetry::Vertical);
	generator->setGridSize(5, 5);
	generator->generateMaze(0x00086, 0, 1);
	generator->generateMaze(0x00087, 1, 1);
	generator->setGridSize(0, 0);
	generator->removeFlagOnce(Generate::Config::StartEdgeOnly);
	generator->generateMaze(0x00059, 1, 0);
	generator->generateMaze(0x00062);
	specialCase->generateSpecialSymMaze(generator, 0x0005C);
	// Rotational Symmetry Mazes
	generator->setSymmetry(Panel::Symmetry::Rotational);
	generator->setGridSize(5, 5);
	generator->generateMaze(0x0008D, 0, 1);
	generator->generateMaze(0x00081, 1, 1);
	generator->removeFlagOnce(Generate::Config::StartEdgeOnly);
	generator->generateMaze(0x00083, 1, 1);
	generator->setGridSize(6, 6);
	generator->generateMaze(0x00084);
	generator->generateMaze(0x00082);
	generator->generateMaze(0x0343A);
	// Symmetry Island Door
	generator->resetConfig();
	generator->setGridSize(4, 4);
	generator->setFlagOnce(Generate::Config::FullGaps);
	generator->generate(
		0x000B0, {{Dot(Deco::Location::Any), 8},
				  {Gap(Deco::Location::Any), 5}});
	// Black Dots
	generator->setFlag(Generate::Config::StartEdgeOnly);
	generator->setGridSize(5, 5);
	generator->setSymmetry(Panel::Symmetry::Horizontal);
	generator->generate(
		0x00022, {{Dot(Deco::Location::Any), 8},
				  {kStart, 1},
				  {kExit, 1}});
	generator->generate(
		0x00023, {{Dot(Deco::Location::Any), 8},
				  {kStart, 1},
				  {kExit, 1}});
	generator->setSymmetry(Panel::Symmetry::Rotational);
	generator->generate(
		0x00024, {{Dot(Deco::Location::Any), 8},
				  {kStart, 1},
				  {kExit, 1}});
	generator->generate(
		0x00025, {{Dot(Deco::Location::Any), 8},
				  {kStart, 1},
				  {kExit, 1}});
	generator->generate(
		0x00026, {{Dot(Deco::Location::Any), 8},
				  {kStart, 1},
				  {kExit, 1}});
	// Colored Dots
	generator->generate(
		0x0007C, {{Dot(Deco::Location::Any, Deco::Parity::Blue), 3},
				  {Dot(Deco::Location::Any, Deco::Parity::Yellow), 3},
				  {kStart, 1},
				  {kExit, 1}});
	generator->generate(
		0x0007E, {{Dot(Deco::Location::Any, Deco::Parity::Blue), 3},
				  {Dot(Deco::Location::Any, Deco::Parity::Yellow), 3},
				  {kStart, 1},
				  {kExit, 2}});
	generator->generate(
		0x00075, {{Dot(Deco::Location::Any, Deco::Parity::Blue), 2},
				  {Dot(Deco::Location::Any, Deco::Parity::Yellow), 2},
				  {Dot(), 3},
				  {kStart, 1},
				  {kExit, 1}});
	generator->generate(
		0x00073, {{Dot(Deco::Location::Any, Deco::Parity::Blue), 2},
				  {Dot(Deco::Location::Any, Deco::Parity::Yellow), 1},
				  {Dot(), 5},
				  {kStart, 1},
				  {kExit, 1}});
	generator->generate(
		0x00077, {{Dot(Deco::Location::Any, Deco::Parity::Blue), 1},
				  {Dot(Deco::Location::Any, Deco::Parity::Yellow), 2},
				  {Dot(), 5},
				  {kStart, 1},
				  {kExit, 1}});
	generator->generate(
		0x00079, {{Dot(Deco::Location::Any, Deco::Parity::Blue), 2},
				  {Dot(Deco::Location::Any, Deco::Parity::Yellow), 2},
				  {Dot(), 4},
				  {kStart, 2},
				  {kExit, 1}});
	// Fading Lines
	generator->setGridSize(6, 6);
	generator->generate(
		0x00065, {{Dot(Deco::Location::Any, Deco::Parity::Blue), 4},
				  {Dot(Deco::Location::Any, Deco::Parity::Yellow), 2},
				  {kStart, 1},
				  {kExit, 1}});
	generator->generate(
		0x0006D, {{Dot(Deco::Location::Any, Deco::Parity::Blue), 3},
				  {Dot(Deco::Location::Any, Deco::Parity::Yellow), 3},
				  {kStart, 1},
				  {kExit, 2}});
	generator->generate(
		0x00072, {{Dot(Deco::Location::Any, Deco::Parity::Blue), 2},
				  {Dot(Deco::Location::Any, Deco::Parity::Yellow), 4},
				  {kStart, 2},
				  {kExit, 1}});
	generator->setSymbol(kStart, 0, 6);
	generator->setSymbol(kStart, 12, 6);
	generator->generate(
		0x0006F, {{Dot(Deco::Location::Any, Deco::Parity::Blue), 3},
				  {Dot(Deco::Location::Any, Deco::Parity::Yellow), 3},
				  {kExit, 1}});
	generator->setSymmetry(Panel::Symmetry::Vertical);
	generator->setSymbol(kStart, 0, 6);
	generator->setSymbol(kStart, 12, 6);
	generator->generate(
		0x00070, {{Dot(Deco::Location::Any, Deco::Parity::Blue), 3},
				  {Dot(Deco::Location::Any, Deco::Parity::Yellow), 3},
				  {kExit, 1}});
	generator->setSymmetry(Panel::Symmetry::Rotational);
	generator->setSymbol(kStart, 0, 6);
	generator->setSymbol(kStart, 12, 6);
	generator->generate(
		0x00071, {{Dot(Deco::Location::Any, Deco::Parity::Blue), 2},
				  {Dot(Deco::Location::Any, Deco::Parity::Yellow), 3},
				  {kStart, 1},
				  {kExit, 1}});
	generator->generate(
		0x00076, {{Dot(Deco::Location::Any, Deco::Parity::Yellow), 5},
				  {kStart, 1},
				  {kExit, 1}});
	// Dot Reflection Dual Panels (before laser)
	generator->resetConfig();
	std::set<Panel::Symmetry> normalSym = {Panel::Symmetry::Horizontal, Panel::Symmetry::Rotational};
	std::set<Panel::Symmetry> weirdSym = {Panel::Symmetry::RotateLeft, Panel::Symmetry::RotateRight, Panel::Symmetry::FlipXY, Panel::Symmetry::FlipNegXY};
	specialCase->generateReflectionDotPuzzle(
		generator,
		0x00A52,
		0x00A61,
		{{Dot(Deco::Location::Any), 10},
		 {kExit, 1},
		 {Gap(), 5}},
		Panel::Symmetry::Vertical, false);
	generator->setSymbol(kStart, 0, 8);
	specialCase->generateReflectionDotPuzzle(
		generator,
		0x00A57,
		0x00A64,
		{{Dot(Deco::Location::Any), 12},
		 {kExit, 1},
		 {Gap(), 1}},
		Utilities::pop_random(normalSym), false);
	generator->setSymbol(kStart, 0, 8);
	generator->setSymbol(kStart, 8, 8);
	generator->setSymbol(kStart, 8, 0);
	generator->setSymbol(kStart, 0, 0);
	generator->setSymbol(kExit, 8, 2);
	specialCase->generateReflectionDotPuzzle(
		generator,
		0x00A5B,
		0x00A68,
		{{Dot(Deco::Location::Any), 12}},
		Utilities::pop_random(weirdSym), false);
}

void PuzzleList::GenerateQuarryN()
{
	generator->setLoadingData(L"Quarry", 39);
	generator->resetConfig();
	// Entry Gates
	generator->setFlag(Generate::Config::EnableFlash);
	generator->setGridSize(5, 5);
	generator->generate(
		0x09E57, {{Stone(Black), 7},
				  {Stone(White), 5},
				  {Gap(), 10}});
	generator->setGridSize(4, 4);
	generator->setFlagOnce(Generate::Config::RequireCombineShapes);
	generator->generate(
		0x17C09, {{Poly(), 2},
				  {Gap(), 5}});
	// Mill Entry Door
	generator->resetConfig();
	generator->setFlag(Generate::Config::EnableFlash);
	generator->generate(
		0x01E5A, {{Stone(White), 5},
				  {Stone(Black), 7}});
	generator->setFlagOnce(Generate::Config::PreserveStructure);
	generator->generate(
		0x01E59, {{Dot(Deco::Location::Any), 15}});
	// Dots
	generator->setGridSize(3, 3);
	generator->generate(
		0x00E0C, {{Dot(Deco::Location::Any), 5},
				  {Eraser(Green), 1}});
	generator->generate(
		0x01489, {{Dot(Deco::Location::Any), 7},
				  {Eraser(Green), 1}});
	generator->setGridSize(4, 3);
	generator->generate(
		0x0148A, {{Dot(Deco::Location::Any), 11},
				  {Eraser(Green), 1}});
	generator->generate(
		0x014D9, {{Dot(Deco::Location::Any), 13},
				  {Eraser(Green), 1}});
	generator->setGridSize(4, 4);
	generator->generate(
		0x014E7, {{Dot(Deco::Location::Any), 15},
				  {Eraser(Green), 1}});
	generator->generate(
		0x014E8, {{Dot(Deco::Location::Any), 15},
				  {Eraser(Green), 1}});
	// Stones
	generator->setFlag(Generate::Config::AlternateColors);
	generator->setGridSize(3, 3);
	generator->generate(
		0x00557, {{Stone(White), 3},
				  {Stone(Black), 3},
				  {Eraser(Green), 1}});
	generator->generate(
		0x005F1, {{Stone(White), 4},
				  {Stone(Black), 4},
				  {Eraser(Green), 1}});
	generator->setFlag(Generate::Config::MakeStonesUnsolvable);
	generator->setGridSize(4, 4);
	generator->generate(
		0x00620, {{Stone(White), 7},
				  {Stone(Black), 6},
				  {Eraser(Green), 1}});
	generator->generate(
		0x009F5, {{Stone(White), 6},
				  {Stone(Black), 7},
				  {Eraser(Green), 1}});
	generator->generate(
		0x0146C, {{Stone(White), 7},
				  {Stone(Black), 6},
				  {Eraser(Green), 1}});
	generator->removeFlag(Generate::Config::MakeStonesUnsolvable);
	generator->setGridSize(3, 3);
	generator->generate(
		0x3C12D, {{Stone(White), 2},
				  {Stone(Black), 2},
				  {Stone(Red), 2},
				  {Eraser(Green), 1}});
	generator->setGridSize(4, 4);
	generator->generate(
		0x03686, {{Stone(Red), 4},
				  {Stone(White), 4},
				  {Stone(Black), 3},
				  {Eraser(Green), 1}});
	generator->generate(
		0x014E9, {{Stone(White), 4},
				  {Stone(Black), 4},
				  {Stone(Red), 3},
				  {Eraser(Green), 1}});
	// Dots + Stones
	generator->resetConfig();
	generator->setSymbol(kStart, 4, 4);
	generator->setFlagOnce(Generate::Config::AlternateColors);
	generator->generate(
		0x0367C, {{Stone(White), 5},
				  {Stone(Black), 5},
				  {Dot(Deco::Location::Any), 4},
				  {Eraser(Green), 1}});
	generator->generate(
		0x3C125, {{Stone(White), 10},
				  {Stone(Black), 10},
				  {Dot(Deco::Location::Any), 10},
				  {Eraser(Purple), 1},
				  {kStart, 3}});
	// Boathouse Ramp Activation
	generator->setFlag(Generate::Config::EnableFlash);
	generator->generate(
		0x034D4, {{Star(White), 8},
				  {Star(Black), 6}});
	generator->setFlagOnce(Generate::Config::RequireCombineShapes);
	generator->generate(
		0x021D5, {{Poly(), 1},
				  {SpinPoly(Any), 1},
				  {Gap(), 5}});
	// Eraser + Shapes
	generator->setFlag(Generate::Config::ResetColors);
	generator->generate(
		0x021B3, {{Poly(), 3},
				  {Eraser(White), 1}});
	Memory::get()->WritePanelData(0x021B4, POWER_OFF_ON_FAIL, 0);
	generator->generate(
		0x021B4, {{Poly(), 3},
				  {Eraser(White), 1}});
	Memory::get()->WritePanelData(0x021B0, POWER_OFF_ON_FAIL, 0);
	generator->generate(
		0x021B0, {{Poly(), 4},
				  {Eraser(White), 1}});
	Memory::get()->WritePanelData(0x021AF, POWER_OFF_ON_FAIL, 0);
	generator->setFlagOnce(Generate::Config::SmallShapes);
	generator->generate(
		0x021AF, {{Poly(), 4},
				  {Eraser(White), 1}});
	Memory::get()->WritePanelData(0x021AE, POWER_OFF_ON_FAIL, 0);
	generator->setFlagOnce(Generate::Config::DisconnectShapes);
	generator->generate(
		0x021AE, {{Poly(), 3},
				  {Eraser(White), 1}});
	generator->removeFlag(Generate::Config::ResetColors);
	// Eraser + Stars
	generator->setFlag(Generate::Config::TreehouseColors);
	generator->generate(
		0x021B5, {{Star(Green), 3},
				  {Star(Magenta), 4},
				  {Eraser(Magenta), 1},
				  {Gap(), 2}});
	generator->generate(
		0x021B6, {{Star(Green), 3},
				  {Star(Magenta), 4},
				  {Eraser(Green), 1},
				  {Gap(), 2}});
	generator->generate(
		0x021B7, {{Star(Green), 6},
				  {Star(Orange), 5},
				  {Eraser(Orange), 1}});
	generator->generate(
		0x021BB, {{Star(Magenta), 7},
				  {Star(Orange), 6},
				  {Eraser(Orange), 1}});
	generator->generate(
		0x09DB5, {{Star(Green), 1},
				  {Eraser(Green), 1},
				  {Star(Magenta), 1},
				  {Eraser(Magenta), 1},
				  {Star(Orange), 1},
				  {Eraser(Orange), 1},
				  {Eraser(White), 1},
				  {Star(White), 1}});
	generator->generate(
		0x09DB1, {{Star(Orange), 9},
				  {Eraser(Orange), 1}});
	generator->generate(
		0x3C124, {{Star(Orange), 6},
				  {Star(Magenta), 5},
				  {Star(Green), 4},
				  {Eraser(Magenta), 1}});
	generator->resetConfig();
	// Eraser + Stars + Shapes
	generator->setFlagOnce(Generate::Config::FixBackground);
	generator->setGridSize(4, 4);
	generator->setSymbol(kStart, 0, 8);
	generator->generate(
		0x09DB3, {{Star(White), 5},
				  {Poly(Green), 2},
				  {Eraser(Black), 1}});
	generator->pathWidth = 0.8f;
	generator->generate(
		0x09DB4, {{Star(White), 6},
				  {Poly(Green), 2},
				  {Eraser(Black), 1}});
	generator->setGridSize(6, 3);
	generator->pathWidth = 0.6f;
	generator->generate(
		0x0A3CB, {{Star(Black), 3},
				  {Poly(Green), 3},
				  {Eraser(Cyan), 1}});
	generator->generate(
		0x0A3CC, {{Star(Black), 5},
				  {Poly(Green), 3},
				  {Eraser(Cyan), 1}});
	generator->generate(
		0x0A3D0, {{Star(Black), 6},
				  {Poly(Green), 3},
				  {Eraser(Cyan), 1}});
}

// void PuzzleList::GenerateBunkerN()
// {
// 	auto texloader = TextureLoader::get();

// 	texloader->forceLoadBunkerTextures();

// 	generator->setLoadingData(L"Bunker", 9);
// 	generator->resetConfig();
// 	generator->setGridSize(3, 3);
// 	generator->generate(
// 	0x09F7D, {
// {Stone(Purple), 1},
// {Stone(Green), 1}});
// 	texloader->generateTexture(0x09F7D);
// 	generator->generate(
// 	0x09FDC, {
// {Stone(Purple), 1},
// {Stone(Green), 1},
// {Stone(White), 1}});
// 	texloader->generateTexture(0x09FDC);
// 	generator->generate(
// 	0x09FF7, {
// {Stone(Purple), 2},
// {Stone(Green), 2},
// {Stone(White), 1}});
// 	texloader->generateTexture(0x09FF7);
// 	generator->generate(
// 	0x09F82, {
// {Stone(Purple), 4},
// {Stone(Green), 1},
// {Stone(White), 4}});
// 	texloader->generateTexture(0x09F82);
// 	generator->setGridSize(4, 4);
// 	generator->generate(
// 	0x09FF8, {
// {Stone(Purple), 2},
// {Stone(Green), 1},
// {Stone(White), 2}});
// 	texloader->generateTexture(0x09FF8);

// 	generator->setGridSize(3, 3);
// 	generator->generate(
// 	0x09D9F, {
// {Stone(White), 1},
// {Stone(Yellow), 1},
// {Stone(Blue), 1},
// {Stone(Magenta), 1}});
// 	texloader->generateTexture(0x09D9F);
// 	generator->setGridSize(4, 4);
// 	generator->generate(
// 	0x09DA1, {
// {Stone(White), 2},
// {Stone(Yellow), 2},
// {Stone(Blue), 2},
// {Stone(Magenta), 2}});
// 	texloader->generateTexture(0x09DA1);
// 	generator->generate(
// 	0x09DA2, {
// {Stone(White), 2},
// {Stone(Yellow), 2},
// {Stone(Blue), 2},
// {Stone(Magenta), 2}});
// 	texloader->generateTexture(0x09DA2);
// 	generator->setGridSize(5, 4);
// 	generator->generate(
// 	0x09DAF, {
// {Stone(White), 4},
// {Stone(Yellow), 3},
// {Stone(Blue), 3},
// {Stone(Magenta), 2},
// {Exit(Direction::UP_RIGHT), 1}});
// 	texloader->generateTexture(0x09DAF);

	// specialCase->generateColorFilterPuzzle(
	// 	0x28A0D,
	// 	{ 4, 4 },
	// 	{{Deco::Star(Deco::Color{1}), 6 },
	// 	 {Deco::Star(Deco::Color{2}), 6 },
	// 	 {Deco::Star(Deco::Color{3}), 4 }},
	// 	{ 1, 1, 0, 0 },
	// 	colorblind);
	// specialCase->generateColorFilterPuzzle(
	// 	0x0A010,
	// 	{ 4, 4 },
	// 	{{Deco::Stone(Deco::Color{1}), 4 },
	// 	 {Deco::Stone(Deco::Color{2}), 8 },
	// 	 {Deco::Stone(Deco::Color{3}), 4 } },
	// 	{ 1, 1, 0, 0 },
	// 	colorblind);
	// texloader->generateTexture(0x0A010);
	// specialCase->generateColorFilterPuzzle(
	// 	0x0A01B,
	// 	{ 4, 4 },
	// 	{{Deco::Stone(Deco::Color{1}), 4 },
	// 	 {Deco::Stone(Deco::Color{2}), 4 },
	// 	 {Deco::Stone(Deco::Color{3}), 4 } },
	// 	{ 0, 1, 1, 0 },
	// 	colorblind);
	// texloader->generateTexture(0x0A01B);
	// specialCase->generateColorFilterPuzzle(
	// 	0x0A01F,
	// 	{ 4, 4 },
	// 	{{Deco::Stone(Deco::Color{1}), 4 },
	// 	 {Deco::Stone(Deco::Color{2}), 3 } },
	// 	{ 0, 1, 0, 0 },
	// 	colorblind);
// 	texloader->generateTexture(0x0A01F);
// 	generator->setGridSize(3, 3);
// 	generator->setFlag(Generate::Config::WriteColors);
// 	generator->generate(
// 	0x17E63, {
// {Stone(Black), 2},
// {Stone(Blue), 5},
// {Stone(White), 2}});
// 	texloader->generateTexture(0x17E63);
// 	generator->setFlag(Generate::Config::ResetColors);
// specialCase->generateColorfulColorFilterPuzzle(
// 	0x17E67,
// 	{ 3, 3 },
// 	{{Deco::Stone(Deco::Color{1}), 2},
// 	 {Deco::Stone(Deco::Color{2}), 5},
// 	 {Deco::Stone(Deco::Color{3}), 2}},
// 	{ 1, 0, 1, 0 },
// 	colorblind);
// 	texloader->generateTexture(0x17E67);

// }

void PuzzleList::GenerateSwampN()
{
	generator->setLoadingData(L"Swamp", 49);
	generator->resetConfig();
	// First row
	generator->setFlag(Generate::Config::SplitShapes);
	generator->setGridSize(3, 3);
	generator->pathWidth = 0.7f;
	generator->generate(
		0x0056E, {{Poly(), 1},
				  {Gap(), 5}});
	generator->generate(
		0x00469, {{Poly(), 2},
				  {Gap(), 5}});
	generator->generate(
		0x00472, {{Poly(), 2},
				  {Gap(), 5}});
	generator->setGridSize(4, 4);
	generator->pathWidth = 0.65f;
	generator->generate(
		0x00262, {{Poly(), 2},
				  {Gap(), 8}});
	generator->generate(
		0x00474, {{Poly(), 2},
				  {Gap(), 8}});
	generator->generate(
		0x00553, {{Poly(), 2},
				  {Gap(), 8}});
	generator->generate(
		0x0056F, {{Poly(), 3},
				  {Gap(), 8}});
	// Second Row
	generator->resetConfig();
	generator->setFlag(Generate::Config::RequireCombineShapes);
	generator->setGridSize(3, 3);
	generator->pathWidth = 0.7f;
	generator->generate(
		0x00390, {{Poly(), 2},
				  {Gap(), 3}});
	generator->generate(
		0x010CA, {{Poly(), 2},
				  {Gap(), 3}});
	generator->setGridSize(4, 4);
	generator->pathWidth = 0.65f;
	generator->generate(
		0x00983, {{Poly(), 2},
				  {Gap(), 5}});
	generator->generate(
		0x00984, {{Poly(), 2},
				  {Gap(), 5}});
	generator->generate(
		0x00986, {{Poly(), 2},
				  {Gap(), 5}});
	generator->generate(
		0x00985, {{Poly(), 2},
				  {Gap(), 5}});
	generator->generate(
		0x00987, {{Poly(), 2},
				  {Gap(), 5}});
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.6f;
	generator->setFlagOnce(Generate::Config::BigShapes);
	generator->generate(
		0x181A9, {{Poly(), 2},
				  {Gap(), 12}});
	// Turn off bridge control until all previous puzzles are solved
	specialCase->setTargetAndDeactivate(0x181A9, 0x00609);
	// Red Panels
	generator->resetConfig();
	generator->setGridSize(4, 4);
	generator->pathWidth = 0.65f;
	generator->generate(
		0x00982, {{Poly(), 3},
				  {Gap(), 2}});
	generator->generate(
		0x0097F, {{Poly(), 3},
				  {Gap(), 2}});
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.6f;
	generator->generate(
		0x0098F, {{Poly(), 3},
				  {Gap(), 5}});
	generator->generate(
		0x00990, {{Poly(), 4}});
	generator->setFlagOnce(Generate::Config::DisableReset);
	generator->setFlagOnce(Generate::Config::LongPath);
	generator->generate(
		0x17C0D, {{Poly(), 3}});
	generator->place_gaps(12);
	generator->write(0x17C0E);
	// Disconnected Shapes
	generator->resetConfig();
	generator->setFlag(Generate::Config::DisconnectShapes);
	generator->setGridSize(3, 3);
	generator->pathWidth = 0.7f;
	generator->generate(
		0x00999, {{Poly(), 2}});
	generator->setGridSize(4, 4);
	generator->pathWidth = 0.65f;
	generator->setFlagOnce(Generate::Config::BigShapes);
	generator->generate(
		0x0099D, {{Poly(), 2}});
	generator->generate(
		0x009A0, {{Poly(), 3}});
	generator->generate(
		0x009A1, {{Poly(), 3}});
	// Rotating Shapes
	generator->resetConfig();
	generator->setGridSize(3, 3);
	generator->pathWidth = 0.7f;
	generator->generate(
		0x00007, {{SpinPoly(Any), 1},
				  {Gap(), 3}});
	generator->setGridSize(4, 4);
	generator->pathWidth = 0.65f;
	generator->generate(
		0x00008, {{SpinPoly(Any), 1},
				  {Poly(), 1},
				  {Gap(), 6}});
	generator->generate(
		0x00009, {{SpinPoly(Any), 2},
				  {Gap(), 6}});
	generator->setFlagOnce(Generate::Config::BigShapes);
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.6f;
	generator->generate(
		0x0000A, {{SpinPoly(Any), 1},
				  {Gap(), 15}});
	// 5x5 Rotating Shapes
	generator->generate(
		0x003B2, {{SpinPoly(Any), 3},
				  {Gap(), 2}});
	generator->generate(
		0x00A1E, {{SpinPoly(Any), 4}});
	generator->generate(
		0x00C2E, {{SpinPoly(Any), 2},
				  {Poly(), 1},
				  {Gap(), 2}});
	generator->setFlagOnce(Generate::Config::BigShapes);
	generator->generate(
		0x00E3A, {{SpinPoly(Any), 3}});
	// Optional Tetris
	generator->resetConfig();
	generator->generate(
		0x009A6, {{Poly(), 5}});
	// Negative Shapes 1
	generator->resetConfig();
	generator->setFlag(Generate::Config::RequireCombineShapes);
	generator->setFlag(Generate::Config::DisableCancelShapes);
	generator->setGridSize(3, 3);
	generator->pathWidth = 0.7f;
	generator->generate(
		0x009AB, {{Poly(), 2},
				  {NegaPoly(Any), 1}});
	generator->setGridSize(4, 4);
	generator->pathWidth = 0.65f;
	generator->generate(
		0x009AD, {{Poly(), 2},
				  {NegaPoly(Any), 1}});
	generator->removeFlag(Generate::Config::RequireCombineShapes);
	generator->setFlag(Generate::Config::BigShapes);
	generator->generate(
		0x009AE, {{Poly(), 2},
				  {NegaPoly(Any), 1}});
	generator->generate(
		0x009AF, {{Poly(), 2},
				  {NegaPoly(Any), 1}});
	generator->removeFlag(Generate::Config::BigShapes);
	generator->setFlagOnce(Generate::Config::DisconnectShapes);
	generator->generate(
		0x00006, {{Poly(), 2},
				  {NegaPoly(Any), 1}});
	// Negative Shapes 2
	generator->setFlagOnce(Generate::Config::RequireCombineShapes);
	generator->generate(
		0x00002, {{Poly(), 2},
				  {NegaPoly(Any), 2}});
	generator->setFlagOnce(Generate::Config::BigShapes);
	generator->generate(
		0x00004, {{Poly(), 2},
				  {NegaPoly(Any), 2}});
	generator->generate(
		0x00005, {{Poly(), 3},
				  {NegaPoly(Any), 1}});
	generator->generate(
		0x013E6, {{Poly(), 3},
				  {NegaPoly(Any), 2}});
	generator->setFlagOnce(Generate::Config::DisconnectShapes);
	generator->setFlagOnce(Generate::Config::BigShapes);
	generator->generate(
		0x00596, {{Poly(), 2},
				  {NegaPoly(Any), 1}});
	// Negative Shapes 3
	generator->resetConfig();
	generator->setFlag(Generate::Config::RequireCancelShapes);
	generator->setGridSize(2, 3);
	generator->pathWidth = 0.7f;
	generator->generate(
		0x00001, {{Poly(), 1},
				  {NegaPoly(Any), 2}});
	generator->setGridSize(4, 4);
	generator->pathWidth = 0.65f;
	generator->generate(
		0x014D2, {{Poly(), 3},
				  {NegaPoly(Any), 2}});
	generator->generate(
		0x014D4, {{Poly(), 3},
				  {NegaPoly(Any), 3}});
	generator->generate(
		0x014D1, {{Poly(), 3},
				  {NegaPoly(Any), 4}});
	// Exit Shortcut
	generator->resetConfig();
	generator->generate(
		0x17C05, {{SpinPoly(Any), 2}});
	specialCase->generateAntiPuzzle(0x17C02);
}

void PuzzleList::GenerateTreehouseN()
{
	generator->setLoadingData(L"Treehouse", 57);
	generator->resetConfig();
	generator->setFlag(Generate::Config::WriteColors); // Have to do this to get a proper looking orange and green
	generator->setGridSize(2, 2);
	generator->generate(
		0x02886, {{Star(Orange), 4}});
	generator->setFlag(Generate::Config::TreehouseLayout);
	// Yellow Bridge
	generator->setGridSize(3, 3);
	generator->generate(
		0x17D72, {{Star(Orange), 4},
				  {Gap(), 5}});
	generator->setGridSize(4, 4);
	generator->generate(
		0x17D8F, {{Star(Orange), 4},
				  {Gap(), 10}});
	generator->generate(
		0x17D74, {{Star(Orange), 4},
				  {Gap(), 10}});
	generator->generate(
		0x17DAC, {{Star(Orange), 6},
				  {Gap(), 5}});
	generator->generate(
		0x17D9E, {{Star(Orange), 6},
				  {Gap(), 5}});
	generator->generate(
		0x17DB9, {{Star(Orange), 6},
				  {Gap(), 5}});
	generator->generate(
		0x17D9C, {{Star(Orange), 8},
				  {Gap(), 2}});
	generator->generate(
		0x17DC2, {{Star(Orange), 8},
				  {Gap(), 2}});
	generator->generate(
		0x17DC4, {{Star(Orange), 8},
				  {Gap(), 2}});
	generator->removeFlagOnce(Generate::Config::TreehouseLayout);
	generator->setGridSize(5, 5);
	generator->generate(
		0x0A182, {{Star(Orange), 10},
				  {Gap(), 3}});
	// Pink Bridge 1
	generator->setFlag(Generate::Config::FullGaps);
	generator->setGridSize(3, 3);
	generator->generate(
		0x17DC8, {{Star(Magenta), 4},
				  {Dot(), 4},
				  {Gap(), 2}});
	generator->setGridSize(4, 4);
	generator->generate(
		0x17DC7, {{Star(Magenta), 6},
				  {Dot(), 6},
				  {Gap(), 3}});
	generator->generate(
		0x17CE4, {{Star(Magenta), 6},
				  {Dot(), 6},
				  {Gap(), 3}});
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.84f;
	generator->generate(
		0x17D2D, {{Star(Magenta), 6},
				  {Dot(), 9},
				  {Gap(), 8}});
	generator->generate(
		0x17D6C, {{Star(Magenta), 8},
				  {Dot(), 9},
				  {Gap(), 5}});
	generator->removeFlag(Generate::Config::FullGaps);
	generator->pathWidth = 1;
	// Pink Bridge 2
	generator->setGridSize(3, 3);
	generator->generate(
		0x17D9B, {{Star(Magenta), 4},
				  {Stone(Black), 2},
				  {Stone(White), 2}});
	generator->setGridSize(4, 4);
	generator->generate(
		0x17D99, {{Star(Magenta), 4},
				  {Stone(Black), 4},
				  {Stone(White), 4}});
	generator->generate(
		0x17DAA, {{Star(Magenta), 6},
				  {Stone(Black), 3},
				  {Stone(White), 3}});
	generator->generate(
		0x17D97, {{Star(White), 4},
				  {Stone(Black), 4},
				  {Stone(Magenta), 4}});
	generator->removeFlag(Generate::Config::WriteColors);
	generator->setFlag(Generate::Config::TreehouseColors);
	generator->generate(
		0x17BDF, {{Star(Orange), 6},
				  {Stone(Magenta), 3},
				  {Stone(Green), 3}});
	generator->setGridSize(5, 4);
	generator->setFlagOnce(Generate::Config::DisableReset);
	generator->setFlagOnce(Generate::Config::LongPath);
	generator->generate(
		0x17D91, {{Star(Magenta), 6},
				  {Stone(Orange), 4},
				  {Stone(Green), 4}});
	generator->place_gaps(8);
	generator->write(0x17DC6);
	generator->removeFlag(Generate::Config::TreehouseColors);
	generator->setFlag(Generate::Config::WriteColors);
	// Orange Bridge 1
	generator->setGridSize(3, 3);
	generator->generate(
		0x17DB3, {{Star(Black), 2},
				  {Stone(Black), 2},
				  {Stone(White), 2}});
	generator->generate(
		0x17DB5, {{Star(White), 2},
				  {Stone(Black), 2},
				  {Stone(White), 2}});
	generator->generate(
		0x17DB6, {{Star(Black), 1},
				  {Stone(Black), 2},
				  {Stone(White), 2}});
	generator->generate(
		0x17DC0, {{Star(Black), 1},
				  {Star(White), 1},
				  {Stone(Black), 1},
				  {Stone(White), 2}});
	generator->setGridSize(4, 4);
	generator->removeFlag(Generate::Config::WriteColors);
	generator->setFlag(Generate::Config::AlternateColors); // Black -> Green, White -> Pink, Purple -> White
	generator->generate(
		0x17DD7, {{Star(White), 1},
				  {Stone(Black), 2},
				  {Stone(White), 2},
				  {Stone(Purple), 2}});
	generator->generate(
		0x17DD9, {{Star(White), 2},
				  {Stone(Black), 2},
				  {Stone(White), 2},
				  {Stone(Purple), 2}});
	generator->generate(
		0x17DB8, {{Star(White), 3},
				  {Stone(Black), 2},
				  {Stone(White), 2},
				  {Stone(Purple), 2}});
	generator->generate(
		0x17DDC, {{Star(White), 1},
				  {Star(Black), 1},
				  {Stone(White), 2},
				  {Stone(Black), 2}});
	generator->setGridSize(5, 5);
	generator->setObstructions({{1, 2}, {1, 4}, {9, 2}, {9, 4}, {2, 1}, {4, 1}, {6, 1}, {8, 1}});
	generator->generate(
		0x17DD1, {{Star(White), 2},
				  {Stone(Black), 4},
				  {Stone(White), 4}});
	generator->setGridSize(3, 3);
	generator->generate(
		0x17DDE, {{Star(Black), 2},
				  {Star(White), 2},
				  {Stone(Black), 2},
				  {Stone(White), 2}});
	generator->setGridSize(4, 4);
	generator->generate(
		0x17DE3, {{Star(Black), 2},
				  {Star(White), 2},
				  {Stone(Black), 3},
				  {Stone(White), 3}});
	generator->removeFlag(Generate::Config::AlternateColors);
	generator->setGridSize(4, 4);
	generator->generate(
		0x17DEC, {{Star(Black), 3},
				  {Star(White), 2},
				  {Stone(Black), 2},
				  {Stone(White), 2}});
	generator->generate(
		0x17DAE, {{Star(Black), 3},
				  {Star(White), 3},
				  {Stone(Black), 2},
				  {Stone(White), 2}});
	generator->generate(
		0x17DB0, {{Star(Black), 3},
				  {Star(White), 4},
				  {Stone(Black), 2},
				  {Stone(White), 2}});
	generator->generate(
		0x17DDB, {{Star(Black), 4},
				  {Star(White), 4},
				  {Stone(Black), 2},
				  {Stone(White), 2}});
	// Orange Bridge 2
	generator->setFlag(Generate::Config::TreehouseColors);
	generator->setGridSize(3, 3);
	generator->generate(
		0x17D88, {{Star(Orange), 4},
				  {Star(Magenta), 4},
				  {Gap(), 2}});
	generator->setGridSize(4, 4);
	generator->generate(
		0x17DB4, {{Star(Orange), 4},
				  {Star(Magenta), 4},
				  {Gap(), 8}});
	generator->generate(
		0x17D8C, {{Star(Orange), 6},
				  {Star(Magenta), 4},
				  {Gap(), 6}});
	generator->setObstructions({{1, 2}, {1, 4}, {7, 2}, {7, 4}, {2, 1}, {4, 1}, {6, 1}});
	generator->generate(
		// Treehouse Right Orange 4
		0x17CE3, {{Star(Orange), 4},
				  {Star(Magenta), 4}});
	generator->generate(
		0x17DCD, {{Star(Magenta), 6},
				  {Star(Orange), 6},
				  {Gap(), 3}});
	generator->generate(
		0x17DB2, {{Star(Magenta), 6},
				  {Star(Orange), 6},
				  {Gap(), 3}});
	generator->generate(
		0x17DCC, {{Star(Orange), 8},
				  {Star(Magenta), 4}});
	generator->generate(
		0x17DCA, {{Star(Magenta), 8},
				  {Star(Orange), 6}});
	generator->generate(
		0x17D8E, {{Star(Orange), 4},
				  {Star(Magenta), 2},
				  {Star(Green), 2},
				  {Gap(), 8}});
	generator->setObstructions({{1, 2}, {1, 4}, {7, 2}, {7, 4}, {2, 1}, {4, 1}, {6, 1}});
	generator->setFlagOnce(Generate::Config::FixBackground);
	generator->generate(
		0x17DB7, {{Star(Orange), 4},
				  {Star(Magenta), 4},
				  {Star(Green), 4}});
	generator->setGridSize(4, 3);
	generator->generate(
		0x17DB1, {{Star(Orange), 6},
				  {Star(Magenta), 4},
				  {Star(Green), 2}});
	generator->setGridSize(4, 4);
	generator->generate(
		0x17DA2, {{Star(Orange), 6},
				  {Star(Magenta), 6},
				  {Star(Green), 4}});
	// Green Bridge
	generator->removeFlag(Generate::Config::TreehouseColors);
	generator->setFlag(Generate::Config::AlternateColors);
	generator->setGridSize(4, 4);
	generator->generate(
		0x17E3C, {{Poly(White), 2},
				  {Star(Black), 4}});
	generator->generate(
		0x17E4D, {{Poly(White), 2},
				  {Star(Black), 4}});
	generator->generate(
		0x17E4F, {{Poly(White), 1},
				  {SpinPoly(White), 1},
				  {Star(Black), 6}});
	generator->setGridSize(5, 5);
	generator->setObstructions({{{1, 2}, {1, 4}, {9, 2}, {9, 4}, {2, 1}, {4, 1}, {6, 1}, {8, 1}},
								{{1, 2}, {1, 4}, {1, 6}, {0, 7}, {9, 2}, {9, 4}, {9, 6}, {10, 7}, {4, 1}, {6, 1}, {8, 1}},
								{{1, 2}, {1, 4}, {1, 6}, {0, 7}, {9, 2}, {9, 4}, {9, 6}, {10, 7}, {2, 1}, {4, 1}, {6, 1}}});
	generator->setFlagOnce(Generate::Config::BigShapes);
	generator->generate(
		0x17E52, {{SpinPoly(White), 1},
				  {Star(Black), 6}});
	generator->setGridSize(5, 5);
	generator->generate(
		0x17E5B, {{Poly(White), 2},
				  {Poly(Black), 1},
				  {Star(Black), 3}});
	generator->setGridSize(4, 4);
	generator->setFlag(Generate::Config::RequireCombineShapes);
	generator->generate(
		0x17E5F, {{Poly(White), 2},
				  {NegaPoly(Black), 1},
				  {Star(Black), 5}});
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.8f;
	generator->generate(
		0x17E61, {{Poly(White), 1},
				  {SpinPoly(White), 1},
				  {Star(Black), 4},
				  {Star(Purple), 4}});
}

void PuzzleList::GenerateTownN()
{
	generator->setLoadingData(L"Town", 20);
	generator->resetConfig();

	// Full Dots + Shapes
	generator->setFlag(Generate::Config::EnableFlash);
	generator->generate(
		0x2899C, {{Dot(), 25},
				  {SpinPoly(Any), 1}});
	generator->generate(
		0x28A33, {{Dot(), 25},
				  {Poly(), 2}});
	generator->generate(
		0x28ABF, {{Dot(), 25},
				  {SpinPoly(Any), 1},
				  {Poly(), 1}});
	generator->generate(
		0x28AC0, {{Dot(), 25},
				  {SpinPoly(Any), 2}});
	generator->generate(
		0x28AC1, {{Dot(), 25},
				  {SpinPoly(Any), 2}});
	generator->generate(
		0x28AD9, {{Dot(), 25},
				  {SpinPoly(Any), 2},
				  {Eraser(White), 1}});

	// Blue Symmetry
	generator->setFlag(Generate::Config::WriteDotColor);
	generator->setSymmetry(Panel::Symmetry::Rotational);
	generator->generate(
		0x28AC7, {{Stone(Black), 6},
				  {Stone(White), 6}});
	generator->generate(
		0x28AC8, {{Stone(Black), 6},
				  {Stone(White), 6}});
	generator->generate(
		0x28ACA, {{Stone(Black), 5},
				  {Stone(White), 5},
				  {Dot(Deco::Location::Any), 3}});
	generator->generate(
		0x28ACB, {{Stone(Black), 5},
				  {Stone(White), 5},
				  {Dot(Deco::Location::Any), 3}});
	generator->generate(
		0x28ACC, {{Stone(Black), 5},
				  {Stone(White), 5},
				  {Dot(Deco::Location::Any), 3}});
	generator->setSymmetry(Panel::Symmetry::None);

	// Glass Door
	generator->setFlagOnce(Generate::Config::SmallShapes);
	generator->generate(
		0x28998, {{SpinPoly(Any), 4},
				  {Star(White), 6}});

	// Church Star Door
	generator->removeFlag(Generate::Config::EnableFlash);
	specialCase->generateColorFilterPuzzle(
		0x28A0D,
		{4, 4},
		// These are placeholder non-Any colors that will be overwritten.
		// They merely need to be distinct, but setting to X is probably trouble too.
		{{Star(Deco::Color{1}), 6},
		 {Star(Deco::Color{2}), 6},
		 {Star(Deco::Color{3}), 4}},
		{1, 1, 0, 0}, // Yellow filter
		false // Hard-coded Colorblind off?
	);
	specialCase->setPower(0x28A69, false);
	(new TownDoorWatchdog())->start();
	// Soundproof Room
	std::vector<int> allPitches = {DOT_SMALL, DOT_SMALL, DOT_MEDIUM, DOT_MEDIUM, DOT_LARGE, DOT_LARGE};
	std::vector<int> pitches;
	for (int i = 0; i < 4; i++)
		pitches.push_back(Utilities::pop_random(allPitches));
	specialCase->generateSoundDotPuzzle(0x034E3, {4, 4}, pitches, false);
	generator->resetConfig();
	// 3-color Room
	specialCase->generateRGBStonePuzzleN(0x03C0C);
	specialCase->generateRGBStarPuzzleN(0x03C08);
	// Orange Crate
	generator->setFlag(Generate::Config::EnableFlash);
	generator->generate(
		0x0A0C8, {{Poly(Orange), 3},
				  {Stone(White), 2},
				  {Stone(Black), 2}});
	// Windmill Puzzles
	// The Witness has a weird issue with these particular puzzles where the edge at (3, 4) gets bypassed by the region calculator if you don't draw over it.
	// Because of this, polyominoes can't be used with these puzzles unless Config::PreserveStructure flag is turned off.
	generator->setFlagOnce(Generate::Config::PreserveStructure);
	generator->generate(
		0x17F89, {{Stone(Black), 5},
				  {Stone(White), 5},
				  {Gap(), 7}});
	generator->setFlag(Generate::Config::FixBackground);
	generator->generate(
		0x0A168, {{Stone(White), 8},
				  {Stone(Black), 8},
				  {Eraser(White), 2}});
	generator->setFlagOnce(Generate::Config::WriteColors);
	generator->generate(
		0x33AB2, {{Poly(Orange), 2},
				  {Stone(White), 4},
				  {Stone(Black), 4},
				  {Gap(), 4}});
}

void PuzzleList::GenerateVaultsN()
{
	generator->setLoadingData(L"Vaults", 5);
	generator->resetConfig();
	// Tutorial Vault
	generator->generate(
		0x033D4, {{Stone(White), 10},
				  {Stone(Black), 10},
				  {Dot(Deco::Location::Any), 10},
				  {kStart, 4}});
	// Tetris Vault
	generator->setFlagOnce(Generate::Config::RequireCancelShapes);
	generator->generate(
		0x0CC7B, {{Dot(), 49},
				  {Poly(), 1},
				  {SpinPoly(Any), 2},
				  {NegaPoly(Any), 3}});
	// Symmetry Vault
	generator->setSymmetry(Panel::Symmetry::Rotational);
	generator->generate(
		0x002A6, {{Stone(White), 4},
				  {Stone(Black), 4},
				  {Dot(Deco::Location::Any), 2},
				  {Dot(Deco::Location::Any, Deco::Parity::Blue), 2},
				  {Dot(Deco::Location::Any, Deco::Parity::Yellow), 2}});
	generator->resetConfig();
	// Shipwreck Vault
	specialCase->generateSoundDotReflectionPuzzle(0x00AFB, {7, 7}, {DOT_MEDIUM, DOT_LARGE, DOT_MEDIUM, DOT_SMALL}, {DOT_LARGE, DOT_SMALL, DOT_MEDIUM}, 3, false);
	generator->resetConfig();
	// Jungle Vault
	specialCase->generateJungleVault(0x15ADD);
}

void PuzzleList::GenerateTrianglePanelsN()
{
	generator->setLoadingData(L"Triangles", 12);
	generator->resetConfig();
	generator->setGridSize(3, 3);
	generator->setSymbol(kStart, 0, 0);
	generator->setSymbol(kStart, 0, 6);
	generator->setSymbol(kStart, 6, 6);
	generator->generate(
		0x17D28, {{Triangle(Orange), 6}});
	generator->setGridSize(4, 4);
	generator->generate(
		0x3C12B, {{Triangle(Orange), 6},
				  {Gap(), 2}});
	generator->generate(
		0x17CF0, {{Triangle(Orange), 6},
				  {Gap(), 2}});
	generator->generate(
		0x17FA9, {{Triangle(Orange), 4},
				  {Gap(), 6}});
	generator->generate(
		0x17FA0, {{Triangle(Orange), 6}});
	generator->generate(
		0x17D27, {{Triangle(Orange), 4},
				  {Gap(), 6}});
	generator->generate(
		0x17CFB, {{Triangle(Orange), 7}});
	generator->generate(
		0x17D01, {{Triangle(Orange), 5},
				  {Gap(), 4}});
	generator->generate(
		0x17C71, {{Triangle(Orange), 7}});
	generator->generate(
		0x17CF7, {{Triangle(Orange), 5},
				  {Gap(), 4}});
	generator->generate(
		0x17C42, {{Triangle(Orange), 8}});
	generator->generate(
		0x17CE7, {{Triangle(Orange), 8}});
}

void PuzzleList::GenerateMountainN()
{
	std::wstring text = L"Mountain Perspective";
	SetWindowText(_handle, text.c_str());
	specialCase->generateMountaintop(
		0x17C34,
		{{Stone(Black), 2},
		 {Stone(White), 1},
		 {Star(Black), 1},
		 {Star(White), 1}});

	generator->setLoadingData(L"Mountain", 39);
	generator->resetConfig();
	// Purple Bridge
	generator->setFlagOnce(Generate::Config::PreserveStructure);
	generator->setFlagOnce(Generate::Config::DecorationsOnly);
	std::set<Point> bpoints1 = {{6, 3}, {5, 4}, {7, 4}};
	std::set<Point> bpoints2 = {{6, 5}, {5, 6}, {6, 7}, {7, 6}};
	std::set<Point> bpoints3 = {{3, 6}, {4, 7}};
	if (Random::rand() % 2 == 0)
		generator->hitPoints = {
			Utilities::pick_random(bpoints1), 
			Utilities::pick_random(bpoints2), 
			Utilities::pick_random(bpoints3)};
	else
		generator->hitPoints = {
			Utilities::pick_random(bpoints3), 
			Utilities::pick_random(bpoints2), 
			Utilities::pick_random(bpoints1)};
	generator->setObstructions({{4, 1}, {6, 1}, {8, 1}});
	generator->blockPos = {{1, 1}, {11, 1}, {1, 11}, {11, 11}};
	generator->setSymbol(Gap(Deco::Location::Row), 3, 4);
	generator->generate(
		0x09E39, {{Stone(Black), 3},
				  {Stone(White), 3},
				  {Stone(Purple), 3},
				  {Eraser(White), 1}});

	// Orange Row
	generator->resetConfig();
	generator->generate(
		0x09E73, {{Dot(Deco::Location::Any), 4},
				  {Stone(Black), 2},
				  {Stone(White), 2},
				  {Star(Black), 2},
				  {Star(White), 2}});
	generator->generate(
		0x09E75, {{Dot(Deco::Location::Any), 4},
				  {Stone(Black), 1},
				  {Stone(White), 3},
				  {Star(Black), 3},
				  {Star(White), 1}});
	generator->generate(
		0x09E78, {{Poly(), 2},
				  {Dot(Deco::Location::Any), 3}});
	generator->generate(
		0x09E79, {{SpinPoly(Any), 1},
				  {Poly(), 2},
				  {Gap(), 2}});
	generator->generate(
		0x09E6C, {{Stone(Black), 4},
				  {Stone(White), 4},
				  {Star(Purple), 4},
				  {Gap(), 2}});
	generator->generate(
		0x09E6F, {{Poly(), 1},
				  {SpinPoly(Any), 1},
				  {Star(Magenta), 4}});
	generator->generate(
		0x09E6B, {{Dot(Deco::Location::Any), 8},
				  {Star(Magenta), 4}});
	// Green Row
	generator->generate(
		0x09E7A, {{Dot(Deco::Location::Any), 6},
				  {Stone(Black), 3},
				  {Stone(White), 3}});
	generator->blockPos = {{0, 0}, {3, 8}, {8, 3}, {0, 5}, {5, 0}, {8, 8}};
	generator->generate(
		0x09E71, {{Dot(Deco::Location::Any), 6},
				  {Stone(Black), 4},
				  {Stone(White), 2}});
	generator->blockPos = {{0, 0}, {3, 8}, {8, 3}, {0, 5}, {5, 0}, {8, 8}};
	generator->generate(
		0x09E72, {{Dot(Deco::Location::Any), 4},
				  {Stone(Black), 3},
				  {Stone(White), 2},
				  {Poly(), 1}});
	generator->blockPos = {{0, 3}, {3, 0}, {5, 8}, {8, 5}};
	generator->generate(
		0x09E69, {{Dot(Deco::Location::Any), 6},
				  {Stone(Black), 3},
				  {Stone(White), 3}});
	generator->generate(
		0x09E7B, {{Dot(Deco::Location::Any), 10},
				  {Stone(Black), 3},
				  {Stone(White), 2}});
	// Purple Panels
	generator->setFlag(Generate::Config::RequireCombineShapes);
	generator->generate(
		0x09EAD, {{Poly(), 2},
				  {Stone(Black), 4},
				  {Stone(White), 3}});
	generator->generate(
		0x09EAF, {{Poly(), 2},
				  {Stone(Black), 4},
				  {Stone(White), 4}});
	generator->resetConfig();
	// Blue Row
	generator->setObstructions({{4, 3}, {5, 4}, {5, 6}, {5, 8}, {5, 10}, {6, 9}, {7, 10}});
	generator->generate(
		0x33AF5, {{Dot(Deco::Location::Any), 3},
				  {Stone(Black), 4},
				  {Stone(White), 4}});
	if (Random::rand() % 2 == 0)
		generator->setObstructions({{5, 4}, {5, 6}, {5, 8}, {5, 10}, {9, 4}, {9, 6}, {9, 8}, {9, 10}, {7, 0}, {7, 2}});
	else
		generator->setObstructions({{3, 4}, {3, 6}, {3, 8}, {3, 10}, {7, 4}, {7, 6}, {7, 8}, {7, 10}, {5, 0}, {5, 2}, {9, 0}, {9, 2}});
	generator->generate(
		0x33AF7, {{Stone(Black), 7},
				  {Stone(White), 5},
				  {Star(Orange), 4}});
	generator->setObstructions({{0, 1}, {0, 3}, {0, 5}, {0, 7}, {9, 4}, {1, 4}, {1, 6}, {1, 8}, {2, 7}, {2, 9}, {3, 8}, {3, 10}, {4, 9}, {5, 8}, {5, 10}, {6, 7}, {6, 9}, {7, 6}, {7, 8}, {7, 10}, {8, 5}, {8, 7}, {8, 9}, {9, 2}, {9, 4}, {9, 6}, {9, 8}, {10, 1}, {10, 3}, {10, 5}});
	generator->blockPos = {{5, 0}, {6, 0}, {4, 2}, {5, 2}, {4, 3}, {3, 4}};
	generator->generate(
		0x09F6E, {{Dot(Deco::Location::Any), 6}});

	// Rainbow Row
	generator->resetConfig();
	generator->setFlag(Generate::Config::WriteColors);
	generator->generate(
		0x09FD3, {{Stone(Magenta), 2},
				  {Stone(Green), 2},
				  {Star(Magenta), 2},
				  {Star(Green), 2}});
	generator->generate(
		0x09FD4, {{Stone(Magenta), 2},
				  {Stone(Green), 2},
				  {Star(Magenta), 2},
				  {Star(Green), 3}});
	generator->generate(
		0x09FD6, {{Stone(Cyan), 2},
				  {Stone(Yellow), 2},
				  {Star(Cyan), 3},
				  {Star(Yellow), 3}});
	generator->setGridSize(5, 5);
	generator->generate(
		0x09FD7,
		{{Stone(Cyan), 2},
		 {Stone(Magenta), 2},
		 {Star(Cyan), 2},
		 {Star(Magenta), 1},
		 {Poly(Cyan), 1},
		 {Poly(Magenta), 1}});
	generator->setFlagOnce(Generate::Config::Write2Color);
	generator->setSymbol(kStart, 10, 0);
	generator->setSymbol(kStart, 0, 10);
	generator->setSymbol(kExit, 0, 0);
	generator->setSymbol(kExit, 10, 10);
	generator->setSymmetry(Panel::Symmetry::Rotational);
	generator->generate(
		0x09FD8, {{Dot(Deco::Location::Any, Deco::Parity::Blue), 4},
				  {Dot(Deco::Location::Any, Deco::Parity::Orange), 4}});
	generator->resetConfig();

	specialCase->generateMultiPuzzle(
		{0x09FCC, 0x09FCE, 0x09FCF, 0x09FD0, 0x09FD1, 0x09FD2},
		{{{Dot(), 5}},

		 {{Stone(Black), 2},
		  {Stone(White), 2}},

		 {{Star(Orange), 2},
		  {Star(Magenta), 2},
		  {Star(Green), 2}},

		 {{SpinPoly(Any), 1}},

		 {{Stone(Cyan), 2},
		  {Stone(Yellow), 1},
		  {Star(Cyan), 1},
		  {Star(Yellow), 1}},

		 {{Poly(), 2}}},
		false);

	specialCase->generate2Bridge(0x09E86, 0x09ED8);

	specialCase->generateMountainFloor();

	// Pillar Puzzles
	generator->setGridSize(6, 3);
	generator->generate(
		0x0383D, {{Dot(Deco::Location::Any), 15},
				  {Gap(), 6}});
	generator->resetConfig();
	generator->setFlag(Generate::Config::WriteColors);
	generator->setFlag(Generate::Config::DecorationsOnly);
	generator->generate(
		0x0383A, {{Star(Orange), 6},
				  {Star(Magenta), 4}});
	generator->generate(
		0x0383F, {{Stone(Black), 4},
				  {Stone(White), 4}});
	generator->removeFlag(Generate::Config::DecorationsOnly);
	generator->generate(
		0x09E56, {{Star(Orange), 4},
				  {Dot(Deco::Location::Any), 8}});
	generator->removeFlag(Generate::Config::WriteColors);
	generator->setFlagOnce(Generate::Config::RequireCombineShapes);
	generator->setFlagOnce(Generate::Config::DecorationsOnly);
	generator->generate(
		0x03859, {{Poly(), 3}});
	generator->setSymbol(kStart, 4, 8);
	generator->generate(
		0x09E5A, {{Dot(), 30},
				  {Gap(), 10}});
	generator->setFlagOnce(Generate::Config::WriteColors);
	generator->setSymmetry(Panel::Symmetry::PillarRotational);
	generator->setFlagOnce(Generate::Config::DecorationsOnly);
	generator->generate(
		0x339BB, {{Star(Orange), 4},
				  {Stone(Black), 2},

				  {Stone(White), 2}});
	generator->setSymmetry(Panel::Symmetry::PillarParallel);
	generator->setFlagOnce(Generate::Config::DisableDotIntersection);
	generator->generate(
		0x33961, {{Dot(Deco::Location::Any), 8}});
}

void PuzzleList::GenerateCavesN()
{
	generator->setLoadingData(L"Caves", 51);
	generator->resetConfig();
	generator->generate(
		0x17FA2, {{Triangle(Orange), 10}});
	generator->generate(
		0x00FF8, {{Stone(Black), 4},
				  {Stone(White), 3},

				  {Triangle(Orange), 4}});

	// Blue Symmetry/Tetris
	generator->setFlag(Generate::Config::RequireCombineShapes);
	generator->setGridSize(5, 5);
	generator->generate(
		0x01A0D, {{Triangle(Orange), 6}});
	generator->generate(
		0x018A0, {{Poly(), 3}});
	generator->resetConfig();
	generator->setFlag(Generate::Config::DisconnectShapes);
	generator->setFlagOnce(Generate::Config::BigShapes);
	generator->generate(
		0x009A4, {{Poly(), 3}});
	generator->generate(
		0x00A72, {{Poly(), 3},
				  {NegaPoly(Any), 1}});
	generator->resetConfig();

	// Triangle Puzzles
	generator->setFlag(Generate::Config::WriteDotColor);
	generator->generate(
		0x00190, {{Dot(Deco::Location::Any), 25},
				  {Triangle(Orange), 4}});
	generator->generate(
		0x00558, {{Dot(Deco::Location::Any), 25},
				  {Triangle(Orange), 5}});
	generator->generate(
		0x00567, {{Dot(Deco::Location::Any), 25},
				  {Triangle(Orange), 5}});
	generator->generate(
		0x006FE, {{Dot(Deco::Location::Any), 25},
				  {Triangle(Orange), 6}});
	generator->resetConfig();

	generator->generate(
		0x008B8, {{Stone(Black), 3},
				  {Stone(White), 3},
				  {Triangle(Orange), 4}});
	generator->generate(
		0x00973, {{Star(Black), 4},
				  {Star(White), 4},
				  {Triangle(Orange), 4}});
	generator->generate(
		0x0097B, {{Star(Orange), 3},
				  {Triangle(Orange), 4}});
	generator->generate(
		0x0097D, {{Star(Black), 2},
				  {Star(White), 2},
				  {Stone(Black), 1},
				  {Stone(White), 1},
				  {Triangle(Orange), 4}});
	generator->generate(
		0x0097E, {{Star(Black), 2},
				  {Star(Orange), 2},
				  {Stone(Black), 1},
				  {Stone(Orange), 1},
				  {Triangle(Orange), 4}});

	generator->setGridSize(4, 4);
	generator->generate(
		0x00994, {{SpinPoly(Any), 1},
				  {Triangle(Orange), 4}});
	generator->setFlagOnce(Generate::Config::BigShapes);
	generator->generate(
		0x334D5, {{SpinPoly(Any), 1},
				  {Triangle(Orange), 3}});
	generator->setGridSize(5, 5);
	generator->setFlagOnce(Generate::Config::BigShapes);
	generator->generate(
		0x00995, {{SpinPoly(Any), 1},
				  {Triangle(Orange), 6}});
	generator->generate(
		0x00996, {{Poly(), 2},
				  {Triangle(Orange), 5}});
	generator->setFlagOnce(Generate::Config::BigShapes);
	generator->generate(
		0x00998, {{Poly(), 2},
				  {Triangle(Orange), 4}});
	generator->resetConfig();

	// First alcove
	generator->setFlagOnce(Generate::Config::DisconnectShapes);
	generator->generate(
		0x32962, {{SpinPoly(Any), 3}});
	generator->setFlagOnce(Generate::Config::WriteColors);
	generator->generate(
		0x32966, {{Star(Black), 4},
				  {Star(White), 4},
				  {Star(Orange), 4},
				  {Stone(Black), 1},
				  {Stone(White), 1}});
	generator->setFlagOnce(Generate::Config::WriteColors);
	generator->generate(
		0x01A31, {{Stone(Cyan), 16},
				  {Stone(Yellow), 14}});
	generator->setFlagOnce(Generate::Config::AlternateColors);
	generator->generate(
		0x00B71, {{Stone(White), 2},
				  {Stone(Black), 1},
				  {Star(White), 2},
				  {Star(Black), 3},
				  {Eraser(White), 1}});
	generator->resetConfig();

	// Perspective
	generator->setFlag(Generate::Config::DecorationsOnly);
	generator->setFlag(Generate::Config::EnableFlash);
	std::vector<std::vector<Point>> obstructions = {{{5, 0}, {5, 2}, {5, 4}}, {{5, 6}, {5, 8}, {5, 10}}, {{0, 5}, {2, 5}, {4, 5}}, {{6, 5}, {8, 5}, {10, 5}}};
	generator->setObstructions(obstructions);
	generator->blockPos = {{5, 5}};
	generator->generate(
		0x288EA, {{Poly(), 3}});
	generator->setObstructions(obstructions);
	generator->blockPos = {{5, 5}};
	generator->generate(
		0x288FC, {{Poly(), 1},
				  {SpinPoly(Any), 1},
				  {Stone(Black), 3},
				  {Stone(White), 3}});
	generator->setObstructions(obstructions);
	generator->blockPos = {{5, 5}};
	generator->generate(
		0x289E7, {{Stone(Black), 4},
				  {Stone(White), 3},
				  {Star(Orange), 6}});
	generator->setObstructions(obstructions);
	generator->blockPos = {{5, 5}};
	generator->setFlagOnce(Generate::Config::RequireCombineShapes);
	generator->generate(
		0x288AA, {{Poly(), 2},
				  {Star(Black), 6},
				  {Star(White), 4}});
	generator->resetConfig();

	// Full Dots
	generator->setFlag(Generate::Config::EnableFlash);
	generator->generate(
		0x0A16B, {{Dot(), 33}});
	generator->generate(
		0x0A2CE, {{Dot(), 25},
				  {Star(Black), 6},
				  {Star(White), 2}});
	generator->generate(
		0x0A2D7, {{Dot(), 25},
				  {Star(Black), 1},
				  {Star(White), 2},
				  {Stone(Black), 2},
				  {Stone(White), 1}});
	generator->generate(
		0x0A2DD, {{Dot(), 25},
				  {Poly(Black), 3}});
	generator->generate(
		0x0A2EA, {{Dot(), 25},
				  {SpinPoly(Black) , 2}});
	generator->setSymbol(kStart, 0, 0);
	generator->setSymbol(kStart, 6, 6);
	generator->setSymbol(kStart, 0, 6);
	generator->generate(
		0x17FB9, {{Dot(), 16},
				  {Poly(Black), 2},
				  {NegaPoly(Black) , 1}});
	generator->resetConfig();

	// Invisible Dots
	generator->generate(
		0x0008F, {{Dot(Deco::Location::Intersection), 2}});
	generator->generate(
		0x0006B, {{Dot(Deco::Location::Any), 3}});
	generator->generate(
		0x0008B, {{Dot(Deco::Location::Any), 6}});
	generator->generate(
		0x0008C, {{Dot(Deco::Location::Any), 6}});
	generator->generate(
		0x0008A, {{Dot(Deco::Location::Any), 9}});
	generator->generate(
		0x00089, {{Dot(Deco::Location::Any), 9}});
	generator->generate(
		0x0006A, {{Dot(Deco::Location::Any), 9}});
	generator->generate(
		0x0006C, {{Dot(Deco::Location::Any), 12}});
	generator->generate(
		0x00027, {{Dot(Deco::Location::Any), 3}});
	generator->generate(
		0x00028, {{Dot(Deco::Location::Any), 7}});
	generator->generate(
		0x00029, {{Dot(Deco::Location::Any), 7}});

	generator->setFlag(Generate::Config::EnableFlash);

	// Triangle Exit
	generator->generate(
		0x17CF2, {{Triangle(Orange, 1), 6}});

	// Stars Exit
	generator->generate(
		0x021D7, {{Triangle(Cyan), 2},
				  {Triangle(Yellow), 2},
				  {Star(Cyan), 2},
				  {Star(Yellow), 2}});

	// Challenge Pillar
	generator->generate(
		0x09DD5, {{Triangle(Orange), 10}});

	// Challenge Entrance
	generator->generate(
		0x0A16E, {{Poly(Green), 4},
				  {Star(Green), 3}});

	// Theater Exit
	generator->generate(
		0x039B4, {{Triangle(Orange, 2), 12}});

	// Town Exit
	generator->generate(
		0x09E85, {{Triangle(Orange), 12},
				  {kStart, 3}});
}

void PuzzleList::GenerateOrchardN() {
	specialCase->generateApplePuzzle(0x032FF, true, false);
}

void PuzzleList::GenerateDesertN() {
	Generate generate;
	Memory *memory = Memory::get();
	// auto texloader = TextureLoader::get();
	// texloader->forceLoadDesertTextures();
	memory->LoadPackage("globals");
	memory->LoadPackage("globals");
	memory->LoadPackage("globals");
	memory->LoadPackage("globals");
	// Surface
	specialCase->generateSpecularPuzzle(0x00698, HEXAGON_GRID);
	specialCase->generateSpecularPuzzle(0x0048F, HEXAGON_GRID, {{3, 9}, {5, 6}, {5, 11}, {6, 12}});
	specialCase->generateSpecularPuzzle(0x09F92, {{0, 1}, {1, 7}, {0, 4}, {4, 10}});
	memory->WritePanelData<float>(0x09FA1, OPEN_RATE, {0.2f}); // Desert Surface 3 Control, 2x
	specialCase->generateSpecularPuzzle(0x0A036);
	specialCase->setOrientation(0x0A036, -3 + 10 * (Random::rand() % 3 - 1), -40 + 10 * (Random::rand() % 3 - 1), 0);
	specialCase->generateSpecularPuzzle(0x09DA6);
	specialCase->setOrientation(0x09DA6, -3 + 10 * (Random::rand() % 3 - 1), -15 + 10 * (Random::rand() % 3 - 1), 0);
	specialCase->generateSpecularPuzzle(0x0A049);
	specialCase->setOrientation(0x0A049, -3 + 10 * (Random::rand() % 3 - 1), 10 + 10 * (Random::rand() % 3 - 1), 0);
	specialCase->generateSpecularPuzzle(0x0A053);
	std::vector<int> sol = memory->ReadArray<int>(0x0A053, SEQUENCE, memory->ReadPanelData<int>(0x0A053, SEQUENCE_LEN));
	while (std::find(sol.begin(), sol.end(), 2) == sol.end() && std::find(sol.begin(), sol.end(), 3) == sol.end() ||
		   std::find(sol.begin(), sol.end(), 0) == sol.end() && std::find(sol.begin(), sol.end(), 1) == sol.end() && std::find(sol.begin(), sol.end(), 4) == sol.end() ||
		   std::find(sol.begin(), sol.end(), 5) == sol.end() && std::find(sol.begin(), sol.end(), 6) == sol.end())
	{
		specialCase->generateSpecularPuzzle(0x0A053);
		sol = memory->ReadArray<int>(0x0A053, SEQUENCE, memory->ReadPanelData<int>(0x0A053, SEQUENCE_LEN));
	}
	specialCase->generateSpecularPuzzle(0x09F94);
	specialCase->setPower(0x09F94, false);						// Turn off desert surface 8
	memory->WritePanelData<float>(0x09F95, OPEN_RATE, {0.04f}); // Desert Surface Final Control, 4x
	// Light Room
	specialCase->generateSpecularPuzzle(0x00422);
	specialCase->generateSpecularPuzzle(0x006E3, {{0, 1}, {1, 7}, {0, 4}, {4, 10}});
	specialCase->generateSpecularPuzzle(0x0A02D, {{0, 1}, {0, 4}, {1, 5}, {4, 5}, {5, 6}, {5, 19}, {19, 20}, {9, 20}, {6, 21}, {21, 22}, {10, 22}, {9, 10}});
	// Pond Room
	specialCase->generateSpecularPuzzle(0x00C72);
	specialCase->generateSpecularPuzzle(0x0129D, HEXAGON_GRID);
	specialCase->generateSpecularPuzzle(0x008BB);
	specialCase->generateSpecularPuzzle(0x0078D);
	specialCase->generateSpecularPuzzle(0x18313);
	// Flood Room
	memory->WritePanelData<float>(0x01300, OPEN_RATE, {0.09f}); // Desert Flood Water Level, 3x
	specialCase->generateSpecularPuzzle(0x04D18);
	specialCase->generateSpecularPuzzle(0x01205);
	specialCase->generateSpecularPuzzle(0x181AB);
	specialCase->generateSpecularPuzzle(0x0117A);
	specialCase->generateSpecularPuzzle(0x17ECA);
	std::vector<float> positions = memory->ReadArray<float>(0x18076, DOT_POSITIONS, 38);
	if (positions[1] > 0.9f)
	{
		for (int i = 1; i < positions.size() - 1; i += 2)
		{
			positions[i] -= 0.03f;
		}
		memory->WriteArray<float>(0x18076, DOT_POSITIONS, positions);
	}
	specialCase->generateSpecularPuzzle(0x18076);		   // Rectangular
	specialCase->setTargetAndDeactivate(0x17ECA, 0x18076); // Change desert floating target to desert flood final
	// Final Room
	specialCase->generateSpecularPuzzle(0x0A15C); // Concave
	specialCase->generateSpecularPuzzle(0x09FFF); // Convex
	generate.setSymbol(kStart, 0, 0);
	generate.setSymbol(kExit, 12, 6);
	generate.setGridSize(6, 3);
	generate.generate(0x0A15F);
	generate.initPanel(0x0A15F);
	generate._panel->minx = 0.05f;
	generate._panel->maxx = 0.9f;
	generate.generate(0x0A15F);
	specialCase->generateSpecularPuzzle(0x0A15F);				// Tall
	specialCase->generateSpecularPuzzle(0x17C31);				// Glass
	specialCase->generateSpecularPuzzle(0x012D7);				// Final
	memory->WritePanelData<float>(0x012C8, OPEN_RATE, {0.06f}); // Desert Final Far Control, 2x

	// Ensure textures are preserved
	memory->LoadPackage("save_58392");
	memory->LoadPackage("save_58473");
	memory->LoadPackage("save_58413");
	memory->LoadPackage("save_58440");
	memory->LoadPackage("globals");
}

void PuzzleList::GenerateKeepN()
{
	generator->setLoadingData(L"Keep", 5);
	generator->resetConfig();

	generator->setSymbol(Gap(Deco::Location::Column), 8, 3);
	generator->setSymbol(Gap(Deco::Location::Column), 4, 5);
	generator->setSymbol(Gap(Deco::Location::Row), 3, 0);
	generator->setSymbol(Gap(Deco::Location::Row), 3, 2);
	generator->setSymbol(Gap(Deco::Location::Row), 5, 6);
	generator->setFlagOnce(Generate::Config::DisableWrite);
	generator->setObstructions({{1, 4}, {2, 3}, {5, 4}, {5, 8}});
	generator->generate(
		0x033EA);
	std::set<Point> path1 = generator->_path;
	std::vector<std::vector<Point>> sets = {{{7, 8}, {8, 7}, {7, 6}, {6, 7}}, {{6, 5}, {7, 4}, {8, 5}}, {{7, 0}, {7, 2}, {6, 1}, {8, 1}, {5, 2}}, {{2, 7}, {4, 7}, {3, 8}, {3, 6}, {1, 6}}, {{0, 1}, {1, 0}, {2, 1}, {1, 2}}};
	for (std::vector<Point> set : sets)
	{
		Point p = Utilities::pick_random(set);
		while (!path1.count(p))
			p = Utilities::pick_random(set);
		if (p.first % 2 == 0) {
			generator->set(p, Dot(Deco::Location::Column));
		} else {
			generator->set(p, Dot(Deco::Location::Row));
		}
	}
	generator->write(0x033EA);

	generator->resetConfig();
	generator->setObstructions({{3, 2}, {8, 5}});
	generator->setFlagOnce(Generate::Config::DisableWrite);
	generator->generate(
		0x01BE9, {{Star(Black), 2},
				  {Star(White), 2},

				  {Stone(Black), 6},
				  {Stone(White), 6}});
	std::set<Point> path2 = generator->_path;
	generator->write(0x01BE9);

	generator->resetConfig();
	std::vector<std::vector<Point>> validHitPoints = {
		{{5, 8}, {3, 4}, {7, 2}, {3, 2}}, {{8, 7}, {7, 2}, {6, 7}, {5, 8}}, {{5, 8}, {4, 5}, {7, 2}, {2, 5}}, {{5, 8}, {3, 6}, {7, 2}, {3, 4}}, {{5, 8}, {1, 6}, {7, 2}, {1, 4}}, {{5, 8}, {4, 3}, {7, 2}, {2, 3}}, {{5, 8}, {3, 4}, {7, 2}, {3, 2}}, {{5, 8}, {1, 4}, {7, 2}, {1, 2}}, {{5, 8}, {3, 2}, {7, 2}, {3, 0}}, {{5, 8}, {1, 2}, {7, 2}, {1, 0}}};
	generator->hitPoints = validHitPoints[Random::rand() % validHitPoints.size()];
	generator->setFlagOnce(Generate::Config::DisableWrite);
	generator->generate(
		0x01CD3, {{Poly(), 2},
				  {Stone(Black), 1},
				  {Stone(White), 1},

				  {Stone(Cyan), 1},
				  {Stone(Magenta), 1}});
	std::set<Point> path3 = generator->_path;
	generator->write(0x01CD3);

	generator->resetConfig();
	generator->setSymmetry(Panel::Symmetry::Rotational);
	generator->setFlagOnce(Generate::Config::DisableWrite);
	generator->generate(
		0x01D3F, {{Poly(), 3}});
	if (generator->_panel->_endpoints[0].GetX() == 0)
	{
		std::swap(generator->_panel->_endpoints[0], generator->_panel->_endpoints[1]); // Need to have endpoints in right order to associate with pressure plates correctly
	}
	std::set<Point> path4 = (generator->_path1.count(Point(0, 8)) ? generator->_path2 : generator->_path1);
	if (generator->_path.count({7, 0}))
		generator->set(7, 0, Dot(Deco::Location::Row));
	else
		generator->set(8, 1, Dot(Deco::Location::Column));
	generator->write(0x01D3F);

	specialCase->generateKeepLaserPuzzle(
		0x03317, path1, path2, path3, path4,
		{{Dot(Deco::Location::Any), 4},
		 {Star(Black), 4},
		 {Star(White), 4},
		 {Stone(Black), 7},
		 {Stone(White), 5},
		 {Stone(Cyan), 1},
		 {Stone(Magenta), 1},
		 {Poly(), 5}});

	specialCase->clearTarget(0x0360E); // Must solve pressure plate side
}

void PuzzleList::GenerateJungleN()
{
	// Can't random sound cues just yet. I found the file names in memory, but have yet to figure out where the actual sound files are.
	// specialCase->testFind({ 'b', 'i', 'r', 'd', '3' }); //0x52a230
	// specialCase->testFind({ 'b', 'i', 'r', 'd', '4' }); //0x52a2d8
	// So the only thing that can be randomized currently, are the actual solutions.

	generator->setLoadingData(L"Jungle", 4);
	generator->resetConfig();
	specialCase->generateSoundDotPuzzle(0x0026D, {2, 2}, {DOT_SMALL, DOT_LARGE}, false);
	specialCase->generateSoundDotPuzzle(0x0026E, {2, 2}, {DOT_SMALL, DOT_LARGE}, false);
	specialCase->generateSoundDotPuzzle(0x0026F, {4, 4}, {DOT_MEDIUM, DOT_MEDIUM, DOT_SMALL, DOT_MEDIUM, DOT_LARGE}, false);
	if (Random::rand() % 2){
		specialCase->generateSoundDotPuzzle(
			0x00C3F,
			{4, 4},
			{DOT_SMALL, DOT_MEDIUM, DOT_SMALL, DOT_LARGE}, true);
	} else {
		specialCase->generateSoundDotPuzzle(
			0x00C3F,
			{4, 4},
			{DOT_LARGE, DOT_MEDIUM, DOT_MEDIUM, DOT_SMALL, DOT_LARGE}, true);
	}

	if (Random::rand() % 2){
		specialCase->generateSoundDotPuzzle(
			0x00C41,
			{4, 4},
			{DOT_SMALL, DOT_SMALL, DOT_LARGE, DOT_MEDIUM, DOT_LARGE}, true);
	} else {
		specialCase->generateSoundDotPuzzle(
			0x00C41,
			{4, 4},
			{DOT_MEDIUM, DOT_MEDIUM, DOT_SMALL, DOT_MEDIUM, DOT_LARGE}, true);
	}

	if (Random::rand() % 2) {
		specialCase->generateSoundDotPuzzle(
			0x014B2,
			{4, 4},
			{DOT_SMALL, DOT_LARGE, DOT_SMALL, DOT_LARGE, DOT_MEDIUM}, true);
	} else {
		specialCase->generateSoundDotPuzzle(
			0x014B2,
			{4, 4},
			{DOT_LARGE, DOT_MEDIUM, DOT_SMALL, DOT_LARGE, DOT_SMALL}, true);
	}
}

//--------------------------HARD MODE-----------------------------

void PuzzleList::GenerateTutorialH()
{
	generator->setLoadingData(L"Tutorial", 21);
	generator->resetConfig();
	Special::drawSeedAndDifficulty(0x00064, seed, true, !seedIsRNG, false);
	Special::drawGoodLuckPanel(0x00182);
	generator->setFlag(Generate::Config::WriteDotColor);
	generator->setFlag(Generate::Config::LongestPath);
	// Mazes
	generator->setFlag(Generate::Config::FullGaps);
	generator->setGridSize(4, 4);
	generator->generate(
		0x00293, {{Dot(), 8},
				  {Gap(), 8}});
	generator->setGridSize(6, 6);
	generator->generate(
		0x00295, {{Dot(), 12},
				  {Gap(), 18}});
	generator->setGridSize(8, 8);
	generator->generate(
		0x002C2, {{Dot(), 21},
				  {Gap(), 32},
				  {kStart, 3}});
	generator->removeFlag(Generate::Config::FullGaps);
	// 2 starts maze
	generator->setGridSize(5, 5);
	generator->setFlagOnce(Generate::Config::DisableWrite);
	generator->generate(
		0x0A3B2, {{Dot(), 36},
				  {Gap(), 12}});
	generator->set(0, 9, 0);
	generator->set(1, 10, 0);
	generator->set(9, 10, 0);
	generator->set(10, 9, 0);
	generator->write(0x0A3B2);
	// 2 exits maze
	generator->setGridSize(5, 5);
	generator->setSymbol(kStart, 0, 0);
	generator->setFlagOnce(Generate::Config::DisableWrite);
	generator->generate(
		0x0A3B5, {{Dot(), 36},
				  {Gap(), 12}});
	generator->set(9, 0, 0);
	generator->set(10, 1, 0);
	generator->set(9, 10, 0);
	generator->set(10, 9, 0);
	generator->write(0x0A3B5);
	generator->resetConfig();
	// Exit Gate
	specialCase->modifyGate(0x03629);
	// Secret back area
	generator->generate(
		0x0A171, {{Dot(), 25},
				  {Triangle(Orange), 6}});
	generator->generate(
		0x04CA4, {{Dot(), 25},
				  {SpinPoly(Yellow), 1},
				  {Poly(Yellow), 1}});
	// Dots Tutorial
	{
		int gridx = 4;
		int gridy = 4;
		generator->setGridSize(gridx, gridy);
		generator->setFlagOnce(Generate::Config::DisableWrite);
		generator->generate(
			0x0005D, {{kStart, 2},
					  {kExit, 1},
					  {Dot(), 25}});
		// Place both a start and a dot at a diagonal grid of nodes.
		for (int x = 0; x <= gridx; x++)
			for (int y = 0; y <= gridy; y++)
				if ((x + y) % 2 != generator->_parity)
					generator->set({x * 2, y * 2}, kStart | Dot()); // Special function Overlay(symbolA, symbolB)? Rely on starts being processed separately?
		generator->write(0x0005D);
	}
	{
		int gridx = 5;
		int gridy = 5;
		generator->setGridSize(gridx, gridy);
		generator->setFlagOnce(Generate::Config::DisableWrite);
		generator->generate(
			0x0005E, {{kStart, 2},
					{kExit, 1},
					{Dot(), 36}});
		// Place both a start and a dot at a diagonal grid of nodes.
		for (int x = 0; x <= gridx; x++)
			for (int y = 0; y <= gridy; y++)
				if ((x + y) % 2 != generator->_parity)
					generator->set({x * 2, y * 2}, kStart | Dot()); // Special function Overlay(symbolA, symbolB)? Rely on starts being processed separately?
		generator->write(0x0005E);
	}
	generator->generate(
		0x0005F, {{kStart, 8},
				  {kExit, 1},
				  {Dot(), 36}});
	generator->generate(
		0x00060, {{kStart, 8},
				  {kExit, 1},
				  {Dot(), 36}});
	generator->setGridSize(6, 6);
	generator->generate(
		0x00061, {{kStart, 10},
				  {kExit, 1},
				  {Dot(), 49}});
	// Stones Tutorial
	generator->resetConfig();
	generator->setFlag(Generate::Config::WriteDotColor);
	generator->setGridSize(5, 5);
	generator->generate(
		0x018AF, {{kExit, 1},
				  {Stone(Black), 11},
				  {Stone(White), 8},
				  {kStart, 3}});
	generator->generate(
		0x0001B, {{kExit, 1},
				  {Stone(Black), 7},
				  {Stone(White), 5},
				  {Gap(), 10},
				  {kStart, 3}});
	generator->setGridSize(4, 4);
	generator->generate(
		0x012C9, {{kExit, 1},
				  {Dot(Deco::Location::Any), 6},
				  {Stone(Black), 5},
				  {Stone(White), 3},
				  {kStart, 1}});
	generator->generate(
		0x0001C, {{kExit, 1},
				  {Dot(Deco::Location::Any), 6},
				  {Stone(Black), 5},
				  {Stone(White), 3},
				  {kStart, 3}});
	generator->setGridSize(5, 5);
	generator->generate(
		0x0001D, {{kExit, 1},
				  {Dot(Deco::Location::Any), 10},
				  {Stone(Black), 7},
				  {Stone(White), 5},
				  {kStart, 3}});
	generator->generate(
		0x0001E, {{kExit, 1},
				  {Dot(Deco::Location::Any), 10},
				  {Stone(Black), 7},
				  {Stone(White), 5},
				  {kStart, 3}});
	generator->setGridSize(4, 4);
	generator->generate(
		0x0001F, {{kExit, 1},
				  {Dot(Deco::Location::Intersection), 25},
				  {Stone(Black), 3},
				  {Stone(White), 3},
				  {kStart, 1}});
	generator->setGridSize(5, 5);
	generator->generate(
		0x00020, {{kExit, 1},
				  {Dot(Deco::Location::Intersection), 36},
				  {Stone(Black), 6},
				  {Stone(White), 4},
				  {kStart, 1}});
	generator->generate(
		0x00021, {{kExit, 1},
				  {Dot(Deco::Location::Intersection), 36},
				  {Stone(Black), 5},
				  {Stone(White), 5},
				  {kStart, 1}});
}

void PuzzleList::GenerateSymmetryH()
{
	generator->setLoadingData(L"Symmetry", 34);
	generator->resetConfig();
	generator->setFlag(Generate::Config::StartEdgeOnly);
	// Symmetry Mazes
	generator->setFlag(Generate::Config::WriteDotColor2);
	generator->setSymmetry(Panel::Symmetry::Vertical);
	generator->setGridSize(7, 7);
	generator->pathWidth = 0.8f;
	generator->generate(
		0x00086, {{Gap(), 12},
				  {Dot(), 8},
				  {kStart, 1},
				  {kExit, 1}});
	generator->removeFlagOnce(Generate::Config::StartEdgeOnly);
	generator->generate(
		0x00087, {{Gap(), 12},
				  {Dot(), 8},
				  {kStart, 3},
				  {kExit, 1}});
	generator->setSymmetry(Panel::Symmetry::Rotational);
	generator->generate(
		0x00059, {{Gap(), 12},
				  {Dot(), 10},
				  {kStart, 1},
				  {kExit, 1}});
	generator->removeFlagOnce(Generate::Config::StartEdgeOnly);
	generator->generate(
		0x00062, {{Gap(), 12},
				  {Dot(), 10},
				  {kStart, 1},
				  {kExit, 1}});
	generator->setGridSize(11, 8);
	generator->pathWidth = 1;
	generator->setSymbol(kStart, 0, 8);
	generator->setSymbol(kStart, 22, 8);
	generator->setSymbol(kExit, 0, 6);
	generator->setSymbol(kExit, 22, 10);
	generator->setObstructions({{0, 15}, {1, 16}, {11, 16}, {12, 15}, {13, 16}});
	generator->hitPoints = {{22, 7}, {22, 5}, {22, 3}, {22, 1}, {21, 0}, {10, 1}};
	generator->setFlagOnce(Generate::Config::DisableWrite);
	generator->generate(
		0x0005C, {{Gap(), 30},
				  {Dot(), 1}});
	generator->set(12, 16, Dot());
	generator->set(0, 16, Dot());
	generator->write(0x0005C);
	// Decoy Exits
	generator->setGridSize(7, 7);
	generator->pathWidth = 0.8f;
	generator->setFlag(Generate::Config::DisableWrite);
	generator->generateMaze(0x0008D, 0, 1);
	specialCase->addDecoyExits(generator, 4);
	generator->write(0x0008D);
	generator->generateMaze(0x00081, 2, 1);
	specialCase->addDecoyExits(generator, 12);
	generator->write(0x00081);
	generator->generateMaze(0x00083, 4, 0);
	specialCase->addDecoyExits(generator, 12);
	generator->write(0x00083);
	generator->removeFlag(Generate::Config::DisableWrite);
	// Weird Symmetry
	generator->setSymmetry(Panel::Symmetry::ParallelV);
	generator->generate(
		0x00084, {{Gap(), 12},
				  {Dot(), 10},
				  {kStart, 2},
				  {kExit, 1}});
	generator->setSymmetry(Panel::Symmetry::ParallelHFlip);
	generator->generate(
		0x00082, {{Gap(), 12},
				  {Dot(), 10},
				  {kStart, 2},
				  {kExit, 1}});
	generator->setSymmetry(Panel::Symmetry::FlipNegXY);
	generator->generate(
		0x0343A, {{Gap(), 10},
				  {Dot(), 8},
				  {kStart, 2},
				  {kExit, 1}});
	generator->resetConfig();
	// Symmetry Island Door
	generator->setGridSize(4, 4);
	generator->generate(
		0x000B0, {{Triangle(Cyan), 8}});
	// Triangle Symmetry
	generator->setFlag(Generate::Config::StartEdgeOnly);
	generator->setGridSize(6, 6);
	generator->setSymmetry(Panel::Symmetry::Horizontal);
	generator->generate(
		0x00022, {{Triangle(Cyan), 12},
				  {kStart, 1},
				  {kExit, 1}});
	generator->setSymmetry(Panel::Symmetry::Rotational);
	generator->generate(
		0x00023, {{Triangle(Cyan), 12},
				  {kStart, 1},
				  {kExit, 1}});
	generator->setGridSize(7, 7);
	generator->pathWidth = 0.7f;
	generator->setSymmetry(Panel::Symmetry::ParallelH);
	generator->generate(
		0x00024, {{Triangle(Cyan), 12},
				  {kStart, 1},
				  {kExit, 1}});
	generator->setSymmetry(Panel::Symmetry::ParallelVFlip);
	generator->generate(
		0x00025, {{Triangle(Cyan), 12},
				  {kStart, 1},
				  {kExit, 1}});
	generator->setSymmetry(Panel::Symmetry::RotateLeft);
	generator->setSymbol(kStart, 4, 4);
	generator->setSymbol(kStart, 10, 4);
	generator->setSymbol(kStart, 4, 10);
	generator->setSymbol(kStart, 10, 10);
	generator->setSymbol(kExit, 4, 0);
	generator->setSymbol(kExit, 14, 4);
	generator->setSymbol(kExit, 0, 10);
	generator->setSymbol(kExit, 10, 14);
	generator->generate(
		0x00026, {{Triangle(Cyan, 4), 1},
				  {Triangle(Cyan), 4}});
	// 2-color Mechanics
	generator->setSymmetry(Panel::Symmetry::Rotational);
	generator->setFlag(Generate::Config::WriteInvisible);
	generator->generate(
		0x0007C, {{Dot(Deco::Location::Any, Deco::Parity::Blue), 4},
				  {Dot(Deco::Location::Any, Deco::Parity::Yellow), 4},
				  {Dot(Deco::Location::Any), 7},
				  {kStart, 1},
				  {kExit, 1}});
	generator->generate(
		0x0007E, {{Stone(Cyan), 6},
				  {Stone(Yellow), 6},
				  {kStart, 1},
				  {kExit, 1}});
	generator->generate(
		0x00075, {{Star(Cyan), 6},
				  {Star(Yellow), 6},
				  {kStart, 1},
				  {kExit, 1}});
	generator->setGridSize(5, 5);
	generator->setFlagOnce(Generate::Config::RequireCombineShapes);
	generator->generate(
		0x00073, {{Poly(Yellow), 3},
				  {kStart, 1},
				  {kExit, 1}});
	generator->setGridSize(7, 7);
	generator->setFlag(Generate::Config::ResetColors);
	generator->generate(
		0x00077, {{Triangle(Yellow), 8},
				  {kStart, 1},
				  {kExit, 1}});
	generator->setSymmetry(Panel::Symmetry::FlipXY);
	generator->setSymbol(kExit, 0, 8);
	generator->setSymbol(kExit, 8, 0);
	generator->generate(
		0x00079, {{Dot(Deco::Location::Any, Deco::Parity::Blue), 2},
				  {Dot(Deco::Location::Any, Deco::Parity::Yellow), 2},
				  {Dot(Deco::Location::Any), 8},
				  {Eraser(White), 1},
				  {kStart, 1}});
	// Secret Symmetry
	generator->setFlag(Generate::Config::DisableDotIntersection);
	std::vector<int> ids = {0x00065, 0x0006D, 0x00072, 0x0006F, 0x00070, 0x00071};
	std::vector<Panel::Symmetry> sym1 = {Panel::Symmetry::Vertical, Panel::Symmetry::Horizontal, Panel::Symmetry::Rotational, Panel::Symmetry::ParallelH, Panel::Symmetry::ParallelV};
	std::vector<Panel::Symmetry> sym2 = {Panel::Symmetry::ParallelHFlip, Panel::Symmetry::ParallelVFlip, Random::rand() % 2 == 0 ? Panel::Symmetry::ParallelV : Panel::Symmetry::ParallelH};
	Panel::Symmetry lastChoice = Panel::Symmetry::None;
	for (int i = 0; i < ids.size(); i++)
	{
		// For the first 4 panels, pick from the easy list, no repeats.
		// For the remaining 2 panels, pick from the weird list, no repeats.
		Panel::Symmetry choice = (i < 4 ? Utilities::pop_random(sym1) : Utilities::pop_random(sym2));
		if (choice == lastChoice) {
			i--;
			continue;
		}
		lastChoice = choice;
		specialCase->initSSGrid(generator);
		generator->setSymmetry(choice);
		if (i < 5) {
			generator->generate(
				ids[i], {{Dot(Deco::Location::Any, Deco::Parity::Cyan), 2},
						 {Dot(Deco::Location::Any, Deco::Parity::Yellow), 2},
						 {Triangle(Orange), 4}});
		} else {
			generator->generate(
				ids[i], {{Triangle(Orange), 6}});
		}
	}
	specialCase->initSSGrid(generator);
	generator->setSymmetry(Panel::Symmetry::None);
	generator->generate(
		0x00076, {{Triangle(Orange), 6}});
	// Symmetry Island Door 2
	specialCase->generateSymmetryGate(0x1C349);
	// Dot Reflection Dual Panels (before laser)
	generator->resetConfig();
	generator->setFlag(Generate::Config::WriteDotColor2);
	std::set<Panel::Symmetry> normalSym = {Panel::Symmetry::Vertical, Panel::Symmetry::Rotational};
	std::set<Panel::Symmetry> weirdSym = {Panel::Symmetry::RotateLeft, Panel::Symmetry::RotateRight, Panel::Symmetry::FlipXY, Panel::Symmetry::FlipNegXY};
	generator->setSymbol(kStart, 0, 8);
	generator->setSymbol(kExit, 8, 0);
	specialCase->generateReflectionDotPuzzle(generator, 0x00A52, 0x00A61, {{{Dot(Deco::Location::Any), 15}}}, Utilities::pop_random(normalSym), true);
	generator->setSymbol(kStart, 0, 8);
	generator->setSymbol(kExit, 8, 0);
	specialCase->generateReflectionDotPuzzle(generator, 0x00A57, 0x00A64, {{{Dot(Deco::Location::Any), 15}}}, Utilities::pop_random(weirdSym), true);
	if (!weirdSym.count(Panel::Symmetry::RotateLeft)) {
		weirdSym.erase(Panel::Symmetry::RotateRight);
	} else if (!weirdSym.count(Panel::Symmetry::RotateRight)) {
		weirdSym.erase(Panel::Symmetry::RotateLeft);
	} else if (!weirdSym.count(Panel::Symmetry::FlipXY)) {
		weirdSym.erase(Panel::Symmetry::FlipNegXY);
	} else if (!weirdSym.count(Panel::Symmetry::FlipNegXY)) {
		weirdSym.erase(Panel::Symmetry::FlipXY);
	}
	generator->setSymbol(kStart, 0, 8);
	generator->setSymbol(kStart, 8, 8);
	generator->setSymbol(kStart, 8, 0);
	generator->setSymbol(kStart, 0, 0);
	generator->setSymbol(kExit, 0, 4);
	generator->setSymbol(kExit, 8, 4);
	generator->setFlag(Generate::Config::DisableDotIntersection);
	specialCase->generateReflectionDotPuzzle(generator, 0x00A5B, 0x00A68, {{Dot(Deco::Location::Any), 15}}, Utilities::pick_random(weirdSym), true);
}

void PuzzleList::GenerateQuarryH()
{
	generator->setLoadingData(L"Quarry", 40);
	generator->resetConfig();
	// Entry Gates
	generator->setGridSize(4, 4);
	generator->setFlag(Generate::Config::EnableFlash);
	generator->generate(
		0x09E57, {{Stone(Black), 2},
				  {Stone(White), 2},
				  {Triangle(Orange), 4}});
	generator->setFlagOnce(Generate::Config::RequireCombineShapes);
	generator->generate(
		0x17C09, {{Poly(), 2},
				  {Triangle(Orange), 2}});
	// Mill Entry Door
	generator->resetConfig();
	generator->setFlag(Generate::Config::EnableFlash);
	generator->generate(
		0x01E5A, {{Stone(White), 3},
				  {Stone(Black), 3},
				  {Star(White), 3},
				  {Star(Black), 2},
				  {Star(Orange), 4}});
	generator->setFlagOnce(Generate::Config::PreserveStructure);
	generator->blockPos = {{1, 7}, {1, 9}, {3, 7}, {3, 9}, {7, 1}, {9, 1}, {7, 3}, {9, 3}};
	generator->generate(
		0x01E59, {{Triangle(Black), 7}});
	// Triangles
	generator->pathWidth = 0.8f;
	generator->setFlag(Generate::Config::ResetColors);
	generator->setGridSize(3, 3);
	generator->generate(
		0x00E0C, {{Triangle(Any), 4},
				  {Eraser(White), 1}});
	generator->generate(
		0x01489, {{Triangle(Any), 8},
				  {Eraser(White), 1}});
	generator->setGridSize(4, 3);
	generator->generate(
		0x0148A, {{Triangle(Any), 8},
				  {Eraser(White), 1}});
	generator->generate(
		0x014D9, {{Triangle(Any), 8},
				  {Eraser(White), 1}});
	generator->setGridSize(4, 4);
	generator->generate(
		0x014E7, {{Triangle(Any), 12},
				  {Eraser(White), 1}});
	generator->generate(
		0x014E8, {{Triangle(Any), 15},
				  {Eraser(White), 1}});
	generator->removeFlag(Generate::Config::ResetColors);
	generator->pathWidth = 1;
	// Stones + Stars
	generator->setGridSize(3, 3);
	generator->setFlag(Generate::Config::AlternateColors);
	generator->generate(
		0x00557, {{Stone(White), 2},
				  {Stone(Black), 2},
				  {Star(White), 2},
				  {Star(Black), 2},
				  {Eraser(Green), 1}});
	generator->generate(
		0x005F1, {{Stone(White), 2},
				  {Stone(Black), 1},
				  {Star(White), 2},
				  {Star(Black), 3},
				  {Eraser(Green), 1}});
	generator->setGridSize(4, 4);
	generator->generate(
		0x00620, {{Stone(White), 6},
				  {Stone(Black), 5},
				  {Star(White), 2},
				  {Star(Black), 2},
				  {Eraser(Green), 1}});
	generator->generate(
		0x009F5, {{Stone(White), 5},
				  {Stone(Black), 4},
				  {Star(White), 3},
				  {Star(Black), 3},
				  {Eraser(Green), 1}});
	generator->generate(
		0x0146C, {{Stone(White), 4},
				  {Stone(Black), 4},
				  {Star(White), 4},
				  {Star(Black), 3},
				  {Eraser(Green), 1}});
	generator->setGridSize(3, 3);
	generator->generate(
		0x3C12D, {{Stone(White), 2},
				  {Stone(Black), 1},
				  {Star(White), 1},
				  {Star(Black), 1},
				  {Stone(Red), 1},
				  {Star(Red), 2},
				  {Eraser(Green), 1}});
	generator->setGridSize(4, 4);
	generator->generate(
		0x03686, {{Stone(White), 2},
				  {Stone(Black), 3},
				  {Stone(Red), 2},
				  {Star(White), 4},
				  {Star(Black), 2},
				  {Star(Red), 2},
				  {Eraser(Green), 1}});
	generator->generate(
		0x014E9, {{Stone(White), 1},
				  {Stone(Black), 2},
				  {Stone(Red), 2},
				  {Star(White), 4},
				  {Star(Black), 3},
				  {Star(Red), 3},
				  {Eraser(Green), 1}});

	// Stones + Stars + Triangles
	generator->setSymbol(kStart, 0, 8);
	generator->generate(
		0x0367C, {{Stone(White), 2},
				  {Stone(Black), 1},
				  {Star(White), 1},
				  {Star(Black), 2},
				  {Triangle(White), 3},
				  {Triangle(Black), 3},
				  {Eraser(Green), 1}});

	// Optional Puzzle
	generator->resetConfig();
	generator->setFlagOnce(Generate::Config::FalseParity);
	generator->setGridSize(8, 8);
	generator->setSymbol(kExit, 0, 0);
	generator->setSymbol(kExit, 16, 0);
	generator->setSymbol(kExit, 0, 16);
	generator->setSymbol(kExit, 16, 16);
	generator->generate(
		0x3C125, {{Stone(White), 10},
				  {Stone(Black), 10},
				  {Dot(), 81},
				  {kStart, 7},
				  {Eraser(Purple), 1}});

	// Boathouse Ramp Activation
	generator->resetConfig();
	generator->setFlag(Generate::Config::EnableFlash);
	generator->generate(
		0x034D4, {{Star(White), 3},
				  {Star(Black), 8},
				  {Star(Magenta), 6},
				  {Eraser(White), 1}});
	generator->setFlagOnce(Generate::Config::DisconnectShapes);
	generator->generate(
		0x021D5, {{Poly(), 4},
				  {Eraser(White), 1}});

	// Eraser + Shapes
	generator->setFlag(Generate::Config::ResetColors);
	generator->generate(
		0x021B3, {{Poly(), 3},
				  {NegaPoly(Any), 1},
				  {Eraser(White), 1}});
	Memory::get()->WritePanelData(0x021B4, POWER_OFF_ON_FAIL, 0);
	generator->generate(
		0x021B4, {{Poly(), 3},
				  {NegaPoly(Any), 1},
				  {Eraser(White), 1}});
	Memory::get()->WritePanelData(0x021B0, POWER_OFF_ON_FAIL, 0);
	generator->generate(
		0x021B0, {{Poly(), 4},
				  {NegaPoly(Any), 1},
				  {Eraser(White), 1}});
	Memory::get()->WritePanelData(0x021AF, POWER_OFF_ON_FAIL, 0);
	generator->setGridSize(4, 4);
	generator->generate(
		0x021AF, {{Poly(), 3},
				  {NegaPoly(Any), 2},
				  {Eraser(White), 1}});
	generator->setFlagOnce(Generate::Config::DisconnectShapes);
	Memory::get()->WritePanelData(0x021AE, POWER_OFF_ON_FAIL, 0);
	generator->generate(
		0x021AE, {{Poly(), 3},
				  {NegaPoly(Any), 1},
				  {Eraser(White), 1}});
	generator->resetConfig();

	// Eraser + Stars
	generator->setFlag(Generate::Config::TreehouseColors);
	generator->initPanel(0x021B5);
	generator->clear();
	generator->set(1, 1, Eraser(Green));
	generator->set(3, 5, Eraser(Magenta));
	generator->set(3, 1, Star(Green));
	generator->set(5, 5, Star(Green));
	generator->setFlagOnce(Generate::Config::DecorationsOnly);
	generator->write(0x021B5);
	generator->initPanel(0x021B6);
	generator->clear();
	generator->set(3, 1, Eraser(Magenta));
	generator->set(1, 3, Star(Magenta));
	generator->set(3, 3, Star(Magenta));
	generator->set(5, 3, Star(Magenta));
	generator->set(3, 5, Eraser(Magenta));
	generator->setFlagOnce(Generate::Config::DecorationsOnly);
	generator->write(0x021B6);
	generator->generate(
		0x021B7, {{Star(Green), 7},
				  {Star(Orange), 5},
				  {Eraser(Green), 1},
				  {Eraser(Orange), 1}});
	generator->setGridSize(5, 5);
	generator->generate(
		0x021BB, {{Star(Magenta), 8},
				  {Star(Orange), 6},
				  {Star(Green), 4},
				  {Eraser(Magenta), 1},
				  {Eraser(Orange), 1}});
	generator->generate(
		0x09DB5, {{Star(Magenta), 7},
				  {Star(Orange), 6},
				  {Star(Green), 5},
				  {Eraser(Magenta), 1},
				  {Eraser(Green), 1}});
	// Eraser + Stars + Shapes
	generator->removeFlag(Generate::Config::TreehouseColors);
	generator->setFlag(Generate::Config::WriteColors);
	generator->setGridSize(4, 4);
	generator->generate(
		0x09DB1, {{Poly(Green), 4},
				  {Eraser(White), 2}});
	generator->setFlagOnce(Generate::Config::DisconnectShapes);
	generator->generate(
		0x3C124, {{Poly(Green), 4},
				  {Eraser(White), 2}});
	generator->setSymbol(kStart, 0, 8);
	generator->generate(
		0x09DB3, {{Poly(Green), 3},
				  {Star(Green), 3},
				  {Eraser(White), 2}});
	generator->generate(
		0x09DB4, {{Poly(Green), 2},
				  {Poly(Magenta), 1},
				  {Star(Green), 2},
				  {Star(Magenta), 2},
				  {Eraser(White), 2}});

	// All together
	generator->removeFlag(Generate::Config::WriteColors);
	generator->setFlag(Generate::Config::ResetColors);
	generator->setGridSize(6, 3);
	generator->pathWidth = 0.6f;
	generator->generate(
		0x0A3CB, {{Star(Orange), 4},
				  {Poly(Orange), 2},
				  {NegaPoly(Magenta), 2},
				  {Eraser(White), 2}});
	generator->generate(
		0x0A3CC, {{Star(Magenta), 4},
				  {Poly(Orange), 2},
				  {NegaPoly(Magenta), 2},
				  {Eraser(White), 2}});
	generator->generate(
		0x0A3D0, {{Star(Magenta), 2},
				  {Star(Orange), 3},
				  {Poly(Orange), 2},
				  {NegaPoly(Magenta), 2},
				  {Eraser(White), 2}});

	// Laser Puzzle
	generator->resetConfig();
	generator->setFlagOnce(Generate::Config::DecorationsOnly);
	generator->initPanel(0x03612);
	generator->clear();
	generator->set(1, 5, Eraser(White));
	generator->set(5, 5, Eraser(Green));
	generator->set(1, 3, Eraser(White));
	generator->set(3, 1, Deco::Triangle(Deco::Color::White, 1));
	generator->set(5, 3, Star(Green));
	generator->write(0x03612);
}

void PuzzleList::GenerateBunkerH() {
	// I would randomize this, if I could get the panels to actually render the symbols.
	// Unfortunately, the path is rendered to a 3D model that doesn't have any geometry between the grid lines.
	// Somehow, I would either have to change the model, or make the puzzle render to the background texture instead.
}

void PuzzleList::GenerateSwampH() {
	generator->setLoadingData(L"Swamp", 55);
	generator->resetConfig();
	generator->setGridSize(3, 3);
	generator->setFlagOnce(Generate::Config::SplitShapes);
	generator->generate(
		0x0056E, {{SpinPoly(Any), 2},
				  {Stone(Black), 1},
				  {Stone(White), 1},
				  {Triangle(Orange), 1}});

	// First row
	generator->setGridSize(4, 4);
	generator->pathWidth = 0.65f;
	generator->generate(
		0x00469, {{Poly(), 2},
				  {Stone(Black), 3},
				  {Stone(White), 3}});
	generator->generate(
		0x00472, {{Poly(), 1},
				  {SpinPoly(Any), 1},
				  {Stone(Black), 3},
				  {Stone(White), 3}});
	generator->generate(
		0x00262, {{SpinPoly(Any), 2},
				  {Stone(Black), 3},
				  {Stone(White), 3}});
	generator->generate(
		0x00474, {{Poly(), 2},
				  {SpinPoly(Any), 1},
				  {Stone(Black), 2},
				  {Stone(White), 2}});
	generator->generate(
		0x00553, {{Poly(), 1},
				  {SpinPoly(Any), 2},
				  {Stone(Black), 2},
				  {Stone(White), 2}});
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.6f;
	generator->generate(
		0x0056F, {{Poly(), 2},
				  {SpinPoly(Any), 1},
				  {Stone(Black), 5},
				  {Stone(White), 3}});

	// Second Row
	generator->resetConfig();
	generator->setGridSize(4, 4);
	generator->pathWidth = 0.65f;
	generator->generate(
		0x00390, {{Poly(), 2},
				  {Triangle(Orange), 3}});
	generator->generate(
		0x010CA, {{Poly(), 1},
				  {SpinPoly(Any), 1},
				  {Triangle(Orange), 3}});
	generator->generate(
		0x00983, {{SpinPoly(Any), 2},
				  {Triangle(Orange), 3}});
	generator->generate(
		0x00984, {{Poly(), 2},
				  {SpinPoly(Any), 1},
				  {Triangle(Orange), 2}});
	generator->generate(
		0x00986, {{Poly(), 1},
				  {SpinPoly(Any), 2},
				  {Triangle(Orange), 2}});
	generator->generate(
		0x00985, {{SpinPoly(Any), 2},
				  {Triangle(Orange), 2},
				  		  {Stone(Black), 2},
				  		  {Stone(White), 2}});
	generator->generate(
		0x00987, {{Poly(), 2},
				  {SpinPoly(Any), 1},
				  {Triangle(Orange), 2},
				  		  {Stone(Black), 2},
				  		  {Stone(White), 2}});
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.6f;
	generator->setFlagOnce(Generate::Config::BigShapes);
	generator->generate(
		0x181A9, {{SpinPoly(Any), 2},
				  {Triangle(Orange), 3},
				  		  {Stone(Black), 3},
				  		  {Stone(White), 3}});

	// Modify bridge
	generator->resetConfig();
	generator->initPanel(0x00609);
	generator->set(5, 1, Stone(White));
	generator->set(11, 1, Stone(White));
	generator->set(3, 5, Stone(Black));
	generator->set(9, 5, Stone(Black));
	generator->setFlag(Generate::Config::DecorationsOnly);
	generator->setFlag(Generate::Config::DisableReset);
	generator->write(0x00609);
	generator->write(0x18488);
	generator->resetConfig();
	// Turn off bridge control until all previous puzzles are solved
	specialCase->setTargetAndDeactivate(0x181A9, 0x00609);

	// Red Panels
	generator->resetConfig();
	generator->setGridSize(4, 4);
	generator->pathWidth = 0.65f;
	generator->setSymbol(kStart, 4, 4);
	generator->generate(
		0x00982, {{SpinPoly(Any), 2},
				  {Gap(), 3}});
	generator->setSymbol(kStart, 4, 4);
	generator->generate(
		0x0097F, {{SpinPoly(Any), 3},
				  {Gap(), 3}});
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.6f;
	generator->setSymbol(kStart, 4, 6);
	generator->generate(
		0x0098F, {{SpinPoly(Any), 3},
				  {Gap(), 3}});
	generator->setSymbol(kStart, 4, 6);
	generator->generate(
		0x00990, {{SpinPoly(Any), 4},
				  {Gap(), 3}});
	generator->setFlagOnce(Generate::Config::DisableReset);
	generator->setFlagOnce(Generate::Config::LongPath);
	generator->setSymbol(kStart, 4, 6);
	generator->generate(
		0x17C0D, {{SpinPoly(Any), 3}});
	generator->place_gaps(6);
	generator->write(0x17C0E);

	// Disconnected Shapes
	generator->resetConfig();
	generator->setFlag(Generate::Config::DisconnectShapes);
	generator->setGridSize(4, 4);
	generator->pathWidth = 0.65f;
	generator->setFlagOnce(Generate::Config::BigShapes);
	generator->generate(
		0x00999, {{SpinPoly(Any), 2}});
	generator->generate(
		0x0099D, {{SpinPoly(Any), 3}});
	generator->setSymbol(kStart, 4, 4);
	generator->generate(
		0x009A0, {{SpinPoly(Any), 3}});
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.6f;
	generator->setSymbol(kStart, 4, 6);
	generator->generate(
		0x009A1, {{SpinPoly(Any), 3}});

	// Modify rotating bridge
	generator->initPanel(0x181F5);
	generator->set(1, 1, Star(Black));
	generator->set(9, 3, Triangle(Black, 3));
	generator->set(9, 7, Triangle(Black, 3));
	generator->set(1, 9, Stone(Black));
	generator->setFlagOnce(Generate::Config::DecorationsOnly);
	generator->write(0x181F5);

	// Full Dot Shapes
	generator->resetConfig();
	generator->setFlag(Generate::Config::WriteDotColor);
	generator->setGridSize(4, 4);
	generator->pathWidth = 0.65f;
	generator->generate(
		0x00007, {{Dot(), 25},
				  {SpinPoly(Any), 2},
				  {kStart, 1}});
	generator->generate(
		0x00008, {{Dot(), 25},
				  {SpinPoly(Any), 2},
				  {kStart, 1}});
	generator->generate(
		0x00009, {{Dot(), 25},
				  {Poly(), 2},
				  {SpinPoly(Any), 1},
				  {kStart, 1}});
	generator->generate(
		0x0000A, {{Dot(), 25},
				  {Poly(), 1},
				  {SpinPoly(Any), 2},
				  {kStart, 1}});
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.6f;
	generator->generate(
		0x003B2, {{Dot(), 36},
				  {Poly(), 3},
				  {kStart, 1}});
	generator->generate(
		0x00A1E, {{Dot(), 36},
				  {Poly(), 2},
				  {SpinPoly(Any), 1},
				  {kStart, 1}});
	generator->setFlagOnce(Generate::Config::BigShapes);
	generator->generate(
		0x00C2E, {{Dot(), 36},
				  {Poly(), 3},
				  {kStart, 1}});
	generator->setFlagOnce(Generate::Config::DisconnectShapes);
	generator->generate(
		0x00E3A, {{Dot(), 36},
				  {Poly(), 3},
				  {kStart, 1}});

	// Optional Tetris
	generator->resetConfig();
	generator->setFlagOnce(Generate::Config::WriteDotColor);
	generator->generate(
		0x009A6, {{Dot(), 42},
				  {Poly(), 2},
				  {SpinPoly(Any), 1},
				  {Triangle(Orange), 3},
				  		  {Stone(Black), 2},
				  		  {Stone(White), 2}});

	// Negative Shapes 1
	generator->resetConfig();
	generator->setGridSize(4, 4);
	generator->pathWidth = 0.65f;
	generator->setFlagOnce(Generate::Config::RequireCancelShapes);
	generator->generate(
		0x009AB, {{Poly(), 3},
				  {NegaPoly(Any), 3}});
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.6f;
	generator->generate(
		0x009AD, {{Poly(), 4},
				  {NegaPoly(Any), 1}});
	generator->setFlagOnce(Generate::Config::RequireCancelShapes);
	generator->generate(
		0x009AE, {{Poly(), 4},
				  {NegaPoly(Any), 3}});
	generator->setFlag(Generate::Config::BigShapes);
	generator->generate(
		0x009AF, {{Poly(), 3},
				  {NegaPoly(Any), 1}});
	generator->setFlagOnce(Generate::Config::RequireCancelShapes);
	generator->generate(
		0x00006, {{Poly(), 3},
				  {NegaPoly(Any), 3}});
	generator->removeFlag(Generate::Config::BigShapes);

	// Negative Shapes 2
	generator->generate(
		0x00002, {{Poly(), 3},
				  {NegaPoly(Any), 1},
				  {Stone(Black), 4},
				  {Stone(White), 3}});
	generator->generate(
		0x00004, {{Poly(), 3},
				  {NegaPoly(Any), 2},
				  {Triangle(Orange), 5}});
	generator->generate(
		0x00005, {{Poly(), 3},
				  {NegaPoly(Any), 1},
				  {Triangle(Orange), 3},
				  {Stone(Black), 2},
				  {Stone(White), 2}});
	generator->generate(
		0x013E6, {{Poly(), 3},
				  {NegaPoly(Any), 2},
				  {Triangle(Orange), 3},
				  {Stone(Black), 2},
				  {Stone(White), 2}});
	generator->setFlagOnce(Generate::Config::DisconnectShapes);
	generator->setFlagOnce(Generate::Config::BigShapes);
	generator->generate(
		0x00596, {{Poly(), 2},
				  {NegaPoly(Any), 1},
				  {Triangle(Orange), 3},
				  {Stone(Black), 2},
				  {Stone(White), 2}});

	// Negative Shapes 3
	generator->setFlag(Generate::Config::WriteDotColor);
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.6f;
	generator->setFlag(Generate::Config::BigShapes);
	generator->generate(
		0x00001, {{Dot(), 36},
				  {Poly(), 2},
				  {NegaPoly(Any), 1},
				  {kStart, 1}});
	generator->generate(
		0x014D2, {{Dot(), 36},
				  {Poly(), 2},
				  {NegaPoly(Any), 2},
				  {kStart, 1}});
	generator->removeFlag(Generate::Config::BigShapes);
	generator->generate(
		0x014D4, {{Dot(), 36},
				  {Poly(), 3},
				  {NegaPoly(Any), 1},
				  {kStart, 1}});
	generator->generate(
		0x014D1, {{Dot(), 36},
				  {Poly(), 3},
				  {NegaPoly(Any), 2},
				  {kStart, 1}});

	// Modify Swamp Island
	generator->resetConfig();
	generator->initPanel(0x17C0A); // Swamp Island Control 1
	generator->set(5, 3, 0);
	generator->set(3, 1, NegaPoly(Any, true) | 0x00130000); // probably a hard-defined shape
	generator->setFlag(Generate::Config::DecorationsOnly);
	generator->setFlag(Generate::Config::DisableReset);
	generator->write(0x17C0A); // Swamp Island Control 1
	generator->write(0x17E07); // Swamp Island Control 2

	// Exit Shortcut
	generator->resetConfig();
	generator->setGridSize(4, 4);
	generator->pathWidth = 0.7f;
	generator->generate(
		0x17C05,
		{{Poly(Orange), 3},
		 {NegaPoly(Blue), 3},
		 {Star(Orange), 3}});
	generator->generate(
		0x17C02,
		{{Poly(Blue), 3},
		 {NegaPoly(Orange), 3},
		 {Star(Orange), 3}});
}

void PuzzleList::GenerateTreehouseH() {
	generator->setLoadingData(L"Treehouse", 58);
	generator->resetConfig();
	generator->setFlag(Generate::Config::TreehouseColors);
	generator->initPanel(0x0288C);
	generator->set(1, 1, Triangle(Orange, 1));
	generator->set(1, 3, Star(Orange));
	generator->write(0x0288C);
	generator->setGridSize(3, 3);
	generator->generate(
		0x02886, {{Star(Orange), 3},
				  {Triangle(Orange), 2}});
	generator->setFlag(Generate::Config::TreehouseLayout);

	// Yellow Bridge
	generator->setGridSize(4, 4);
	generator->generate(
		0x17D72, {{Star(Orange), 1},
				  {Triangle(Orange), 7}});
	generator->generate(
		0x17D8F, {{Star(Orange), 2},
				  {Triangle(Orange), 6}});
	generator->generate(
		0x17D74, {{Star(Orange), 2},
				  {Triangle(Orange), 6}});
	generator->generate(
		0x17DAC, {{Star(Orange), 3},
				  {Triangle(Orange), 5}});
	generator->generate(
		0x17D9E, {{Star(Orange), 3},
				  {Triangle(Orange), 5}});
	generator->generate(
		0x17DB9, {{Star(Orange), 4},
				  {Triangle(Orange), 4}});
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.8f;
	generator->generate(
		0x17D9C, {{Star(Orange), 5},
				  {Triangle(Orange), 6}});
	generator->generate(
		0x17DC2, {{Star(Orange), 4},
				  {Triangle(Orange), 6}});
	generator->generate(
		0x17DC4, {{Star(Orange), 6},
				  {Triangle(Orange), 5}});
	generator->removeFlagOnce(Generate::Config::TreehouseLayout);
	generator->generate(
		0x0A182, {{Star(Orange), 5},
				  {Triangle(Orange), 3},
				  {Stone(Orange), 3}});

	// Pink Bridge 1
	generator->setGridSize(4, 4);
	generator->pathWidth = 1;
	generator->generate(
		0x17DC8, {{Star(Magenta), 4},
				  {Dot(), 25}});
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.8f;
	generator->generate(
		0x17DC7, {{Star(Magenta), 4},
				  {Dot(), 36}});
	generator->generate(
		0x17CE4, {{Star(Magenta), 6},
				  {Dot(), 36}});
	generator->generate(
		0x17D2D, {{Star(Magenta), 4},
				  {Star(Orange), 4},
				  {Dot(), 36}});
	generator->generate(
		0x17D6C, {{Star(Magenta), 4},
				  {Star(Orange), 6},
				  {Dot(), 36}});

	// Pink Bridge 2
	generator->setGridSize(4, 4);
	generator->pathWidth = 1;
	generator->removeFlag(Generate::Config::TreehouseColors);
	generator->setFlag(Generate::Config::WriteColors);
	generator->generate(
		0x17D9B, {{Triangle(Magenta), 4},
				  {Stone(Black), 2},
				  {Stone(White), 2},
				  {Star(Black), 1},
				  {Star(White), 1}});
	generator->generate(
		0x17D99, {{Triangle(Magenta), 4},
				  {Stone(Black), 1},
				  {Stone(White), 1},
				  {Star(Black), 2},
				  {Star(White), 2}});
	generator->generate(
		0x17DAA, {{Triangle(Magenta), 4},
				  {Stone(Black), 2},
				  {Stone(White), 2},
				  {Star(Magenta), 2}});
	generator->generate(
		0x17D97, {{Triangle(Magenta), 4},
				  {Stone(Black), 1},
				  {Star(Black), 1},
				  {Stone(Magenta), 2},
				  {Star(White), 2}});
	generator->removeFlag(Generate::Config::WriteColors);
	generator->setFlag(Generate::Config::TreehouseColors);
	generator->generate(
		0x17BDF, {{Triangle(Orange), 2},
				  {Triangle(Magenta), 2},
				  {Star(Magenta), 2},
				  {Star(Green), 2},
				  {Stone(Orange), 2},
				  {Stone(Green), 2}});
	generator->setGridSize(5, 4);
	generator->pathWidth = 0.8f;
	generator->generate(
		0x17D91, {{Triangle(Orange), 2},
				  {Triangle(Magenta), 2},
				  {Star(Magenta), 3},
				  {Star(Green), 4},
				  {Stone(Orange), 2},
				  {Stone(Green), 2}});
	generator->generate(
		0x17DC6, {{Triangle(Green), 3},
				  {Triangle(Magenta), 2},
				  {Star(Magenta), 3},
				  {Star(Orange), 3},
				  {Stone(Orange), 2},
				  {Stone(Green), 2}});

	// Orange Bridge 1
	generator->removeFlag(Generate::Config::TreehouseColors);
	generator->setFlag(Generate::Config::WriteColors);
	generator->setGridSize(4, 4);
	generator->pathWidth = 1;
	generator->generate(
		0x17DB3, {{Star(Black), 1},
				  {Star(White), 2},
				  {Stone(Black), 2},
				  {Stone(White), 1},
				  {SpinPoly(Orange), 1},
				  {Poly(Orange), 1}});
	generator->generate(
		0x17DB5, {{Star(Black), 1},
				  {Star(White), 2},
				  {Stone(Black), 2},
				  {Stone(White), 1},
				  {SpinPoly(Black), 1},
				  {Poly(Black), 1}});
	generator->generate(
		0x17DB6, {{Star(Black), 1},
				  {Star(White), 2},
				  {Stone(Black), 2},
				  {Stone(White), 1},
				  {SpinPoly(White), 1},
				  {Poly(White), 1}});
	generator->generate(
		0x17DC0, {{Star(Black), 1},
				  {Star(White), 2},
				  {Stone(Black), 2},
				  {Stone(White), 1},
				  {SpinPoly(Black), 1},
				  {Poly(White), 1}});
	generator->removeFlag(Generate::Config::WriteColors);
	generator->setFlag(Generate::Config::AlternateColors); // Black -> Green, White -> Pink, Purple -> White, Green -> Black
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.8f;
	generator->generate(
		0x17DD7, {{Star(Black), 2},
				  {Star(White), 4},
				  {Stone(Black), 2},
				  {Stone(White), 2},
				  {SpinPoly(Black), 1},
				  {Poly(Black), 1}});
	generator->generate(
		0x17DD9, {{Star(Black), 3},
				  {Star(White), 3},
				  {Stone(Black), 2},
				  {Stone(White), 2},
				  {Poly(Black), 1},
				  {SpinPoly(White), 1}});
	generator->setFlag(Generate::Config::BigShapes);
	generator->generate(
		0x17DB8, {{Star(Black), 4},
				  {Star(White), 3},
				  {Stone(Black), 2},
				  {Stone(White), 2},
				  {SpinPoly(White), 1},
				  {Poly(White), 1}});
	generator->generate(
		0x17DDC, {{Star(Black), 3},
				  {Star(White), 4},
				  {Stone(Black), 2},
				  {Stone(White), 2},
				  {SpinPoly(Black), 1},
				  {Poly(White), 1}});
	generator->removeFlag(Generate::Config::BigShapes);
	generator->setObstructions({{1, 2}, {1, 4}, {9, 2}, {9, 4}, {2, 1}, {4, 1}, {6, 1}, {8, 1}});
	generator->generate(
		0x17DD1, {{Star(Black), 3},
				  {Star(White), 3},
				  {Stone(White), 2},
				  {Stone(Purple), 2},
				  {SpinPoly(Purple), 1},
				  {Poly(Black), 1}});
	generator->generate(
		0x17DDE, {{Star(Purple), 2},
				  {Star(Black), 5},
				  {Stone(Black), 2},
				  {Stone(Purple), 1},
				  {Poly(Purple), 1},
				  {SpinPoly(White), 1}});
	generator->setFlag(Generate::Config::BigShapes);
	generator->generate(
		0x17DE3, {{Star(Purple), 4},
				  {Star(White), 4},
				  {Stone(Black), 2},
				  {Stone(Purple), 1},
				  {SpinPoly(Purple), 1},
				  {Poly(Black), 1}});
	generator->removeFlag(Generate::Config::BigShapes);
	generator->removeFlag(Generate::Config::AlternateColors);
	generator->generate(
		0x17DEC, {{Star(Black), 2},
				  {Star(White), 3},
				  {Stone(Black), 2},
				  {Stone(White), 1},
				  {Triangle(Black), 2},
				  {Triangle(White), 1},
				  {SpinPoly(Any), 1},
				  {Poly(White), 1}});
	specialCase->clearTarget(0x17DEC);
	generator->generate(
		0x17DAE, {{Star(Black), 3},
				  {Star(White), 3},
				  {Stone(Black), 2},
				  {Stone(White), 1},
				  {Triangle(Black), 1},
				  {Triangle(White), 2},
				  {SpinPoly(Black), 1},
				  {Poly(White), 1}});
	generator->generate(
		0x17DB0, {{Star(Black), 4},
				  {Star(White), 2},
				  {Stone(Black), 1},
				  {Stone(White), 2},
				  {Triangle(Black), 1},
				  {Triangle(White), 1},
				  {Poly(Black), 1},
				  {SpinPoly(White), 1}});
	generator->generate(
		0x17DDB, {{Star(Black), 3},
				  {Star(White), 3},
				  {Stone(Black), 1},
				  {Stone(White), 1},
				  {SpinPoly(Black), 1},
				  {SpinPoly(White), 1},
				  {Triangle(Black), 1},
				  {Triangle(White), 1}});
	(new TreehouseWatchdog(0x17DAE))->start();
	// Orange Bridge 2
	generator->setFlag(Generate::Config::TreehouseColors);
	generator->pathWidth = 1;
	generator->setGridSize(3, 3);
	generator->generate(
		0x17D88, {{Star(Orange), 2},
				  {Star(Magenta), 1},
				  {Triangle(Orange), 1},
				  {Triangle(Magenta), 1}});
	generator->setGridSize(4, 4);
	generator->generate(
		0x17DB4, {{Star(Orange), 3},
				  {Star(Magenta), 2},
				  {Triangle(Orange), 2},
				  {Triangle(Magenta), 2}});
	generator->generate(
		0x17D8C, {{Star(Orange), 1},
				  {Star(Magenta), 2},
				  {Triangle(Orange), 3},
				  {Triangle(Magenta), 2}});
	specialCase->generatePivotPanel(
		0x17CE3, // Treehouse Right Orange 4
		{4, 4},
		{{Triangle(Orange), 3},
		 {Triangle(Magenta), 2}},
		generator->colorblind);
	generator->setGridSize(5, 4);
	generator->pathWidth = 0.8f;
	generator->generate(
		0x17DCD, {{Star(Orange), 3},
				  {Star(Magenta), 3},
				  {Triangle(Orange), 3},
				  {Triangle(Magenta), 3}});
	generator->generate(
		0x17DB2, {{Star(Orange), 4},
				  {Star(Magenta), 4},
				  {Triangle(Orange), 2},
				  {Triangle(Magenta), 2}});
	generator->generate(
		0x17DCC, {{Star(Orange), 3},
				  {Star(Magenta), 3},
				  {Star(Green), 6},
				  {Triangle(Orange), 2},
				  {Triangle(Magenta), 2}});
	generator->generate(
		0x17DCA, {{Star(Orange), 4},
				  {Star(Magenta), 4},
				  {Star(Green), 6},
				  {Triangle(Orange), 1},
				  {Triangle(Magenta), 1}});
	generator->generate(
		0x17D8E, {{Star(Orange), 3},
				  {Star(Magenta), 3},
				  {Star(Green), 2},
				  {Triangle(Orange), 2},
				  {Triangle(Magenta), 2},
				  {Triangle(Green), 2}});
	specialCase->generatePivotPanel(
		0x17DB7, // Treehouse Right Orange 10
		{4, 4},
		{{Triangle(Orange), 2},
		 {Triangle(Magenta), 2},
		 {Triangle(Green), 1}},
		generator->colorblind);
	generator->generate(
		0x17DB1, {{Star(Orange), 3},
				  {Star(Magenta), 3},
				  {Star(White), 3},
				  {Star(Green), 3},
				  {Triangle(Orange), 1},
				  {Triangle(Magenta), 1},
				  {Triangle(White), 1},
				  {Triangle(Green), 1}});
	generator->generate(
		0x17DA2, {{Star(Orange), 2},
				  {Star(Magenta), 2},
				  {Star(White), 2},
				  {Star(Green), 2},
				  {Star(Black), 2},
				  {Triangle(Orange), 1},
				  {Triangle(Magenta), 1},
				  {Triangle(White), 1},
				  {Triangle(Green), 1},
				  {Triangle(Black), 1}});

	// Green Bridge
	generator->removeFlag(Generate::Config::TreehouseColors);
	generator->setFlag(Generate::Config::AlternateColors);
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.7f;
	generator->generate(
		0x17E3C, {{Poly(White), 3},
				  {NegaPoly(Green), 1},
				  {Star(White), 5},
				  {Star(Green), 3}});
	generator->generate(
		0x17E4D, {{Poly(Green), 3},
				  {NegaPoly(White), 2},
				  {Star(White), 5},
				  {Star(Green), 3}});
	generator->generate(
		0x17E4F, {{Poly(White), 2},
				  {NegaPoly(Green), 1},
				  {Poly(Green), 1},
				  {NegaPoly(White), 1},
				  {Star(White), 5},
				  {Star(Green), 4}});
	generator->setObstructions(
		{{{1, 2}, {1, 4}, {9, 2}, {9, 4}, {2, 1}, {4, 1}, {6, 1}, {8, 1}},
		 {{1, 2}, {1, 4}, {1, 6}, {0, 7}, {9, 2}, {9, 4}, {9, 6}, {10, 7}, {4, 1}, {6, 1}, {8, 1}},
		 {{1, 2}, {1, 4}, {1, 6}, {0, 7}, {9, 2}, {9, 4}, {9, 6}, {10, 7}, {2, 1}, {4, 1}, {6, 1}}});
	generator->generate(
		0x17E52, {{Poly(Green), 1},
				  {Poly(Black), 1},
				  {NegaPoly(White), 3},
				  {Star(White), 3},
				  {Star(Green), 2},
				  {Star(Black), 3}});
	generator->generate(
		0x17E5B, {{Poly(White), 1},
				  {Poly(Black), 2},
				  {NegaPoly(White), 2},
				  {NegaPoly(Green), 2},
				  {Star(White), 3},
				  {Star(Green), 3},
				  {Star(Black), 3}});
	generator->generate(
		0x17E5F, {{Poly(Black), 2},
				  {NegaPoly(Green), 2},
				  {Star(Black), 3},
				  {Star(Green), 3},
				  {Triangle(Black), 2},
				  {Triangle(Green), 2}});
	generator->generate(
		0x17E61, {{Poly(White), 1},
				  {Poly(Green), 1},
				  {Poly(Cyan), 1},
				  {NegaPoly(Green), 1},
				  {NegaPoly(Black), 2},
				  {Star(Cyan), 2},
				  {Star(Black), 1},
				  {Star(White), 1},
				  {Star(Green), 2},
				  {Triangle(Black), 2},
				  {Triangle(White), 1},
				  {Triangle(Cyan), 2}});
}

void PuzzleList::GenerateTownH() {
	generator->setLoadingData(L"Town", 21);
	generator->resetConfig();

	// Full Dots + Triangles
	generator->setFlag(Generate::Config::EnableFlash);
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.9f;
	generator->generate(
		0x2899C, {{Dot(), 36},
				  {Triangle(Orange, 1), 4},
				  {kStart, 1}});
	generator->generate(
		0x28A33, {{Dot(), 36},
				  {Triangle(Orange, 3), 6},
				  {kStart, 1}});
	generator->generate(
		0x28ABF, {{Dot(), 36},
				  {Triangle(Orange, 2), 10},
				  {kStart, 1}});
	generator->generate(
		0x28AC0, {{Dot(), 36},
				  {Triangle(Orange), 6},
				  {kStart, 1}});
	generator->generate(
		0x28AC1, {{Dot(), 36},
				  {Triangle(Orange), 8},
				  {kStart, 1}});
	generator->setSymbol(kStart, 0, 10);
	generator->setFlagOnce(Generate::FalseParity);
	generator->generate(
		0x28AD9, {{Dot(), 36},
				  {Triangle(Orange), 7},
				  {Eraser(White), 1}});

	// Blue Symmetry
	generator->setFlag(Generate::Config::RequireCombineShapes);
	generator->setGridSize(7, 7);
	generator->pathWidth = 0.6f;
	generator->setSymmetry(Panel::Symmetry::Vertical);
	generator->setSymbol(kStart, 0, 14);
	generator->setSymbol(kStart, 14, 14);
	generator->setSymbol(kExit, 0, 0);
	generator->setSymbol(kExit, 14, 0);
	generator->generate(
		0x28AC7, {{Poly(Orange), 5}});
	generator->setSymmetry(Panel::Symmetry::ParallelV);
	generator->setSymbol(kStart, 0, 14);
	generator->setSymbol(kStart, 8, 14);
	generator->setSymbol(kExit, 6, 0);
	generator->setSymbol(kExit, 14, 0);
	generator->generate(
		0x28AC8, {{Poly(Orange), 5}});
	generator->setSymmetry(Panel::Symmetry::ParallelHFlip);
	generator->setSymbol(kStart, 0, 14);
	generator->setSymbol(kStart, 14, 6);
	generator->setSymbol(kExit, 0, 0);
	generator->setSymbol(kExit, 14, 8);
	generator->generate(
		0x28ACA, {{Poly(Orange), 5}});
	generator->setSymmetry(Panel::Symmetry::ParallelVFlip);
	generator->setSymbol(kStart, 0, 14);
	generator->setSymbol(kStart, 8, 0);
	generator->setSymbol(kExit, 6, 0);
	generator->setSymbol(kExit, 14, 14);
	generator->generate(
		0x28ACB, {{Poly(Orange), 5}});
	generator->setSymmetry(Panel::Symmetry::Rotational);
	generator->setSymbol(kStart, 0, 14);
	generator->setSymbol(kStart, 14, 0);
	generator->setSymbol(kExit, 0, 0);
	generator->setSymbol(kExit, 14, 14);
	generator->generate(
		0x28ACC, {{Poly(Orange), 5}});
	generator->resetConfig();

	// Glass Door
	generator->setFlagOnce(Generate::Config::SmallShapes);
	generator->setFlagOnce(Generate::Config::EnableFlash);
	generator->generate(
		0x28998, {{SpinPoly(Black), 3},
				  {SpinPoly(White), 2},
				  {Star(Black), 4},
				  {Star(White), 5}});

	// Church Star Door
	specialCase->generateColorFilterPuzzle(
		0x28A0D,
		{5, 5},
		// These are placeholder non-Any colors that will be overwritten.
		// They merely need to be distinct, but setting to X is probably trouble too.
		{{Star(Deco::Color{1}), 6},
		 {Star(Deco::Color{2}), 6},
		 {Star(Deco::Color{3}), 6},
		 {Star(Deco::Color{4}), 6}},
		{1, 1, 0, 0}, // Yellow filter
		false // Hard-coded Colorblind off?
	);

	// Mess with targets
	specialCase->copyTarget(0x03C08, 0x28A0D);
	specialCase->copyTarget(0x28A0D, 0x28998);
	specialCase->setTargetAndDeactivate(0x28998, 0x28A0D);
	specialCase->setTargetAndDeactivate(0x03C0C, 0x03C08);
	specialCase->setPower(0x28A69, false);
	(new TownDoorWatchdog())->start();

	// Soundproof Room
	std::vector<int> allPitches = {DOT_SMALL, DOT_SMALL, DOT_MEDIUM, DOT_MEDIUM, DOT_LARGE, DOT_LARGE};
	std::vector<int> pitches;
	for (int i = 0; i < 4; i++)
		pitches.push_back(Utilities::pop_random(allPitches));
	specialCase->generateSoundDotPuzzle(0x034E3, {4, 4}, pitches, false);
	generator->resetConfig();

	// 3-color Room
	// Modify switch to remove green
	generator->initPanel(0x334D8);
	generator->set(7, 5, Triangle(Orange, 3));
	generator->setFlagOnce(Generate::Config::DecorationsOnly);
	generator->write(0x334D8);
	specialCase->generateRGBStonePuzzleH(0x03C0C);
	specialCase->generateRGBDotPuzzleH(0x03C08);

	// Orange Crate
	generator->setGridSize(5, 5);
	generator->setFlagOnce(Generate::Config::EnableFlash);
	generator->generate(
		0x0A0C8, {{Poly(Yellow), 7},
				  {Stone(White), 3},
				  {Stone(Black), 3},
				  {Triangle(Orange), 6}});

	// Windmill Puzzles
	generator->resetConfig();
	generator->setFlagOnce(Generate::Config::PreserveStructure);
	generator->setFlag(Generate::Config::EnableFlash);
	generator->generate(
		0x17F89, {{Stone(Black), 3},
				  {Stone(White), 2},
				  {Triangle(Orange), 5},
				  {Eraser(White), 1}});
	generator->setFlag(Generate::Config::FixBackground);
	generator->generate(
		0x0A168, {{Stone(White), 3},
				  {Stone(Black), 3},
				  {Star(White), 3},
				  {Star(Black), 3},
				  {Eraser(White), 2}});
	generator->setFlagOnce(Generate::Config::WriteColors);
	generator->generate(
		0x33AB2, {{Poly(Yellow), 3},
				  {Triangle(Orange), 5},
				  {Eraser(White), 1}});
}

void PuzzleList::GenerateVaultsH() {
	generator->setLoadingData(L"Vaults", 5);
	generator->resetConfig();

	// Tutorial Vault
	generator->setGridSize(8, 8);
	generator->setSymbol(kExit, 0, 0);
	generator->setSymbol(kExit, 16, 0);
	generator->setSymbol(kExit, 0, 16);
	generator->setSymbol(kExit, 16, 16);
	generator->generate(
		0x033D4, {{Stone(White), 10},
				  {Stone(Black), 10},
				  {Dot(), 81},
				  {kStart, 8}});

	// Desert Vault
	generator->resetConfig();
	generator->generate(
		0x0CC7B, {{Dot(), 49},
				  {Poly(Orange), 1},
				  {Poly(Blue), 1},
				  {NegaPoly(Blue), 2},
				  {NegaPoly(Orange), 1},
				  {Star(Orange), 4},
				  {Star(Blue), 3},
				  {Triangle(Orange), 2},
				  {Triangle(Blue), 1},
				  {Stone(Orange), 1},
				  {Stone(Blue), 2},
				  {Eraser(White), 1}});

	// Symmetry Vault
	generator->resetConfig();
	generator->setGridSize(8, 8);
	generator->setSymmetry(Panel::Symmetry::FlipXY);
	generator->setSymbol(kStart, 0, 16);
	generator->setSymbol(kStart, 16, 0);
	generator->setSymbol(kExit, 8, 0);
	generator->setSymbol(kExit, 8, 16);
	generator->setSymbol(kExit, 0, 8);
	generator->setSymbol(kExit, 16, 8);
	generator->generate(
		0x002A6, {{Triangle(Cyan), 3},
				  {Triangle(Yellow), 2},
				  {Star(Cyan), 3},
				  {Star(Yellow), 3},
				  {Stone(Cyan), 2},
				  {Stone(Yellow), 3}});

	// Shipwreck Vault
	generator->resetConfig();
	specialCase->generateSoundDotReflectionPuzzle(
		0x00AFB,
		{6, 6},
		{DOT_MEDIUM, DOT_LARGE, DOT_MEDIUM, DOT_SMALL},
		{DOT_MEDIUM, DOT_LARGE, DOT_MEDIUM, DOT_SMALL},
		2, true);

	// Jungle Vault
	generator->resetConfig();
	specialCase->generateJungleVault(0x15ADD);
}

void PuzzleList::GenerateTrianglePanelsH() {
	generator->setLoadingData(L"Arrows", 14);
	generator->resetConfig();
	generator->backgroundColor = {0.5f, 0.5f, 0.5f, 1};
	generator->arrowColor = {0.6f, 0, 1, 1};
	specialCase->createArrowPuzzle(0x17CFB, 5, 3, 0, 1, {{0, 3}, {6, 1}});
	specialCase->createArrowPuzzle(0x3C12B, 1, 1, 0, 2, {{1, 4}, {6, 3}, {5, 0}});
	specialCase->createArrowPuzzle(0x17CE7, 1, 3, 5, 1, {{0, 5}, {6, 3}});
	specialCase->createArrowPuzzle(0x17CF0, 5, 5, 6, 1, {{1, 4}, {1, 6}, {3, 0}, {3, 2}});
	specialCase->createArrowPuzzle(0x17C71, 5, 1, 7, 1, {{0, 1}, {3, 6}});
	generator->setSymbol(kStart, 0, 0);
	specialCase->createArrowPuzzle(0x17CF7, 5, 5, 3, 1, {{4, 1}, {1, 2}, {3, 6}, {6, 1}});
	specialCase->createArrowPuzzle(0x17D01, 1, 1, 4, 2, {{4, 5}, {6, 1}});
	generator->setGridSize(2, 1);
	specialCase->createArrowPuzzle(0x17F9B, 1, 1, 2, 2, {});
	generator->setGridSize(3, 3);
	specialCase->createArrowPuzzle(0x17D27, 1, 5, 5, 2, {{3, 0}, {0, 5}});
	specialCase->createArrowPuzzle(0x17D28, 5, 1, 0, 2, {{2, 1}, {4, 1}, {3, 4}});
	specialCase->createArrowPuzzle(0x17FA9, 1, 3, 2, 2, {{3, 4}, {5, 6}});
	generator->setSymbol(kStart, 6, 6);
	specialCase->createArrowPuzzle(0x17FA0, 1, 3, 2, 1, {{4, 1}, {6, 1}, {3, 2}, {4, 5}, {5, 4}});
	specialCase->createArrowPuzzle(0x17C42, 1, 5, 5, 2, {{4, 5}, {2, 3}, {3, 2}, {5, 2}, {5, 4}});
	generator->setGridSize(2, 1);
	specialCase->createArrowPuzzle(0x17F93, 1, 1, 2, 1, {});
	generator->resetConfig();
}

void PuzzleList::GenerateMountainH()
{
	std::wstring text = L"Mountain Perspective";
	SetWindowText(_handle, text.c_str());
	specialCase->generateMountaintop(
		0x17C34, {{Triangle(White), 2},
				  {Triangle(Black), 1},
				  {Star(White), 1},
				  {Star(Black), 1},
				  {Stone(White), 1},
				  {Stone(Black), 1}});

	generator->setLoadingData(L"Mountain", 39);
	generator->resetConfig();
	// Purple Bridge
	generator->setFlagOnce(Generate::Config::PreserveStructure);
	generator->setFlagOnce(Generate::Config::DecorationsOnly);
	std::set<Point> bpoints1 = {{6, 3}, {5, 4}, {7, 4}};
	std::set<Point> bpoints2 = {{6, 5}, {5, 6}, {6, 7}, {7, 6}};
	std::set<Point> bpoints3 = {{3, 6}, {4, 7}};
	if (Random::rand() % 2 == 0)
		generator->hitPoints = {
			Utilities::pick_random(bpoints1),
			Utilities::pick_random(bpoints2),
			Utilities::pick_random(bpoints3)};
	else
		generator->hitPoints = {
			Utilities::pick_random(bpoints3),
			Utilities::pick_random(bpoints2),
			Utilities::pick_random(bpoints1)};
	generator->setObstructions({{4, 1}, {6, 1}, {8, 1}});
	generator->blockPos = {{3, 1}, {5, 1}, {7, 1}, {9, 1}, {1, 1}, {11, 1}, {1, 11}, {11, 11}};
	generator->setSymbol(Gap(Deco::Location::Row), 3, 4);
	generator->generate(
		0x09E39, {{Triangle(Purple), 8},
				  {Eraser(Purple), 1}});
	// Orange Row
	generator->resetConfig();
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.9f;
	generator->setFlag(Generate::Config::ResetColors);
	generator->generate(
		0x09E73, {{kStart, 1},
				  {Dot(Deco::Location::Any), 6},
				  {Triangle(Yellow), 5},
				  {Stone(Black), 4},
				  {Stone(White), 4}});
	generator->generate(
		0x09E75, {{kStart, 1},
				  {Dot(Deco::Location::Any), 6},
				  {Poly(Blue), 1},
				  {SpinPoly(Blue), 1},
				  {Stone(Black), 2},
				  {Stone(White), 2}});
	generator->generate(
		0x09E78, {{kStart, 1},
				  {kExit, 1},
				  {Poly(Blue), 1},
				  {SpinPoly(Blue), 1},
				  {Triangle(Yellow), 3},
				  {Star(Yellow), 3}});
	generator->generate(
		0x09E79, {{kStart, 1},
				  {Triangle(Purple), 2},
				  {Triangle(Yellow), 2},
				  {Stone(Purple), 1},
				  {Stone(Yellow), 2},
				  {Star(Purple), 2},
				  {Star(Yellow), 1},
				  {Eraser(Purple), 1}});
	generator->generate(
		0x09E6C, {{kStart, 1},
				  {kExit, 1},
				  {Poly(Blue), 3},
				  {NegaPoly(Blue), 3},
				  {Star(Blue), 3},
				  {Eraser(Blue), 1}});
	generator->setGridSize(6, 6);
	generator->pathWidth = 0.8f;
	generator->setSymmetry(Panel::Symmetry::Rotational);
	generator->generate(
		0x09E6F, {{kStart, 1},
				  {kExit, 1},
				  {Star(White), 4},
				  {Star(Black), 4},
				  {Star(Purple), 2},
				  {Stone(Purple), 1},
				  {Stone(Black), 1},
				  {Stone(White), 1}});
	generator->setGridSize(7, 7);
	generator->setSymmetry(Panel::Symmetry::Horizontal);
	generator->setSymbol(kStart, 0, 0);
	generator->setSymbol(kStart, 0, 14);
	generator->setSymbol(kExit, 14, 0);
	generator->setSymbol(kExit, 14, 14);
	generator->generate(
		0x09E6B, {{Dot(), 64},
				  {Triangle(Blue), 8}});
	// Green Row
	generator->resetConfig();
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.9f;
	generator->generate(
		0x09E7A, {{Dot(Deco::Location::Any), 5},
				  {Stone(Black), 2},
				  {Stone(White), 2},
				  {Star(Black), 3},
				  {Star(White), 4}});
	generator->blockPos = {{1, 1}, {3, 3}, {7, 7}, {9, 9}, {1, 7}, {7, 1}, {3, 9}, {9, 3}};
	generator->generate(
		0x09E71, {{Stone(Black), 3},
				  {Stone(White), 3},
				  {Triangle(Black), 3},
				  {Triangle(White), 3}});
	generator->blockPos = {{1, 1}, {3, 3}, {7, 7}, {9, 9}, {1, 7}, {7, 1}, {3, 9}, {9, 3}};
	generator->setFlagOnce(Generate::Config::RequireCombineShapes);
	generator->generate(
		0x09E72, {{Star(Black), 3},
				  {Star(White), 4},
				  {Triangle(Black), 1},
				  {Triangle(White), 1},
				  {Poly(Black), 1},
				  {Poly(White), 1}});
	generator->blockPos = {{7, 3}, {3, 7}, {1, 9}, {9, 1}, {1, 3}, {3, 1}, {7, 9}, {9, 7}};
	generator->generate(
		0x09E69, {{Stone(Black), 2},
				  {Stone(White), 2},
				  {Star(Black), 2},
				  {Star(White), 3},
				  {SpinPoly(Black), 1}});
	generator->generate(
		0x09E7B, {{Dot(Deco::Location::Any), 4},
				  {Star(Black), 2},
				  {Stone(Black), 2},
				  {Triangle(Black), 2},
				  {Eraser(Black), 1},
				  {Poly(Black), 2}});
	// Purple Panels
	generator->setFlag(Generate::Config::DisconnectShapes);
	generator->generate(
		0x09EAD, {{SpinPoly(Any), 2},
				  {Star(Magenta), 6}});
	generator->generate(
		0x09EAF, {{SpinPoly(Any), 2},
				  {Triangle(Orange), 3}});
	// Blue Row
	generator->resetConfig();
	generator->setSymmetry(Panel::Symmetry::RotateLeft);
	generator->setObstructions(
		{{4, 3}, {5, 4}, {5, 6}, {5, 8}, {5, 10}, {6, 9}, {7, 10}});
	specialCase->initRotateGrid(generator);
	generator->generate(
		0x33AF5, {{Triangle(Orange), 2},
				  {Stone(Black), 4},
				  {Stone(White), 4}});
	if (Random::rand() % 2 == 0)
		generator->setObstructions(
			{{5, 4}, {5, 6}, {5, 8}, {5, 10}, {9, 4}, {9, 6}, {9, 8}, {9, 10},
			 {7, 0}, {7, 2}});
	else
		generator->setObstructions(
			{{3, 4}, {3, 6}, {3, 8}, {3, 10}, {7, 4}, {7, 6}, {7, 8}, {7, 10},
			 {5, 0}, {5, 2}, {9, 0}, {9, 2}});
	specialCase->initRotateGrid(generator);
	generator->generate(
		0x33AF7, {{Triangle(Orange), 2},
				  {Star(Orange), 3}});
	generator->setObstructions(
		{{0, 5}, {3, 0}, {3, 2}, {3, 10}, {4, 1}, {4, 3}, {4, 9}, {5, 0},
		 {5, 2}, {5, 4}, {5, 6}, {5, 8}, {5, 10}, {6, 1}, {6, 3}, {6, 5},
		 {6, 7}, {6, 9}, {7, 0}, {7, 2}, {7, 4}, {7, 6}, {7, 8}, {7, 10},
		 {8, 1}, {8, 3}, {8, 5}, {8, 7}, {8, 9}, {9, 0}, {9, 2}, {9, 4},
		 {9, 6}, {9, 8}, {9, 10}, {10, 1}, {10, 3}, {10, 5}, {10, 7}, {10, 9}});
	generator->blockPos = {{5, 9}, {7, 7}};
	specialCase->initRotateGrid(generator);
	generator->setFlag(Generate::Config::ShortPath);
	generator->generate(
		0x09F6E, {{Poly(Yellow), 3},
				  {Star(Yellow), 3}});

	// Rainbow Row
	generator->resetConfig();
	generator->setGridSize(5, 5);
	generator->setFlag(Generate::Config::WriteColors);
	generator->generate(
		0x09FD3, {{Stone(Cyan), 3},
				  {Stone(Magenta), 3},
				  {Star(Cyan), 3},
				  {Star(Magenta), 4},
				  {Star(Yellow), 6}});
	generator->generate(
		0x09FD4, {{Star(Cyan), 3},
				  {Star(Magenta), 2},
				  {Star(Yellow), 2},
				  {Triangle(Cyan), 2},
				  {Triangle(Magenta), 1},
				  {Triangle(Yellow), 2}});
	generator->generate(
		0x09FD6, {{Poly(Cyan), 1},
				  {Poly(Yellow), 2},
				  {NegaPoly(Cyan), 1},
				  {NegaPoly(Yellow), 1},
				  {NegaPoly(Purple), 1},
				  {Star(Cyan), 4},
				  {Star(Purple), 3},
				  {Star(Yellow), 2}});
	generator->setGridSize(6, 6);
	generator->generate(
		0x09FD7, {{Star(Yellow), 6},
				  {Star(Green), 6},
				  {Star(Cyan), 6},
				  {Star(Magenta), 6}});
	generator->setGridSize(5, 5);
	generator->blockPos = {{1, 1}, {1, 5}, {3, 3}, {5, 3}, {5, 5}, {5, 7},
	                       {7, 1}, {7, 5}, {7, 9}, {9, 1}, {9, 5}, {9, 9}};
	generator->generate(
		0x09FD8, {{Star(Green), 4},
				  {Star(Magenta), 5},
				  {SpinPoly(Green), 1},
				  {SpinPoly(Magenta), 1},
				  {Eraser(Magenta), 1}});
	generator->resetConfig();

	specialCase->generateMultiPuzzle(
		{0x09FCC, 0x09FCE, 0x09FCF, 0x09FD0, 0x09FD1, 0x09FD2},
		{{{Triangle(Orange), 2}},

		 {{Stone(Black), 1},
		  {Stone(White), 1},
		  {Star(Black), 1},
		  {Star(White), 2}},

		 {{Star(Magenta), 2},
		  {Star(Cyan), 1},
		  {Triangle(Cyan), 1}},

		 {{{SpinPoly(Any), 1},
		   {NegaPoly(Blue, true), 2}}}, // rotating negative

		 {{Dot(), 8}},

		 {{SpinPoly(Any), 1}}},
		true);

	specialCase->generate2BridgeH(0x09E86, 0x09ED8);
	(new BridgeWatchdog(0x09E86, 0x09ED8))->start();

	generator->resetConfig();
	specialCase->generateMountainFloorH();

	// Pillar Puzzles
	generator->resetConfig();
	generator->setGridSize(6, 4);
	generator->setFlag(Generate::Config::ResetColors);
	generator->generate(
		0x0383D, {{Star(Black), 2},
				  {Star(White), 3},
				  {Stone(Black), 2},
				  {Stone(White), 2}});
	generator->removeFlagOnce(Generate::Config::ResetColors);
	generator->setFlagOnce(Generate::Config::TreehouseColors);
	generator->generate(
		0x0383A, {{Star(Orange), 4},
				  {Star(Magenta), 4},
				  {Star(Green), 4},
				  {Star(White), 1},
				  {Triangle(White), 1},
				  {Eraser(White), 1}});
	generator->setGridSize(6, 5);
	specialCase->initPillarSymmetry(generator, 0x0383F, Panel::Symmetry::PillarParallel);
	generator->generate(
		0x0383F, {{Triangle(Orange), 8}});
	specialCase->initPillarSymmetry(generator, 0x09E56, Panel::Symmetry::PillarHorizontal);
	generator->generate(
		0x09E56, {{Triangle(Orange), 4},
				  {Dot(), 36}});
	generator->setFlagOnce(Generate::Config::RequireCombineShapes);
	generator->setGridSize(6, 4);
	specialCase->initPillarSymmetry(generator, 0x03859, Panel::Symmetry::PillarRotational);
	generator->generate(
		0x03859, {{Poly(), 3},
				  {Stone(Black), 2},
				  {Stone(White), 2}});
	generator->setFlagOnce(Generate::Config::RequireCancelShapes);
	generator->setFlagOnce(Generate::Config::DisableDotIntersection);
	generator->setGridSize(6, 5);
	specialCase->initPillarSymmetry(generator, 0x09E5A, Panel::Symmetry::PillarVertical);
	generator->generate(
		0x09E5A, {{Poly(Orange), 1},
				  {NegaPoly(Blue), 2},
				  {Poly(Blue), 1},
				  {NegaPoly(Orange), 2},
				  {Star(Orange), 1},
				  {Star(Blue), 1},
				  {Dot(Deco::Location::Any), 4},
				  {kStart, 3}});
	generator->setFlag(Generate::Config::Write2Color);
	generator->setFlag(Generate::Config::WriteInvisible);
	specialCase->initPillarSymmetry(generator, 0x339BB, Panel::Symmetry::PillarHorizontal);
	generator->generate(
		0x339BB, {{Star(Black), 2},
				  {Star(White), 1},
				  {Stone(Black), 1},
				  {Stone(White), 1},
				  {Triangle(Orange), 2},
				  {Dot(Deco::Parity::Cyan), 2},
				  {Dot(Deco::Parity::Yellow), 2}});
	specialCase->initPillarSymmetry(generator, 0x33961, Panel::Symmetry::PillarParallel);
	generator->generate(
		0x33961, {{Triangle(Orange), 2},
				  {Triangle(Magenta), 1},
				  {Star(Orange), 1},
				  {Star(Magenta), 1},
				  {Poly(Orange), 1},
				  {NegaPoly(Magenta), 2},
				  {Eraser(Magenta), 1}});
}

void PuzzleList::GenerateCavesH()
{
	generator->setLoadingData(L"Caves", 51);
	generator->resetConfig();

	specialCase->createArrowSecretDoor(0x17FA2);

	// Arrow Puzzles
	generator->setGridSize(5, 5);
	generator->backgroundColor = {0, 0, 0.5f, 1};
	generator->arrowColor = {0, 1, 0.8f, 1};
	generator->successColor = {0, 1, 0.8f, 1};
	generator->generate(
		0x00FF8, {{Stone(Black), 5},
				  {Stone(White), 4},
				  {Arrow(), 6}});

	generator->backgroundColor = {0, 0, 0.1f, 1};
	generator->arrowColor = {0.6f, 0, 1, 1};
	generator->successColor = {0.6f, 0, 1, 1};
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.85f;
	generator->generate(
		0x01A0D, {{Arrow(), 6}});
	generator->setGridSize(6, 6);
	generator->pathWidth = 0.7f;
	generator->generate(
		0x018A0, {{Arrow(), 8}});
	generator->pathWidth = 1;
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.85f;
	generator->generate(
		0x009A4, {{Arrow(), 6},
				  {Star(Cyan), 6},
				  {Star(Magenta), 6}});
	generator->generate(
		0x00A72, {{Arrow(), 4},
				  {Poly(), 3},
				  {NegaPoly(Any), 1}});

	generator->setGridSize(4, 4);
	generator->pathWidth = 1.0f;
	generator->generate(
		0x00190, {{Arrow(), 8}});
	generator->generate(
		0x00558, {{Arrow(), 8}});
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.85f;
	generator->generate(
		0x00567, {{Arrow(), 12}});
	generator->generate(
		0x006FE, {{Arrow(), 12}});

	generator->setGridSize(4, 4);
	generator->pathWidth = 1.0f;
	generator->generate(
		0x008B8, {{Arrow(), 4},
				  {Triangle(Orange), 4}});
	generator->generate(
		0x00973, {{Arrow(), 5},
				  {Triangle(Orange), 3}});
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.85f;
	generator->generate(
		0x0097B, {{Arrow(), 4},
				  {Triangle(Orange), 8}});
	generator->generate(
		0x0097D, {{Arrow(), 6},
				  {Triangle(Orange), 6}});
	generator->generate(
		0x0097E, {{Arrow(), 8},
				  {Triangle(Orange), 4}});
	generator->setGridSize(4, 4);
	generator->pathWidth = 1.0f;
	generator->generate(
		0x00994, {{Arrow(), 2},
				  {Poly(), 1},
				  {SpinPoly(Any), 1}});
	generator->generate(
		0x334D5, {{Arrow(), 2},
				  {SpinPoly(Any), 2}});
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.85f;
	generator->generate(
		0x00995, {{Arrow(), 6},
				  {Poly(), 1},
				  {SpinPoly(Any), 1}});
	generator->setFlagOnce(Generate::Config::BigShapes);
	generator->generate(
		0x00996, {{Arrow(), 4},
				  {Poly(), 1},
				  {SpinPoly(Any), 1}});
	generator->generate(
		0x00998, {{Arrow(), 4},
				  {Poly(), 3}});

	// First alcove
	generator->resetConfig();
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.85f;
	generator->setFlagOnce(Generate::Config::DisconnectShapes);
	generator->generate(
		0x32962, {{Dot(), 36},
				  {SpinPoly(Any), 3},
				  {kStart, 1}});
	generator->setFlagOnce(Generate::Config::ResetColors);
	generator->setFlagOnce(Generate::Config::TreehouseLayout);
	generator->setFlagOnce(Generate::Config::BigShapes);
	generator->generate(
		0x32966, {{Star(Black), 2},
				  {Star(White), 2},
				  {Stone(Black), 1},
				  {Stone(White), 1},
				  {Triangle(Black), 1},
				  {Triangle(White), 1},
				  {SpinPoly(Black), 1},
				  {SpinPoly(White), 1}});
	generator->setGridSize(6, 6);
	generator->pathWidth = 1.0f;
	generator->setFlagOnce(Generate::Config::WriteColors);
	generator->generate(
		0x01A31, {{Star(Cyan), 8},
				  {Star(Yellow), 6},
				  {Star(Magenta), 6}});
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.85f;
	generator->setFlagOnce(Generate::Config::RequireCancelShapes);
	generator->setFlagOnce(Generate::Config::WriteDotColor);
	generator->generate(
		0x00B71, {{Stone(Black), 1},
				  {Star(Black), 1},
				  {Star(White), 2},
				  {Poly(White), 1},
				  {NegaPoly(White), 2},
				  {Dot(Deco::Location::Any), 36}, // Is this Full Dots, or just many?
				  {Eraser(Black), 1},
				  {kStart, 1}});

	// Perspective
	generator->resetConfig();
	generator->setFlag(Generate::Config::EnableFlash);
	generator->setFlag(Generate::Config::DecorationsOnly);
	specialCase->generateCenterPerspective(
		0x288EA, {{Star(Black), 8},
				  {Star(White), 6}},
				  Deco::Symbol::Star);
	specialCase->generateCenterPerspective(
		0x288FC, {{Poly(), 4},
				  {Eraser(White), 1}},
				  Deco::Symbol::Eraser);
	specialCase->generateCenterPerspective(
		0x289E7, {{Triangle(Orange), 10},
				  {Eraser(White), 1}},
				  Deco::Symbol::Triangle);
	generator->removeFlag(Generate::Config::DecorationsOnly);
	generator->setFlagOnce(Generate::Config::RequireCancelShapes);
	specialCase->generateCenterPerspective(
		0x288AA, {{Dot(), 36},
				  {Poly(), 2},
				  {NegaPoly(Blue), 4}},
				 Deco::Symbol::Poly);

	// Full Dots + Arrows
	generator->resetConfig();
	generator->backgroundColor = {0, 0.7f, 0.2f, 1};
	generator->arrowColor = {0, 0, 0, 1};
	generator->successColor = {0.6f, 1, 0.25f, 1};
	generator->generate(
		0x0A16B, {{Dot(), 25},
				  {Arrow(), 4}});
	generator->generate(
		0x0A2CE, {{Dot(), 25},
				  {Arrow(), 4},
				  {kStart, 1}});
	generator->setGridSize(5, 5);
	generator->pathWidth = 0.85f;
	generator->generate(
		0x0A2D7, {{Dot(), 36},
				  {Arrow(), 4},
				  {kStart, 1}});
	generator->generate(
		0x0A2DD, {{Dot(), 36},
				  {Arrow(), 6},
				  {kStart, 1}});
	generator->generate(
		0x0A2EA, {{Dot(), 36},
				  {Arrow(), 8},
				  {kStart, 1}});
	generator->pathWidth = 0.7f;
	generator->generate(
		0x17FB9, {{Dot(), 36},
				  {Arrow(), 4},
				  {kStart, 1},
				  {Stone(Black), 4},
				  {Stone(White), 3}});

	// Invisible Symbols
	generator->resetConfig();
	generator->setFlag(Generate::Config::WriteColors);
	generator->setGridSize(3, 3);
	generator->generate(
		0x0008F, {{Dot(), 3},
				  {Stone(Black), 2},
				  {Stone(X), 3}});
	generator->setGridSize(4, 4);
	generator->generate(
		0x0006B, {{Stone(Black), 2},
				  {Stone(X), 3},
				  {Star(Black), 3},
				  {Star(X), 2}});
	generator->generate(
		0x0008B, {{Stone(Black), 3},
				  {Stone(X), 2},
				  {Triangle(Black), 2},
				  {Star(X), 3}});
	generator->generate(
		0x0008C, {{Star(Black), 4},
				  {Star(X), 4},
				  {SpinPoly(X), 1},
				  {Poly(X), 1}});
	generator->generate(
		0x0008A, {{Star(Black), 2},
				  {Triangle(Black), 2},
				  {SpinPoly(Black), 1},
				  {Poly(X), 1},
				  {Eraser(X), 1}});
	generator->setGridSize(0, 0);
	generator->generate(
		0x00089, {{Poly(X), 2},
				  {NegaPoly(X), 4},
				  {Dot(Deco::Location::Any), 8}});
	generator->generate(
		0x0006A, {{Dot(Deco::Location::Any), 4},
				  {Star(Black), 4},
				  {Star(Orange), 4},
				  {Star(X), 6}});
	generator->generate(
		0x0006C, {{Dot(Deco::Location::Any), 6},
				  {Star(Black), 4},
				  {Star(Orange), 4},
				  {Star(X), 6},
				  {Eraser(X), 1}});
	generator->generate(
		0x00027, {{Stone(Black), 2},
				  {Stone(X), 2},
				  {Eraser(Black), 1}});
	generator->generate(
		0x00028, {{Poly(Black), 2},
				  {Poly(X), 2}});
	generator->generate(
		0x00029, {{Triangle(Black), 8},
				  {Eraser(X), 1}});
	generator->resetConfig();

	// Arrows Exit
	generator->setGridSize(4, 4);
	generator->backgroundColor = {0.5f, 0.5f, 0.5f, 1};
	generator->arrowColor = {0.6f, 0, 1, 1};
	generator->successColor = {0.6f, 0, 1, 1};
	generator->generate(
		0x17CF2, {{Arrow(Any, 1), 12}});

	// Stars Exit
	generator->resetConfig();
	generator->setGridSize(4, 4);
	generator->setFlagOnce(Generate::Config::EnableFlash);
	generator->generate(
		0x021D7, {{Triangle(Cyan), 3},
				  {Triangle(Yellow), 3},
				  {Star(Cyan), 3},
				  {Star(Yellow), 3},
				  {Eraser(Cyan), 1}});

	// Arrow Pillar
	generator->resetConfig();
	// Memory::get()->WritePanelData(0x09DD5, PATH_COLOR, { 0.01f, 0, 0.02f, 1 });
	generator->backgroundColor = {0, 0, 0, 1};
	generator->arrowColor = {0.6f, 0, 1, 1};
	generator->successColor = {1, 1, 1, 1};
	generator->pathWidth = 0.5f;
	generator->generate(
		0x09DD5, {{Arrow(), 8}});
	generator->pathWidth = 1;
	generator->successColor = {0, 0, 0, 0};

	// Challenge Entrance
	generator->backgroundColor = {0.5, 0, 0, 0};
	generator->arrowColor = {0, 0.65f, 0, 1};
	generator->generate(
		0x0A16E, {{Star(Green), 5},
				  {Arrow(Green), 6}});

	// Theater Exit
	generator->backgroundColor = {0.5f, 0.5f, 0.5f, 1};
	generator->arrowColor = {0.6f, 0, 1, 1};
	generator->generate(
		0x039B4, {{Arrow(Any, 2), 12}});

	// Town Exit
	generator->generate(
		0x09E85, {{Arrow(), 16},
				  {kStart, 4},
				  {kExit, 1}});
}

void PuzzleList::GenerateOrchardH()
{
	specialCase->generateApplePuzzle(0x00143, false, true);
	specialCase->generateApplePuzzle(0x0003B, false, Random::rand() % 2 == 0);
	specialCase->generateApplePuzzle(0x00055, false, Random::rand() % 2 == 0);
	specialCase->generateApplePuzzle(0x032F7, false, Random::rand() % 2 == 0);
	specialCase->generateApplePuzzle(0x032FF, true, true);
}

void PuzzleList::GenerateDesertH()
{
	specialCase->setPosition(0x2752B, -174.4f, 173.8f, 15.3f);
	specialCase->setPosition(0x2752F, -174.7f, 173.73f, 15.32f);
	specialCase->setPosition(0x27530, -174.7f, 173.4f, 15.34f);
	specialCase->setOrientation(0x0048F, -23, 5, 0);
	specialCase->generateSpecularPuzzle(
		0x0048F,
		GRID_4x4,
		{{0, 5}, {15, 20}, {1, 6}, {11, 16}, {16, 21}, {17, 22}, {3, 8}, {8, 13},
		 {0, 1}, {1, 2}, {2, 3}, {3, 4}, {5, 6}, {10, 11}, {11, 12},
		 {12, 13}, {13, 14}, {15, 16}, {16, 17}, {20, 21}, {21, 22}, {22, 23}});
}

void PuzzleList::GenerateKeepH()
{
	generator->setLoadingData(L"Keep", 5);
	generator->resetConfig();

	generator->setObstructions({{8, 3}, {4, 5}, {3, 0}, {3, 2}, {5, 6}});
	generator->setFlagOnce(Generate::Config::DisableWrite);
	generator->generate(
		0x033EA, {{Triangle(Yellow), 4},
				  {Star(Yellow), 3},
				  {Stone(Yellow), 2}});
	std::set<Point> path1 = generator->_path;
	generator->write(0x033EA);

	generator->resetConfig();
	generator->setObstructions({{8, 5}, {6, 5}, {1, 8}, {1, 6}, {1, 4}, {2, 3}, {4, 3}, {6, 3}});
	generator->hitPoints = {{3, 2}, {1, 0}};
	generator->setFlagOnce(Generate::Config::DisableWrite);
	generator->generate(
		0x01BE9, {{Star(Black), 3},
				  {Star(White), 3},
				  {Stone(Black), 2},
				  {Stone(White), 2},
				  {SpinPoly(Black), 1},
				  {Poly(White), 1}});
	std::set<Point> path2 = generator->_path;
	generator->write(0x01BE9);

	generator->resetConfig();
	std::vector<std::vector<Point>> validHitPoints = {
		{{3, 4}, {7, 2}, {3, 2}},
		{{4, 5}, {7, 2}, {2, 5}},
		{{4, 3}, {7, 2}, {2, 3}},
		{{3, 4}, {7, 2}, {3, 2}},
		{{1, 4}, {7, 2}, {1, 2}},
		{{3, 2}, {7, 2}, {3, 0}},
		{{1, 2}, {7, 2}, {1, 0}}};
	generator->hitPoints = validHitPoints[Random::rand() % validHitPoints.size()];
	generator->setObstructions({{5, 8}});
	generator->setFlagOnce(Generate::Config::SplitShapes);
	generator->setFlagOnce(Generate::Config::DisableWrite);
	generator->generate(
		0x01CD3, {{Stone(Black), 3},
				  {Stone(White), 2},
				  {Triangle(Yellow), 5},
				  {Poly(), 1},
				  {SpinPoly(Any), 1}});
	std::set<Point> path3 = generator->_path;
	generator->write(0x01CD3);

	generator->resetConfig();
	generator->setSymmetry(Panel::Symmetry::None);
	generator->setFlagOnce(Generate::Config::DisableWrite);
	generator->setObstructions({{0, 7}, {2, 7}, {4, 7}, {6, 7}, {8, 7}, {8, 5}, {7, 6}});
	generator->setSymbol(kStart, 8, 0);
	generator->setSymbol(kExit, 8, 4);
	generator->setFlagOnce(Generate::Config::ShortPath);
	generator->generate(
		0x01D3F, {{Poly(Yellow), 2},
				  {Poly(Blue), 1},
				  {Star(Yellow), 2},
				  {Star(Blue), 2},
				  {Triangle(Yellow), 3},
				  {Triangle(Blue), 3}});
	generator->_panel->_startpoints.push_back({0, 8});
	generator->_panel->_endpoints.push_back(Endpoint(0, 4, Endpoint::Direction::LEFT, kExit));
	if (generator->_panel->_endpoints[0].GetX() == 0) {
		// Need to have endpoints in right order to associate with pressure plates correctly
		std::swap(generator->_panel->_endpoints[0], generator->_panel->_endpoints[1]);
	}
	std::set<Point> path4 = generator->_path;
	generator->write(0x01D3F);

	specialCase->generateKeepLaserPuzzle(
		0x03317, path1, path2, path3, path4,
		{{Triangle(Yellow), 10},
		 {Star(Yellow), 4},
		 {Stone(Yellow), 2},
		 {Star(Black), 3},
		 {Star(White), 3},
		 {Stone(Black), 5},
		 {Stone(White), 4},
		 {SpinPoly(Black), 1},
		 {SpinPoly(White), 1},
		 {SpinPoly(Yellow), 2},
		 {Poly(Yellow), 3},
		 {Poly(Blue), 3},
		 {Star(Blue), 2},
		 {Triangle(Yellow), 2},
		 {Triangle(Blue), 2}});

	specialCase->clearTarget(0x0360E); // Must solve pressure plate side
}

void PuzzleList::GenerateJungleH() {
	generator->setLoadingData(L"Jungle", 6);
	generator->resetConfig();
	specialCase->generateSoundDotPuzzle(0x0026D, {3, 3}, {DOT_SMALL, DOT_LARGE}, false);
	specialCase->generateSoundDotReflectionPuzzle(
		0x0026E, {5, 5}, {DOT_SMALL, DOT_LARGE}, {DOT_SMALL, DOT_LARGE}, 0, true);
	specialCase->generateSoundDotReflectionPuzzle(
		0x0026F,
		{7, 7},
		{DOT_MEDIUM, DOT_MEDIUM, DOT_SMALL, DOT_MEDIUM, DOT_LARGE},
		{DOT_MEDIUM, DOT_MEDIUM, DOT_SMALL, DOT_MEDIUM, DOT_LARGE},
		0,
		true);
	specialCase->generateSoundDotReflectionPuzzle(
		0x00C3F,
		{7, 7},
		{DOT_SMALL, DOT_MEDIUM, DOT_SMALL, DOT_LARGE},
		{DOT_LARGE, DOT_MEDIUM, DOT_MEDIUM, DOT_SMALL, DOT_LARGE},
		0,
		true);
	if (Random::rand() % 2)
		specialCase->generateSoundDotReflectionPuzzle(
			0x00C41,
			{7, 7},
			{DOT_SMALL, DOT_SMALL, DOT_LARGE, DOT_MEDIUM, DOT_LARGE},
			{DOT_SMALL, DOT_SMALL, DOT_LARGE, DOT_MEDIUM, DOT_LARGE},
			0,
			true);
	else
		specialCase->generateSoundDotReflectionPuzzle(
			0x00C41,
			{7, 7},
			{DOT_MEDIUM, DOT_MEDIUM, DOT_SMALL, DOT_MEDIUM, DOT_LARGE},
			{DOT_MEDIUM, DOT_MEDIUM, DOT_SMALL, DOT_MEDIUM, DOT_LARGE},
			0,
			true);

	switch (Random::rand() % 4) {
	case 0:
		specialCase->generateSoundDotReflectionPuzzle(
			0x014B2,
			{7, 7},
			{DOT_SMALL, DOT_LARGE, DOT_SMALL, DOT_LARGE, DOT_MEDIUM},
			{DOT_SMALL, DOT_LARGE, DOT_SMALL, DOT_LARGE, DOT_MEDIUM},
			0,
			true);
		break;
	case 1:
		specialCase->generateSoundDotReflectionPuzzle(
			0x014B2,
			{7, 7},
			{DOT_LARGE, DOT_MEDIUM, DOT_SMALL, DOT_LARGE, DOT_SMALL},
			{DOT_LARGE, DOT_MEDIUM, DOT_SMALL, DOT_LARGE, DOT_SMALL},
			0,
			true);
		break;
	case 2:
		specialCase->generateSoundDotReflectionPuzzle(
			0x014B2,
			{7, 7},
			{DOT_LARGE, DOT_MEDIUM, DOT_SMALL, DOT_LARGE, DOT_SMALL},
			{DOT_SMALL, DOT_LARGE, DOT_SMALL, DOT_LARGE, DOT_MEDIUM},
			0,
			true);
		break;
	case 3:
		specialCase->generateSoundDotReflectionPuzzle(
			0x014B2,
			{7, 7},
			{DOT_SMALL, DOT_LARGE, DOT_SMALL, DOT_LARGE, DOT_MEDIUM},
			{DOT_LARGE, DOT_MEDIUM, DOT_SMALL, DOT_LARGE, DOT_SMALL},
			0,
			true);
		break;
	}
}