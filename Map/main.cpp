#include "main.h"

int main()
{
	vector<unsigned int> seeds;
	//unsigned int startSeed = (unsigned int)time(NULL);
	unsigned int startSeed = 500;
	seeds.push_back(startSeed);

	for (int i = 1; i < 10; i++)
	{
		seeds.push_back(seeds[i - 1] + 1000);
	}
	int numOfProblems = 0;

	Main::Answer repeat;

	do
	{
		Main::Answer mode;
		Main::Answer speed;
		Main::Answer size;
		AI::Algorithm algo;

		int agentSpeed;

		const int maxElements = 100;
		int seedIndex;
		vector<AI::Algorithm> algorithms;
		vector<Main::Results> results;

		do
		{
			system("CLS");
			printf("Please choose which mode to run:\n\n");
			printf("\t1. Automatic (Will run 10 different instances in all algorithms and will present results)\n");
			printf("\t2. Manual\n\n");

			printf("Choice: ");

			scanf("%d", &mode);
		} while ((mode != Main::automatic) &&
			(mode != Main::manual));

		switch (mode)
		{
		case Main::automatic:
			agentSpeed = 0;

			algorithms.push_back(AI::Algorithm::MC);
			algorithms.push_back(AI::Algorithm::BT);
			algorithms.push_back(AI::Algorithm::BJ);
			algorithms.push_back(AI::Algorithm::BF);
			algorithms.push_back(AI::Algorithm::AS);

			for (int k = 2; k < 3; k++)
			{
				AI::Algorithm algo = algorithms[k];
				string algorithm;

				switch (algo)
				{
				case AI::Algorithm::MC:
					algorithm = "Min-Conflicts";
					break;
				case AI::Algorithm::BT:
					algorithm = "Backtracking";
					break;
				case AI::Algorithm::BJ:
					algorithm = "Backjumping";
					break;
				}

				seedIndex = 0;

				string suffix = "-3";

				for (int elements = 10; elements <= maxElements; elements += 10)
				{
					const unsigned int seed = seeds[seedIndex++];

					system("CLS");
					printf("Algorithm\t: %s%s", algorithm.c_str(), suffix.c_str());
					printf("\nElements\t: %d", elements);
					printf("\nSize\t\t: %dx%d", elements, elements);
					printf("\n\nCreating Map...");
					Map map(elements, elements, seed);

					system("CLS");
					printf("Algorithm\t: %s%s", algorithm.c_str(), suffix.c_str());
					printf("\nElements\t: %d", elements);
					printf("\nSize\t\t: %dx%d", elements, elements);
					printf("\n\nSolving...");
					AI solver(map, agentSpeed, algo, 3);
					solver.Solve();

					milliseconds performance = duration_cast<milliseconds>(solver._performance);
					const bool success = solver._success;

					Main::Results result = { seed, algo, algorithm + suffix, elements, success, performance };
					results.push_back(result);
				}

				seedIndex = 0;
				suffix = "-4";

				for (int elements = 10; elements <= maxElements; elements += 10)
				{
					const unsigned int seed = seeds[seedIndex++];

					system("CLS");
					printf("Algorithm\t: %s%s", algorithm.c_str(), suffix.c_str());
					printf("\nElements\t: %d", elements);
					printf("\nSize\t\t: %dx%d", elements, elements);
					printf("\n\nCreating Map...");
					Map map(elements, elements, seed);

					system("CLS");
					printf("Algorithm\t: %s%s", algorithm.c_str(), suffix.c_str());
					printf("\nElements\t: %d", elements);
					printf("\nSize\t\t: %dx%d", elements, elements);
					printf("\n\nSolving...");
					AI solver(map, agentSpeed, algo, 4);
					solver.Solve();

					milliseconds performance = duration_cast<milliseconds>(solver._performance);
					const bool success = solver._success;

					Main::Results result = { seed, algo, algorithm + suffix, elements, success, performance };
					results.push_back(result);
				}
			}

			/*for (int k = 3; k < 5; k++)
			{
				AI::Algorithm algo = algorithms[k];
				string algorithm;

				switch (algo)
				{
				case AI::Algorithm::BF:
					algorithm = "BestFirstSearch";
					break;
				case AI::Algorithm::AS:
					algorithm = "A*";
					break;
				}

				seedIndex = 0;

				for (int elements = 10; elements <= maxElements; elements += 10)
				{
					const unsigned int seed = seeds[seedIndex++];

					system("CLS");
					printf("Algorithm\t: %s", algorithm.c_str());
					printf("\nElements\t: %d", elements);
					printf("\nSize\t\t: %dx%d", elements, elements);
					printf("\n\nCreating Map...");
					Map map(elements, elements, seed);

					system("CLS");
					printf("Algorithm\t: %s", algorithm.c_str());
					printf("\nElements\t: %d", elements);
					printf("\nSize\t\t: %dx%d", elements, elements);
					printf("\n\nSolving...");
					AI solver(map, agentSpeed, algo);
					solver.Solve();

					milliseconds performance = duration_cast<milliseconds>(solver._performance);
					const bool success = solver._success;

					Main::Results result = { seed, algo, algorithm, elements, success, performance };
					results.push_back(result);
				}
			}*/
			break;
		case Main::manual:
			int elements;
			do
			{
				system("CLS");
				printf("Please choose the agent's speed:\n\n");
				printf("\t1. Instant (not recommended because the solution will not be visual)\n");
				printf("\t2. Fast\n");
				printf("\t3. Normal\n");
				printf("\t4. Slow\n\n");

				printf("Choice: ");

				scanf("%d", &speed);
			} while ((speed != Main::instant)	&&
					 (speed != Main::fast)		&&
					 (speed != Main::normal)	&&
					 (speed != Main::slow));

			switch (speed)
			{
			case Main::instant:
				agentSpeed = 0;
				break;
			case Main::fast:
				agentSpeed = 30;
				break;
			case Main::normal:
				agentSpeed = 100;
				break;
			case Main::slow:
				agentSpeed = 300;
				break;
			}

			do
			{
				system("CLS");
				printf("Please type the number of elements:\n\n");

				printf("Choice: ");

				scanf("%d", &elements);
			} while (elements < 0);

			do
			{
				system("CLS");
				printf("Please choose the algorithm to solve with:\n\n");
				printf("\t1. Minimum Conflicts\n");
				printf("\t2. Backtracking\n");
				printf("\t3. Backjumping\n\n");
				printf("\t4. BestFirst\n");
				printf("\t5. A Star\n\n");

				printf("Choice: ");

				scanf("%d", &algo);
			} while ((algo != AI::MC) &&
				(algo != AI::BT) &&
				(algo != AI::BJ) &&
				(algo != AI::BF) &&
				(algo != AI::AS));

			string algorithm;

			switch (algo)
			{
			case AI::Algorithm::MC:
				algorithm = "Min-Conflicts";
				break;
			case AI::Algorithm::BT:
				algorithm = "Backtracking";
				break;
			case AI::Algorithm::BJ:
				algorithm = "Backjumping";
				break;
			case AI::Algorithm::BF:
				algorithm = "BestFirstSearch";
				break;
			case AI::Algorithm::AS:
				algorithm = "A*";
				break;

			}

			system("CLS");
			printf("Algorithm\t: %s", algorithm.c_str());
			printf("\nElements\t: %d", elements);
			printf("\nSize\t\t: %dx%d", elements, elements);
			printf("\n\nCreating Map...");
			Map map(elements, elements, startSeed);

			if (algo == AI::MC || algo == AI::BT || algo == AI::BJ)
			{
				Main::Answer colorChoice;
				int colorsNum;
				do
				{
					system("CLS");
					printf("Please choose the number of colors:\n\n");
					printf("\t1. Three\n");
					printf("\t2. Four\n");

					printf("Choice: ");

					scanf("%d", &colorChoice);
				} while ((colorChoice != Main::Answer::three) &&
						(colorChoice != Main::Answer::four));

				switch (colorChoice)
				{
				case Main::three:
					colorsNum = 3;
					break;
				case Main::four:
					colorsNum = 4;
					break;
				default:
					colorsNum = 3;
					break;
				}

				system("CLS");
				printf("Algorithm\t: %s", algorithm.c_str());
				printf("\nElements\t: %d", elements);
				printf("\nSize\t\t: %dx%d", elements, elements);
				printf("\n\nSolving...");
				AI solver(map, agentSpeed, algo, colorsNum);
				solver.Solve();

				string suffix = static_cast<ostringstream*>(&(ostringstream() << colorsNum))->str();

				milliseconds performance = duration_cast<milliseconds>(solver._performance);
				const bool success = solver._success;

				Main::Results result = { startSeed, algo, algorithm + "-" + suffix, elements, success, performance };
				results.push_back(result);
			}
			else
			{
				system("CLS");
				printf("Algorithm\t: %s", algorithm.c_str());
				printf("\nElements\t: %d", elements);
				printf("\nSize\t\t: %dx%d", elements, elements);
				printf("\n\nSolving...");
				AI solver(map, agentSpeed, algo);
				solver.Solve();

				milliseconds performance = duration_cast<milliseconds>(solver._performance);
				const bool success = solver._success;

				Main::Results result = { startSeed, algo, algorithm, elements, success, performance };
				results.push_back(result);
			}
			break;
		}

		system("CLS");
		/*for each (Main::Results result in results)
		{
			printf("Seed\t\t: %d", result.seed);
			printf("\nAlgorithm\t: %s", result.algorithm.c_str());
			printf("\nElements\t: %d", result.elements);
			printf("\nSize\t\t: %dx%d", result.elements, result.elements);
			printf("\nSuccess\t\t: %s", result.success ? "yes" : "no");
			printf("\nPerformance\t: %.2f\n\n", (double)result.performance.count());
		}*/
		
		for each (Main::Results result in results)
		{
			printf("%s | %d: %.2fms (%s)\n\n",
				result.algorithm.c_str(), result.elements,
				(double)result.performance.count(), result.success ? "succeeded" : "failed");
		}

		do
		{
		printf("Run again?\n\n");
		printf("\t1. Yes\n");
		printf("\t2. No\n\n");

		printf("Choice: ");

		scanf("%d", &repeat);
		} while ((repeat != Main::yes) &&
		(repeat != Main::no));

	} while (repeat == Main::yes);
}






//	while (true)
//	{
//		/*int seed = (unsigned int)time(NULL);
//		const int elements = rand() % 80;
//		int size;
//
//		do
//		{
//		size = rand() % 80;
//		} while (size < elements);*/
//		elements += 1;
//		//size += 1;
//		//seed += 100;
//
//
//		//Map map(10, 10, seed);
//
//		//Map map2(elements, size, seed);
//
//		//Map map2(57, 62, 5200);
//
//		AI::Algorithm algo = AI::Algorithm::MC;
//
//		//AI solver(map, 1000, algo);
//		//		AI solver(map, 1000, algo, 4);
//		//AI solver2(map2, 0, 3);
//
//		//solver.Solve();
//		//solver2.Solve(AI::BJ);
//
//		/*if (solver._success != solver2._success)
//		{
//		cout << "SHIT";
//		}*/
//
//		//numOfProblems++;
//		//performance += duration_cast<milliseconds>(solver._performance);
//		////map.ColorPrintAll();
//
//
//		/*if (solver2._success)
//		{
//		solved++;
//		}
//		else
//		{
//		failed++;
//		}
//
//		Results(solved, failed, performance);*/
//
//		//Sleep(500);
//	}
//
////	Results(solved, failed, performance);
//
//	return 0;
//}