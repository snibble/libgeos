/**********************************************************************
 * $Id$
 *
 * GEOS - Geometry Engine Open Source
 * http://geos.refractions.net
 *
 * Copyright (C) 2001-2002 Vivid Solutions Inc.
 *
 * This is free software; you can redistribute and/or modify it under
 * the terms of the GNU Lesser General Public Licence as published
 * by the Free Software Foundation. 
 * See the COPYING file for more information.
 *
 **********************************************************************
 * $Log$
 * Revision 1.11  2003/11/07 01:23:42  pramsey
 * Add standard CVS headers licence notices and copyrights to all cpp and h
 * files.
 *
 *
 **********************************************************************/


#include "../headers/geosAlgorithm.h"
#include "stdio.h"

namespace geos {

NonRobustCGAlgorithms::NonRobustCGAlgorithms(){
	li=new RobustLineIntersector();
}

NonRobustCGAlgorithms::~NonRobustCGAlgorithms(){
	delete li;
}

/**
* ring is expected to contain a closing point;
* i.e. ring[0]=ring[length-1]
*/
bool
NonRobustCGAlgorithms::isPointInRing(const Coordinate& p,
		const CoordinateList* ring) const {
	int i,i1;		// point index;i1=i-1 mod n
	double xInt;		// x intersection of e with ray
	int	crossings=0;	// number of edge/ray crossings
	double x1,y1,x2,y2;
	int nPts=ring->getSize();

	/* For each line edge l=(i-1,i),see if it crosses ray from test point in positive x direction. */
	for(i=1;i<nPts;i++){
		i1=i-1;
		const Coordinate& p1=ring->getAt(i);
		const Coordinate& p2=ring->getAt(i1);
		x1=p1.x-p.x;
		y1=p1.y-p.y;
		x2=p2.x-p.x;
		y2=p2.y-p.y;
		if(((y1>0) && (y2<=0)) || ((y2>0)&&(y1<=0))) {
			/* e straddles x axis,so compute intersection. */
			xInt=(x1*y2-x2*y1)/(y2-y1);
			//xsave=xInt;
			/* crosses ray if strictly positive intersection. */
			if(0.0<xInt)
				crossings++;
		}
	}
	/* p is inside if an odd number of crossings. */
	if((crossings%2)==1)
		return true;
	else
		return false;
}



bool NonRobustCGAlgorithms::isOnLine(const Coordinate& p,const CoordinateList* pt) const {
	for(int i=1;i<pt->getSize();i++){
		const Coordinate& p0=pt->getAt(i-1);
		const Coordinate& p1=pt->getAt(i);
		li->computeIntersection(p,p0,p1);
		if(li->hasIntersection())
			return true;
	}
	return false;
}

bool NonRobustCGAlgorithms::isCCW(const CoordinateList* ring) const {
	Coordinate hip,p,prev,next;
	int hii,i;
	int nPts=ring->getSize();

    // check that this is a valid ring - if not, simply return a dummy value
    if (nPts<4) return false;

	// algorithm to check if a Ring is stored in CCW order
	// find highest point
	hip.setCoordinate(ring->getAt(0));
	hii=0;
	for(i=1;i<nPts;i++)	{
		p.setCoordinate(ring->getAt(i));
		if(p.y>hip.y){
			hip.setCoordinate(p);
			hii=i;
		}
	}
	// find points on either side of highest
	int iPrev=hii-1;
	if(iPrev<0) iPrev=nPts-2;
	int iNext=hii+1;
	if(iNext>=nPts) iNext=1;
	prev.setCoordinate(ring->getAt(iPrev));
	next.setCoordinate(ring->getAt(iNext));
	// translate so that hip is at the origin.
	// This will not affect the area calculation,and will avoid
	// finite-accuracy errors(i.e very small vectors with very large coordinates)
	// This also simplifies the discriminant calculation.
	double prev2x=prev.x-hip.x;
	double prev2y=prev.y-hip.y;
	double next2x=next.x-hip.x;
	double next2y=next.y-hip.y;
	// compute cross-product of vectors hip->next and hip->prev
	//(e.g. area of parallelogram they enclose)
	double disc=next2x*prev2y-next2y*prev2x;
	/* If disc is exactly 0,lines are collinear.  There are two possible cases:
	(1)the lines lie along the x axis in opposite directions
	(2)the line lie on top of one another
	(2)should never happen,so we're going to ignore it!
	(Might want to assert this)
	(1)is handled by checking if next is left of prev==>CCW
	*/
	if(disc==0.0){
		// poly is CCW if prev x is right of next x
		return(prev.x>next.x);
	} else {
		// if area is positive,points are ordered CCW
		return(disc>0.0);
	}
}

int
NonRobustCGAlgorithms::computeOrientation(const Coordinate& p1,
		const Coordinate& p2,const Coordinate& q) const {
	double dx1=p2.x-p1.x;
	double dy1=p2.y-p1.y;
	double dx2=q.x-p2.x;
	double dy2=q.y-p2.y;
	double det=dx1*dy2-dx2*dy1;
	if(det>0.0)return 1;
	if(det<0.0)return-1;
	return 0;
}
}

