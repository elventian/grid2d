#ifndef CELL_H
#define CELL_H

#include <string>

class Cell
{
public:
	enum State {
		Unknown,
		Empty,
		Wall,
		
		StatesNum
	};
	static bool isPassable(State state) { return state == Empty; }
	static State stateFromStr(std::string str) {
		if (str == "#") { return Wall; }
		else if (str == "_") { return Empty; }
		else return Unknown;
	}
};


#endif // CELL_H
