#include <iostream>

#include "global.h"
#include "agent.h"
#include "environment.h"

using namespace :: mia;
using namespace :: std;

class MyAgent : public Agent
{
public:
    MyAgent():Agent(1, 1){};

    virtual void born(){};
    virtual void act( const InteractStr & sensor, InteractStr & actuator ){};
    virtual void die(){};

    static MyAgent* nursery(int type){ return new MyAgent; };
};

typedef listias_environment< MyAgent, MyAgent::nursery > World;

// World::Agent::nurcery(){
//     Atest* ag= new Atest();
//     return (World::Agent*)ag;  
// }

int main(int argc, char **argv) {
    
    // Global::load( "../../moinag/config.yaml" );
   
    World world;
    MyAgent bob;
    
    cout << "Hello, world!" << endl;
    return 0;
}
