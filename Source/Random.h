#pragma once
#include <random>
#include <stdlib.h>

// TODO: Make this thread-safe.
// Ensure that each thread that needs to use this is started in the same order.
// Each thread started gets a new MT engine based off of the original seed, and then they are kept thread-local.
// This MAY allow for multi-threaded deterministic puzzle generation.
struct Random {

	static std::mt19937 gen;

	static void seed(int val) {
		gen = std::mt19937(val);
	}

	static int rand() {
		return abs((int)gen());
	}
	static double rand_float() {
		std::uniform_real_distribution<> dist(0, 1);
		return dist(gen);
	}
};
