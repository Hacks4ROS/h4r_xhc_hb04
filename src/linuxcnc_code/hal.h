/*
 * Adapter for hal_h_ for the ros node
 */

#ifndef HAL_H_
#define HAL_H_
#include <libusb-1.0/libusb.h>
#include <inttypes.h>
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

typedef enum {
    HAL_DIR_UNSPECIFIED = -1,
    HAL_IN = 16,
    HAL_OUT = 32,
    HAL_IO = (HAL_IN | HAL_OUT),
} hal_pin_dir_t;

int hal_ready(int)
{
	return 0;
}



#endif /* HAL_H_ */
