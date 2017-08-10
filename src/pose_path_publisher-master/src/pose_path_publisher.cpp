/*
 * Copyright (c) 2015, Khelifa Baizid <Khelifa.Baizid@mines-douai.fr and Zhi Yan zhi.yan@mines-douai.fr>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the University of California, Berkeley nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/PoseStamped.h>
# include <nav_msgs/Path.h>
# include <nav_msgs/Odometry.h>
//our path vector
nav_msgs::Path mPathToShow;
int s=0;
void convert_pose_to_path(geometry_msgs::PoseStamped message, std::string _target_f)
{
  mPathToShow.header.frame_id = "/"+_target_f;//"/odom";//std::cout<<"/"+_target_f<<std::endl;
  mPathToShow.poses.push_back(message);
  s++;
}

bool getTransform( const tf::Transformer &lst,
                   const std::string &trg, const std::string &src,
                   geometry_msgs::PoseStamped &ps )
{
    bool tf_ok(true);
    tf::StampedTransform transform;

    try{
      lst.lookupTransform( trg, src, ros::Time(0), transform);
    }
    catch (tf::TransformException ex){
    //       ROS_ERROR("%s",ex.what());
    //       ros::Duration(1.0).sleep();
      tf_ok= false;
    }

    if( tf_ok )
    {
        ps.header.stamp= transform.stamp_;
        ps.pose.position.x= transform.getOrigin().getX();
        ps.pose.position.y= transform.getOrigin().getY();
        ps.pose.position.z= transform.getOrigin().getZ();

        ps.pose.orientation.x= transform.getRotation().getX();
        ps.pose.orientation.y= transform.getRotation().getY();
        ps.pose.orientation.z= transform.getRotation().getZ();
        ps.pose.orientation.w= transform.getRotation().getW();

        return true;
    }
    return false;
}

/*void publish_pose( const geometry_msgs::PoseStamped &ps )
{
    geometry_msgs::PoseWithCovarianceStamped pwcs;

    pwcs.header.frame_id= ps.header.frame_id;
    pwcs.header.stamp= ps.header.stamp;

    pwcs.pose.pose.position.x= ps.pose.position.x;
    pwcs.pose.pose.position.y= ps.pose.position.y;
    pwcs.pose.pose.position.z= ps.pose.position.z;

    pwcs.pose.pose.orientation.x= ps.pose.orientation.x;
    pwcs.pose.pose.orientation.y= ps.pose.orientation.y;
    pwcs.pose.pose.orientation.z= ps.pose.orientation.z;
    pwcs.pose.pose.orientation.w= ps.pose.orientation.w;

    _pose_publisher.publish( pwcs );
}*/
int main(int argc, char **argv) {
  ros::init(argc, argv, "pose_path_publisher");
  ros::NodeHandle nh;
  ros::NodeHandle private_nh("~");
  
  double publish_frequency;
  std::string map_frame, base_frame;
  ros::Publisher pose_publisher;
  ros::Publisher odom_publisher;
  /* declar path publisher - only once */
  ros::Publisher path_publisher;
  /* we get the target frame from and topic tobe published from launch file */
  std::string _target_frm, _pose_tpc, _path_tpc;
  
  /*do we wana publish pose and path or both*/
  bool pubpose=true;bool pubpath=true;
  private_nh.getParam("target_frame_name", _target_frm);
  private_nh.getParam("pose_topic_name",   _pose_tpc);
  private_nh.getParam("path_topic_name",   _path_tpc);
  if(_pose_tpc=="")
      pubpose=false;
  else if(  _path_tpc=="")
      pubpath=false;
 std::cout<<_target_frm<<std::endl;
  /*we chose the frame*/
  private_nh.param<double>("publish_frequency_", publish_frequency);
  if(_target_frm=="map")private_nh.param<std::string>("map_frame", map_frame, _target_frm);
  else private_nh.param<std::string>("map_frame", map_frame, _target_frm);
  private_nh.param<std::string>("base_frame", base_frame, "base_link");
  
  pose_publisher = nh.advertise<geometry_msgs::PoseStamped>(_pose_tpc, 1);
  odom_publisher = nh.advertise<nav_msgs::Odometry>("/odomgmapping", 1);
  
  tf::TransformListener listener;
  std::string tf_prefix = tf::getPrefixParam(private_nh);
  
  /* then register publisher - only once */
  path_publisher = nh.advertise<nav_msgs::Path>(_path_tpc, 1);

  /* for screen info.*/
  if(!pubpose)
      std::cout<<"generating "+ _path_tpc + " in "+_target_frm +" frame"<<std::endl;
  else if(!pubpath)
      std::cout<<"generating "+ _pose_tpc + " in "+_target_frm +" frame"<<std::endl;
  else
      std::cout<<"generating "+ _pose_tpc +" and "+ _path_tpc + " in "+_target_frm +" frame"<<std::endl;

  //std::cout<<"::::::::::::::::::::::::::::::::::::"<<publish_frequency<<std::endl;

  geometry_msgs::PoseStamped pose1, pose2;
  nav_msgs::Odometry odm;
  ros::Rate rate(100);
  bool tf1(false), tf2(false);
  /*while(nh.ok()) {
    tf::StampedTransform transform;
    bool tf_ok = true;
    try {
      listener.lookupTransform(map_frame, base_frame, ros::Time(0), transform);
    } catch(tf::TransformException ex) {
      //ROS_ERROR("-------> %s", ex.what());
      tf_ok = false;
    }
    //std::cout<<map_frame<<"  ::  "<<base_frame<<std::endl;
    if(tf_ok) {
//std::cout<<map_frame<<"  working  "<<base_frame<<std::endl;
      geometry_msgs::PoseStamped pose_stamped;
      pose_stamped.header.stamp = ros::Time::now();
      pose_stamped.header.frame_id = tf_prefix+"/"+map_frame;
      
      pose_stamped.pose.position.x = transform.getOrigin().getX();
      pose_stamped.pose.position.y = transform.getOrigin().getY();
      pose_stamped.pose.position.z = transform.getOrigin().getZ();
      
      pose_stamped.pose.orientation.x = transform.getRotation().getX();
      pose_stamped.pose.orientation.y = transform.getRotation().getY();
      pose_stamped.pose.orientation.z = transform.getRotation().getZ();
      pose_stamped.pose.orientation.w = transform.getRotation().getW();
      //std::cout<<"generating "<<std::endl;
      if(pubpose)
        pose_publisher.publish(pose_stamped);
      
      if(pubpath){
        //convert to path
        convert_pose_to_path(pose_stamped,_target_frm);
        //we publish the path
        path_publisher.publish(mPathToShow);
      }
    }
    //ros::spinOnce();
    rate.sleep();
  }
  
  return 0;
}*/
    while( nh.ok() )
    {
        rate.sleep();

        tf1= getTransform(listener, "/map", "/odom", pose1);
        tf2= getTransform(listener, "/odom", "/base_link", pose2);

        if( tf2 )
        {
            geometry_msgs::PoseStamped pose_stamped, pose_stamped_ok;
            nav_msgs::Odometry odom;

            pose_stamped.header.stamp = pose1.header.stamp;
            pose_stamped.header.frame_id = "/odom";

            pose_stamped.pose.position.x = pose2.pose.position.x;
            pose_stamped.pose.position.y = pose2.pose.position.y;
            pose_stamped.pose.position.z = pose2.pose.position.z;

            pose_stamped.pose.orientation.x = pose2.pose.orientation.x;
            pose_stamped.pose.orientation.y = pose2.pose.orientation.y;
            pose_stamped.pose.orientation.z = pose2.pose.orientation.z;
            pose_stamped.pose.orientation.w = pose2.pose.orientation.w;

            try{
                listener.transformPose("/map", pose_stamped, pose_stamped_ok);
            }
            catch (tf::TransformException ex){
                ROS_ERROR("%s",ex.what());
                ros::Duration(1.0).sleep();
            }

    //             cout << pose_stamped.pose.position.x << ", "
    //                 << pose_stamped.pose.position.y << " - ";
    //             cout << pose_stamped_ok.pose.position.x << ", "
    //                 << pose_stamped_ok.pose.position.y << endl;

            if(pubpath){
              //convert to path
              convert_pose_to_path(pose_stamped,_target_frm);
              //we publish the path
              path_publisher.publish(mPathToShow);
            }

            pose_publisher.publish( pose_stamped_ok );

            odm.header.frame_id="map";
            odm.child_frame_id="base_link";
            odm.pose.pose = pose_stamped_ok.pose;
            odom_publisher.publish( odm );
            //publish_pose( pose_stamped_ok );
        }
    }

    return 0;
};



