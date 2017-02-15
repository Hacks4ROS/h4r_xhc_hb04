/**
 * @todo add license and move to source file
 * Class source
 * h4r_xhc_hb04
 * HB04
 */
#include "HB04.hpp"
#include "linuxcnc_code/xhc-hb04.cc"

namespace h4r_xhc_hb04
{


#define CHANNEL_REGISTER(NO,CHAN)\
case NO:\
	chan=Channel::CHAN;\
	topic_name=#CHAN;\
	break

#define CHANNEL_SET_LCD(CHAN)\
case Channel::CHAN:\
	hal_data_.data.CHAN=msg->data;\
	break




HB04::HB04()
:n_()
,nh_("~")
,rate_(10)
,abs_pos_ {0}
{
  nh_.getParam("rate",rate_);



	  for (int c = 0; c < 12; ++c)
	  {

		  Channel chan;
		  std::string topic_name;
		  switch(c)
		  {
			  CHANNEL_REGISTER(0,x_mc);
			  CHANNEL_REGISTER(1,y_mc);
			  CHANNEL_REGISTER(2,z_mc);
			  CHANNEL_REGISTER(3,a_mc);
			  CHANNEL_REGISTER(4,x_wc);
			  CHANNEL_REGISTER(5,y_wc);
			  CHANNEL_REGISTER(6,z_wc);
			  CHANNEL_REGISTER(7,a_wc);
			  CHANNEL_REGISTER(8,feedrate);
			  CHANNEL_REGISTER(9,feedrate_override);
			  CHANNEL_REGISTER(10,spindle_rps);
			  CHANNEL_REGISTER(11,spindle_override);
		  }
		  sub_[c]=nh_.subscribe<std_msgs::Float64>("pos_"+topic_name,1,bind(&HB04::displayCallback, this, _1,chan));

	  }



	  for (int c = 0; c < 6; ++c)
	  {

		  Channel chan;
		  std::string topic_name;
		  switch(c)
		  {
			  CHANNEL_REGISTER(0,x_mc);
			  CHANNEL_REGISTER(1,y_mc);
			  CHANNEL_REGISTER(2,z_mc);
			  CHANNEL_REGISTER(3,a_mc);
			  CHANNEL_REGISTER(4,feedrate_override);
			  CHANNEL_REGISTER(5,spindle_override);
		  }
		  pub_delta_[c]=nh_.advertise<std_msgs::Float64>("delta"+topic_name,1);
		  pub_absolute_[c]=nh_.advertise<std_msgs::Float64>("abs_"+topic_name,1);
	  }

}



HB04::~HB04()
{

}

void HB04::displayCallback(const std_msgs::Float64::ConstPtr &msg,Channel channel )
{
	ROS_INFO_STREAM("Callback "<<(int)channel<<": "<<msg->data);

	switch(channel)
	{
	  CHANNEL_SET_LCD(x_mc);
	  CHANNEL_SET_LCD(y_mc);
	  CHANNEL_SET_LCD(z_mc);
	  CHANNEL_SET_LCD(a_mc);
	  CHANNEL_SET_LCD(x_wc);
	  CHANNEL_SET_LCD(y_wc);
	  CHANNEL_SET_LCD(z_wc);
	  CHANNEL_SET_LCD(a_wc);
	  CHANNEL_SET_LCD(feedrate);
	  CHANNEL_SET_LCD(feedrate_override);
	  CHANNEL_SET_LCD(spindle_rps);
	  CHANNEL_SET_LCD(spindle_override);
	}
}


int HB04::run()
{
	//Code taken from LinuxCNC main

	libusb_device **devs;
    libusb_device_handle *dev_handle;
	libusb_context *ctx = NULL;
	int r,idx;
	ssize_t cnt;
#define MAX_WAIT_SECS 10
	int wait_secs = 0;

    int opt;
    bool hal_ready_done = false;

    init_xhc(&xhc);
    xhc_hal_t hal;
    hal_data_.linkXHC(&hal);
    xhc.hal=&hal;



    // compute the last valid idx for use with stepsize-down
    stepsize_sequence = stepsize_sequence_1;

    for (idx=0; idx < MAX_STEPSIZE_SEQUENCE; idx++) {
       if (stepsize_sequence[idx] == 0) break;
    }
    stepsize_last_idx  =  idx - 1;


    signal(SIGINT, quit);
    signal(SIGTERM, quit);


    while (!do_exit) {
    	//on reconnect wait for device to be gone
    	if (do_reconnect == 1) {
    		sleep(5);
    		do_reconnect = 0;
    	}

		r = libusb_init(&ctx);

		if(r < 0) {
			perror("libusb_init");
			return 1;
		}
		libusb_set_debug(ctx, 2);
		// use environmental variable LIBUSB_DEBUG if needed

		printf("%s: waiting for XHC-HB04 device\n",modname);
		*(xhc.hal->connected) = 0;
		wait_secs = 0;
		*(xhc.hal->require_pendant) = wait_for_pendant_before_HAL;
		*(xhc.hal->stepsize) = stepsize_sequence[0];

		do {
			cnt = libusb_get_device_list(ctx, &devs);
			if (cnt < 0) {
				perror("libusb_get_device_list");
				return 1;
			}

			dev_handle = libusb_open_device_with_vid_pid(ctx, 0x10CE, 0xEB70);
			libusb_free_device_list(devs, 1);
			if (dev_handle == NULL) {
				if (wait_for_pendant_before_HAL) {
					wait_secs++;
					if (wait_secs >= MAX_WAIT_SECS/2) {
						printf("%s: waiting for XHC-HB04 device (%d)\n",modname,wait_secs);
					}
					if (wait_secs > MAX_WAIT_SECS) {
						printf("%s: MAX_WAIT_SECS exceeded, exiting\n",modname);
						exit(1);
					}
				}
				sleep(1);
			}
		} while(dev_handle == NULL && !do_exit);

		printf("%s: found XHC-HB04 device\n",modname);

		if (dev_handle) {
			if 	(libusb_kernel_driver_active(dev_handle, 0) == 1) {
				libusb_detach_kernel_driver(dev_handle, 0);
			}

			r = libusb_claim_interface(dev_handle, 0);
			if (r < 0) {
				perror("libusb_claim_interface");
				return 1;
			}
			//allocate the transfer struct here only once after successful connection
			transfer_in  = libusb_alloc_transfer(0);
		}

		*(xhc.hal->connected) = 1;



		if (dev_handle) {
			setup_asynch_transfer(dev_handle);
			xhc_set_display(dev_handle, &xhc);

			while (!do_exit && !do_reconnect) {
				struct timeval tv;
				tv.tv_sec  = 0;
				tv.tv_usec = 30000;
				r = libusb_handle_events_timeout(ctx, &tv);
				compute_velocity(&xhc);



				if (simu_mode) linuxcnc_simu(&xhc);



				handle_step(&xhc);

				xhc_set_display(dev_handle, &xhc);
				countDelta();

				ros::spinOnce();
				ROS_INFO_STREAM(""<<hal_data_.data.x_mc);




			}
			*(xhc.hal->connected) = 0;
			printf("%s: connection lost, cleaning up\n",modname);
			libusb_cancel_transfer(transfer_in);
			libusb_free_transfer(transfer_in);
			libusb_release_interface(dev_handle, 0);
			libusb_close(dev_handle);
		}
		else {
			while (!do_exit) usleep(70000);

		}

		libusb_exit(ctx);
	}



//	//
//	ros::Rate loop_rate(rate_);
//
//	while(ros::ok())
//	{
//
//		ros::spinOnce();
//		loop_rate.sleep();
//	}
//	return 0;
}

}/* namespace h4r_xhc_hb04 */
