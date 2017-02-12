/*
 * HB04.hpp
 *
 *  Created on: Feb 12, 2017
 *      Author: Christian Holl
 *      
 * @todo insert LICENSE!
 *      All rights reserved! (©2017)
 */

#ifndef HB04_HPP_
#define HB04_HPP_

#include <ros/ros.h>

namespace h4r_xhc_hb04
{
/**
 * @todo add license and move to header file
 * h4r_xhc_hb04
 * HB04
 */
class HB04
{
private:
	ros::NodeHandle n_;
	ros::NodeHandle nh_;
	double rate_;

public:
	HB04();
	virtual ~HB04();
	int run();
};/* class HB04 */



} /* namespace h4r_xhc_hb04 */


#endif /* HB04_HPP_ */
