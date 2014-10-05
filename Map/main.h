#include "Map.h"
#include "AI.h"

#include <string>
#include <chrono>

#pragma once

using namespace std::chrono;

class Main
{
public:
	enum Answer
	{
		automatic	= 1,
		manual		= 2,

		instant = 1,
		fast	= 2,
		normal	= 3,
		slow	= 4,

		three	= 1,
		four	= 2,

		smallMaze	= 1,
		mediumMaze	= 2,
		bigMaze		= 3,

		yes = 1,
		no	= 2
	};

	struct Results
	{
		int seed;

		AI::Algorithm algo;
		string algorithm;

		int elements;
		bool success;
		milliseconds performance;
	};

	//void Results(const int solved, const int failed, const milliseconds perf);
};