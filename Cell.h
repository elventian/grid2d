#ifndef CELL_H
#define CELL_H


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
};


#endif // CELL_H
