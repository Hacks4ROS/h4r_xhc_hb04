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
#include <std_msgs/Float64.h>
#include <std_msgs/UInt8.h>

#include <inttypes.h>
#include "linuxcnc_code/hal_replace.h"


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
	XhcHalAdapter hal_data_;

	ros::NodeHandle n_;
	ros::NodeHandle nh_;

	double abs_pos_[6];

	ros::Publisher pub_delta_[6];
	ros::Publisher pub_absolute_[6];

	ros::Subscriber sub_[12];
	double rate_;

	void displayCallback(const std_msgs::Float64::ConstPtr &msg,Channel channel );


	hal_s32_t last_jog_counts_;

#define CNT_DELTA_AXIS(NUM)\
{\
	pub_delta_[NUM].publish(msg_delta);\
	abs_pos_[NUM]+=delta;\
	msg_abs.data=abs_pos_[NUM];\
	pub_absolute_[NUM].publish(msg_abs);\
}

	void countDelta()
	{

		if (hal_data_.data.jog_counts != last_jog_counts_)
		{
			int delta_int = hal_data_.data.jog_counts - last_jog_counts_;
			float delta = delta_int * hal_data_.data.jog_scale;
			last_jog_counts_=hal_data_.data.jog_counts;

			std_msgs::Float64 msg_delta;
			std_msgs::Float64 msg_abs;

			msg_delta.data=delta;

			if (hal_data_.data.jog_enable_x)CNT_DELTA_AXIS(0)
			if (hal_data_.data.jog_enable_y)CNT_DELTA_AXIS(1)
			if (hal_data_.data.jog_enable_z)CNT_DELTA_AXIS(2)
			if (hal_data_.data.jog_enable_a)CNT_DELTA_AXIS(3)
			if (hal_data_.data.jog_enable_spindle)CNT_DELTA_AXIS(4)
			if (hal_data_.data.jog_enable_feedrate)CNT_DELTA_AXIS(5)

		}

	}



public:
	HB04();
	virtual ~HB04();
	int run();
};/* class HB04 */



} /* namespace h4r_xhc_hb04 */


#endif /* HB04_HPP_ */
