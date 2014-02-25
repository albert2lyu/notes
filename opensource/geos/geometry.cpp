
#include <iostream>
#include <sstream>

#include <cppdb/frontend.h>

// geos
#include <geos/io/WKBReader.h>
#include <geos/io/WKBConstants.h>
#include <geos/geom/PrecisionModel.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/Point.h>

int main(int argc, char* argv[])
{
	try {
		std::string cs("postgresql:@pool_size=10;@blob=bytea;host=127.0.0.1;port=8999;dbname=stdb;user=stadmin;password=stadmin");
		cppdb::session sql(cs);
		{
			std::string cityname;
			std::string st_city_catalog("global_citycode");
			double x = 12318052.18, y = 2231640;
			std::ostringstream ostr;
			ostr.precision(12);

			// select cityname_en from public.global_citycode where ST_Intersects(geo,ST_MakePoint(x,y))
			ostr << "SELECT cityname_en FROM " << st_city_catalog << " WHERE ST_Intersects(geo,ST_MakePoint(?,?))";

			cppdb::result res = sql << ostr.str() << x << y << cppdb::row;
			if(!res.empty()) {
				cityname = res.get<std::string>("cityname_en");
			}
		}
		{
			std::string cityname, citycode("0003");
			std::string wkt;
			std::string st_city_catalog("global_citycode");
			std::ostringstream oss;
			std::ostringstream ostr;
			ostr.precision(12);

			// select cityname_en from public.global_citycode where citycode = ?
			//ostr << "SELECT ST_AsBinary(geo) as geo,cityname_en FROM " << st_city_catalog << " WHERE citycode = ?";
			/*ostr << "SELECT geo, cityname_en FROM " << st_city_catalog << " WHERE citycode = ?";

			cppdb::result res = sql << ostr.str() << citycode << cppdb::row;
			if(!res.empty()) {
				cityname = res.get<std::string>("cityname_en");
				oss = res.get<std::ostringstream>("geo");
			}*/

			ostr << "SELECT \"Geometry\" as geo, ST_AsBinary(\"Geometry\") as wkb,ST_AsText(\"Geometry\") as wkt,\"ID\" FROM beijing_beijing_cfg LIMIT 1 ";
			cppdb::result res = sql << ostr.str() << cppdb::row;
			if(!res.empty()) {
				cityname = res.get<std::string>("\"ID\"");
				//oss = res.get<std::ostringstream>("geo");
				//oss = res.get<std::ostringstream>("wkb");
				res.fetch("wkb", oss);
				wkt = res.get<std::string>("wkt");
			}

			std::cout << "wkt is " << wkt << std::endl;

			std::string s = oss.str();
			std::cout << "geo size is " << s.size() << std::endl;

			{
                // For Linux
				std::istringstream iss(s);
                // For Windows
				//std::istringstream iss(s.substr(1)); // x01...
				geos::geom::PrecisionModel pm;
				geos::geom::GeometryFactory gf;
				geos::io::WKBReader wkbreader;

				typedef std::auto_ptr<geos::geom::Geometry> GeomPtr;
                // For Linux
				GeomPtr gWKB(wkbreader.read(iss));
                // For Windows
				//GeomPtr gWKB(wkbreader.readHEX(iss));
				std::string gtype = gWKB->getGeometryType();
				std::cout << "geometry type is " << gtype << std::endl;

				if (gWKB->getGeometryTypeId() == geos::geom::GEOS_POINT) {
					geos::geom::Point *gPoint = dynamic_cast<geos::geom::Point*>(gWKB.get());
					std::cout.precision(12);
					std::cout << "X is " << gPoint->getX() << ", Y is " << gPoint->getY() << std::endl;
				}
			}
		}
	}
	catch(std::exception const &e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}

