#include <ros/ros.h>
#include <grid_map_ros/grid_map_ros.hpp>
#include <grid_map_msgs/GridMap.h>
#include <cmath>

using namespace grid_map;

int main(int argc, char** argv)
{
  // Initialize node and publisher.
  // ros::init(argc, argv, "grid_map_simple_demo");
  ros::init(argc, argv, "extend_include_demo");
  ros::NodeHandle nh("~");
  ros::Publisher publisher = nh.advertise<grid_map_msgs::GridMap>("grid_map", 1, true);
  ros::Publisher new_publisher = nh.advertise<grid_map_msgs::GridMap>("new_grid_map", 1, true);

  // Create grid map.
  GridMap map;
  map.setFrameId("map");
  map.setGeometry(Length(3.6, 6.0), 0.03);
  map.add("elevation", -1.0);
  // ROS_INFO("Created map with size %f x %f m (%i x %i cells).",
  //   map.getLength().x(), map.getLength().y(),
  //   map.getSize()(0), map.getSize()(1));

  // Create new grid map;
  // GridMap newMap;
  // newMap.setFrameId("map");
  // newMap.setGeometry(Length(1.2, 2.0), 0.03);
  // newMap.add("elevation", 0.0);



  //Set data for original map
  // for (GridMapIterator it(map); !it.isPastEnd(); ++it) {
  //     Position position;
  //     map.getPosition(*it, position);
  //     map.at("elevation", *it) = 0.0;
  // }

  // Work with grid map in a loop.
  ros::Rate rate(1.0);
  int shift = 0;
  ros::Time startTime = ros::Time::now();
  ros::Duration duration(0.0);
  while (nh.ok()) {
    shift++;
    // Add data to grid map.
    ros::Time time = ros::Time::now();

    // Create new grid map to extend
    
    // for (GridMapIterator it(map); !it.isPastEnd(); ++it) {
    //   Position position;
    //   map.getPosition(*it, position);
    //   map.at("elevation", *it) = -0.04 + 0.2 * std::sin(3.0 * time.toSec() + 5.0 * position.y()) * position.x();
    // }



    // Move and publish new grid map.

    GridMap newMap;
    newMap.setFrameId("map");
    Position position(shift, shift);
    newMap.setGeometry(Length(3.6, 6.0), 0.03, position);
    newMap.add("elevation", 0.0);
    std::vector<std::string> layers;
    layers.push_back("elevation");
    map.addDataFrom(newMap, false, false, false, layers);
    // for (GridMapIterator it(newMap); !it.isPastEnd(); ++it) {
    //   Position position;
    //   newMap.getPosition(*it, position);
    //   newMap.at("elevation", *it) = 0.0;
    // }

    if(shift > 5) 
      shift = 0;

    // Publish extended grid map.
    map.setTimestamp(time.toNSec());
    grid_map_msgs::GridMap message;
    GridMapRosConverter::toMessage(map, message);
    publisher.publish(message);  
    newMap.setTimestamp(time.toNSec());
    grid_map_msgs::GridMap new_message;
    GridMapRosConverter::toMessage(newMap, new_message);
    new_publisher.publish(new_message); 
    ROS_INFO_THROTTLE(1.0, "New grid map(size of %d * %d) published.", newMap.getSize()[0], newMap.getSize()[1]);
    // ROS_INFO_THROTTLE(1.0, "New grid map (timestamp %f) published.", new_message.info.header.stamp.toSec());

    // Wait for next cycle.
    rate.sleep();
  }

  return 0;
}
