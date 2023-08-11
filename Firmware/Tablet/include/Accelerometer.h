//#include <stdio.h>


#define ACC_CTRL1_XL	    0x10
#define ACC_CTRL2_G	    0x11
#define ACC_CTRL4_C	    0x13
#define ACC_OUTX_L_XL	    0x28

#define PI                  (3.14159265)

struct Sensor_Data{
    struct Accelerometer{
	uint16_t x;
	uint16_t y;
	uint16_t z;
        float x_ang;
        float y_ang;
        float z_ang;
        float x_rad;
        float y_rad;
        float z_rad;
    } acc;
    //struct RTC {
	
    //}
};

void acc_init(void);
void acc_pwr_on(void);
void acc_pwr_off(void);
void acc_set_registers(void);
void acc_write(uint32_t reg, uint32_t val);
void acc_read(uint32_t reg, uint8_t bytes, uint16_t* ret_ptr);
void acc_read_xyz(void);
void acc_calculate_xyz_angles(void);