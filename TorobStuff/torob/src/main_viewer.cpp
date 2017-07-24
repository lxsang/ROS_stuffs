#include <ros/ros.h>
#include <tf/transform_listener.h>

#include "rosvmap.h"
#include "frame.h"
#include "roscontrol.h"

using namespace std;

// global element :
RosVmap * _vmap;
//bool verbose;
bool node_info;

mia::Frame * _frame; // SDL viewer
mia::RosControl * _control; // SDL interaction

// Callback :
void vmap_subscriber(const torob_msgs::VectorMap & vmap){
  if( !_control->pause() )
    _vmap->vmap_subscriber( vmap );
}

void process(const ros::TimerEvent& time){
  _control->process();
  
  if( _control->end() ){ ros::shutdown(); }
  
  _frame->drawVisiMap( _vmap->visimap, mia::Float2(0.f, 0.f), node_info );
  _frame->refresh();
}

void process_only_obstacle(const ros::TimerEvent& time){
  _control->process();
  
  if( _control->end() ){ ros::shutdown(); }
  
  _frame->drawVisiMapObstacle( _vmap->visimap, mia::Float2(0.f, 0.f) );
  _frame->refresh();
}

int main(int argc, char **argv)
{
  // ROS:
  cout << "Initialize torob copy_map" << endl;
  ros::init(argc, argv, "torob_viewer");
  ros::NodeHandle node;
  ros::NodeHandle node_private("~");

  // Configuration : 
  std::string vmap_topic;
  bool only_obstacle;
  
  if( !node_private.getParam("vmap_topic", vmap_topic) ) vmap_topic= "/vmap";
  if( !node_private.getParam("node_info", node_info) ) node_info= true;
  if( !node_private.getParam("only_obstacle", only_obstacle) ) only_obstacle= false;
  
//  if( !node_private.getParam("verbose", verbose) ) verbose= false;

  // Moinag: 
  _vmap= new RosVmap();
  
  // SDL 2:
  cout << "Initialize SDL window" << endl;
  _frame= new mia::Frame("Torob Viewer");
  _frame->scale( 120.f );
  _control= new mia::RosControl(_frame);
  
  // Callback function:
  ros::Timer loopTimer;
  if( only_obstacle )
  {
    cout << "Initialize vewer process oo" << endl;
    loopTimer= node.createTimer( ros::Duration( 0.03 ), process_only_obstacle );
  }
  else
  {
    cout << "Initialize vewer process" << endl;
    loopTimer= node.createTimer( ros::Duration( 0.03 ), process );
  }
  
  // subscribers function:
  ros::Subscriber sub1 = node.subscribe( vmap_topic, 1, vmap_subscriber );

  // publisher function:
  
  // get the hand to ros:
  cout << "Run torob viewer on "  << vmap_topic << endl;
  ros::spin();
  ros::spin();

  // Properly stop the program:
  cout << "Close program" << endl;

  return 0;
}
