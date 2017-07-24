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

#ifndef CONTROL_H
#define CONTROL_H

//#include "world.h"
#include "frame.h"

namespace mia{

class Control
{
protected:
    Frame * a_frame;
//    World * a_target;
    void * a_target;
    bool a_end, a_pause, a_autoTarget;
    bool a_mouseControl;
    int a_bodid;

public:
//    Control( Frame * frame, World * target= nullptr );
    Control( Frame * frame );
    
    // Getter and Setter :
    virtual bool end() const { return a_end; }
    virtual void setEnd(bool set){ a_end= set; }
    virtual bool isPaused() const { return a_pause; }
    virtual void setPause(bool set){ a_pause= set; }
    virtual void setControledBody(int bodid){ a_bodid= bodid; }
    virtual int getControledBody() const { return a_bodid; }
 
    //Process function :
    virtual void process();
};

};
#endif // CONTROL_H
