#ifndef MAP_H
#define MAP_H

#include "Coord2.h"
#include "Contour.h"
#include "Cell.h"
#include <functional>


class Map
{
public:
	Map(int width, int height); //Filled with Unknown Cells
	Map(const Map &other);
	~Map();
	
	Size2 getSize() const { return m_size; }
	Cell::State getState(const Coord2 &coord) const { 
		if (coord.isInBoundaries(m_origin, m_origin + m_size + (-1)))
			return getMapCell(coord).state;
		else
			return Cell::Wall;
			
	}
	void setState(const Coord2 &coord, Cell::State newState) { 
		MapCell &cell = getMapCell(coord);
		if (coord.isInBoundaries(m_origin, m_origin + m_size + (-1))) { cell.state = newState; }
	}
	bool isEmpty(const Coord2 &coord) const { return getState(coord) == Cell::Empty; }
	bool isPassable(const Coord2 &coord) const { return Cell::isPassable(getState(coord)); }
	CoordsList getPathEmptyInSet(const Coord2 &src, const CoordsSet &target) const;
	CoordsList getPath(const Coord2 &src, 
		std::function<bool (CoordsList &, Coord2 &)> &found,
		std::function<bool (CoordsList &, Coord2 &)>&canPass,
		std::function<std::vector<Coord2> (CoordsList &)> &getAdjacent) const;
	void removeUnpassable(CoordsSet &coords) const;
	const Contour &getBoundary() const { return m_boundary; }
	
private:
	struct MapCell {
		Cell::State state;
	};
	
	void allocateCells();
	void fill(Contour &border, Cell::State state);
	MapCell &getMapCell(const Coord2 &coord) const { 
		return m_cells[coord.x() - m_origin.x()][coord.y() - m_origin.y()]; 
	}
	
	Size2 m_size;
	Coord2 m_origin;
	MapCell **m_cells;
	MapCell *m_cellsBuf;
	Contour m_boundary;
};


#endif // MAP_H
