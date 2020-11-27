#include "Direction.h"


const char Direction::chars[Direction::DirsNum] = { 'D', 'S', 'A', 'W' };

const char Rotation::chars[Rotation::RotsNum] = { 'E', 'Q' };


Coord2 Direction::offset() const
{
	return offset(dir);
}

Coord2 Direction::offset(Direction::Dir dir)
{
	switch (dir) {
		case Right: return Coord2(1, 0);
		case Left:  return Coord2(-1, 0);
		case Up:    return Coord2(0, -1);
		case Down:  return Coord2(0, 1);
		default: {}
	}
	return Coord2();
}


int Direction::rotationsTo(Direction other, Rotation *rot)
{
	switch (other.dir - dir) {
		case 1: case -3: { *rot = Rotation::Clock; return 1; }
		case -1: case 3: { *rot = Rotation::CounterClock; return 1; }
		case 2: { *rot = Rotation::Clock; return 2; }
	}
	*rot = Rotation::Clock;
	return 0;
}


Direction Direction::rotatedOnce(Rotation rot) const
{
	int newDir = dir + rot.getSign();
	if (newDir < 0) { newDir += DirsNum; }
	else if (newDir >= DirsNum) { newDir -= DirsNum; }
	return Direction((Dir)newDir);
}


Coord2 Direction::rightCoordRotated(const Coord2 &rightCoord) const
{
	switch (dir) {
		case Right: { return rightCoord; }
		case Left: { return Coord2(rightCoord.x() * -1, rightCoord.y() * -1); }
		case Up:   { return Coord2(rightCoord.y() * -1, rightCoord.x()); }
		case Down: { return Coord2(rightCoord.y(),      rightCoord.x() * -1); }
		default: {}
	}
	return Coord2();
}


Coord2 Direction::rotatedCoordToRight(const Coord2 &rotCoord) const
{
	switch (dir) {
		case Right: { return rotCoord; }
		case Left: { return Coord2(rotCoord.x() * -1, rotCoord.y() * -1); }
		case Up:   { return Coord2(rotCoord.y(),      rotCoord.x() * -1); }
		case Down: { return Coord2(rotCoord.y() * -1, rotCoord.x()); }
		default: {}
	}
	return Coord2();
}


Coord2 Direction::offsetOfContourNormal() const
{
	switch (dir) {
		case Right: return Coord2(0, 0);
		case Left:  return Coord2(-1, -1);
		case Up:    return Coord2(-1, 0);
		case Down:  return Coord2(0, -1);
		default: {}
	}
	return Coord2();
}


Direction Direction::opposite() const
{
	switch (dir) {
		case Right: return Direction(Left);
		case Left:  return Direction(Right);
		case Up:    return Direction(Down);
		case Down:  return Direction(Up);
		case NoDir: return Direction(NoDir);
		default: {}
	}
	return Direction(DirsNum);
}


Direction::Direction(const Coord2 &from, const Coord2 &to)
{
	Coord2 diff = to - from;
	if (diff.x() != 0 && diff.y() != 0) { dir = DirsNum; return; }
	if      (diff.x() > 0) { dir = Right; return; }
	else if (diff.x() < 0) { dir = Left; return; }
	else if (diff.y() > 0) { dir = Down; return; }
	else if (diff.y() < 0) { dir = Up; return; }
	dir = NoDir;
}


bool Ray::intersection(const Ray &other, Coord2 *point, bool *onRay) const
{
	if (isVertical() == other.isVertical()) {
		if (normalCoord() != other.normalCoord()) { return false; }
		*onRay = true;
		return true;
	}
	Coord2 inter = isVertical()? Coord2(origin.x(), other.origin.y())
		: Coord2(other.origin.x(), origin.y());
	if (!looksAt(inter) || !other.looksAt(inter)) { return false; }
	*point = inter;
	*onRay = false;
	return true;
}
