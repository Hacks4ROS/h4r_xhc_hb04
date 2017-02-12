/**
 * @todo add license and move to source file
 * Class source
 * h4r_xhc_hb04
 * HB04
 */
#include "HB04.hpp"

namespace h4r_xhc_hb04
{
#include "linuxcnc_code/xhc-hb04.cc"


class XhcHalAdapter
{
public:

struct
{
	hal_float_t x_wc, y_wc, z_wc, a_wc;
	hal_float_t x_mc, y_mc, z_mc, a_mc;

	hal_float_t feedrate_override, feedrate;
	hal_float_t spindle_override, spindle_rps;

	hal_bit_t button_pin[NB_MAX_BUTTONS];

    hal_bit_t jog_enable_off;
	hal_bit_t jog_enable_x;
	hal_bit_t jog_enable_y;
	hal_bit_t jog_enable_z;
	hal_bit_t jog_enable_a;
	hal_bit_t jog_enable_feedrate;
	hal_bit_t jog_enable_spindle;
	hal_float_t jog_scale;
	hal_s32_t jog_counts, jog_counts_neg;

	hal_float_t jog_velocity;
	hal_float_t jog_max_velocity;
	hal_float_t jog_increment;
	hal_bit_t jog_plus_x, jog_plus_y, jog_plus_z, jog_plus_a;
	hal_bit_t jog_minus_x, jog_minus_y, jog_minus_z, jog_minus_a;

	hal_bit_t stepsize_up;
	hal_bit_t stepsize_down;
	hal_s32_t stepsize;
	hal_bit_t sleeping;
	hal_bit_t connected;
	hal_bit_t require_pendant;
	hal_bit_t inch_icon;
	hal_bit_t zero_x;
	hal_bit_t zero_y;
	hal_bit_t zero_z;
	hal_bit_t zero_a;
	hal_bit_t gotozero_x;
	hal_bit_t gotozero_y;
	hal_bit_t gotozero_z;
	hal_bit_t gotozero_a;
	hal_bit_t half_x;
	hal_bit_t half_y;
	hal_bit_t half_z;
	hal_bit_t half_a;
}data;

	XhcHalAdapter(xhc_hal_t *d)
	{
		memset(&data, 0, sizeof(data));

		d->x_wc=&(data.x_wc);
		d->y_wc=&(data.y_wc);
		d->z_wc=&(data.z_wc);
		d->a_wc=&(data.a_wc);
		d->x_mc=&(data.x_mc);
		d->y_mc=&(data.y_mc);
		d->z_mc=&(data.z_mc);
		d->a_mc=&(data.a_mc);
		d->feedrate_override=&(data.feedrate_override);
		d->feedrate=&(data.feedrate);
		d->spindle_override=&(data.spindle_override);
		d->spindle_rps=&(data.spindle_rps);

		for (int var = 0; var < NB_MAX_BUTTONS; ++var)
		{
			d->button_pin[var]=&(data.button_pin[var]);
		}


		d->jog_enable_off=&(data.jog_enable_off);
		d->jog_enable_x=&(data.jog_enable_x);
		d->jog_enable_y=&(data.jog_enable_y);
		d->jog_enable_z=&(data.jog_enable_z);
		d->jog_enable_a=&(data.jog_enable_a);
		d->jog_enable_feedrate=&(data.jog_enable_feedrate);
		d->jog_enable_spindle=&(data.jog_enable_spindle);
		d->jog_scale=&(data.jog_scale);
		d->jog_counts=&(data.jog_counts);
		d->jog_counts_neg=&(data.jog_counts_neg);
		d->jog_velocity=&(data.jog_velocity);
		d->jog_max_velocity=&(data.jog_max_velocity);
		d->jog_increment=&(data.jog_increment);
		d->jog_plus_x=&(data.jog_plus_x);
		d->jog_plus_y=&(data.jog_plus_y);
		d->jog_plus_z=&(data.jog_plus_z);
		d->jog_plus_a=&(data.jog_plus_a);
		d->jog_minus_x=&(data.jog_minus_x);
		d->jog_minus_y=&(data.jog_minus_y);
		d->jog_minus_z=&(data.jog_minus_z);
		d->jog_minus_a=&(data.jog_minus_a);
		d->stepsize_up=&(data.stepsize_up);
		d->stepsize_down=&(data.stepsize_down);
		d->stepsize=&(data.stepsize);
		d->sleeping=&(data.sleeping);
		d->connected=&(data.connected);
		d->require_pendant=&(data.require_pendant);
		d->inch_icon=&(data.inch_icon);
		d->zero_x=&(data.zero_x);
		d->zero_y=&(data.zero_y);
		d->zero_z=&(data.zero_z);
		d->zero_a=&(data.zero_a);
		d->gotozero_x=&(data.gotozero_x);
		d->gotozero_y=&(data.gotozero_y);
		d->gotozero_z=&(data.gotozero_z);
		d->gotozero_a=&(data.gotozero_a);
		d->half_x=&(data.half_x);
		d->half_y=&(data.half_y);
		d->half_z=&(data.half_z);
		d->half_a=&(data.half_a);
	}


};


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

    //


    xhc_hal_t hal;
    XhcHalAdapter hal_data(&hal);
    xhc.hal=&hal;

//    while ((opt = getopt(argc, argv, "HhI:xs:")) != -1) {
//        switch (opt) {
//        case 'I':
//            if (read_ini_file(optarg)) {
//                printf("Problem reading ini file: %s\n\n",optarg);
//                Usage(argv[0]);
//                exit(EXIT_FAILURE);
//            }
//            break;
//        case 'H':
//        	simu_mode = false;
//        	break;
//        case 's':
//            switch (optarg[0]) {
//              case '1': stepsize_sequence = stepsize_sequence_1;break;
//              case '2': stepsize_sequence = stepsize_sequence_2;break;
//              case '3': stepsize_sequence = stepsize_sequence_3;break;
//              case '4': stepsize_sequence = stepsize_sequence_4;break;
//              case '5': stepsize_sequence = stepsize_sequence_5;break;
//              default:
//                printf("Unknown sequence: %s\n\n",optarg);
//                Usage(argv[0]);
//                exit(EXIT_FAILURE);
//                break;
//            }
//            break;
//        case 'x':
//        	wait_for_pendant_before_HAL = true;
//        	break;
//        default:
//        	Usage(argv[0]);
//            exit(EXIT_FAILURE);
//        }
//    }

    // compute the last valid idx for use with stepsize-down
    stepsize_sequence = stepsize_sequence_1;

    for (idx=0; idx < MAX_STEPSIZE_SEQUENCE; idx++) {
       if (stepsize_sequence[idx] == 0) break;
    }
    stepsize_last_idx  =  idx - 1;

	//hal_setup();

//    signal(SIGINT, quit);
//	signal(SIGTERM, quit);

    if (!wait_for_pendant_before_HAL && !simu_mode) {
    	hal_ready(hal_comp_id);
    	hal_ready_done = true;
    }

    while (ros::ok()) {
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
		//} while(dev_handle == NULL && !do_exit);
		} while(dev_handle == NULL && ros::ok());

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

	    if (!hal_ready_done && !simu_mode) {
	    	hal_ready(hal_comp_id);
	    	hal_ready_done = true;
	    }

		if (dev_handle) {
			setup_asynch_transfer(dev_handle);
			xhc_set_display(dev_handle, &xhc);
		}

		if (dev_handle) {
			//while (!do_exit && !do_reconnect) {
			while (ros::ok() && !do_reconnect) {
				struct timeval tv;
				tv.tv_sec  = 0;
				tv.tv_usec = 30000;
				r = libusb_handle_events_timeout(ctx, &tv);
				compute_velocity(&xhc);
			    if (simu_mode) linuxcnc_simu(&xhc);
				handle_step(&xhc);
				xhc_set_display(dev_handle, &xhc);

				ROS_INFO_STREAM("Axis: "<<xhc.axis);
			}
			*(xhc.hal->connected) = 0;
			printf("%s: connection lost, cleaning up\n",modname);
			libusb_cancel_transfer(transfer_in);
			libusb_free_transfer(transfer_in);
			libusb_release_interface(dev_handle, 0);
			libusb_close(dev_handle);
		}
		else {
			//while (!do_exit) usleep(70000);
			while (ros::ok()) usleep(70000);

		}
		libusb_exit(ctx);
	}



	//
	ros::Rate loop_rate(rate_);

	while(ros::ok())
	{

		ros::spinOnce();
		loop_rate.sleep();
	}
	return 0;
}

}/* namespace h4r_xhc_hb04 */
