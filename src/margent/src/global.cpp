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

#include "global.h"

#include <iostream>

#include <yaml-cpp/yaml.h>

using namespace std;
using namespace mia;

std::default_random_engine Global::as_random_generator;
YAML::Node Global::config;
bool Global::as_initialized= false;

bool Global :: initialize( const char * path_config )
{
    std::cout << "Initialize global" << std::endl;

    mia::Global::config= YAML::LoadFile( path_config );
    
    as_initialized= true;
    return as_initialized;
}
