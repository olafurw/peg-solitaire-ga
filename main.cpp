#include <random>
#include <algorithm>
#include <chrono>
#include <vector>
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <iterator>

static const int locBoardSize = 7;
static const auto locSeed = std::chrono::system_clock::now().time_since_epoch().count();

enum Peg
{
	Peg_Invalid = 0,
	Peg_Empty = 1,
	Peg_Filled = 2
};

enum Move
{
	Move_North = 0,
	Move_South = 1,
	Move_East = 2,
	Move_West = 3
};

int
RandomPosition()
{
	static std::mt19937 gen(locSeed);
	static std::uniform_int_distribution<> dis(0, locBoardSize - 1);

	return dis(gen);
}

int
RandomParent()
{
	static std::mt19937 gen(locSeed);
	static std::uniform_int_distribution<> dis(0, 49);

	return dis(gen);
}

int
RandomActionLocation()
{
	static std::mt19937 gen(locSeed);
	static std::uniform_int_distribution<> dis(0, 999);

	return dis(gen);
}

Move
RandomMove()
{
	static std::mt19937 gen(locSeed);
	static std::uniform_int_distribution<> dis(0, 3);

	return static_cast<Move>(dis(gen));
}

struct Action
{
					Action(): myMove(Move_North), myX(0), myY(0) {}

	Move			myMove;
	unsigned int	myX;
	unsigned int	myY;
};

struct Actions
{
							Actions(): myScore(0) {}

	std::vector<Action>		myActions;
	unsigned int			myScore;
};

static Peg locOriginalBoard[locBoardSize][locBoardSize] = {
	{ Peg_Invalid,	Peg_Invalid,	Peg_Filled,		Peg_Filled,		Peg_Filled,		Peg_Invalid,	Peg_Invalid },
	{ Peg_Invalid,	Peg_Invalid,	Peg_Filled,		Peg_Filled,		Peg_Filled,		Peg_Invalid,	Peg_Invalid },
	{ Peg_Filled,	Peg_Filled,		Peg_Filled,		Peg_Filled,		Peg_Filled,		Peg_Filled,		Peg_Filled },
	{ Peg_Filled,	Peg_Filled,		Peg_Filled,		Peg_Empty,		Peg_Filled,		Peg_Filled,		Peg_Filled },
	{ Peg_Filled,	Peg_Filled,		Peg_Filled,		Peg_Filled,		Peg_Filled,		Peg_Filled,		Peg_Filled },
	{ Peg_Invalid,	Peg_Invalid,	Peg_Filled,		Peg_Filled,		Peg_Filled,		Peg_Invalid,	Peg_Invalid },
	{ Peg_Invalid,	Peg_Invalid,	Peg_Filled,		Peg_Filled,		Peg_Filled,		Peg_Invalid,	Peg_Invalid }
};

static Peg locBoard[locBoardSize][locBoardSize] = {
	{ Peg_Invalid,	Peg_Invalid,	Peg_Filled,		Peg_Filled,		Peg_Filled,		Peg_Invalid,	Peg_Invalid },
	{ Peg_Invalid,	Peg_Invalid,	Peg_Filled,		Peg_Filled,		Peg_Filled,		Peg_Invalid,	Peg_Invalid },
	{ Peg_Filled,	Peg_Filled,		Peg_Filled,		Peg_Filled,		Peg_Filled,		Peg_Filled,		Peg_Filled },
	{ Peg_Filled,	Peg_Filled,		Peg_Filled,		Peg_Empty,		Peg_Filled,		Peg_Filled,		Peg_Filled },
	{ Peg_Filled,	Peg_Filled,		Peg_Filled,		Peg_Filled,		Peg_Filled,		Peg_Filled,		Peg_Filled },
	{ Peg_Invalid,	Peg_Invalid,	Peg_Filled,		Peg_Filled,		Peg_Filled,		Peg_Invalid,	Peg_Invalid },
	{ Peg_Invalid,	Peg_Invalid,	Peg_Filled,		Peg_Filled,		Peg_Filled,		Peg_Invalid,	Peg_Invalid }
};

void
ResetBoard()
{
	for (size_t x = 0; x < locBoardSize; x++)
	{
		for (size_t y = 0; y < locBoardSize; y++)
		{
			locBoard[x][y] = locOriginalBoard[x][y];
		}
	}
}

bool
PerformAction(
	const Action&	aAction)
{
	Peg& movePeg = locBoard[aAction.myX][aAction.myY];
	if(movePeg != Peg_Filled)
	{
		return false;
	}

	if (aAction.myMove == Move_North)
	{
		if (aAction.myY < 2)
		{
			return false;
		}

		Peg& jumpPeg = locBoard[aAction.myX][aAction.myY - 1];
		if (jumpPeg != Peg_Filled)
		{
			return false;
		}

		Peg& destPeg = locBoard[aAction.myX][aAction.myY - 2];
		if (destPeg != Peg_Empty)
		{
			return false;
		}

		movePeg = Peg_Empty;
		jumpPeg = Peg_Empty;
		destPeg = Peg_Filled;

		return true;
	}

	if (aAction.myMove == Move_South)
	{
		if (aAction.myY > 4)
		{
			return false;
		}

		Peg& jumpPeg = locBoard[aAction.myX][aAction.myY + 1];
		if (jumpPeg != Peg_Filled)
		{
			return false;
		}

		Peg& destPeg = locBoard[aAction.myX][aAction.myY + 2];
		if (destPeg != Peg_Empty)
		{
			return false;
		}

		movePeg = Peg_Empty;
		jumpPeg = Peg_Empty;
		destPeg = Peg_Filled;

		return true;
	}

	if (aAction.myMove == Move_West)
	{
		if (aAction.myX > 4)
		{
			return false;
		}

		Peg& jumpPeg = locBoard[aAction.myX + 1][aAction.myY];
		if (jumpPeg != Peg_Filled)
		{
			return false;
		}

		Peg& destPeg = locBoard[aAction.myX + 2][aAction.myY];
		if (destPeg != Peg_Empty)
		{
			return false;
		}

		movePeg = Peg_Empty;
		jumpPeg = Peg_Empty;
		destPeg = Peg_Filled;

		return true;
	}

	if (aAction.myMove == Move_East)
	{
		if (aAction.myX < 2)
		{
			return false;
		}

		Peg& jumpPeg = locBoard[aAction.myX - 1][aAction.myY];
		if (jumpPeg != Peg_Filled)
		{
			return false;
		}

		Peg& destPeg = locBoard[aAction.myX - 2][aAction.myY];
		if (destPeg != Peg_Empty)
		{
			return false;
		}

		movePeg = Peg_Empty;
		jumpPeg = Peg_Empty;
		destPeg = Peg_Filled;

		return true;
	}

	return false;
}

int
CountEmptySpaces()
{
	int empty = 0;
	for (size_t x = 0; x < locBoardSize; x++)
	{
		for (size_t y = 0; y < locBoardSize; y++)
		{
			if(locBoard[x][y] == Peg_Empty)
			{
				++empty;
			}
		}
	}

	return empty;
}

void
BoardToFile(
	const int						aId)
{
	std::stringstream ss;
	for (size_t x = 0; x < locBoardSize; x++)
	{
		for (size_t y = 0; y < locBoardSize; y++)
		{
			ss << static_cast<unsigned int>(locBoard[x][y]);
		}

		ss << std::endl;
	}

	std::ofstream out("data/" + std::to_string(aId) + ".txt", std::ios::out);
	out << ss.str();
	out.close();
}

void
ActionsToFile(
	const std::vector<Action>&		aActions)
{
	ResetBoard();

	int id = 0;
	BoardToFile(id++);

	for (const auto& action : aActions)
	{
		PerformAction(action);
		BoardToFile(id++);
	}

	ResetBoard();
}

void
GenerateRandomAction(
	Action&					aOutAction)
{
	aOutAction.myMove = RandomMove();
	aOutAction.myX = RandomPosition();
	aOutAction.myY = RandomPosition();
}

void
GenerateRandomActions(
	std::vector<Action>&	aOutActions)
{
	for (size_t i = 0; i < 1000; i++)
	{
		Action action;
		action.myMove = RandomMove();
		action.myX = RandomPosition();
		action.myY = RandomPosition();
		aOutActions.emplace_back(std::move(action));
	}
}

void
CreateChildren(
	const Actions&		aParentA,
	const Actions&		aParentB,
	Actions&			aChildA,
	Actions&			aChildB)
{
	std::copy(aParentA.myActions.begin(),			aParentA.myActions.begin() + 500,		std::back_inserter(aChildA.myActions));
	std::copy(aParentB.myActions.begin() + 500,		aParentB.myActions.end(),				std::back_inserter(aChildA.myActions));

	std::copy(aParentB.myActions.begin(),			aParentB.myActions.begin() + 500,		std::back_inserter(aChildB.myActions));
	std::copy(aParentA.myActions.begin() + 500,		aParentA.myActions.end(),				std::back_inserter(aChildB.myActions));

	for (size_t i = 0; i < 180; i++)
	{
		Action action;
		GenerateRandomAction(action);

		aChildA.myActions[RandomActionLocation()] = action;
	}

	for (size_t i = 0; i < 180; i++)
	{
		Action action;
		GenerateRandomAction(action);

		aChildB.myActions[RandomActionLocation()] = action;
	}
}

int
main()
{
	std::vector<Actions> population;

	for (size_t i = 0; i < 100; i++)
	{
		Actions actions;
		GenerateRandomActions(actions.myActions);
		population.emplace_back(std::move(actions));
	}

	ResetBoard();

	int highestKnownScore = 0;

	while(true)
	{
		for (Actions& actions : population)
		{
			ResetBoard();

			for (const Action& action : actions.myActions)
			{
				PerformAction(action);
			}

			actions.myScore = CountEmptySpaces();
		}

		std::sort(population.begin(), population.end(), [](const Actions& actionA, const Actions& actionB) { return actionA.myScore > actionB.myScore; });

		const int highestScore = population[0].myScore;
		if(highestScore > highestKnownScore)
		{
			highestKnownScore = highestScore;

			ActionsToFile(population[0].myActions);
		}

		int avgScoreSum = 0;

		for (const Actions& actions : population)
		{
			avgScoreSum += actions.myScore;
		}

		std::cout << highestScore << " " << (avgScoreSum / 100.0f) << std::endl;

		population.resize(50);

		for (size_t i = 0; i < 25; i++)
		{
			const Actions& parentA = population[RandomParent()];
			const Actions& parentB = population[RandomParent()];

			Actions childA;
			Actions childB;
			CreateChildren(parentA, parentB, childA, childB);

			population.emplace_back(std::move(childA));
			population.emplace_back(std::move(childB));
		}
	}

	return 0;
}