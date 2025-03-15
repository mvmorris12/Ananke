#include "LCD.h"
#include "Power.h"
#include "FFT.h"
#include "I2C.h"
#include "RTC.h"
#include "Microphone.h"
#include "Colors.h"
#include "Fonts.h"
//#include "arm_math.h"
#include "Interrupts.h"
#include "Pathfinder.h"
#include "Pathfinder_images.h"
#include <stdlib.h>




const uint8_t image_microphone[90][62] = {
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,1,1,1,
    1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,
    1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,
    1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,
    1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,
    1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,
    1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,
    1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,
    1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,
    1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,
    1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,
    1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,
    1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,
    1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,
    1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
};

int xxtemp = 7;

extern volatile unsigned char I2C_MasterBuffer[I2C_PORT_NUM][I2C_BUFSIZE];
extern volatile unsigned char I2C_SlaveBuffer[I2C_PORT_NUM][I2C_BUFSIZE];
extern volatile unsigned int I2C_Count[I2C_PORT_NUM];
extern volatile unsigned int I2C_ReadLength[I2C_PORT_NUM];
extern volatile unsigned int I2C_WriteLength[I2C_PORT_NUM];
volatile uint8_t get_touch_coordinates_flag;
extern volatile RTCTime local_time;
typedef uint32_t lcd_arr_t[PIXELS_COL];
extern float32_t fft_bin_output[TEST_LENGTH_SAMPLES/2];
uint16_t fft_bin_output_old[TEST_LENGTH_SAMPLES/2][2];

volatile uint8_t fft_scale_flag = 0;
extern volatile int32_t I2SRXBuffer[MEASUREMENTS_TO_TAKE];
uint16_t audio_signal_output_old[TEST_LENGTH_SAMPLES/2][2];
extern volatile uint8_t fft_buttons_active;
extern volatile uint8_t speech_buttons_active;
//uint8_t get_touch_coordinates_flag = 0;
uint8_t get_touch_coordinates_block = 0;
uint8_t record_button_status = FALSE;

extern const int audio_signal_r[];
extern const int audio_signal_r0[];
extern const int audio_signal_r1[];
extern const int audio_signal_r2[];
extern const int audio_signal_r3[];
extern const int audio_signal_r4[];
extern const int audio_signal_r5[];
extern const int audio_signal_r6[];
extern const int audio_signal_r7[];
extern const int audio_signal_g[];
extern const int audio_signal_g0[];
extern const int audio_signal_g1[];
extern const int audio_signal_g2[];
extern const int audio_signal_g3[];
extern const int audio_signal_g4[];
extern const int audio_signal_g5[];
extern const int audio_signal_g6[];
extern const int audio_signal_g7[];
extern const int audio_signal_b[];
extern const int audio_signal_b0[];
extern const int audio_signal_b1[];
extern const int audio_signal_b2[];
extern const int audio_signal_b3[];
extern const int audio_signal_b4[];
extern const int audio_signal_b5[];
extern const int audio_signal_b6[];
extern const int audio_signal_b7[];
//extern volatile I2S_array_t *I2S_arr;
//extern fft_buffer_t *speech_fft_test;
//extern fft_buffer_t *speech_fft_model_r0;
//extern fft_buffer_t *speech_fft_model_r1;
//extern fft_buffer_t *speech_fft_model_r2;
//extern fft_buffer_t *speech_fft_model_r3;
//extern fft_buffer_t *speech_fft_model_r4;
//extern fft_buffer_t *speech_fft_model_r5;
//extern fft_buffer_t *speech_fft_model_r6;
//extern fft_buffer_t *speech_fft_model_r7;
//extern fft_buffer_t *speech_fft_model_g0;
//extern fft_buffer_t *speech_fft_model_g1;
//extern fft_buffer_t *speech_fft_model_g2;
//extern fft_buffer_t *speech_fft_model_g3;
//extern fft_buffer_t *speech_fft_model_g4;
//extern fft_buffer_t *speech_fft_model_g5;
//extern fft_buffer_t *speech_fft_model_g6;
//extern fft_buffer_t *speech_fft_model_g7;
//extern fft_buffer_t *speech_fft_model_b0;
//extern fft_buffer_t *speech_fft_model_b1;
//extern fft_buffer_t *speech_fft_model_b2;
//extern fft_buffer_t *speech_fft_model_b3;
//extern fft_buffer_t *speech_fft_model_b4;
//extern fft_buffer_t *speech_fft_model_b5;
//extern fft_buffer_t *speech_fft_model_b6;
//extern fft_buffer_t *speech_fft_model_b7;

extern volatile uint8_t run_fft_app;
extern volatile uint8_t pf_skip;

extern volatile uint8_t pf_buttons_active;
extern volatile uint8_t pf_run_game;
//float audio_signal_test_current[256];



uint32_t *pFB32, col;
uint16_t touch_x[5], touch_y[5];
lcd_arr_t *lcd_arr = (lcd_arr_t*) 0xA0000000; // TODO was 0xA0000800;


void lcd_draw_time(void){
    char time_old[6]; 
    char time_new[6];
    sprintf(time_old, "%02d:%02d", local_time.RTC_Hour, local_time.RTC_Min); 
    local_time = RTCGetTime();
    sprintf(time_new, "%02d:%02d", local_time.RTC_Hour, local_time.RTC_Min);
    lcd_draw_text(time_old, 730,10,BLACK);
    lcd_draw_text(time_new, 730,10,GRAY_LIGHT);
}

void lcd_draw_text(char *string, uint16_t x, uint16_t y, uint32_t color){
    //uint16_t x = 300, y = 240;
    //uint32_t color = WHITE;
    //printf("printing %s at (%d,%d)\n", string, x, y);
    for (uint8_t l=0; l<strlen(string); l++){
        uint16_t box_w = 0, box_h = 0, ofs_x = 0, ofs_y = 0, ypo = 0, idx = 0, idx_len = 0, bit_idx = 0;
        uint16_t current_bitmap = 0, current_bit = 0, i = 0;
    
        //printf("%c %d %d\n", string[l], string[l], string[l]-31);
        idx = string[l]-31;
        if (idx >= 60){
            idx--;
        }
        idx_len = glyph_dsc[idx+1].bitmap_index - glyph_dsc[idx].bitmap_index;
        box_w = glyph_dsc[idx].box_w;
        box_h = glyph_dsc[idx].box_h;
        ofs_x = glyph_dsc[idx].ofs_x;
        ofs_y = glyph_dsc[idx].ofs_y;
        ypo = y + ofs_y;
        //printf("0x%02x\n", current_bitmap);
        while (i < idx_len){
            if (i < idx_len){
                current_bitmap = glyph_bitmap[glyph_dsc[idx].bitmap_index+i];
                //printf("0x%02x\n", current_bitmap);
                for (uint8_t j=0; j<box_h; j++){
                    for (uint8_t k=0; k<(box_w); k++){
                        if ((current_bitmap>>(7-bit_idx))&0x01){
                            lcd_draw_pixel(x+k, ypo+j, color);
                            //printf("1");
                        } else {
                            //printf("0");
                        }
                        bit_idx++;
                        if (bit_idx>7){
                            bit_idx = 0;
                            i++;
                            current_bitmap = glyph_bitmap[glyph_dsc[idx].bitmap_index+i];
                        }
                    }
                }
                if (idx == 68){
                    i++;
                }
            }
        }
        x += (box_w + ofs_x);
    }

}

void lcd_test(void){
    lcd_draw_line(50,50,100,400,2,0xFFFFFFFF);
    //delay_long();
    lcd_draw_line(60,50,110,400,2,RED);
    //delay_long();
    lcd_draw_line(70,50,120,400,2,GREEN);
    //delay_long();
    lcd_draw_line(80,50,130,400,2,BLUE);
    //delay_long();
    //lcd_draw_line(50,50,100,400,2,0xFFFFFFFF);
    //lcd_draw_line(50,200,400,200,0,0x00000000);
    //lcd_draw_text("AB!\"#");
    //lcd_draw_line(260,257,799,257,1,RED);
    //lcd_draw_text("0123456789");
}





void lcd_draw_pixel(uint16_t x, uint16_t y, uint32_t color){
    lcd_arr[y][x] = color;
}


void lcd_draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t thickness, uint32_t color){
    if (thickness < 1){
        thickness = 1;
    }
    uint32_t white = RGB(255,255,255);
    uint16_t x0_i = x0;
    uint16_t y0_i = y0;    
    int16_t dx =  abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int16_t dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1; 
    int16_t err = dx + dy, e2; /* error value e_xy */ 
    //printf("dx: %x\ndy: %d\nerr: %d\n", dx, dy, err);
    uint16_t i, j;
    for (;;){  /* loop */
        for (i=0; i<thickness; i++){
            for (j=0; j<thickness; j++){
                lcd_draw_pixel(x0+i, y0+j, color);
            }
        }
        if (x0 == x1 && y0 == y1){
            if (i == thickness){
                break;
            } else {
                i++;
                x0 = x0_i;
                y0 = y0_i;
            }
        }
        e2 = 2 * err;
        if (e2 >= dy){ 
            err += dy; 
            x0 += sx;  /* e_xy+e_x > 0 */
        } 
        if (e2 <= dx){ 
            err += dx; 
            y0 += sy;  /* e_xy+e_y < 0 */
        } 
    }
}


void lcd_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t fill, uint8_t width, uint32_t color){
    if (fill){
        for (uint16_t i=x1; i<x2; i++){
            for (uint16_t j=y1; j<y2; j++){
                lcd_draw_pixel(j,i,color);
            }
                
        }
    } else {
        lcd_draw_line(x1,y1,x2,y1,width,color);
        lcd_draw_line(x1,y1,x1,y2,width,color);
        lcd_draw_line(x1,y2,x2,y2,width,color);
        lcd_draw_line(x2,y1,x2,y2,width,color);
    }
}


void lcd_draw_square(uint16_t x, uint16_t y, uint16_t thickness, uint32_t color){
    for (uint16_t i=x-thickness; i<x+thickness; i++){
        for (uint16_t j=y-thickness; j<y+thickness; j++){
            lcd_draw_pixel(i,j,color);
        }
    }
}


void lcd_draw_circle(int16_t xm, int16_t ym, int16_t r, uint32_t border_color, uint8_t fill, uint32_t fill_color){
    if (fill == 0){
        int x = -r, y = 0, err = 2-2*r; /* II. Quadrant */ 
        do {
            lcd_draw_pixel(xm-x, ym+y, border_color);  /*   I. Quadrant */
            lcd_draw_pixel(xm-y, ym-x, border_color);  /*  II. Quadrant */
            lcd_draw_pixel(xm+x, ym-y, border_color);  /* III. Quadrant */
            lcd_draw_pixel(xm+y, ym+x, border_color);  /*  IV. Quadrant */
            r = err;
            if (r >  x){
                err += ++x*2+1; /* e_xy+e_x > 0 */
            }
            if (r <= y){
                err += ++y*2+1; /* e_xy+e_y < 0 */
            }
        } while (x < 0);
    } else {
        int x = -r, y = 0, err = 2-2*r; /* II. Quadrant */ 
        do {
            lcd_draw_line(xm-x, ym+y, xm+x, ym+y, 1, fill_color);
            lcd_draw_line(xm-x, ym-y, xm+x, ym-y, 1, fill_color);
            r = err;
            if (r >  x){
                err += ++x*2+1; /* e_xy+e_x > 0 */
            }
            if (r <= y){
                err += ++y*2+1; /* e_xy+e_y < 0 */
            }
        } while (x < 0);
        x = -r, y = 0, err = 2-2*r; /* II. Quadrant */ 
        do {
            lcd_draw_pixel(xm-x/2, ym+y/2, border_color);  /*   I. Quadrant */
            lcd_draw_pixel(xm-y/2, ym-x/2, border_color);  /*  II. Quadrant */
            lcd_draw_pixel(xm+x/2, ym-y/2, border_color);  /* III. Quadrant */
            lcd_draw_pixel(xm+y/2, ym+x/2, border_color);  /*  IV. Quadrant */
            r = err;
            if (r >  x){
                err += ++x*2+1; /* e_xy+e_x > 0 */
            }
            if (r <= y){
                err += ++y*2+1; /* e_xy+e_y < 0 */
            }
        } while (x < 0);
    }
}


void lcd_read_touch_coords(void){
    //printf("getting touch coordinates\n");
    I2C_ReadLength[0] = 28;
    I2C_WriteLength[0] = 2;
    I2C_MasterBuffer[0][0] = (0x38<<1) | 0x00;
    I2C_MasterBuffer[0][1] = 0x03;
    I2C_MasterBuffer[0][2] = (0x38<<1) | 0x01;

    I2C_Engine(0);
    touch_x[0] = ((I2C_SlaveBuffer[0][0] & 0x0F)<<8) + I2C_SlaveBuffer[0][1];
    touch_y[0] = ((I2C_SlaveBuffer[0][2])<<8) + I2C_SlaveBuffer[0][3];
    touch_x[1] = ((I2C_SlaveBuffer[0][6] & 0x0F)<<8) + I2C_SlaveBuffer[0][7];
    touch_y[1] = ((I2C_SlaveBuffer[0][8] & 0x0F)<<8) + I2C_SlaveBuffer[0][9];
    touch_x[2] = ((I2C_SlaveBuffer[0][12] & 0x0F)<<8) + I2C_SlaveBuffer[0][13];
    touch_y[2] = ((I2C_SlaveBuffer[0][14] & 0x0F)<<8) + I2C_SlaveBuffer[0][15];
    touch_x[3] = ((I2C_SlaveBuffer[0][18] & 0x0F)<<8) + I2C_SlaveBuffer[0][19];
    touch_y[3] = ((I2C_SlaveBuffer[0][20] & 0x0F)<<8) + I2C_SlaveBuffer[0][21];
    touch_x[4] = ((I2C_SlaveBuffer[0][24] & 0x0F)<<8) + I2C_SlaveBuffer[0][25];
    touch_y[4] = ((I2C_SlaveBuffer[0][26] & 0x0F)<<8) + I2C_SlaveBuffer[0][27];
    //if (touch_x[0] < 480 && touch_y[0] < 800){
    //    printf("\n%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t", touch_x[0], touch_y[0], touch_x[1], touch_y[1], touch_x[2], touch_y[2], touch_x[3], touch_y[3], touch_x[4], touch_y[4]);
    //} 
    //if (touch_x[1] < 480 && touch_y[1] < 800){
        //printf("%d\t%d\t", touch_x[1], touch_y[1]);
    //} 
    //else {
    //    printf("x");
    //}
}

extern void touch_check(void){
    if (get_touch_coordinates_flag){
        get_touch_coordinates_flag = 0;
        //printf("getting coordinates\n");
        lcd_read_touch_coords();
        //TODO write button detection function
        //if (fft_buttons_active){
        //    if (touch_x[0] > BUTTON_FFT_ERASE_X1 && touch_x[0] < BUTTON_FFT_ERASE_X2 && touch_y[0] > BUTTON_FFT_ERASE_Y1 && touch_y[0] < BUTTON_FFT_ERASE_Y2){
        //        //printf("FFT ERASE BUTTON PRESSED\n");
        //        LPC_TIM1->TCR = (0x1 << 1);
        //        lcd_draw_fft_graph(1);

        //    }
        //    if (touch_x[0] > BUTTON_FFT_SCALE_X1 && touch_x[0] < BUTTON_FFT_SCALE_X2 && touch_y[0] > BUTTON_FFT_SCALE_Y1 && touch_y[0] < BUTTON_FFT_SCALE_Y2){
        //        //printf("FFT SCALE BUTTON PRESSED\n");
        //        LPC_TIM1->TCR = (0x1 << 1);
        //        lcd_toggle_fft_scale_button();
        //    }
        //    if (touch_x[0] > BUTTON_PAUSE_X1 && touch_x[0] < BUTTON_PAUSE_X2 && touch_y[0] > BUTTON_PAUSE_Y1 && touch_y[0] < BUTTON_PAUSE_Y2){
        //        //printf("FFT SCALE BUTTON PRESSED\n");
        //        LPC_TIM1->TCR = (0x1 << 1);
        //        lcd_toggle_pause_button();
        //    }
        //    if (touch_x[0] > BUTTON_FFT_SPEECH_X1 && touch_x[0] < BUTTON_FFT_SPEECH_X2 && touch_y[0] > BUTTON_FFT_SPEECH_Y1 && touch_y[0] < BUTTON_FFT_SPEECH_Y2){
        //        //printf("FFT SCALE BUTTON PRESSED\n");
        //        LPC_TIM1->TCR = (0x1 << 1);
        //        run_fft_app = 0;
        //        fft_buttons_active = 0;
        //        speech_buttons_active = 1;
        //        //mic_start_speech_analyzer();
        //        speech_detection_app();
        //    }
        //} else if (speech_buttons_active){
            //if (touch_x[0] > SPEECH_BUTTON_MICROPHONE_X1 && touch_x[0] < SPEECH_BUTTON_MICROPHONE_X2 && touch_y[0] > SPEECH_BUTTON_MICROPHONE_Y1 && touch_y[0] < SPEECH_BUTTON_MICROPHONE_Y2){
            //    printf("SPEECH RECORD BUTTON PRESSED\n");
            //    LPC_TIM1->TCR = (0x1 << 1);
            //    mic_init();
            //    delay_ms(50);
            //    mic_start_speech_analyzer();
            //    lcd_toggle_record_button();
            //    //lcd_speech_draw_test_signal();
            //    //fft_compare_models_test();
            //}
            //if (touch_x[0] > BUTTON_SPEECH_TO_FFT_X1 && touch_x[0] < BUTTON_SPEECH_TO_FFT_X2 && touch_y[0] > BUTTON_SPEECH_TO_FFT_Y1 && touch_y[0] < BUTTON_SPEECH_TO_FFT_Y2){
            //    //printf("FFT SCALE BUTTON PRESSED\n");
            //    LPC_TIM1->TCR = (0x1 << 1);
            //    speech_buttons_active = 0;
            //    fft_buttons_active = 1;
            //    lcd_clear_screen();
            //    mic_start_fft();

            //}
        //} else 
        if (pf_buttons_active){
            if (!pf_skip){            
               if (touch_x[0] > PF_BUTTON_SHUFFLE_Y1 && touch_x[0] < PF_BUTTON_SHUFFLE_Y2 && touch_y[0] > PF_BUTTON_SHUFFLE_X1 && touch_y[0] < PF_BUTTON_SHUFFLE_X2){
                   //printf("PF SHUFFLE BUTTON PRESSED\n");
                   //LPC_TIM1->TCR = (0x1 << 1);
                   pf_run_game = 1;
                   //printf("shuffle\n");
               } 
            }
        }

        //LPC_TIM1->TC = (0x0);
        //LPC_GPIOINT->IO0IntClr |= (0x1<<12);
    }
}

uint32_t rst_cnt = 0;

void lcd_init(void){
    InitLCDPorts();
    enable_10v_boost();
    lcd_config();
    lcd_touch_interrupt_enable();
    lcd_fill_screen(BLACK);
}


void lcd_config(void){
    lcd_param_t lcd_param;

    lcd_param.HBP = HBP_4DLCD;
    lcd_param.HFP = HFP_4DLCD;
    lcd_param.HSW = HSW_4DLCD;
    lcd_param.PPL = PPL_4DLCD;
    lcd_param.VBP = VBP_4DLCD;
    lcd_param.VFP = VFP_4DLCD;
    lcd_param.VSW = VSW_4DLCD;
    lcd_param.LPP = LPP_4DLCD;
    lcd_param.IOE = IOE_4DLCD;
    lcd_param.IPC = IPC_4DLCD;
    lcd_param.IHS = IHS_4DLCD;
    lcd_param.IVS = IVS_4DLCD;
    lcd_param.ACB = ACB_4DLCD;
    lcd_param.BPP = BPP_4DLCD;
    lcd_param.Clk = CLK_4DLCD;
    lcd_param.LCD = LCD_4DLCD;
    
    LPC_SC->PCONP |= (1UL << 0);  // Power the LCDC
    LPC_LCD->CTRL &= ~(1UL << 0); // Disable the LCDC
    LPC_LCD->TIMH = ((((lcd_param.PPL/16)-1))<<2) | (lcd_param.HSW-1)<<8 | (lcd_param.HFP-1)<<16 | (lcd_param.HBP-1)<<24;
    LPC_LCD->TIMV = (((lcd_param.LPP-1)&0x3FF)<<0) | (((lcd_param.VSW)&0x3F)<<10) | (lcd_param.VFP-1)<<16 | (lcd_param.VBP-1)<<24;
    LPC_LCD->POL  = ((lcd_param.Clk)<<0 | (lcd_param.IVS)<<11 | (lcd_param.IHS)<<12 | (lcd_param.IPC)<<13 | (lcd_param.PPL-1)<<16 | (lcd_param.BCD)<<26);
    LPC_LCD->CTRL = (lcd_param.BPP)<<1 | (lcd_param.LCD)<<5 | (0)<<8;
    LPC_SC->LCD_CFG = 0x0; // No panel clock prescaler
    LPC_LCD->UPBASE = (uint32_t)lcd_arr;//<<3;
    LPC_LCD->CTRL |= (0x1);
    LPC_LCD->CTRL |= (0x1 << 11);// Enable LCD signals
    LPC_SC->LCD_CFG = 0x4; // No panel clock prescaler
    printf("LCD + SDRAM started\r\n");
    delay_short();
    LPC_SC->LCD_CFG = 0x3; // No panel clock prescaler
}

void InitLCDPorts(void) {
    LPC_IOCON->P0_4  |= (0x7 | (0x1 << 9));  //  LCD_VD_0   R0
    LPC_IOCON->P0_5  |= (0x7 | (0x1 << 9));  //  LCD_VD_1   R1
    LPC_IOCON->P4_28 |= (0x7 | (0x1 << 9));  //  LCD_VD_2   R2
    LPC_IOCON->P4_29 |= (0x7 | (0x1 << 9));  //  LCD_VD_3   R3
    LPC_IOCON->P2_6  |= (0x7 | (0x1 << 9));  //  LCD_VD_4   R4
    LPC_IOCON->P2_7  |= (0x7 | (0x1 << 9));  //  LCD_VD_5   R5
    LPC_IOCON->P2_8  |= (0x7 | (0x1 << 9));  //  LCD_VD_6   R6
    LPC_IOCON->P2_9  |= (0x7 | (0x1 << 9));  //  LCD_VD_7   R7

    LPC_IOCON->P0_6  |= (0x7 | (0x1 << 9));  //  LCD_VD_8   G0
    LPC_IOCON->P0_7  |= (0x7 | (0x1 << 9));  //  LCD_VD_9   G1
    LPC_IOCON->P1_20 |= (0x7 | (0x1 << 9));  //  LCD_VD_10  G2
    LPC_IOCON->P1_21 |= (0x7 | (0x1 << 9));  //  LCD_VD_11  G3
    LPC_IOCON->P1_22 |= (0x7 | (0x1 << 9));  //  LCD_VD_12  G4
    LPC_IOCON->P1_23 |= (0x7 | (0x1 << 9));  //  LCD_VD_13  G5
    LPC_IOCON->P1_24 |= (0x7 | (0x1 << 9));  //  LCD_VD_14  G6
    LPC_IOCON->P1_25 |= (0x7 | (0x1 << 9));  //  LCD_VD_15  G7  

    LPC_IOCON->P0_8  |= (0x7 | (0x1 << 9));  //  LCD_VD_16  B0
    LPC_IOCON->P0_9  |= (0x7 | (0x1 << 9));  //  LCD_VD_17  B1
    LPC_IOCON->P2_12 |= (0x7 | (0x1 << 9));  //  LCD_VD_18  B2
    LPC_IOCON->P2_13 |= (0x7 | (0x1 << 9));  //  LCD_VD_19  B3
    LPC_IOCON->P1_26 |= (0x7 | (0x1 << 9));  //  LCD_VD_20  B4
    LPC_IOCON->P1_27 |= (0x7 | (0x1 << 9));  //  LCD_VD_21  B5
    LPC_IOCON->P1_28 |= (0x7 | (0x1 << 9));  //  LCD_VD_22  B6
    LPC_IOCON->P1_29 |= (0x7 | (0x1 << 9));  //  LCD_VD_23  B7

    LPC_IOCON->P1_7  |= (0x7 | (0x1 << 9));   // LCD_PWR
    LPC_IOCON->P2_0  |= (0x7 | (0x1 << 9));   // LCD_PWR
    LPC_IOCON->P2_2  |= (0x7 | (0x1 << 9));   // LCD_DCLK
    LPC_IOCON->P2_3  |= (0x7 | (0x1 << 9));   // LCD_FP VSYNC
    LPC_IOCON->P2_4  |= (0x7 | (0x1 << 9));   // LCD_ENAB_M DEN
    LPC_IOCON->P2_5  |= (0x7 | (0x1 << 9));   // LCD_LP HSYNC

    LPC_GPIO1->DIR |= (0x1 << 7);
}


void lcd_fill_screen(uint32_t color){
    uint32_t* ram_addr = (uint32_t *)0xA0000000;
    for (uint32_t j=0; j<480; j++){
        for (uint32_t i=0; i<800; i++){
            *ram_addr = color;
            ram_addr++;
        }
    }
}


void lcd_draw_fft_graph(uint8_t init){
    if (init){
        lcd_draw_rectangle(FFT_GRAPH_ORIGIN_X,50,750,400,1,0,BLACK); // black fill
        lcd_draw_text("FFT",FFT_GRAPH_ORIGIN_X,FFT_GRAPH_TOP_PIXEL-19,GRAY);
    }
    lcd_draw_line(FFT_GRAPH_ORIGIN_X,FFT_GRAPH_TOP_PIXEL,FFT_GRAPH_ORIGIN_X,400,2,GRAY_DARK);    // y axis
    lcd_draw_line(FFT_GRAPH_ORIGIN_X,401,750,401,2,GRAY_DARK);  // x axis
    lcd_draw_line(750,FFT_GRAPH_TOP_PIXEL,750,401,2,GRAY_DARK);  // right side
    for (uint8_t i=10; i<17; i++){
        lcd_draw_line(FFT_GRAPH_ORIGIN_X,FFT_GRAPH_Y_GRID_SPACING*i,750,FFT_GRAPH_Y_GRID_SPACING*i,2,GRAY_DARK);  // major y axes
    }

    //lcd_draw_text("0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16",FFT_GRAPH_ORIGIN_X,415,RED);
    //lcd_draw_text("0",FFT_GRAPH_ORIGIN_X+2,415,YELLOW);
    //lcd_draw_text("1",FFT_GRAPH_ORIGIN_X+92,415,YELLOW);
    //lcd_draw_text("2",FFT_GRAPH_ORIGIN_X+186,415,YELLOW);
    //lcd_draw_text("3",FFT_GRAPH_ORIGIN_X+282,415,YELLOW);
    //lcd_draw_text("4",FFT_GRAPH_ORIGIN_X+372,415,YELLOW);
    //lcd_draw_text("5",FFT_GRAPH_ORIGIN_X+465,415,YELLOW);
    lcd_draw_text("0",FFT_GRAPH_ORIGIN_X-5,415,YELLOW);
    lcd_draw_text("1",FFT_GRAPH_ORIGIN_X+28,415,YELLOW);
    lcd_draw_text("2",FFT_GRAPH_ORIGIN_X+60,415,YELLOW);
    lcd_draw_text("3",FFT_GRAPH_ORIGIN_X+92,415,YELLOW);
    lcd_draw_text("4",FFT_GRAPH_ORIGIN_X+124,415,YELLOW);
    lcd_draw_text("5",FFT_GRAPH_ORIGIN_X+156,415,YELLOW);
    lcd_draw_text("6",FFT_GRAPH_ORIGIN_X+186,415,YELLOW);
    lcd_draw_text("7",FFT_GRAPH_ORIGIN_X+218,415,YELLOW);
    lcd_draw_text("8",FFT_GRAPH_ORIGIN_X+250,415,YELLOW);
    lcd_draw_text("9",FFT_GRAPH_ORIGIN_X+282,415,YELLOW);
    lcd_draw_text("10",FFT_GRAPH_ORIGIN_X+305,415,YELLOW);
    lcd_draw_text("11",FFT_GRAPH_ORIGIN_X+339,415,YELLOW);
    lcd_draw_text("12",FFT_GRAPH_ORIGIN_X+372,415,YELLOW);
    lcd_draw_text("13",FFT_GRAPH_ORIGIN_X+404,415,YELLOW);
    lcd_draw_text("14",FFT_GRAPH_ORIGIN_X+435,415,YELLOW);
    lcd_draw_text("15",FFT_GRAPH_ORIGIN_X+465,415,YELLOW);
    lcd_draw_text("16",FFT_GRAPH_ORIGIN_X+498,415,YELLOW);
    lcd_draw_text("KHZ",475,445,GRAY);

    for (uint8_t i=1; i<16; i++){
        lcd_draw_line(FFT_GRAPH_ORIGIN_X+(32*i), FFT_GRAPH_TOP_PIXEL, FFT_GRAPH_ORIGIN_X+(32*i), FFT_GRAPH_ORIGIN_Y+10, 1, GRAY_DARK);
    }
}

void lcd_draw_fft_bins(float32_t max_val){
    float32_t scale;
    if (fft_scale_flag){
        scale = 148/(max_val);
    } else {
        scale = 1;
    }
    uint8_t width = 1;
    uint16_t x_coord_0, x_coord_1;
    uint16_t y_coord_0, y_coord_1;
    lcd_clear_fft_graph();
    //delay_long();
    for (uint16_t bin=0; bin<(TEST_LENGTH_SAMPLES/4-1); bin++){
        for (uint8_t i=0; i<width; i++){
            x_coord_0 = FFT_SIGNAL_ORIGIN_X+bin;
            x_coord_1 = x_coord_0+1;
            //float32_t bin_val = fft_bin_output[bin];
            y_coord_0 = floor(FFT_SIGNAL_ORIGIN_Y-(fft_bin_output[bin]*scale));//*scale);
            y_coord_1 = floor(FFT_SIGNAL_ORIGIN_Y-(fft_bin_output[bin+1]*scale));//*scale);
            
            if (y_coord_0 < FFT_GRAPH_TOP_PIXEL){
                y_coord_0 = FFT_GRAPH_TOP_PIXEL;
            }
            if (y_coord_1 < FFT_GRAPH_TOP_PIXEL){ 
                y_coord_1 = FFT_GRAPH_TOP_PIXEL;
            }
            if (x_coord_0 != 0 && y_coord_0 != 0 && x_coord_1 != 0 && y_coord_1 != 0){
                lcd_draw_line(x_coord_0, y_coord_0, x_coord_1, y_coord_1, 1, YELLOW);
            }
            fft_bin_output_old[bin][0] = x_coord_0;
            fft_bin_output_old[bin][1] = y_coord_0;
        }
    }
    fft_bin_output_old[512][0] = 750;
    fft_bin_output_old[512][1] = FFT_SIGNAL_ORIGIN_Y;
}


void lcd_clear_fft_graph(void){
    uint16_t x_coord_0, x_coord_1;
    uint16_t y_coord_0, y_coord_1;
    for (uint16_t bin=0; bin<(TEST_LENGTH_SAMPLES/4-1); bin++){
        x_coord_0 = fft_bin_output_old[bin][0];
        x_coord_1 = x_coord_0+1;
        //float32_t bin_val = fft_bin_output[bin];
        y_coord_0 = fft_bin_output_old[bin][1];//*scale);
        y_coord_1 = fft_bin_output_old[bin+1][1];//*scale);
        if (x_coord_0 != 0 && y_coord_0 != 0 && x_coord_1 != 0 && y_coord_1 != 0){
            lcd_draw_line(x_coord_0, y_coord_0, x_coord_1, y_coord_1, 1, RED);
        }
    }
}



void lcd_draw_audio_graph(uint8_t init){
    if (init){
        lcd_draw_rectangle(AUDIO_GRAPH_ORIGIN_X,49,750,200,1,0,BLACK); // black fill
        lcd_draw_text("SIGNAL",AUDIO_GRAPH_ORIGIN_X,AUDIO_GRAPH_TOP_PIXEL-19,GRAY);
    }
    lcd_draw_line(AUDIO_GRAPH_ORIGIN_X,AUDIO_GRAPH_TOP_PIXEL,AUDIO_GRAPH_ORIGIN_X,AUDIO_GRAPH_ORIGIN_Y,2,GRAY_DARK);    // y axis
    lcd_draw_line(AUDIO_GRAPH_ORIGIN_X,AUDIO_GRAPH_ORIGIN_Y,750,AUDIO_GRAPH_ORIGIN_Y,2,GRAY_DARK);  // x axis
    lcd_draw_line(750,AUDIO_GRAPH_TOP_PIXEL,750,AUDIO_GRAPH_ORIGIN_Y,2,GRAY_DARK);  // right side
    lcd_draw_line(AUDIO_GRAPH_ORIGIN_X,AUDIO_GRAPH_TOP_PIXEL,750,AUDIO_GRAPH_TOP_PIXEL,2,GRAY_DARK);  // x axis
    
    //for (uint8_t i=2; i<9; i++){
    //    lcd_draw_line(AUDIO_GRAPH_ORIGIN_X,AUDIO_GRAPH_Y_GRID_SPACING*i,750,AUDIO_GRAPH_Y_GRID_SPACING*i,2,GRAY_DARKER);  // major y axes
    //}
}


void lcd_draw_audio_signal(void){
    lcd_clear_audio_signal();
    uint8_t width = 1;
    uint16_t x_coord_0, x_coord_1;
    uint16_t y_coord_0, y_coord_1;
    audio_signal_output_old[511][0] = 749;
    audio_signal_output_old[511][1] = AUDIO_GRAPH_ORIGIN_Y;
    audio_signal_output_old[512][0] = 750;
    audio_signal_output_old[512][1] = AUDIO_GRAPH_ORIGIN_Y;
    for (uint16_t bin=150; bin<(TEST_LENGTH_SAMPLES/4-1)+150; bin++){
        for (uint8_t i=0; i<width; i++){
            x_coord_0 = AUDIO_SIGNAL_ORIGIN_X+bin-150;
            x_coord_1 = x_coord_0+1;
            //float32_t bin_val = fft_bin_output[bin];
            //printf("%d\n%d\n\n", AUDIO_SIGNAL_ORIGIN_Y-I2SRXBuffer[bin]/2048,
            //        AUDIO_SIGNAL_ORIGIN_Y-I2SRXBuffer[bin+1]/2048);
            y_coord_0 = floor((float)AUDIO_SIGNAL_ORIGIN_Y-((float)I2SRXBuffer[bin]/2048.0*6.0));//*scale);
            y_coord_1 = floor((float)AUDIO_SIGNAL_ORIGIN_Y-((float)I2SRXBuffer[bin+1]/2048.0*6.0));//*scale);
            
            if (y_coord_0 < 50 || y_coord_0 > 400){
                //y_coord_0 = AUDIO_SIGNAL_ORIGIN_Y;
                y_coord_0 = floor((float)AUDIO_SIGNAL_ORIGIN_Y-(((float)I2SRXBuffer[bin]-262143)/2048.0*6.0));
                //pr
            }
            if (y_coord_1 < 50 || y_coord_1 > 400){ 
                //y_coord_1 = AUDIO_SIGNAL_ORIGIN_Y;
                y_coord_1 = floor((float)AUDIO_SIGNAL_ORIGIN_Y-(((float)I2SRXBuffer[bin+1]-262143)/2048.0*6.0));
            }

            if (y_coord_0 < 50){
                y_coord_0 = 50;
            } else if (y_coord_0 > 200){
                y_coord_0 = 200;
            }

            if (y_coord_1 < 50){
                y_coord_1 = 50;
            } else if (y_coord_1 > 200){
                y_coord_1 = 200;
            }

            
            lcd_draw_line(x_coord_0, y_coord_0, x_coord_1, y_coord_1, 1, YELLOW);
            audio_signal_output_old[bin-150][0] = x_coord_0;
            audio_signal_output_old[bin-150][1] = y_coord_0;
        }
    }
}

void lcd_clear_audio_signal(void){
    uint16_t x_coord_0, x_coord_1;
    uint16_t y_coord_0, y_coord_1;
    for (uint16_t bin=0; bin<(TEST_LENGTH_SAMPLES/4-1); bin++){
        x_coord_0 = audio_signal_output_old[bin][0];
        x_coord_1 = x_coord_0+1;
        //float32_t bin_val = fft_bin_output[bin];
        y_coord_0 = audio_signal_output_old[bin][1];//*scale);
        y_coord_1 = audio_signal_output_old[bin+1][1];//*scale);
        lcd_draw_line(x_coord_0, y_coord_0, x_coord_1, y_coord_1, 1, BLACK);
    }
    lcd_draw_line(748,AUDIO_GRAPH_TOP_PIXEL+2,748,AUDIO_GRAPH_ORIGIN_Y-2,2,BLACK);
    //lcd_draw_audio_graph(0);
}


void lcd_fft_draw_buttons(void){
    // clear left side of screen
    lcd_draw_rectangle(0,0,450,200,1,0,BLACK);
    //erase fft button
    lcd_draw_rectangle(30,225,200,325,0,2,GRAY_DARK);
    lcd_draw_text("FFT SCALE",53,268,GRAY);
    //toggle scale button
    lcd_draw_rectangle(30,350,200,450,0,2,GRAY_DARK);
    lcd_draw_text("FFT ERASE",53,393,GRAY);

    lcd_draw_rectangle(0,0,200,450,1,0,BLACK);
    // speech analyizer button
    //lcd_draw_rectangle(BUTTON_FFT_SPEECH_X1,BUTTON_FFT_SPEECH_Y1,BUTTON_FFT_SPEECH_X2,BUTTON_FFT_SPEECH_Y2,0,2,GRAY_DARK);
    //lcd_draw_text("SPEECH",BUTTON_FFT_SPEECH_TXT_X,BUTTON_FFT_SPEECH_TXT_Y,GRAY);
    //// erase fft button
    //lcd_draw_rectangle(BUTTON_FFT_ERASE_X1,BUTTON_FFT_ERASE_Y1,BUTTON_FFT_ERASE_X2,BUTTON_FFT_ERASE_Y2,0,2,GRAY_DARK);
    //lcd_draw_text("FFT SCALE",BUTTON_FFT_SCALE_TXT_X,BUTTON_FFT_SCALE_TXT_Y,GRAY);
    //// toggle scale button
    //lcd_draw_rectangle(BUTTON_FFT_SCALE_X1,BUTTON_FFT_SCALE_Y1,BUTTON_FFT_SCALE_X2,BUTTON_FFT_SCALE_Y2,0,2,GRAY_DARK);
    //lcd_draw_text("FFT ERASE",BUTTON_FFT_ERASE_TXT_X,BUTTON_FFT_ERASE_TXT_Y,GRAY);
    //// pause button
    //lcd_draw_rectangle(BUTTON_PAUSE_X1,BUTTON_PAUSE_Y1,BUTTON_PAUSE_X2,BUTTON_PAUSE_Y2,0,2,GRAY_DARK);
    //lcd_draw_text("PAUSE",BUTTON_PAUSE_TXT_X,BUTTON_PAUSE_TXT_Y,GRAY);
    fft_buttons_active = 1;
}



void lcd_toggle_fft_scale_button(void){
    fft_scale_flag++;
    if (fft_scale_flag == 2){
        fft_scale_flag = 0;
    }
    for (uint16_t i=0; i<sizeof(fft_bin_output_old)/2; i++){
        fft_bin_output_old[i][1] = 0;
    }
    lcd_draw_fft_graph(1);
    lcd_draw_audio_graph(1);
    //if (fft_scale_flag){
    //    lcd_draw_rectangle(BUTTON_FFT_SCALE_X1+2,BUTTON_FFT_SCALE_Y1+2, BUTTON_FFT_SCALE_X2, BUTTON_FFT_SCALE_Y2, 1, 0, MAROON);
    //    lcd_draw_text("FFT SCALE",BUTTON_FFT_SCALE_TXT_X,BUTTON_FFT_SCALE_TXT_Y,WHITE);
    //} else {
    //    lcd_draw_rectangle(BUTTON_FFT_SCALE_X1+2,BUTTON_FFT_SCALE_Y1+2, BUTTON_FFT_SCALE_X2, BUTTON_FFT_SCALE_Y2, 1, 0, BLACK);
    //    lcd_draw_text("FFT SCALE",BUTTON_FFT_SCALE_TXT_X,BUTTON_FFT_SCALE_TXT_Y,GRAY);
    //}
}



void lcd_toggle_pause_button(void){
    //pause_flag++;
    //if (pause_flag == 2){
    //    pause_flag = 0;
    //}
    //if (pause_flag){
    //    lcd_draw_rectangle(BUTTON_PAUSE_X1+2,BUTTON_PAUSE_Y1+2,BUTTON_PAUSE_X2,BUTTON_PAUSE_Y2,1,0,MAROON);
    //    lcd_draw_text("PAUSE",BUTTON_PAUSE_TXT_X,BUTTON_PAUSE_TXT_Y,WHITE);
    //} else {
    //    lcd_draw_rectangle(BUTTON_PAUSE_X1+2,BUTTON_PAUSE_Y1+2,BUTTON_PAUSE_X2,BUTTON_PAUSE_Y2,1,0,BLACK);
    //    lcd_draw_text("PAUSE",BUTTON_PAUSE_TXT_X,BUTTON_PAUSE_TXT_Y,GRAY);
    //}
}


//void lcd_speech_draw_buttons(void){
//    // clear left side of screen
//    lcd_draw_rectangle(0,0,200,450,1,0,BLACK);
//    //erase fft button
//    //lcd_draw_rectangle(BUTTON_FFT_ERASE_X1,BUTTON_FFT_ERASE_Y1,BUTTON_FFT_ERASE_X2,BUTTON_FFT_ERASE_Y2,0,2,GRAY_DARK);
//    //lcd_draw_text("RECORD",BUTTON_FFT_SCALE_TXT_X,BUTTON_FFT_SCALE_TXT_Y,GRAY);
//    //toggle scale button
//    lcd_draw_rectangle(BUTTON_SPEECH_RECORD_X1,BUTTON_SPEECH_RECORD_Y1,BUTTON_SPEECH_RECORD_X2,BUTTON_SPEECH_RECORD_Y2,0,2,GRAY_DARK);
//    lcd_draw_text("RECORD",BUTTON_SPEECH_RECORD_TXT_X,BUTTON_SPEECH_RECORD_TXT_Y,GRAY);
//    //pause button
//    //lcd_draw_rectangle(BUTTON_PAUSE_X1,BUTTON_PAUSE_Y1,BUTTON_PAUSE_X2,BUTTON_PAUSE_Y2,0,2,GRAY_DARK);
//    //lcd_draw_text("PAUSE",BUTTON_PAUSE_TXT_X,BUTTON_PAUSE_TXT_Y,GRAY);
//    speech_buttons_active = 1;
//}



void lcd_draw_speech_models(void){
    //lcd_draw_rectangle(0,0,200,450,1,0,BLACK);
    //lcd_draw_rectangle(SPEECH_GRAPH_RED_X1,SPEECH_GRAPH_RED_Y1,SPEECH_GRAPH_RED_X2,SPEECH_GRAPH_RED_Y2,0,2,GRAY_DARK);
    //lcd_draw_rectangle(SPEECH_GRAPH_GREEN_X1,SPEECH_GRAPH_GREEN_Y1,SPEECH_GRAPH_GREEN_X2,SPEECH_GRAPH_GREEN_Y2,0,2,GRAY_DARK);
    //lcd_draw_rectangle(SPEECH_GRAPH_BLUE_X1,SPEECH_GRAPH_BLUE_Y1,SPEECH_GRAPH_BLUE_X2,SPEECH_GRAPH_BLUE_Y2,0,2,GRAY_DARK);
    //lcd_draw_rectangle(SPEECH_GRAPH_TEST_X1,SPEECH_GRAPH_TEST_Y1,SPEECH_GRAPH_TEST_X2,SPEECH_GRAPH_TEST_Y2,0,2,GRAY_DARK);
    
    int signal_start_red = 0;
    int signal_start_green = 0;
    int signal_start_blue = 0;
    uint16_t offset = 0;

    //signal_start_red = speech_find_audio_start("red");
    signal_start_red = 0;
    //signal_start_green = speech_find_audio_start("green");
    signal_start_green = 0;
    //signal_start_blue = speech_find_audio_start("blue");
    signal_start_blue = 0;
    float datapoint0 = 0.0;
    float datapoint1 = 0.0;

    int r_offset = 60;
    int g_offset = 60;
    int b_offset = 60;
    
    //datapoint0 = ((float)(audio_signal_r[signal_start_red-offset]/131072.0*72*8)+SPEECH_GRAPH_RED_Y_ORIGIN-10);    
    //datapoint1 = ((float)(audio_signal_r[signal_start_red-offset+16]/131072.0*72*8)+SPEECH_GRAPH_RED_Y_ORIGIN-r_offset);
    //datapoint0 = datapoint1;    
    //for (int i=1; i<256; i++){
    //    datapoint1 = ((float)(audio_signal_r[signal_start_red-offset+i*16]/131072.0*72*8)+SPEECH_GRAPH_RED_Y_ORIGIN-r_offset);
    //    if (datapoint0 < SPEECH_GRAPH_RED_Y1){
    //        datapoint0 = SPEECH_GRAPH_RED_Y1+2;
    //    } else if (datapoint0 > SPEECH_GRAPH_RED_Y2){
    //        datapoint0 = SPEECH_GRAPH_RED_Y2-1;
    //    }
    //    if (datapoint1 < SPEECH_GRAPH_RED_Y1){
    //        datapoint1 = SPEECH_GRAPH_RED_Y1+2;
    //    } else if (datapoint1 > SPEECH_GRAPH_RED_Y2){
    //        datapoint1 = SPEECH_GRAPH_RED_Y2-1;
    //    }
    //    lcd_draw_line(11+i, datapoint0, 12+i, datapoint1, 1, RED);
    //    datapoint0 = datapoint1;
    //}

    //datapoint0 = ((float)(audio_signal_r[signal_start_green-offset]/131072.0*72*8+SPEECH_GRAPH_GREEN_Y_ORIGIN-g_offset));  
    //for (int i=1; i<256; i++){
    //    datapoint1 = ((float)(audio_signal_g[signal_start_green-offset+i*16]/131072.0*72*8)+SPEECH_GRAPH_GREEN_Y_ORIGIN-g_offset);
    //    if (datapoint0 < SPEECH_GRAPH_GREEN_Y1){
    //        datapoint0 = SPEECH_GRAPH_GREEN_Y1+2;
    //    } else if (datapoint0 > SPEECH_GRAPH_GREEN_Y2){
    //        datapoint0 = SPEECH_GRAPH_GREEN_Y2-1;
    //    }
    //    if (datapoint1 < SPEECH_GRAPH_GREEN_Y1){
    //        datapoint1 = SPEECH_GRAPH_GREEN_Y1+2;
    //    } else if (datapoint1 > SPEECH_GRAPH_GREEN_Y2){
    //        datapoint1 = SPEECH_GRAPH_GREEN_Y2-1;
    //    }
    //    lcd_draw_line(11+i, datapoint0, 12+i, datapoint1, 1, YELLOW);
    //    datapoint0 = datapoint1;
    //}
    
    //datapoint0 = ((float)(audio_signal_r[signal_start_blue-offset]/131072.0*72*8)+SPEECH_GRAPH_BLUE_Y_ORIGIN-b_offset);  
    //for (int i=1; i<256; i++){
    //    datapoint1 = ((float)(audio_signal_b[signal_start_blue-offset+i*16]/131072.0*72*8)+SPEECH_GRAPH_BLUE_Y_ORIGIN-b_offset);
    //    if (datapoint0 < SPEECH_GRAPH_BLUE_Y1){
    //        datapoint0 = SPEECH_GRAPH_BLUE_Y1+2;
    //    } else if (datapoint0 > SPEECH_GRAPH_BLUE_Y2){
    //        datapoint0 = SPEECH_GRAPH_BLUE_Y2-1;
    //    }
    //    if (datapoint1 < SPEECH_GRAPH_BLUE_Y1){
    //        datapoint1 = SPEECH_GRAPH_BLUE_Y1+2;
    //    } else if (datapoint1 > SPEECH_GRAPH_BLUE_Y2){
    //        datapoint1 = SPEECH_GRAPH_BLUE_Y2-1;
    //    }
    //    lcd_draw_line(11+i, datapoint0, 12+i, datapoint1, 1, BLUE);
    //    datapoint0 = datapoint1;
    //}
    
    //for (int i=0; i<SPEECH_TEST_LENGTH_SAMPLES; i++){
    //    speech_fft_model_r0[i] = 0.0;
    //    speech_fft_model_r1[i] = 0.0;
    //    speech_fft_model_r2[i] = 0.0;
    //    speech_fft_model_r3[i] = 0.0;
    //    speech_fft_model_g0[i] = 0.0;
    //    speech_fft_model_g1[i] = 0.0;
    //    speech_fft_model_g2[i] = 0.0;
    //    speech_fft_model_g3[i] = 0.0;
    //    speech_fft_model_b0[i] = 0.0;
    //    speech_fft_model_b1[i] = 0.0;
    //    speech_fft_model_b2[i] = 0.0;
    //    speech_fft_model_b3[i] = 0.0;
    //}

    //for (int i=0; i<SPEECH_TEST_LENGTH_SAMPLES; i++){
    //    speech_fft_model_r0[i] = ((float)audio_signal_r0[SPEECH_TEST_LENGTH_SAMPLES-i-1])/65536;
    //    speech_fft_model_r1[i] = ((float)audio_signal_r1[SPEECH_TEST_LENGTH_SAMPLES-i-1])/65536;
    //    speech_fft_model_r2[i] = ((float)audio_signal_r2[SPEECH_TEST_LENGTH_SAMPLES-i-1])/65536;
    //    speech_fft_model_r3[i] = ((float)audio_signal_r3[SPEECH_TEST_LENGTH_SAMPLES-i-1])/65536;
    //    speech_fft_model_r4[i] = ((float)audio_signal_r4[SPEECH_TEST_LENGTH_SAMPLES-i-1])/65536;
    //    speech_fft_model_r5[i] = ((float)audio_signal_r5[SPEECH_TEST_LENGTH_SAMPLES-i-1])/65536;
    //    speech_fft_model_r6[i] = ((float)audio_signal_r6[SPEECH_TEST_LENGTH_SAMPLES-i-1])/65536;
    //    speech_fft_model_r7[i] = ((float)audio_signal_r7[SPEECH_TEST_LENGTH_SAMPLES-i-1])/65536;
    //    speech_fft_model_g0[i] = ((float)audio_signal_g0[SPEECH_TEST_LENGTH_SAMPLES-i-1])/65536;
    //    speech_fft_model_g1[i] = ((float)audio_signal_g1[SPEECH_TEST_LENGTH_SAMPLES-i-1])/65536;
    //    speech_fft_model_g2[i] = ((float)audio_signal_g2[SPEECH_TEST_LENGTH_SAMPLES-i-1])/65536;
    //    speech_fft_model_g3[i] = ((float)audio_signal_g3[SPEECH_TEST_LENGTH_SAMPLES-i-1])/65536;
    //    speech_fft_model_g4[i] = ((float)audio_signal_g4[SPEECH_TEST_LENGTH_SAMPLES-i-1])/65536;
    //    speech_fft_model_g5[i] = ((float)audio_signal_g5[SPEECH_TEST_LENGTH_SAMPLES-i-1])/65536;
    //    speech_fft_model_g6[i] = ((float)audio_signal_g6[SPEECH_TEST_LENGTH_SAMPLES-i-1])/65536;
    //    speech_fft_model_g7[i] = ((float)audio_signal_g7[SPEECH_TEST_LENGTH_SAMPLES-i-1])/65536;
    //    speech_fft_model_b0[i] = ((float)audio_signal_b0[SPEECH_TEST_LENGTH_SAMPLES-i-1])/65536;
    //    speech_fft_model_b1[i] = ((float)audio_signal_b1[SPEECH_TEST_LENGTH_SAMPLES-i-1])/65536;
    //    speech_fft_model_b2[i] = ((float)audio_signal_b2[SPEECH_TEST_LENGTH_SAMPLES-i-1])/65536;
    //    speech_fft_model_b3[i] = ((float)audio_signal_b3[SPEECH_TEST_LENGTH_SAMPLES-i-1])/65536;
    //    speech_fft_model_b4[i] = ((float)audio_signal_b4[SPEECH_TEST_LENGTH_SAMPLES-i-1])/65536;
    //    speech_fft_model_b5[i] = ((float)audio_signal_b5[SPEECH_TEST_LENGTH_SAMPLES-i-1])/65536;
    //    speech_fft_model_b6[i] = ((float)audio_signal_b6[SPEECH_TEST_LENGTH_SAMPLES-i-1])/65536;
    //    speech_fft_model_b7[i] = ((float)audio_signal_b7[SPEECH_TEST_LENGTH_SAMPLES-i-1])/65536;
    //}
    //for (int i=SPEECH_TEST_LENGTH_SAMPLES/2; i<SPEECH_TEST_LENGTH_SAMPLES; i++){
    //    speech_fft_model_r[i] = 0.0;
    //    speech_fft_model_g[i] = 0.0;
    //    speech_fft_model_b[i] = 0.0;
    //}
    //for (int i=0; i<SPEECH_TEST_LENGTH_SAMPLES; i++){
    //    //printf("%d\n", I2S_arr[signal_start_test+i]);
    //    printf("%0.5f\n", speech_fft_model_g0[i]);
    //    delay(5000);
    //}
    //int j = 0;
}


void lcd_speech_draw_buttons(void){
    //lcd_draw_rectangle(SPEECH_BUTTON_MICROPHONE_X1,SPEECH_BUTTON_MICROPHONE_Y1,SPEECH_BUTTON_MICROPHONE_X2,SPEECH_BUTTON_MICROPHONE_Y2,0,2,GRAY_DARK);
    //for (uint8_t i=0; i<90; i++){
    //    for(uint8_t j=0; j<62; j++){
    //        if (image_microphone[i][j] == 0){
    //            lcd_draw_pixel(SPEECH_BUTTON_MICROPHONE_X1+20+j,SPEECH_BUTTON_MICROPHONE_Y1+6+i,GRAY_DARK);
    //        } else {
    //            lcd_draw_pixel(SPEECH_BUTTON_MICROPHONE_X1+20+j,SPEECH_BUTTON_MICROPHONE_Y1+6+i,BLACK);
    //        }
    //    }
    //}
    //lcd_draw_rectangle(BUTTON_SPEECH_TO_FFT_X1,BUTTON_SPEECH_TO_FFT_Y1,BUTTON_SPEECH_TO_FFT_X2,BUTTON_SPEECH_TO_FFT_Y2,0,2,GRAY_DARK);
    //lcd_draw_text("FFT",BUTTON_SPEECH_TO_FFT_TXT_X,BUTTON_SPEECH_TO_FFT_TXT_Y,GRAY);

    //record_button_status = FALSE;
    //speech_buttons_active = 1;
}


void lcd_toggle_record_button(void){
    uint32_t color_mic;

    if (record_button_status == FALSE){
        color_mic = RED;
        record_button_status = TRUE;
    } else {
        color_mic = GRAY_DARK;
        record_button_status = FALSE;
    }
    for (uint8_t i=0; i<90; i++){
        for(uint8_t j=0; j<62; j++){
            if (image_microphone[i][j] == 0){
                //lcd_draw_pixel(SPEECH_BUTTON_MICROPHONE_X1+20+j,SPEECH_BUTTON_MICROPHONE_Y1+6+i,color_mic);
            }
        }
    }

}


void lcd_speech_draw_test_signal(int iteration){
    //lcd_draw_rectangle(SPEECH_GRAPH_TEST_X1,SPEECH_GRAPH_TEST_Y1,SPEECH_GRAPH_TEST_X2,SPEECH_GRAPH_TEST_Y2,0,2,GRAY_DARK);
    //lcd_speech_clear_test_signal();
    //int signal_start_test = 0;
    ////signal_start_test = speech_find_audio_start("test");
    //uint16_t offset = 0;
    //float datapoint0 = 0.0;
    //float datapoint1 = 0.0;
    //float datapointe = 0.0;
    //float slope = 0.0;
    //float eq_b = 0.0;
    //uint8_t graph_offset = 30;
    //datapoint0 = ((float)(I2S_arr[signal_start_test-offset]/131072.0*72*4)+SPEECH_GRAPH_TEST_Y_ORIGIN+graph_offset);    
    //datapointe = ((float)(I2S_arr[signal_start_test-offset+255*2]/131072.0*72*6)+SPEECH_GRAPH_TEST_Y_ORIGIN+graph_offset);    
    ////audio_signal_test_current[0] = datapoint0;
    //eq_b = ((datapointe-(float)SPEECH_GRAPH_TEST_Y_ORIGIN) - (datapoint0-(float)SPEECH_GRAPH_TEST_Y_ORIGIN))/2.0;
    
    ////printf("s:%f e:%f d0:%f de:%f\n", datapoint0-SPEECH_GRAPH_TEST_Y_ORIGIN, datapointe-SPEECH_GRAPH_TEST_Y_ORIGIN, datapoint0-(float)SPEECH_GRAPH_TEST_Y_ORIGIN, datapointe-(float)SPEECH_GRAPH_TEST_Y_ORIGIN);
   
    //slope = ((datapoint0-(float)SPEECH_GRAPH_TEST_Y_ORIGIN) - (datapointe-(float)SPEECH_GRAPH_TEST_Y_ORIGIN))/255.0;
    ////printf("slope %f\n", slope);
    
    ////datapoint0 = ((float)(I2S_arr[signal_start_test-offset]/131072.0*72*6)*SPEECH_GRAPH_TEST_Y_ORIGIN-10)+eq_b;    
    //datapoint1 = ((float)(I2S_arr[signal_start_test-offset+16]/131072.0*72*4)+SPEECH_GRAPH_TEST_Y_ORIGIN+graph_offset)+slope*(16+1)+eq_b;
    //datapoint0 = datapoint1;  
    ////printf("%0.2f\n", datapoint0);
    //for (int i=1; i<256; i++){
    //    datapoint1 = ((float)(I2S_arr[signal_start_test-offset+i*2]/131072.0*72*4)+SPEECH_GRAPH_TEST_Y_ORIGIN+graph_offset)+slope*(i+1)+eq_b;
    //    if (datapoint0 < SPEECH_GRAPH_TEST_Y1+2){
    //        datapoint0 = SPEECH_GRAPH_TEST_Y1+2;
    //    } else if (datapoint0 > SPEECH_GRAPH_TEST_Y2){
    //        datapoint0 = SPEECH_GRAPH_TEST_Y2-1;
    //    }
    //    if (datapoint1 < SPEECH_GRAPH_TEST_Y1+2){
    //        datapoint1 = SPEECH_GRAPH_TEST_Y1+2;
    //    } else if (datapoint1 > SPEECH_GRAPH_TEST_Y2){
    //        datapoint1 = SPEECH_GRAPH_TEST_Y2-1;
    //    }
    //    //lcd_draw_line(SPEECH_GRAPH_TEST_X1+1+i, datapoint0*(slope*i/(255*3))+(datapointe+(slope)/3), SPEECH_GRAPH_TEST_X1+2+i, datapoint1*(slope*(i+1)/(255*3))+(datapointe+(slope)/3), 1, WHITE);
    //    lcd_draw_line(SPEECH_GRAPH_TEST_X1+1+i, datapoint0, SPEECH_GRAPH_TEST_X1+2+i, datapoint1, 1, WHITE);
    //    datapoint0 = datapoint1;
    //    //audio_signal_test_current[i] = datapoint0;
    //}
    ////for (int i=0; i<SPEECH_TEST_LENGTH_SAMPLES; i++){
    ////    speech_fft_test[i] = 0.0;
    ////}
    //for (int i=0; i<SPEECH_TEST_LENGTH_SAMPLES; i++){
    //    //speech_fft_test[SPEECH_TEST_LENGTH_SAMPLES*2-i] = ((float)I2S_arr[signal_start_test+i])/65536;
    //    speech_fft_test[i] = ((float)I2S_arr[i])/65536;
    //}

    ////if (iteration == xxtemp){
    ////    for (int i=0; i<SPEECH_TEST_LENGTH_SAMPLES; i++){
    ////        printf("%d\n", I2S_arr[i]);
    ////        //printf("%0.5f\n", speech_fft_test[i]);
    ////        delay(5000);
    ////    }
    ////    int ll = 0;
    ////}
    
    //fft_speech_test(iteration);
    //speech_cross_correlation_test(iteration);

}


void lcd_speech_clear_test_signal(void){
    //lcd_draw_rectangle(SPEECH_GRAPH_TEST_X1+2,SPEECH_GRAPH_TEST_Y1+2,SPEECH_GRAPH_TEST_X2,SPEECH_GRAPH_TEST_Y2,1,0,BLACK);
}


void lcd_speech_clear_answer(void){
    //lcd_draw_rectangle(SPEECH_ANSWER_X, SPEECH_ANSWER_Y, SPEECH_ANSWER_X+100, SPEECH_ANSWER_Y+20, 1, 0, BLACK);
}


void lcd_speech_draw_answer(uint32_t word){
    //lcd_speech_clear_answer();
    //if (word == RED){
    //    lcd_draw_text("RED", SPEECH_ANSWER_X+24, SPEECH_ANSWER_Y, RED);
    //} else if (word == YELLOW){
    //    lcd_draw_text("YELLOW", SPEECH_ANSWER_X, SPEECH_ANSWER_Y, YELLOW);
    //} else if (word == BLUE){
    //    lcd_draw_text("BLUE", SPEECH_ANSWER_X+19, SPEECH_ANSWER_Y, BLUE);
    //} else {
    //    lcd_draw_text("ERROR", SPEECH_ANSWER_X+10, SPEECH_ANSWER_Y, GRAY_LIGHT);
    //}

}


void lcd_pf_draw_buttons(void){
    //lcd_draw_rectangle(SPEECH_BUTTON_MICROPHONE_X1,SPEECH_BUTTON_MICROPHONE_Y1,SPEECH_BUTTON_MICROPHONE_X2,SPEECH_BUTTON_MICROPHONE_Y2,0,2,GRAY_DARK);
    for (uint8_t i=0; i<55; i++){
        for(uint8_t j=0; j<65; j++){
            if (pf_shuffle_button[i][j] == 1){
                lcd_draw_pixel(PF_BUTTON_SHUFFLE_X1+j,PF_BUTTON_SHUFFLE_Y1+i,GRAY_DARK);
            } else {
                lcd_draw_pixel(PF_BUTTON_SHUFFLE_X1+j,PF_BUTTON_SHUFFLE_Y1+i,BLACK);
            }
        }
    }
    //lcd_draw_rectangle(BUTTON_SPEECH_TO_FFT_X1,BUTTON_SPEECH_TO_FFT_Y1,BUTTON_SPEECH_TO_FFT_X2,BUTTON_SPEECH_TO_FFT_Y2,0,2,GRAY_DARK);
    //lcd_draw_text("FFT",BUTTON_SPEECH_TO_FFT_TXT_X,BUTTON_SPEECH_TO_FFT_TXT_Y,GRAY);

    //record_button_status = FALSE;
    //speech_buttons_active = 1;
}
