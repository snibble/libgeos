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
 * Revision 1.10  2003/11/07 01:23:42  pramsey
 * Add standard CVS headers licence notices and copyrights to all cpp and h
 * files.
 *
 *
 **********************************************************************/


#include "../headers/graph.h"
#include <stdio.h>

namespace geos {

int Quadrant::quadrant(double dx, double dy) {
	char buffer[255];
	if (dx == 0.0 && dy == 0.0) {
		sprintf(buffer,"Cannot compute the quadrant for point (%g, %g)\n",dx,dy);
		throw new IllegalArgumentException(buffer);
	}
	if (dx >= 0) {
		if (dy >= 0)
			return 0;
		else
			return 3;
	} else {
		if (dy >= 0)
			return 1;
		else
		return 2;
	}
}

int Quadrant::quadrant(const Coordinate& p0, const Coordinate& p1) {
	double dx=p1.x-p0.x;
	double dy=p1.y-p0.y;
	if (dx==0.0 && dy==0.0)
		throw new IllegalArgumentException("Cannot compute the quadrant for two identical points " + p0.toString());
	return quadrant(dx, dy);
}

bool Quadrant::isOpposite(int quad1, int quad2){
	if (quad1==quad2) return false;
	int diff=(quad1-quad2+4)%4;
	// if quadrants are not adjacent, they are opposite
	if (diff==2) return true;
	return false;
}

/**
 * Two adjacent quadrants have a unique halfplane in common.
 * Halfplanes are indexed with their right hand quadrant.
 */
int Quadrant::commonHalfPlane(int quad1, int quad2){
	// if quadrants are the same they do not determine a unique common halfplane.
	// Simply return one of the two possibilities
	if (quad1==quad2) return quad1;
	int diff=(quad1-quad2+4)%4;
	// if quadrants are not adjacent, they do not share a common halfplane
	if (diff==2) return -1;
	//
	int min=(quad1<quad2)? quad1:quad2;
	int max=(quad1>quad2)? quad1:quad2;
	// for this one case, the righthand plane is NOT the minimum index;
	if (min==0 && max==3) return 3;
	// in general, the halfplane index is the minimum of the two adjacent quadrants
	return min;
}

bool Quadrant::isInHalfPlane(int quad, int halfPlane){
	if (halfPlane==3) {
		return quad==3 || quad==0;
	}
	return quad==halfPlane || quad==halfPlane+1;
}

bool Quadrant::isNorthern(int quad) {
	return quad==0 || quad==1;
}
}

