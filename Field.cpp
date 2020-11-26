#include "Field.h"
#include "Map.h"
#include <queue>
#include <cstring>

Field::Field(const Map *map): map(map) {
	size = map->getSize();
	value = new int *[size.x()];
	for(int i = 0; i < size.x(); i++) {
		value[i] = new int[size.y()]();
	}
}

void Field::floodFrom(const Coord2 &coord) {
	clear();
	std::queue<Coord2> searchQueue;
	std::set<Coord2> handled;
	searchQueue.push(coord);
	handled.insert(coord);
	value[coord.x()][coord.y()] = 0;
	
	while (!searchQueue.empty()) {
		Coord2 coord = searchQueue.front();
		searchQueue.pop();
		
		int nextDist = value[coord.x()][coord.y()] + 1;
		std::vector<Coord2> adjacent = coord.getAdjacent();
		for (Coord2 adj: adjacent)	{
			if (!map->isPassable(adj) || handled.count(adj)) continue;
			value[adj.x()][adj.y()] = nextDist;
			searchQueue.push(adj);
			handled.insert(adj);
		}
	}
	center = coord;
}

void Field::floodFrom(const Coord2 &coord, const CoordsSet *onlyContained) {
	clear();
	std::queue<Coord2> searchQueue;
	std::set<Coord2> handled;
	searchQueue.push(coord);
	handled.insert(coord);
	
	while (!searchQueue.empty()) {
		Coord2 coord = searchQueue.front();
		searchQueue.pop();
		
		int nextDist = value[coord.x()][coord.y()] + 1;
		std::vector<Coord2> adjacent = coord.getAdjacent();
		for (Coord2 adj: adjacent)
		{
			if (!map->isPassable(adj) || handled.count(adj) || !onlyContained->count(adj)) continue;
			value[adj.x()][adj.y()] = nextDist;
			searchQueue.push(adj);
			handled.insert(adj);
		}
	}
	center = coord;
}

void Field::clear() {
	for(int i = 0; i < size.x(); i++) {
		memset(value[i], 0, sizeof(value[i][0]) * size.y());
	}
}

Field::~Field() {
	for(int i = 0; i < size.x(); i++) {
		delete value[i];
	}
	delete value;
}
