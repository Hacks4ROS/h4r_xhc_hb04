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

	ros::Publisher pub_delta_[6];
	ros::Subscriber sub_[12];
	double rate_;

	void displayCallback(const std_msgs::Float64::ConstPtr &msg,Channel channel );


	hal_s32_t last_jog_counts_;
	void countDelta()
	{

		if (hal_data_.data.jog_counts != last_jog_counts_)
		{
			int delta_int = hal_data_.data.jog_counts - last_jog_counts_;
			float delta = delta_int * hal_data_.data.jog_scale;
			last_jog_counts_=hal_data_.data.jog_counts;



			std_msgs::Float64 msg_delta;
			msg_delta.data=delta;

			if (hal_data_.data.jog_enable_x)
			{
				pub_delta_[0].publish(msg_delta);
			}

			if (hal_data_.data.jog_enable_y)
			{
				pub_delta_[1].publish(msg_delta);
			}

			if (hal_data_.data.jog_enable_z)
			{
				pub_delta_[2].publish(msg_delta);
			}

			if (hal_data_.data.jog_enable_a)
			{
				pub_delta_[3].publish(msg_delta);
			}

			if (hal_data_.data.jog_enable_spindle)
			{
				pub_delta_[4].publish(msg_delta);
			}


			if (hal_data_.data.jog_enable_feedrate)
			{
				pub_delta_[5].publish(msg_delta);
			}

		}

	}



public:
	HB04();
	virtual ~HB04();
	int run();
};/* class HB04 */



} /* namespace h4r_xhc_hb04 */


#endif /* HB04_HPP_ */
