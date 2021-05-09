#ifndef HEX_H
#define HEX_H

#include <string>
#include <list>

class Hex;
typedef std::list<Hex> HexList;

class Hex
{

public:
	enum Axis { Q, R, DefinitiveAxesNum, S = DefinitiveAxesNum, AxesNum };
	enum Dir { DirNE, DirSE, DirS, DirSW, DirNW, DirN, DirsNum };
	enum Corner { CornNE, CornE, CornSE, CornSW, CornW, CornNW, CornersNum };
	
	Hex() {}
	Hex(int q, int r) { c[Q] = q; c[R] = r; calcS(); }
	Hex(const Hex &other) { *this = other; }
	Hex &operator=(const Hex &other) { for (int i=0; i < AxesNum; i++) { c[i] = other.c[i]; } return *this; }
	bool operator==(const Hex &other) const { return other.q() == q() && other.r() == r(); }
	bool operator!=(const Hex &other) const { return !(*this == other); }
	bool operator<(const Hex &other) const;
	Hex operator+(const Hex &other) const { return Hex(q() + other.q(), r() + other.r()); }
	Hex operator-(const Hex &other) const { return Hex(q() - other.q(), r() - other.r()); }
	int q() const { return c[Q]; }
	int r() const { return c[R]; }
	int s() const { return c[S]; }
	void setQ(int newQ) { c[Q] = newQ; calcS(); }
	void setR(int newR) { c[R] = newR; calcS(); }
	std::string toString() const { return "(" + std::to_string(q()) + "; " + std::to_string(r()) + ")"; }
	bool isAdjacent(const Hex &hex) const;
	Hex neighbour(Dir dir, int dist = 1) const;
	HexList neighbours(Dir dir, int dist) const;
	HexList neighbours(int dist) const;
	Dir dirToNeighbour(const Hex &other) const;
	Hex mirror() const { 
		return Hex(-c[Q], -c[R]);
	}
	
	static Dir oppositeDir(Dir dir) { return (Dir)((dir + DirsNum/2) % DirsNum); }
	static Dir nextDir(Dir dir, int stepNum = 1) { return (Dir)((dir + stepNum) % DirsNum); }
	static double dirRadians(Dir dir);
	static double cornerRadians(Corner corner);
		
private:
	void calcS() { c[S] = -q() - r(); }
	
	int c[AxesNum];
	static const int neighbOffsets[DirsNum][AxesNum];
	static const Dir neighbOffsetDirs[3][3]; // both indexes correspond -1/0/1, for [q][r] offsets
};


#endif // HEX_H
