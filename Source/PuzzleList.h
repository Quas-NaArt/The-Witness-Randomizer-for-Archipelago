#pragma once

#include "Generate.h"
#include "Special.h"
#include "Random.h"

// This class exists to send a lot of calls to the Generator on the initial call to the Randomizer.
// Then once the generation is over, it gets destructed, a job well done.
class PuzzleList {

public: 

	void GenerateAllN();
	void GenerateAllH();

	// All of these values are known by the sole caller at the time of calling.
	// Construct with them rather than calling setSeed on a half-constructed object.
	PuzzleList(HWND loadingHandle, int seed, bool isRNG, bool colorblind)
		: _handle{loadingHandle}
		, seed{seed}
		, seedIsRNG{isRNG}
		, colorblind{colorblind}
	{
		generator = std::make_shared<Generate>(loadingHandle, seed, colorblind);
		specialCase = std::make_shared<Special>(generator);
	}

	// PuzzleList(std::shared_ptr<Generate> generator) {
	// 	this->generator = generator;
	// 	this->specialCase = std::make_shared<Special>(generator);
	// }

	// void setLoadingHandle(HWND handle) {
	// 	_handle = handle;
	// 	generator->setLoadingHandle(handle);
	// }

	// void setSeed(int seed, bool isRNG, bool colorblind) {
	// 	this->seed = seed;
	// 	this->seedIsRNG = isRNG;
	// 	this->colorblind = colorblind;
	// 	if (seed >= 0) generator->seed(seed);
	// 	else generator->seed(Random::rand());
	// 	generator->colorblind = colorblind;
	// }

	void CopyTargets();

private: // None of these are called externally
	//--------------------------Normal difficulty---------------------------

	void GenerateTutorialN();
	void GenerateSymmetryN();
	void GenerateQuarryN();
	//void GenerateBunkerN(); //Can't randomize because panels refuse to render the symbols
	void GenerateSwampN();
	void GenerateTreehouseN();
	void GenerateTownN();
	void GenerateVaultsN();
	void GenerateTrianglePanelsN();
	void GenerateMountainN();
	void GenerateCavesN();

	//Environmental areas - unless I can figure out how to mess with the game's assets, randomizing most of these puzzles isn't happening anytime soon
	void GenerateOrchardN();
	void GenerateDesertN(); //Just scramble the positions for now
	void GenerateShadowsN(); //Can't randomize
	void GenerateKeepN();
	void GenerateMonasteryN(); //Can't randomize
	void GenerateJungleN();

	//-------------------------Hard difficulty--------------------------

	void GenerateTutorialH();
	void GenerateSymmetryH();
	void GenerateQuarryH();
	void GenerateBunkerH();
	void GenerateSwampH();
	void GenerateTreehouseH();
	void GenerateTownH();
	void GenerateVaultsH();
	void GenerateTrianglePanelsH();
	void GenerateMountainH();
	void GenerateCavesH();

	//Environmental areas - unless I can figure out how to mess with the game's assets, randomizing some of these puzzles isn't happening anytime soon
	void GenerateOrchardH();
	void GenerateDesertH(); //Just scramble the positions for now
	void GenerateShadowsH(); //Can't randomize
	void GenerateKeepH();
	void GenerateMonasteryH(); //Can't randomize
	void GenerateJungleH();

private:
	std::shared_ptr<Generate> generator;
	std::shared_ptr<Special> specialCase;
	HWND _handle = nullptr;
	int seed = 0; // What if this class didn't hold onto these?
	bool seedIsRNG = false; // What if this class didn't hold onto these?
	bool colorblind = false; // What if this class didn't hold onto these?

};
