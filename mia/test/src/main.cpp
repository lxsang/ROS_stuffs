/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (C) 2015  Guillaume L. <guillaume.lozenguez@mines-douai.fr>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <iostream>
#include <fstream>

#include "frame.h"
#include "visimap.h"

#include "control.h"
#include "tools.h"

#include "impact.h"

#include <boost/geometry/io/wkt/wkt.hpp>

using namespace :: mia;
using namespace :: std;

int main(int, char**) {

    OGMap ogmap;
    VisiMap visimap;
    Frame frame("Test Program");
    Control control( &frame );

    if ( !frame.isInitialized() ){
        std::cout << "not cool !!!" <<  std::endl;
        return -1;
    }
    
    // Camera initialization :
    frame.scale( 50.f );
    frame.moveTo( Float2(0.0f, 0.0f) );

    bool stepByStep= true; // Global::config["loop_paused"].as<bool>();
    control.setPause(true);

    // Map initialization :
    ogmap.load("../map/test_map.pgm", Float2(-23.4f, -17.0f), 0.05f );
    ogmap.reduce(2);
    ogmap.clear(512);
        
    visimap.initialize( ogmap );
    cout << "Visimap size: <" << visimap.verticesSize() << ", " << visimap.edgesSize() << ">" << endl;

    while( !control.end() )
    {
        control.process();

        if( !control.isPaused() ){
            cout << "Increase epsilon..." << endl;
	    visimap.setEpsilon( visimap.getEpsilon()*2.f );
	    visimap.obstacle_regression();
	    
            cout << "Visimap size: <" << visimap.verticesSize() << ", " << visimap.edgesSize() << ">" << endl;

            if( stepByStep )
                control.setPause(true);
        }

	frame.drawGridMap( ogmap, mia::Float2(0.f, 0.f), true );
	frame.drawVisiMap( visimap, mia::Float2(0.f, 0.f), false );
	frame.refresh();

        SDL_Delay(10);
    }

    ogmap.save("../map/map.pgm");

    ofstream osfile;
    osfile.open( "../map/map.svg", std::ofstream::out );
    visimap.toSVG( osfile );
    osfile.close();

    std::cout << "cool !!!" <<  std::endl;

    return 0;
}
