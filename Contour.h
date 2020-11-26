#ifndef CONTOUR_H
#define CONTOUR_H

#include "Coord2.h"
#include "Direction.h"
#include <map>
class SegmentContour;


class Contour : public CoordsVector {

public:
	Contour() {}
	Contour(const CoordsVector &vec): CoordsVector(vec) {}
	Contour(const SegmentContour& segCont);
	Coord2 getOrigin() const;
	Coord2 getTopRight() const;
	CoordsSet innerCoords() const;
};


struct Segment {
	Segment(const Coord2 &from, const Coord2 &to): from(from), to(to) {}
	Segment() {}
	Direction getDirection() const { return Direction(from, to); }
	int getLength() const { return from.manhDist(to); }
	bool operator==(const Segment &other) const { return other.from == from && other.to == to; }
	bool onSameLine(const Segment &other);
	
	Coord2 from, to;
};


struct OuterCell {
	OuterCell(const Coord2 &coord, const Segment *seg): coord(coord) { segs.push_back(seg); }
	Coord2 coord;
	std::vector <const Segment *> segs;
};


// May contain not unique coords, but never two identical in a row
class OuterCells : public std::vector<OuterCell> {
public:
	void add(const Coord2 &coord, const Segment *seg);
	
private:
	const Segment *lastSegment;
};


class SegmentContour : public std::vector<Segment> {

public:
	SegmentContour(const Contour &contour);
	SegmentContour(const OuterCells &outerCells, unsigned from, unsigned to);
	OuterCells getOuterCells() const;
	
private:
	
};


#endif // CONTOUR_H
