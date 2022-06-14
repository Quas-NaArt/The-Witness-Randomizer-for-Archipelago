#pragma once

#include <map>
#include <chrono>
#include "..\Watchdog.h"
#include "..\Generate.h"
#include "APRandomizer.h"
#include "..\DateTime.h"

class APWatchdog : public Watchdog {
public:
	APWatchdog(APClient* client, std::map<int, int> mapping, int lastPanel, HWND skipButton1, HWND availableSkips1) : Watchdog(1) {
		generator = std::make_shared<Generate>();
		ap = client;
		panelIdToLocationId = mapping;
		finalPanel = lastPanel;
		skipButton = skipButton1;
		availableSkips = availableSkips1;
	}

	int skippedPuzzles = 0;
	int availablePuzzleSkips = 0;

	HWND skipButton;
	HWND availableSkips;

	virtual void action();

	void MarkLocationChecked(int locationId);
	void ApplyTemporarySpeedBoost();
	void ApplyTemporarySlow();
	void TriggerPowerSurge();
	void ResetPowerSurge();
	void AddPuzzleSkip();
	void SkipPuzzle();

	void SkipPreviouslySkippedPuzzles();

private:
	APClient* ap;
	std::shared_ptr<Generate> generator;
	std::map<int, int> panelIdToLocationId;
	int finalPanel;
	bool isCompleted = false;

	bool hasPowerSurge = false;
	std::chrono::system_clock::time_point powerSurgeStartTime;

	float baseSpeed = 2.0f;
	float currentSpeed = 2.0f;
	bool hasTemporarySpeedModdification = false;
	std::chrono::system_clock::time_point temporarySpeedModdificationStartTime;

	void CheckSolvedPanels();
	void HandleMovementSpeed();
	void HandlePowerSurge();
	void UpdateChallengeLock();
	boolean CheckIfCanSkipPuzzle();
};

class APServerPoller : public Watchdog {
public:
	APServerPoller(APClient* client) : Watchdog(0.1f) {
		ap = client;
	}

	virtual void action() {
		ap->poll();
	}

private:
	APClient* ap;
};