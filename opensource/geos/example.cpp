/**********************************************************************
 *
 * GEOS - Geometry Engine Open Source
 * http://geos.osgeo.org
 *
 * Copyright (C) 2001-2002 Vivid Solutions Inc.
 * Copyright (C) 2005 Refractions Research Inc.
 *
 * This is free software; you can redistribute and/or modify it under
 * the terms of the GNU Lesser General Public Licence as published
 * by the Free Software Foundation. 
 * See the COPYING file for more information.
 *
 *********************************************************************
 *
 * This file should document by example usage of the GEOS library.
 * It could actually be a live discuss-by-example board for
 * architectural design choices.
 *
 * 			--strk;
 * 
 * DEBUGGING TIPS:
 *  use -D__USE_MALLOC at compile time for gcc 2.91, 2.95, 3.0 and 3.1
 *  and GLIBCXX_FORCE_NEW or GLIBCPP_FORCE_NEW at run time with gcc 3.2.2+
 *  to force libstdc++ avoid caching memory. This should remove some
 *  obscure reports from memory checkers like valgrind.
 *
 **********************************************************************/

#include <geos/geom/PrecisionModel.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/Point.h>
#include <geos/geom/LinearRing.h>
#include <geos/geom/LineString.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/GeometryCollection.h>
#include <geos/geom/Coordinate.h>
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/CoordinateArraySequence.h>
#include <geos/geom/IntersectionMatrix.h>
#include <geos/io/WKTReader.h>
#include <geos/io/WKBReader.h>
#include <geos/io/WKBWriter.h>
#include <geos/io/WKTWriter.h>
#include <geos/util/GeometricShapeFactory.h>
#include <geos/geom/util/SineStarFactory.h>
#include <geos/util/GEOSException.h>
#include <geos/util/IllegalArgumentException.h>
#include <geos/opLinemerge.h>
#include <geos/opPolygonize.h>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cstdlib> // exit()

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

// Set to 0 to skip section
#define GEOMETRIC_SHAPES 1
#define RELATIONAL_OPERATORS 1
#define COMBINATIONS 1
#define UNARY_OPERATIONS 1
#define LINEMERGE 1
#define POLYGONIZE 1


using namespace std;
using namespace geos;
using namespace geos::geom;
using namespace geos::operation::polygonize;
using namespace geos::operation::linemerge;
using geos::util::GEOSException;
using geos::util::IllegalArgumentException;


typedef std::auto_ptr<geos::geom::Geometry> GeomAutoPtr;

// Prototypes
void wkt_print_geoms(vector<Geometry *> *geoms);


// This object will be used to construct our geometries.
// It might be bypassed by directly call geometry constructors,
// but that would be boring because you'd need to specify
// a PrecisionModel and a SRID everytime: those infos are
// cached inside a GeometryFactory object.
GeometryFactory *global_factory;

//#define DEBUG_STREAM_STATE 1


// This function will print given geometries in WKT
// format to stdout. As a side-effect, will test WKB
// output and input, using the WKBtest function.
void
wkt_print_geoms(vector<Geometry *> *geoms)
{
	//WKBtest(geoms); // test WKB parser

	// WKT-print given geometries
	io::WKTWriter *wkt = new io::WKTWriter();
	for (unsigned int i=0; i<geoms->size(); i++) {
		const Geometry *g = (*geoms)[i];
		string tmp=wkt->write(g);
		cout<<"["<<i<<"] (WKT) "<<tmp<<endl;
	}
	delete wkt;
}

// This is the simpler geometry you can get: a point.
Point *
create_point(double x, double y)
{
	Coordinate c(x, y);
	Point *p = global_factory->createPoint(c);
	return p;
}

// This function will create a LinearString
// geometry with the shape of the letter U
// having top-left corner at given coordinates
// and 'side' height and width 
LineString *
create_ushaped_linestring(double xoffset, double yoffset, double side)
{
	// We will use a coordinate list to build the linestring
	CoordinateSequence *cl = new CoordinateArraySequence();

	cl->add(Coordinate(xoffset, yoffset));
	cl->add(Coordinate(xoffset, yoffset+side));
	cl->add(Coordinate(xoffset+side, yoffset+side));
	cl->add(Coordinate(xoffset+side, yoffset));

	// Now that we have a CoordinateSequence we can create 
	// the linestring.
	// The newly created LineString will take ownership
	// of the CoordinateSequence.
	LineString *ls = global_factory->createLineString(cl);

	// This is what you do if you want the new LineString
	// to make a copy of your CoordinateSequence:
	// LineString *ls = global_factory->createLineString(*cl);
	
	return ls; // our LineString
}

// This function will create a LinearRing
// geometry rapresenting a square with the given origin 
// and side 
LinearRing *
create_square_linearring(double xoffset, double yoffset, double side)
{
	// We will use a coordinate list to build the linearring
	CoordinateSequence *cl = new CoordinateArraySequence();

	cl->add(Coordinate(xoffset, yoffset));
	cl->add(Coordinate(xoffset, yoffset+side));
	cl->add(Coordinate(xoffset+side, yoffset+side));
	cl->add(Coordinate(xoffset+side, yoffset));
	cl->add(Coordinate(xoffset, yoffset));

	// Now that we have a CoordinateSequence we can create 
	// the linearring.
	// The newly created LinearRing will take ownership
	// of the CoordinateSequence.
	LinearRing *lr = global_factory->createLinearRing(cl);

	// This is what you do if you want the new LinearRing
	// to make a copy of your CoordinateSequence:
	// LinearRing *lr = global_factory->createLinearRing(*cl);
	
	return lr; // our LinearRing
}

// This function will create a Polygon
// geometry rapresenting a square with the given origin 
// and side and with a central hole 1/3 sided.
Polygon *
create_square_polygon(double xoffset, double yoffset, double side)
{
	// We need a LinearRing for the polygon shell 
	LinearRing *outer = create_square_linearring(xoffset,yoffset,side);

	// And another for the hole 
	LinearRing *inner = create_square_linearring(xoffset+(side/3),
			yoffset+(side/3),(side/3));
	
	// If we need to specify any hole, we do it using
	// a vector of Geometry pointers (I don't know why
	// not LinearRings)
	vector<Geometry *> *holes = new vector<Geometry *>;

	// We add the newly created geometry to the vector
	// of holes.
	holes->push_back(inner);

	// And finally we call the polygon constructor.
	// Both the outer LinearRing and the vector of holes
	// will be referenced by the resulting Polygon object,
	// thus we CANNOT delete them, neither the holes, nor
	// the vector containing their pointers, nor the outer
	// LinearRing. Everything will be deleted at Polygon
	// deletion time (this is inconsistent with LinearRing
	// behaviour... what should we do?).
	Polygon *poly = global_factory->createPolygon(outer, holes);

	return poly;
}

//
// This function will create a GeometryCollection
// containing copies of all Geometries in given vector.
//
GeometryCollection *
create_simple_collection(vector<Geometry *> *geoms)
{
	return global_factory->createGeometryCollection(*geoms);
	// if you wanted to transfer ownership of vector end
	// its elements you should have call:
	// return global_factory->createGeometryCollection(geoms);
}

//
// This function uses GeometricShapeFactory to render
// a circle having given center and radius
//
Polygon *
create_circle(double centerX, double centerY, double radius)
{
	geos::util::GeometricShapeFactory shapefactory(global_factory);
	shapefactory.setCentre(Coordinate(centerX, centerY));
	shapefactory.setSize(radius);
	// same as:
	//	shapefactory.setHeight(radius);
	//	shapefactory.setWidth(radius);
	return shapefactory.createCircle();
}

//
// This function uses GeometricShapeFactory to render
// an ellipse having given center and axis size
//
Polygon *
create_ellipse(double centerX, double centerY, double width, double height)
{
	geos::util::GeometricShapeFactory shapefactory(global_factory);
	shapefactory.setCentre(Coordinate(centerX, centerY));
	shapefactory.setHeight(height);
	shapefactory.setWidth(width);
	return shapefactory.createCircle();
}

//
// This function uses GeometricShapeFactory to render
// a rectangle having lower-left corner at given coordinates
// and given sizes.
//
Polygon *
create_rectangle(double llX, double llY, double width, double height)
{
	geos::util::GeometricShapeFactory shapefactory(global_factory);
	shapefactory.setBase(Coordinate(llX, llY));
	shapefactory.setHeight(height);
	shapefactory.setWidth(width);
	shapefactory.setNumPoints(4); // we don't need more then 4 points for a rectangle...
	// can use setSize for a square
	return shapefactory.createRectangle();
}

//
// This function uses GeometricShapeFactory to render
// an arc having lower-left corner at given coordinates,
// given sizes and given angles. 
//
LineString *
create_arc(double llX, double llY, double width, double height, double startang, double endang)
{
	geos::util::GeometricShapeFactory shapefactory(global_factory);
	shapefactory.setBase(Coordinate(llX, llY));
	shapefactory.setHeight(height);
	shapefactory.setWidth(width);
	// shapefactory.setNumPoints(100); // the default (100 pts)
	// can use setSize for a square
	return shapefactory.createArc(startang, endang);
}

auto_ptr<Polygon>
create_sinestar(double cx, double cy, double size, int nArms, double armLenRat)
{
	geos::geom::util::SineStarFactory fact(global_factory);
	fact.setCentre(Coordinate(cx, cy));
	fact.setSize(size);
	fact.setNumPoints(nArms*5);
  fact.setArmLengthRatio(armLenRat);
	fact.setNumArms(nArms);
	return fact.createSineStar();
}

void polygonDemo() 
{
	// Define a precision model using 0,0 as the reference origin
	// and 2.0 as coordinates scale.
	PrecisionModel *pm = new PrecisionModel(2.0);

	// Initialize global factory with defined PrecisionModel
	// and a SRID of -1 (undefined).
	global_factory = new GeometryFactory(pm);

	// We do not need PrecisionMode object anymore, it has
	// been copied to global_factory private storage
	delete pm;

    geos::io::WKTReader wktReader(global_factory);     

    /*
    std::string str = "POLYGON ((112.3903710739794900  22.3851129617344210, 114.1261324827294900 22.3851129617344210,  114.1261324827294900 23.5479374992368590, 112.3903710739794900  23.5479374992368590, 112.3903710739794900 22.3851129617344210)) ";
    geos::geom::Geometry* pgeom0 = wktReader.read( str );
    str = "POLYGON ((114.0876590000000000 22.5249449999999990,  114.1853440000000000 22.5531790000000000, 114.1704040000000000  22.5438380000000010, 114.1423630000000000 22.5351350000000000,  114.0874290000000000 22.5245199999999990, 114.0892680000000000  22.5255820000000000, 114.0936350000000100 22.5266430000000000,  114.0876590000000000 22.5249449999999990))";
    geos::geom::Geometry* pgeom1 = wktReader.read( str );
    geos::geom::Geometry* pint = pgeom0->intersection(pgeom1);
    */

    std::string str = "POLYGON ((0 0, 5 0, 5 5, 0 5)) ";
    geos::geom::Polygon* pgeom0 = dynamic_cast<geos::geom::Polygon*>(wktReader.read( str ));
    str = "POLYGON ((1 1, 2 1, 6 6, 1 2))";
    geos::geom::Polygon* pgeom1 = dynamic_cast<geos::geom::Polygon*>(wktReader.read( str ));
    geos::geom::Geometry* pint = pgeom0->intersection(pgeom1);

    
    GeomAutoPtr gRealRes(pint);

    gRealRes->normalize();

	io::WKTWriter *wkt = new io::WKTWriter();
    string tmp=wkt->write(gRealRes.get());
    cout<<" (WKT Intersection) "<<tmp<<endl;

    Geometry* geoa = wktReader.read("POINT(116.404412 39.915267)");
    Geometry* geob = wktReader.read("POINT(116.360719 39.949791)");
    
    printf("Distance: %lf\n", geoa->distance(geob));
	wkt = new io::WKTWriter();
    tmp=wkt->write(pint);
    cout<<" (WKT) "<<tmp<<endl;
	delete wkt;
}

void polygonIntersection() {

	// Define a precision model using 0,0 as the reference origin
	// and 2.0 as coordinates scale.
	PrecisionModel *pm = new PrecisionModel(2.0);

	// Initialize global factory with defined PrecisionModel
	// and a SRID of -1 (undefined).
	global_factory = new GeometryFactory(pm);

	// We do not need PrecisionMode object anymore, it has
	// been copied to global_factory private storage
	delete pm;

    Polygon *pol1 = create_square_polygon(0,200,300);
    Polygon *pol2 = create_square_polygon(0,250,300);

    geos::geom::Geometry *pint = pol1->intersection(pol2);

	io::WKTWriter *wkt = new io::WKTWriter();
    string tmp=wkt->write(pint);
    cout<<" (WKT Intersection) "<<tmp<<endl;

    delete pint;
    delete wkt;
    delete pol1;
    delete pol2;
}


void coordinateSeq() {
    CoordinateSequence *cl = new CoordinateArraySequence();
    cl->add(Coordinate(100,100));
    cl->add(Coordinate(100,200));
    cl->add(Coordinate(200,200));
    cl->add(Coordinate(200,100));
    cl->add(Coordinate(180,180));
    //cl->add(Coordinate(100,100));
    cl->add(geos::geom::Coordinate(150, 150));
    cl->add(geos::geom::Coordinate(190, 190));
    cl->add(geos::geom::Coordinate(150, 250));
    cl->add(geos::geom::Coordinate(250, 250));
    cl->add(geos::geom::Coordinate(250, 150));
    //cl->add(geos::geom::Coordinate(150, 150));
 
    LinearRing *lr = global_factory->createLinearRing(cl);
    geos::geom::Polygon *poly=NULL;
    poly = global_factory->createPolygon(lr,NULL);     

    CoordinateSequence *cr = new CoordinateArraySequence();
    cr->add(geos::geom::Coordinate(120, 120));
    cr->add(geos::geom::Coordinate(120, 170));
    cr->add(geos::geom::Coordinate(170, 170));
    cr->add(geos::geom::Coordinate(170, 120));
    //cr->add(geos::geom::Coordinate(120, 120));

    LinearRing *li = global_factory->createLinearRing(cr);
    geos::geom::Polygon *poly1=global_factory->createPolygon(li,NULL);

    geos::geom::Geometry *pint = poly1->intersection(poly);
	io::WKTWriter *wkt = new io::WKTWriter();
    string tmp=wkt->write(pint);
    cout<<" (WKT coordinateSeq Intersection) "<<tmp<<endl;
}

void coordinateSeq2() {
    CoordinateSequence *cl = new CoordinateArraySequence();
    cl->add(Coordinate(100,100));
    cl->add(Coordinate(100,200));
    cl->add(Coordinate(200,200));
    cl->add(Coordinate(200,100));
    cl->add(Coordinate(180,180));
    cl->add(Coordinate(100,100));
    LinearRing *lr = global_factory->createLinearRing(cl);
    geos::geom::Polygon *poly=NULL;
    poly = global_factory->createPolygon(lr,NULL);     

    CoordinateSequence *cr = new CoordinateArraySequence();
    cr->add(geos::geom::Coordinate(150, 150));
    cr->add(geos::geom::Coordinate(190, 190));
    cr->add(geos::geom::Coordinate(150, 250));
    cr->add(geos::geom::Coordinate(250, 250));
    cr->add(geos::geom::Coordinate(250, 150));
    cr->add(geos::geom::Coordinate(150, 150));

    //geos::geom::Polygon *poly1 = create_circle(150, 150, 10);

    LinearRing *li = global_factory->createLinearRing(cr);
    geos::geom::Polygon *poly1=global_factory->createPolygon(li,NULL);

    geos::geom::Geometry *pint = poly1->intersection(poly);
	io::WKTWriter *wkt = new io::WKTWriter();
    string tmp=wkt->write(pint);
    cout<<" (WKT coordinateSeq Intersection) "<<tmp<<endl;

    printf("getNumPoints: %lu\n", pint->getNumPoints());

}



int
main()
{
	cout<<"GEOS "<<geosversion()<<" ported from JTS "<<jtsport()<<endl;
	try
	{
		//do_all();
        //polygonDemo();
        polygonIntersection();
        //coordinateSeq();
        coordinateSeq2();
	}
	// All exception thrown by GEOS are subclasses of this
	// one, so this is a catch-all 
	catch (const GEOSException& exc)
	{
		cerr <<"GEOS Exception: "<<exc.what()<<"\n";
		exit(1);
	}
	catch (const exception &e)
	{
		cerr <<"Standard exception thrown: "<<e.what()<<endl;
		exit(1);
	}
	// and this is a catch-all non standard ;)
	catch (...)
	{
		cerr <<"unknown exception trown!\n";
		exit(1);
	}

	// Unload is no more necessary
	//io::Unload::Release();

	exit(0);
}
