#include "Cube.h"
#include <stdio.h>
#include <stdlib.h>
#include <LPC177x_8x.h>
#include "LCD.h"
#include "system_LPC177x_8x.h"
#include <math.h>
#include "Colors.h"
#include "ESP32.h"

volatile uint8_t cube_data_updated_flag = 0;

float angle_x = 0.0;
float angle_y = 0.0;
float angle_z = 0.0;

float points[][3] = {{-1,-1,1}, {1,-1,1}, {1,1,1}, {-1,1,1}, {-1,-1,-1}, {1,-1,-1}, {1,1,-1}, {-1,1,-1}};
float proj_matrix[3][3] = {{1.0,0,0}, {0,1.0,0}, {0,0,0}};

float rot_x[3][3]; // = {{,,}, {,,}, {,,}};
float rot_y[3][3]; // = {{,,}, {,,}, {,,}};
float rot_z[3][3]; // = {{cos(angle*180.0/3.14),-sin(angle*180.0/3.14),0}, {sin(angle*180.0/3.14),cos(angle*180.0/3.14),0}, {0.0,0.0,1.0}};

float proj_array[3] = {0.0,0.0,0.0};
float current_spots[8][2] = {{0.0,0.0}, {0.0,0.0}, {0.0,0.0}, {0.0,0.0}, {0.0,0.0}, {0.0,0.0}, {0.0,0.0}, {0.0,0.0}};
uint16_t current_spots_avg[NUMBER_SAMPLES_AVG_DRAW][8][2];
uint8_t  current_spots_avg_idx = 0;

uint16_t draw_spot_x[8]; draw_spot_y[8];
uint8_t draw_spot_flag = 1;
volatile uint16_t x_accel, y_accel, z_accel;

float x_deg, y_deg, z_deg;
char x_deg_txt[8], y_deg_txt[8], z_deg_txt[8];

void xyz_calc(void){
    float x_var, y_var, z_var;
    float temp_var, t1, t2, t3, t4, t5, t6, t7;
    uint8_t count = 6;
    
    //if (current_tab == 2){
	//x_accel = strtol(dev2.x_txt_raw, NULL, 16);
	//y_accel = strtol(dev2.y_txt_raw, NULL, 16);
	//z_accel = strtol(dev2.z_txt_raw, NULL, 16);
	//x_accel = 0x1234;
	//y_accel = 0x5678;
	//z_accel = 0x9ABC;
    //} 

    //temp_var = sin((((x_accel-32768)/32768)));
    //printf("%d %0.3f\n", temp_var, temp_var);
    //printf("%d %0.3f\n", PI/2.0,PI/2.0);
    /*
    printf("%0.3f\n", );
    */
    t1 = x_accel + 0.0;
    x_var = asin((x_accel-32768)/65535.0)*180.0/PI*1000.0;
    //printf("%0.3f\n", x_var);
    if (x_var < 0){
	x_var += 30000;
    } else {
	x_var -= 30000;
    }
    //printf("%0.3f\n", x_var);
    x_var = x_var*90/15;//*90/93.138;
    //printf("%0.3f\n", x_var);
    if (x_var > 90000){
	//NRF_LOG_INFO("var1: %d", x_var);
	x_var -= 2*(x_var-90000);
	//NRF_LOG_INFO("var2: %d", x_var);
    }
    if (x_var < -90000){
	//NRF_LOG_INFO("var1: %d", x_var);
	x_var += 2*(x_var+90000);
	//NRF_LOG_INFO("var2: %d", x_var);
    }
    
    y_var = asin((y_accel-32768)/65535.0)*180.0/PI*1000.0;
    if (y_var < 0){
	y_var += 30000;
    } else {
	y_var -= 30000;
    }
    y_var = y_var*90/15*90/93.138;
    if (y_var > 90000){
	//NRF_LOG_INFO("var1: %d", y_var);
	y_var -= 2*(y_var-90000);
	//NRF_LOG_INFO("var2: %d", y_var);
    }
    if (y_var < -90000){
	//NRF_LOG_INFO("var1: %d", y_var);
	y_var += 2*(-90000-y_var);
	//NRF_LOG_INFO("var2: %d", y_var);
    }

    z_var = asin((z_accel-32768)/65535.0)*180.0/PI*1000.0;
    if (z_var < 0){
	z_var += 30000;
    } else {
	z_var -= 30000;
    }
    z_var = z_var*90/15*90/93.138;
    if (z_var > 90000){
	z_var -= 2*(z_var-90000);
    }
    if (z_var < -90000){
	z_var += 2*(z_var+90000);
    }

    sprintf(x_deg_txt, "%d", x_var);
    sprintf(y_deg_txt, "%d", y_var);
    sprintf(z_deg_txt, "%d", z_var);

    //int16_t tmpxacc = (int)strtol(dev2.x_txt_raw, NULL, 16);
    //int16_t tmpyacc = (int)strtol(dev2.y_txt_raw, NULL, 16);
    //int16_t tmpzacc = (int)strtol(dev2.z_txt_raw, NULL, 16);

    angle_x = floor((atan2(x_var , (sqrt((y_var*y_var)+(z_var*z_var)))))*180/PI);
    angle_y = floor((atan2(y_var , (sqrt((x_var*x_var)+(z_var*z_var)))))*180/PI);
    angle_z = floor((atan2(sqrt((x_var*x_var)+(y_var*y_var)),z_var))*180/PI);
    //printf("\nangle_x : %0.2f\n", angle_x);
    //printf("angle_y : %0.2f\n", angle_y);
    //printf("angle_z : %0.2f\n", angle_z);
    if (angle_z >= 90){
        if (angle_y > 0){
            if (angle_x < 0){
                angle_x = 180 - angle_x;
            } else {
                angle_x = 180 - angle_x;
            }
        } else {
            //if (angle_x < 0){
            //} else {
            //}
        }
    } else { 
        if (angle_y < 0){
            if (angle_x < 0){
                //angle_x = 180 +angle_x;
            } else {
                //angle_x = 360 + angle_x;
            }
        } else {
            //if (angle_x < 0){
            //    angle_x = 180 - angle_x;
            //} else {
            //    angle_x = 180 - angle_x;
            //}
        }
    }

    //angle_x = -angle_x;
    //angle_z = angle_y;
    angle_y = 15.0;
    

	//angle_x = 1.0;
	//angle_y = 1.0;
	//angle_z = 0.234;
	//angle_z = 80.0;

}


void dot(float projection_matrix[3][3], float points[3], float * p_array){
    p_array[0] = (projection_matrix[0][0] * points[0] + projection_matrix[0][1] * points[1] + projection_matrix[0][2] * points[2]); 
    p_array[1] = (projection_matrix[1][0] * points[0] + projection_matrix[1][1] * points[1] + projection_matrix[1][2] * points[2]); 
    p_array[2] = (projection_matrix[2][0] * points[0] + projection_matrix[2][1] * points[1] + projection_matrix[2][2] * points[2]); 
}


void calculate_cube_points(void){
	//angle_x = angle_x >= 360.0 ? angle_x -= 360 : angle_x;
	//angle_y = angle_y >= 360.0 ? angle_y -= 360 : angle_y;
	//angle_z = angle_z >= 360.0 ? angle_z -= 360 : angle_z;

	

	
	for (uint8_t i=0; i<8; i++){
	    dot(proj_matrix, points[i], proj_array); 
	    if (draw_spot_flag == 1){
                lcd_draw_square(current_spots[i][0], current_spots[i][1], SQ_SIZE+5, BLACK);
		//p_lcd->lcd_rect_draw_fast(draw_spot_x[i],draw_spot_y[i],SQ_SIZE,SQ_SIZE,BLACK);
	    }
	}

	//angle_x += 1.0;
	//angle_y += 1.0;
	angle_z += 0.234;


	rot_x[0][0] = 1.0;
	rot_x[0][1] = 0.0;
	rot_x[0][2] = 0.0;
	rot_x[1][0] = 0.0;
	rot_x[1][1] = cos(angle_x*3.14/180.0);
	rot_x[1][2] = -sin(angle_x*3.14/180.0);
	rot_x[2][0] = 0.0;
	rot_x[2][1] = sin(angle_x*3.14/180.0);
	rot_x[2][2] = cos(angle_x*3.14/180.0);

	rot_y[0][0] = cos(angle_y*3.14/180.0);
	rot_y[0][1] = 0.0;
	rot_y[0][2] = sin(angle_y*3.14/180.0);
	rot_y[1][0] = 0.0;
	rot_y[1][1] = 1.0;
	rot_y[1][2] = 0.0;
	rot_y[2][0] = -sin(angle_y*3.14/180.0);
	rot_y[2][1] = 0.0;
	rot_y[2][2] = cos(angle_y*3.14/180.0);

	rot_z[0][0] = cos(angle_z*3.14/180.0);
	rot_z[0][1] = -sin(angle_z*3.14/180.0);
	rot_z[0][2] = 0.0;
	rot_z[1][0] = sin(angle_z*3.14/180.0);
	rot_z[1][1] = cos(angle_z*3.14/180.0);
	rot_z[1][2] = 0.0;
	rot_z[2][0] = 0.0;
	rot_z[2][1] = 0.0;
	rot_z[2][2] = 1.0;
	uint16_t color_corner;


        //for (uint8_t i=0; i<3; i++){
        //    for (uint8_t j=0; j<3; j++){
        //        printf("%d-%d x%f  y%f  z%f\n",i,j, rot_x[i][j], rot_y[i][j], rot_z[i][j]);
        //    }
        //}


	for (uint8_t i=0; i<8; i++){

	    dot(rot_z, points[i], proj_array); 
	    dot(rot_y, proj_array, proj_array); 
	    dot(rot_x, proj_array, proj_array); 

	    dot(proj_matrix, proj_array, proj_array);
	    
	    current_spots[i][0] = floor(proj_array[0]*CUBE_SIZE+CUBE_OFFSET_Y);
	    current_spots[i][1] = floor(proj_array[1]*CUBE_SIZE+CUBE_OFFSET_X);

	    current_spots_avg[current_spots_avg_idx][i][0] = current_spots[i][0];
	    current_spots_avg[current_spots_avg_idx][i][1] = current_spots[i][1];
	    
	 //   switch(i){
		//case 0:
		//    color_corner = WHITE;
		//    break;
		//case 1:
		//    color_corner = WHITE;
		//    break;
		//case 2:
		//    color_corner = WHITE;
		//    break;
		//case 3:
		//    color_corner = WHITE;
		//    break;
		//case 4:
		//    color_corner = BLUE;
		//    break;
		//case 5:
		//    color_corner = BLUE;
		//    break;
		//case 6:
		//    color_corner = BLUE;
		//    break;
		//case 7:
		//    color_corner = BLUE;
		//    break;
	    //}
	    //p_lcd->lcd_rect_draw_fast(proj_array[0]*30+120,proj_array[1]*30+120,SQ_SIZE,SQ_SIZE,color_corner);
	}
	/*
	for (uint8_t i=0; i<8; i++){
	    //color_corner = i > 3 ? BLUE : WHITE;
	    //p_lcd->lcd_rect_draw_fast(current_spots[i][0],current_spots[i][1],SQ_SIZE,SQ_SIZE,color_corner);
	    
	    for (uint8_t j = 0; j<NUMBER_SAMPLES_AVG_DRAW; j++){
		    draw_spot_x[i] += current_spots_avg[j][i][0];
		    draw_spot_y[i] += current_spots_avg[j][i][1];
	    }
	    draw_spot_x[i] /= NUMBER_SAMPLES_AVG_DRAW;
	    draw_spot_y[i] /= NUMBER_SAMPLES_AVG_DRAW;
	    //color_corner = (i > 3) ? BLUE : WHITE;
	    
	    //NRF_LOG_INFO("draw_spot_flag %d\t\tavg_cnt %d", draw_spot_flag, current_spots_avg_idx);
	    
	}
	*/
	if (draw_spot_flag == 1){
	    if (angle_x > 40){
                lcd_draw_square(current_spots[2][0], current_spots[2][1], SQ_SIZE, RED);
                lcd_draw_square(current_spots[3][0], current_spots[3][1], SQ_SIZE, RED);
                lcd_draw_square(current_spots[0][0], current_spots[0][1], SQ_SIZE, GREEN);
                lcd_draw_square(current_spots[1][0], current_spots[1][1], SQ_SIZE, GREEN);
                lcd_draw_square(current_spots[6][0], current_spots[6][1], SQ_SIZE, BLUE);
                lcd_draw_square(current_spots[7][0], current_spots[7][1], SQ_SIZE, BLUE);
                lcd_draw_square(current_spots[4][0], current_spots[4][1], SQ_SIZE, WHITE);
                lcd_draw_square(current_spots[5][0], current_spots[5][1], SQ_SIZE, WHITE);
	    } else {	
                lcd_draw_square(current_spots[0][0], current_spots[0][1], SQ_SIZE, GREEN);
                lcd_draw_square(current_spots[1][0], current_spots[1][1], SQ_SIZE, GREEN);
                lcd_draw_square(current_spots[2][0], current_spots[2][1], SQ_SIZE, RED);
                lcd_draw_square(current_spots[3][0], current_spots[3][1], SQ_SIZE, RED);
                lcd_draw_square(current_spots[4][0], current_spots[4][1], SQ_SIZE, WHITE);
                lcd_draw_square(current_spots[5][0], current_spots[5][1], SQ_SIZE, WHITE);
                lcd_draw_square(current_spots[6][0], current_spots[6][1], SQ_SIZE, BLUE);
                lcd_draw_square(current_spots[7][0], current_spots[7][1], SQ_SIZE, BLUE);
	    }
            //printf("x_accel : %d\n", x_accel);
            //printf("y_accel : %d\n", y_accel);
            //printf("z_accel : %d\n", z_accel);
            //for (uint8_t i=0; i<8; i++){
            //    printf("%d  ( %d , %d )\n", i, draw_spot_x[i], draw_spot_y[i]);
            //}
            //printf("\n");

            //for (uint8_t i=0; i<8; i++){
            //    printf("%d  %f  %f\n", i, current_spots[i][0], current_spots[i][1]);
            //}

	}
        
	
	//for (uint8_t j = 0; j<10; j++){    
	//}
	if (current_spots_avg_idx > NUMBER_SAMPLES_AVG_DRAW-2){
	    draw_spot_flag = 1;
	}
	current_spots_avg_idx = ++current_spots_avg_idx > NUMBER_SAMPLES_AVG_DRAW-1 ? 0 : current_spots_avg_idx;
	//p_lcd->
	//nrf_delay_ms(10);
}


void cube_run_app(void){
    uint8_t cube_run = 1;
    esp32_start_ble();
    esp32_interrupt_enable();
    printf("Starting cube app\n");
    //current_spots_avg[NUMBER_SAMPLES_AVG_DRAW][8][2];
    for (uint16_t i=0; i<(NUMBER_SAMPLES_AVG_DRAW); i++){
        for (uint8_t j=0; j<8; j++){
            current_spots_avg[i][j][0] = CUBE_OFFSET_X;
            current_spots_avg[i][j][1] = CUBE_OFFSET_Y;
        }
    }
    while(cube_run){
        if (cube_data_updated_flag){
        //delay_ms(10);
            xyz_calc();
            //printf("calc\n");
            calculate_cube_points();
            //printf("calc2\n");
            cube_data_updated_flag = 0;
        }
        // clear flag 
        // if not fast enough, may need a second mutex-like locking flag
        // TODO
        //
        // figure out problem with averages array and plot points
        // use mag data to calculate z rotation
    }

}