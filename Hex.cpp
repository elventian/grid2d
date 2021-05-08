#include "Hex.h"
#include <math.h>


const int Hex::neighbOffsets[DirsNum][AxesNum] = {
	{+1, -1,  0},
	{+1,  0, -1},
	{ 0, +1, -1},
	{-1, +1,  0},
	{-1,  0, +1},
	{ 0, -1, +1},
};

const Hex::Dir Hex::neighbOffsetDirs[3][3] = {
	{ DirsNum, DirNW,   DirSW   }, // q diff is -1
	{ DirN,    DirsNum, DirS    }, // q diff is  0
	{ DirNE,   DirSE,   DirsNum }, // q diff is  1
};

static constexpr int invalidIndex = -1;


static int indexFromDiff(int diff)
{
	if (diff > 0) { return 2; }
	if (diff < 0) { return 0; }
	if (diff == 0) { return 1; }
	return invalidIndex;
}


bool Hex::operator<(const Hex &other) const
{
	if (q() < other.q()) { return true; }
	if (q() > other.q()) { return false; }
	return r() < other.r();
}


bool Hex::isAdjacent(const Hex &hex) const
{
	int diffNums[3] = { 0, 0, 0 };
	for (int i=0; i < AxesNum; i++) {
		int diff = hex.c[i] - c[i];
		if (diff < -1 || diff > 1) { return false; }
		diffNums[diff+1]++;
	}
	for (int i=0; i < 3; i++) {
		if (diffNums[i] != 1) { return false; }
	}
	return true;
}


Hex Hex::neighbour(Dir dir, int dist) const
{
	Hex hex;
	const int *offsets = (const int *)(&neighbOffsets[dir]);
	for (int i=0; i < AxesNum; i++) {
		hex.c[i] = c[i] + offsets[i] * dist;
	}
	return hex;
}

HexList Hex::neighbours(Hex::Dir dir, int dist) const
{
	HexList res;
	while (dist > 0) {
		res.push_back(neighbour(dir, dist));
		dist--;
	}
	return res;
}

HexList Hex::neighbours(int dist) const
{
	HexList res;
	while (dist > 0) {
		for (int dir = 0; dir < DirsNum; dir++)	{
			Hex neigh = neighbour((Dir)dir, dist);
			res.push_back(neigh);
			int gapHexesNum = dist - 1;
			for (int i = 0; i < gapHexesNum; i++) {
				Hex gap = neigh.neighbour(nextDir((Dir)dir, 2), i + 1);
				res.push_back(gap);
			}
		}
		dist--;
	}
	return res;
}


Hex::Dir Hex::dirToNeighbour(const Hex &other) const
{
	const Hex diff = other - *this;
	const int qIndex = indexFromDiff(diff.q());
	if (qIndex == invalidIndex) { return DirsNum; }
	const int rIndex = indexFromDiff(diff.r());
	if (rIndex == invalidIndex) { return DirsNum; }
	return neighbOffsetDirs[qIndex][rIndex];
}


double Hex::dirRadians(Dir dir)
{
	return 2*M_PI * (dir / (double)DirsNum) - M_PI/6;
}


double Hex::cornerRadians(Corner corner)
{
	return 2*M_PI * (corner / (double)CornersNum) - M_PI/3;
}
