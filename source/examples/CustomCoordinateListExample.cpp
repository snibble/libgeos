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
 * Revision 1.5  2003/11/07 01:23:42  pramsey
 * Add standard CVS headers licence notices and copyrights to all cpp and h
 * files.
 *
 *
 **********************************************************************/


#include <iostream>

#include "CustomCoordinateListExample.h"
#include "../headers/io.h"
#include "../headers/util.h"
#include "../headers/geom.h"
#include "../headers/geosAlgorithm.h"

using namespace std;
using namespace geos;

int main(int argc, char** argv) {
	try {
	cout << "Start:" << endl << endl;

	//CustomPointCoordinateList is a sample implementation of a user-defined
	//wrapper around their internal storage format (array of point_3d struct {3 x double})

	point_3d p1={11,11,DoubleNotANumber};
	point_3d p2={140,200,DoubleNotANumber};
	point_3d p3={240,200,DoubleNotANumber};
	point_3d p4={55,55,DoubleNotANumber};
	point_3d p5={140,120,DoubleNotANumber};
	
	//Array of point_3d (internal storage format)
	point_3d points[5];
	points[0]=p1;
	points[1]=p2;
	points[2]=p3;
	points[3]=p4;
	points[4]=p5;

	//Creating CoordinateList
	CoordinateList *cl=new CustomPointCoordinateList(points,5);

	cout << endl << "CoordinateList cl: " << cl->toString() << endl;
	//Changing point
	//Points can be set using Coordinates
	cl->setAt(*(new Coordinate(240,120)),3);
	//or using native CustomPointCoordinateList format
	point_3d pn={140,120,DoubleNotANumber};
	((CustomPointCoordinateList*) cl)->setAt(pn,0);
	cout << "CoordinateList cl: " << cl->toString() << endl;

	//Since the underlying storage format is a fixed size array
	//points can't be added or deleted
	//cl->add(*(new Coordinate(240,120)),4); //Would cause exception
	//cl->deleteAt(2); //Would cause exception

	//To do the operations:
	//First we need to create a GeometryFactory
	GeometryFactory *gf=new GeometryFactory(new PrecisionModel(),0);
	//Now we can create a Geometry
	Geometry *geom=gf->createPolygon(gf->createLinearRing(cl),NULL);
	cout << endl << "Geometry:" << endl << geom->toString() << endl;
	//And see if the geometry is valid
	cout << "Geometry is valid? " << (geom->isValid()?"true":"false") << endl;

	cout << "End" << endl;
	} catch (GEOSException *ge) {
		cout << ge->toString() << endl;

	}

	return 0;
}

