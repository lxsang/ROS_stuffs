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

#include "control.h"
#include "tools.h"

using namespace :: mia;

Control::Control( Frame * frame)://, World * target ):
    a_frame(frame),
    a_target(nullptr),
    a_end(false),
    a_pause(false),
    a_autoTarget(true),
    a_mouseControl(false),
    a_bodid(0)
{
}

//Process function :
void Control::process()
{
    SDL_Event event;
    Color white( 255, 255, 255 );
//     float bodTheta(0.f), speed(0.f);
//     
//     if( a_target != nullptr )
//     {
//         bodTheta= a_target->body(a_bodid)->shape.theta;
//         speed= a_target->body(a_bodid)->shape.ptrDynamic->position.length();
//         
//         if ( !a_autoTarget && speed > 5.0f )
//             a_target->body(a_bodid)->shape.ptrDynamic->position= Float2( cos(bodTheta), sin(bodTheta) )*speed;
//     }

    Int2 mouseMove(0, 0);
    while( SDL_PollEvent( &event ) ) {
        a_end= a_end || event.type == SDL_QUIT;
                    
        if ( event.type == SDL_KEYDOWN )
        {
            switch( event.key.keysym.sym )
            {
                case SDLK_PAUSE :
                    a_pause= !a_pause;
                    break;

                default :
                    break;
            }
        }
        
//         if ( event.type == SDL_KEYDOWN && a_target != nullptr && !a_autoTarget )
//         {
//             switch( event.key.keysym.sym )
//             {
//                 case SDLK_z :
//                     a_target->body(a_bodid)->shape.ptrDynamic->position= Float2( cos(bodTheta)*8.f, sin(bodTheta)*8.f );
//                 break;
//                 case SDLK_s :
//                     a_target->body(a_bodid)->shape.ptrDynamic->position= 0.0f;
//                 break;
//                 case SDLK_q :
//                     a_target->body(a_bodid)->shape.ptrDynamic->theta= _2PI;
//                 break;
//                 case SDLK_d :
//                     a_target->body(a_bodid)->shape.ptrDynamic->theta= -_2PI;
//                 break;
//                 
//                 default :
//                     break;
//             }
//         }
//             
//         if ( event.type == SDL_KEYUP && a_target != nullptr && !a_autoTarget )
//         {
// //            float bodTheta= a_target->body(a_bodid)->shape.theta;
//             
//             switch( event.key.keysym.sym )
//             {
//                 case SDLK_q :
//                     a_target->body(a_bodid)->shape.ptrDynamic->theta= 0.f;
//                 break;
//                 case SDLK_d :
//                     a_target->body(a_bodid)->shape.ptrDynamic->theta= 0.f;
//                 break;
//                 
//                 default :
//                     break;
//             }
//         }

        a_mouseControl= a_mouseControl || ( event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT );
        a_mouseControl= a_mouseControl && !(event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT);

        if( event.type == SDL_MOUSEWHEEL ) {
            if( event.wheel.y < 0 )
                a_frame->scaleOut();
            else a_frame->scaleIn();
        }
        else if( event.type == SDL_MOUSEMOTION ) {
            mouseMove.x+= event.motion.xrel;
            mouseMove.y+= event.motion.yrel;
        }
    }

    if( a_mouseControl && mouseMove.x != 0 &&  mouseMove.y != 0 ) {
        mouseMove*= -1;
        a_frame->movePixel( mouseMove );
    }

}

