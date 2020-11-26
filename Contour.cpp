#include "Contour.h"
#include <algorithm>


bool cmpByY(const Coord2 &a, const Coord2 &b)
{
	return a.y() < b.y();
}


Contour::Contour(const SegmentContour &segCont)
{
	if (segCont.size() < 2) { return; } // TODO: analyze and fix
	resize(segCont.size());
	(*this)[0] = segCont[0].from;
	for (unsigned i = 1; i < size(); i++) {
		(*this)[i] = segCont[i-1].to;
	}
}


Coord2 Contour::getOrigin() const
{
	Coord2 origin(255, 255);
	for (const Coord2 &coord: *this)
	{
		if (coord.x() < origin.x()) origin.x() = coord.x();
		if (coord.y() < origin.y()) origin.y() = coord.y();
	}
	return origin;
}

Coord2 Contour::getTopRight() const
{
	Coord2 result;
	for (const Coord2 &coord: *this)
	{
		if (coord.x() > result.x()) result.x() = coord.x();
		if (coord.y() > result.y()) result.y() = coord.y();
	}
	
	return result + (-1);
}


CoordsSet Contour::innerCoords() const
{
	CoordsSet result;
	if (size() == 0) { return result; }
	Contour sorted = *this;
	std::sort(sorted.begin(), sorted.end(), cmpByY);
	
	int currY = getOrigin().y();
	auto currPoint = sorted.begin();
	auto be = sorted.end();
	std::multiset<int> ael; 
	do
	{
		while(currPoint != be && currPoint->y() == currY)
		{
			
			bool newEdge = true;
			for(auto edgeX = ael.begin(), e = ael.end(); edgeX !=e; ++edgeX)
			{
				if (*edgeX == currPoint->x())
				{
					ael.erase(edgeX);
					newEdge = false;
					break;
				}
			}
			
			if (newEdge)
			{
				ael.insert(currPoint->x());
			}
			
			++currPoint;
		}
		
		for(auto edgeX = ael.begin(), e = ael.end(); edgeX !=e; ++edgeX)
		{
			int from = *edgeX;
			++edgeX;
			int to = *edgeX;
			while (from != to)
			{
				result.insert(Coord2(from, currY));
				from++;
			}
		}
		currY++;
	}
	while(!ael.empty());
	return result;
}


bool Segment::onSameLine(const Segment &other)
{
	Ray ray(from, to);
	Ray othRay(other.from, other.to);
	return ray.isVertical() == othRay.isVertical() && ray.normalCoord() == othRay.normalCoord();
}


void OuterCells::add(const Coord2 &coord, const Segment *seg)
{
	if (size() == 0) {
		push_back(OuterCell(coord, seg));
		return;
	}
	OuterCell &lastCell = back();
	if (coord == lastCell.coord) {
		lastCell.segs.push_back(seg);
	}
	else {
		if (!coord.isAdjacent(lastCell.coord)) { // insert intermediate cell on concave contour corner
			const Segment *lastCellLastSeg = lastCell.segs.back();
			Coord2 interCoord = lastCell.coord + lastCellLastSeg->getDirection().offset();
			push_back(OuterCell(interCoord, lastCellLastSeg));
			back().segs.push_back(seg);
		}
		push_back(OuterCell(coord, seg));
	}
}


SegmentContour::SegmentContour(const Contour &contour)
{
	resize(contour.size());
	for (unsigned i = 1; i < size(); i++) {
		(*this)[i-1] = Segment(contour[i-1], contour[i]);
	}
	(*this)[size() - 1] = Segment(contour[size()-1], contour[0]);
}


SegmentContour::SegmentContour(const OuterCells &outerCells, unsigned from, unsigned to)
{
	for (unsigned i = from; i <= to; i++) {
		const OuterCell &cell = outerCells[i];
		for (const Segment *seg : cell.segs) {
			if (size() == 0 || !(back() == *seg)) { push_back(*seg); }
		}
	}
	
	// close loop
	if (size() < 2) { return; } // TODO: analyze and fix
	if (front().from == back().to) { return; }
	
	if (front().onSameLine(back())) {
		front().from = back().from;
		resize(size() - 1);
		return;
	}
	
	Direction frontDir = front().getDirection();
	Direction invBackDir = back().getDirection().opposite();
	Ray frontRays[3], backRays[3];
	for (int dir = 0, ray = 0; dir < Direction::DirsNum; dir++) {
		if (Direction::fromInt(dir) != frontDir) { frontRays[ray++].init(front().from, Direction::fromInt(dir)); }
	}
	for (int dir = 0, ray = 0; dir < Direction::DirsNum; dir++) {
		if (Direction::fromInt(dir) != invBackDir) { backRays[ray++].init(back().to, Direction::fromInt(dir)); }
	}
	
	bool found = false;
	for (int frontRay = 0; frontRay < 3; frontRay++) {
		for (int backRay = 0; backRay < 3; backRay++) {
			Coord2 point;
			bool onRay;
			bool isect = frontRays[frontRay].intersection(backRays[backRay], &point, &onRay);
			if (isect) {
				if (onRay) {
					if (front().getDirection().isVertical() == back().getDirection().isVertical()) {
						push_back({back().to, front().from});
					}
					else {
						Segment newSeg(back().to, front().from);
						if (newSeg.getDirection().isVertical() == back().getDirection().isVertical()) {
							back().to = front().from;
						}
						else {
							front().from = back().to;
						}
					}
				}
				else {
					Segment newSeg1(back().to, point);
					if (newSeg1.onSameLine(back())) { back().to = point; }
					else { push_back(newSeg1); }
					
					Segment newSeg2(point, front().from);
					if (newSeg2.onSameLine(front())) { front().from = point; }
					else { push_back(newSeg2); }
				}
				found = true;
				break;
			}
		}
		if (found) { break; }
	}
	if (!found) { resize(0); } // TODO: analyze and fix
}


OuterCells SegmentContour::getOuterCells() const
{
	OuterCells result;
	for (const Segment &seg : *this) {
		Direction dir = seg.getDirection();
		Coord2 offset = dir.offset();
		Coord2 normalOffset = dir.offsetOfContourNormal();
		int len = seg.getLength();
		Coord2 segCoord = seg.from;
		for (int i=0; i < len; i++) {
			result.add(segCoord + normalOffset, &seg);
			segCoord += offset;
		}
	}
	return result;
}
