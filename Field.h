#ifndef FIELD_H
#define FIELD_H

#include "Coord2.h"
class Map;

class Field {
public:
	Field(const Map *map);
	void floodFrom(const Coord2 &coord);
	void floodFrom(const Coord2 &coord, const CoordsSet *onlyContained);
	int getValue(const Coord2 &coord) const { return value[coord.x()][coord.y()]; }
	void setValue(const Coord2 &coord, int newValue) { value[coord.x()][coord.y()] = newValue; }
	void clear();
	~Field();
	bool isEnabled() const { return enabled; }
	void enable(bool setEnabled = true) { enabled = setEnabled; }
	Coord2 getCenter() const { return center; }
private:
	const Map *map;
	int **value;
	Size2 size;
	bool enabled;
	Coord2 center;
};

#endif // FIELD_H
