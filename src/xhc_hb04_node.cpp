#include <ros/ros.h>
#include "HB04.hpp"

int main(int argc, char** argv)
{
	ros::init(argc,argv,"xhc_hb04_node");
	ros::Time::init();

	h4r_xhc_hb04::HB04 node;
	return node.run();
}





