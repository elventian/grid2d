#include "Map.h"
#include <queue>
#include <set>
#include <math.h>
#include <cstring>

#define DEBUG_PRINTS 1


Map::Map(int width, int height)
{
	m_size = Size2(width, height);
	allocateCells();
}

Map::Map(const Map &other) {
	m_size = other.m_size;
	m_origin = other.m_origin;
	allocateCells();
	memcpy(m_cellsBuf, other.m_cellsBuf, m_size.x() * m_size.y() * sizeof(MapCell));
	m_boundary = other.m_boundary;
}

Map::~Map() {
	delete[] m_cells;
	delete[] m_cellsBuf;
}

void Map::allocateCells() {
	m_cellsBuf = new MapCell[m_size.x() * m_size.y()](); // filled by zeros
	m_cells =  new MapCell*[m_size.x()];
	for (int x = 0; x < m_size.x(); x++) {
		m_cells[x] = &m_cellsBuf[x * m_size.y()];
	}
}

void Map::fill(Contour &border, Cell::State state) {
	CoordsSet innerCoords = border.innerCoords();
	for (const Coord2 &coord : innerCoords) {
		setState(coord, state);
	}
}

CoordsList Map::getPathEmptyInSet(const Coord2 &src, const CoordsSet &target) const {
	std::function<bool (CoordsList &, Coord2 &)> found = 
		[&target, this] (CoordsList &path, Coord2 &coord) {
			(void) path;
			return isEmpty(coord) && target.count(coord);
		};
	
	std::function<bool (CoordsList &, Coord2 &)> canPass = 
		[this] (CoordsList &path, Coord2 &coord) {
			(void) path;
			Cell::State cell = getState(coord);
			return cell == Cell::Empty;
		};
	
	std::function<std::vector<Coord2> (CoordsList &)> getAdjacent =
		[] (CoordsList &path) {
		return path.back().getAdjacent();
	};
	
	return getPath(src, found, canPass, getAdjacent);
}

CoordsList Map::getPath(const Coord2 &src, const CoordsSet &target) const
{
	std::function<bool (CoordsList &, Coord2 &)> found = 
		[&target, this] (CoordsList &, Coord2 &coord) {
			return target.count(coord);
		};
	
	std::function<bool (CoordsList &, Coord2 &)> canPass = 
		[this] (CoordsList &path, Coord2 &coord) {
			(void) path;
			Cell::State cell = getState(coord);
			return cell == Cell::Empty;
		};
	
	std::function<std::vector<Coord2> (CoordsList &)> getAdjacent =
		[] (CoordsList &path) {
		return path.back().getAdjacent();
	};
	
	return getPath(src, found, canPass, getAdjacent);
}

CoordsList Map::getPath(const Coord2 &src, 
	std::function<bool (CoordsList &, Coord2 &)> &found, 
	std::function<bool (CoordsList &, Coord2 &)> &canPass,
	std::function<std::vector<Coord2> (CoordsList &)> &getAdjacent
) const {
	std::queue<CoordsList> searchQueue;
	std::set<Coord2> handled;
	CoordsList initial;
	initial.push_back(src);
	searchQueue.push(initial);
	handled.insert(src);
	
	while (!searchQueue.empty()) {
		CoordsList path = searchQueue.front();
		searchQueue.pop();
		
		std::vector<Coord2>adjacent = getAdjacent(path);
		for (Coord2 adj: adjacent) {
			if (found(path, adj)) {
				path.pop_front();
				path.push_back(adj);
				return path;
			}
			
			if (!canPass(path, adj) ||
				handled.count(adj)) continue;
			
			CoordsList nextPath(path);
			nextPath.push_back(adj);
			searchQueue.push(nextPath);
			handled.insert(adj);
		}
	}
	
	return CoordsList();
}


void Map::removeUnpassable(CoordsSet &coords) const {
	for (auto it = coords.begin(); it != coords.end();) {
		if (!isPassable(*it)) { it = coords.erase(it); }
		else { ++it; }
	}
}
