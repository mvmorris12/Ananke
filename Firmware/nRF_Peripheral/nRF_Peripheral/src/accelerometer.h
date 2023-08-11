#include "stdint.h"

#define ACC_ENABLE_PIN  31
#define ACC_INT1_PIN    28
#define ACC_INT2_PIN    30
#define ACC_CS_PIN      29

#define ACC_WHOAMI	    0x0F
#define ACC_CTRL1_XL	    0x10
#define ACC_CTRL2_G	    0x11
#define ACC_CTRL3_C	    0x12
#define ACC_CTRL4_C	    0x13
#define ACC_CTRL5_C	    0x14
#define ACC_CTRL6_C	    0x15
#define ACC_CTRL7_G	    0x16
#define ACC_CTRL8_XL	    0x17

#define ACC_OUTX_L_G	    0x22
#define ACC_OUTX_H_G	    0x23
#define ACC_OUTY_L_G	    0x24
#define ACC_OUTY_H_G	    0x25
#define ACC_OUTZ_L_G	    0x26
#define ACC_OUTZ_H_G	    0x27
#define ACC_OUTX_L_XL	    0x28
#define ACC_OUTX_H_XL	    0x29
#define ACC_OUTY_L_XL	    0x2A
#define ACC_OUTY_H_XL	    0x2B
#define ACC_OUTZ_L_XL	    0x2C
#define ACC_OUTZ_H_XL	    0x2D