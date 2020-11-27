#ifndef DIRECTION_H
#define DIRECTION_H

#include "Coord2.h"


class Rotation {
public:
	enum Rot {Clock, CounterClock, RotsNum };
	Rotation(Rot rot): rot(rot) {}
	Rotation (const Rotation &other): rot(other.rot) {}
	Rotation(): rot(RotsNum) {}
	char getChar() const { return chars[rot]; }
	int getSign() const { return rot == Clock? +1 : -1; }
	
private:
	static const char chars[RotsNum];
	Rot rot;
};


class Direction {
public:
	enum Dir {Right, Down, Left, Up, DirsNum, NoDir};
	Direction(Dir dir): dir(dir) {}
	Direction(const Direction &other): dir(other.dir) {}
	Direction(const Coord2 &from, const Coord2 &to);
	Direction(): dir(DirsNum) {}
	
	bool operator==(const Direction &other) const { return dir == other.dir; }
	bool operator!=(const Direction &other) const { return dir != other.dir; }
	bool isInvalid() const { return dir == DirsNum; }
	bool isNowhere() const { return dir == NoDir; }
	char getChar() const { return chars[dir]; }
	Coord2 offset() const;
	static Coord2 offset(Dir dir);
	int rotationsTo(Direction other, Rotation *rot);
	Direction rotatedOnce(Rotation rot) const;
	Coord2 rightCoordRotated(const Coord2 &rightCoord) const;
	Coord2 rotatedCoordToRight(const Coord2 &rotCoord) const;
	Coord2 offsetOfContourNormal() const;
	Direction opposite() const;
	static Direction fromInt(int dir) { return Direction((Dir)dir); }
	int toInt() const { return dir; }
	bool isVertical() const { return dir == Down || dir == Up; }
	
private:
	static const char chars[DirsNum];
	Dir dir;
};


class Ray {
public:
	Ray(const Coord2 &origin, const Direction &dir): origin(origin), dir(dir) {}
	Ray(const Coord2 &origin, const Coord2 &to): origin(origin), dir(origin, to) {}
	Ray() {}
	void init(const Coord2 &newOrigin, const Direction &newDir) { origin = newOrigin; dir = newDir; }
	bool isVertical() const { return dir.isVertical(); }
	bool intersection(const Ray &other, Coord2 *point, bool *onRay) const;
	int longitCoord() const { return isVertical()? origin.y() : origin.x(); }
	int normalCoord() const { return isVertical()? origin.x() : origin.y(); }
	bool looksAt(const Coord2 &point) const { return dir == Direction(origin, point); }
	
private:
	Coord2 origin;
	Direction dir;
};


#endif // DIRECTION_H
