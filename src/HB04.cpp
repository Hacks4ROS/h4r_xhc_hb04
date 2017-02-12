/**
 * @todo add license and move to source file
 * Class source
 * h4r_xhc_hb04
 * HB04
 */
#include "HB04.hpp"

namespace h4r_xhc_hb04
{


HB04::HB04()
:n_()
,nh_("~")
,rate_(10)
{
  nh_.getParam("rate",rate_);
}

HB04::~HB04()
{

}


int HB04::run()
{
	ros::Rate loop_rate(rate_);

	while(ros::ok())
	{

		ros::spinOnce();
		loop_rate.sleep();
	}
	return 0;
}

}/* namespace h4r_xhc_hb04 */
