/*
 * Adapter for hal_h_ for the ros node
 */

#ifndef HAL_H_
#define HAL_H_
#include <libusb-1.0/libusb.h>
#include <inttypes.h>


//Linux CNC unit definitions:
typedef int8_t rtapi_s8;
typedef int16_t rtapi_s16;
typedef int32_t rtapi_s32;
typedef int64_t rtapi_s64;
typedef uint8_t rtapi_u8;
typedef uint16_t rtapi_u16;
typedef uint32_t rtapi_u32;
typedef uint64_t rtapi_u64;
typedef volatile bool hal_bit_t;
typedef volatile rtapi_u32 hal_u32_t;
typedef volatile rtapi_s32 hal_s32_t;
typedef double real_t __attribute__((aligned(8)));
typedef rtapi_u64 ireal_t __attribute__((aligned(8))); // integral type as wide as real_t / hal_float_t
#define hal_float_t volatile real_t
#define NB_MAX_BUTTONS 32


typedef enum {
    HAL_DIR_UNSPECIFIED = -1,
    HAL_IN = 16,
    HAL_OUT = 32,
    HAL_IO = (HAL_IN | HAL_OUT),
} hal_pin_dir_t;

enum class Channel
{
	x_mc,
	y_mc,
	z_mc,
	a_mc,
	x_wc,
	y_wc,
	z_wc,
	a_wc,
	feedrate,
	feedrate_override,
	spindle_rps,
	spindle_override,
};

//Functions in xhc-hb04.cc
typedef struct {
	hal_float_t *x_wc, *y_wc, *z_wc, *a_wc;
	hal_float_t *x_mc, *y_mc, *z_mc, *a_mc;

	hal_float_t *feedrate_override, *feedrate;
	hal_float_t *spindle_override, *spindle_rps;

	hal_bit_t *button_pin[NB_MAX_BUTTONS];

    hal_bit_t *jog_enable_off;
	hal_bit_t *jog_enable_x;
	hal_bit_t *jog_enable_y;
	hal_bit_t *jog_enable_z;
	hal_bit_t *jog_enable_a;
	hal_bit_t *jog_enable_feedrate;
	hal_bit_t *jog_enable_spindle;
	hal_float_t *jog_scale;
	hal_s32_t *jog_counts, *jog_counts_neg;

	hal_float_t *jog_velocity;
	hal_float_t *jog_max_velocity;
	hal_float_t *jog_increment;
	hal_bit_t *jog_plus_x, *jog_plus_y, *jog_plus_z, *jog_plus_a;
	hal_bit_t *jog_minus_x, *jog_minus_y, *jog_minus_z, *jog_minus_a;

	hal_bit_t *stepsize_up;
	hal_bit_t *stepsize_down;
	hal_s32_t *stepsize;
	hal_bit_t *sleeping;
	hal_bit_t *connected;
	hal_bit_t *require_pendant;
	hal_bit_t *inch_icon;
	hal_bit_t *zero_x;
	hal_bit_t *zero_y;
	hal_bit_t *zero_z;
	hal_bit_t *zero_a;
	hal_bit_t *gotozero_x;
	hal_bit_t *gotozero_y;
	hal_bit_t *gotozero_z;
	hal_bit_t *gotozero_a;
	hal_bit_t *half_x;
	hal_bit_t *half_y;
	hal_bit_t *half_z;
	hal_bit_t *half_a;
} xhc_hal_t;

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

	XhcHalAdapter(){}
	void linkXHC(xhc_hal_t *d)
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


#endif /* HAL_H_ */
