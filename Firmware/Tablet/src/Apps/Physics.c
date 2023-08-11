#include "Physics.h"
#include "Accelerometer.h"
#include "Timer.h"
#include "LCD.h"
#include "Colors.h"
#include "SPI.h"



uint8_t bodies_idx = 0;

struct physics_bodies_struct body[10];
extern struct Sensor_Data sensor_data;


float physics_distance(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2){
    //printf("%d %d %d %d\n", x1, y1, x2, y2);
    return sqrt(((x2-x1)*(x2-x1))+((y2-y1)*(y2-y1)));
}


void physics_add_body(uint16_t x, uint16_t y){
    body[bodies_idx].x = x;
    body[bodies_idx].y = y;
    body[bodies_idx].a = 0;
    body[bodies_idx].v = 0;
    body[bodies_idx].d = 0;
    switch(bodies_idx%10){
        case 0:
            body[bodies_idx].color = WHITE;
            break;
        case 1:
            body[bodies_idx].color = RED;
            break;
        case 2:
            body[bodies_idx].color = GREEN;
            break;
        case 3:
            body[bodies_idx].color = BLUE;
            break;
        case 4:
            body[bodies_idx].color = ORANGE;
            break;
        case 5:
            body[bodies_idx].color = PURPLE;
            break;
        case 6:
            body[bodies_idx].color = MAGENTA;
            break;
        case 7:
            body[bodies_idx].color = CYAN;
            break;
        case 8:
            body[bodies_idx].color = YELLOW;
            break;
        case 9:
            body[bodies_idx].color = TEAL;
            break;
    }
    lcd_draw_circle(body[bodies_idx].x, body[bodies_idx].y, BODY_RADIUS, body[bodies_idx].color);
    bodies_idx++;
}




void physics_draw_bodies(void){
    for (uint16_t i=0; i<bodies_idx; i++){
        lcd_draw_circle(body[i].x, body[i].y, BODY_RADIUS, body[i].color);
    }
}

void physics_erase_bodies(void){
    for (uint16_t i=0; i<bodies_idx; i++){
        lcd_draw_circle(body[i].x_prev, body[i].y_prev, BODY_RADIUS, BLACK);
    }
}

void physics_calculate_position(void){
    float temp_location_x, temp_location_y;
    uint16_t body_collision = 0;
    //uint16_t collision_flag = 0;
    //float y_eq, m_eq, x_eq, b_eq;

    // Detect wall collision
    for (uint16_t i=0; i<bodies_idx; i++){
        //body[i].y += 5;
        temp_location_x = (float)body[i].x + MOVE_DISTANCE*sin(sensor_data.acc.x_rad);
        temp_location_y = (float)body[i].y + GRAVITY*sin(sensor_data.acc.y_rad);
        
        if (temp_location_y-BODY_RADIUS < 1){             // breaching top wall
            temp_location_y = BODY_RADIUS;
        } else if (temp_location_y+BODY_RADIUS > 479){    // breaching bottom wall
            temp_location_y = 479-BODY_RADIUS;
        }

        if (temp_location_x-BODY_RADIUS < 1){             // breaching left wall
            temp_location_x = BODY_RADIUS;
        } else if (temp_location_x+BODY_RADIUS > 799){    // breaching right wall
            temp_location_x = 799-BODY_RADIUS;
        }

        body[i].x = temp_location_x;
        body[i].y = temp_location_y;
        if (physics_detect_body_collision(i, &body_collision)){
            delay_short();
            physics_calculate_direction_collision(i, body_collision);
        }
        //printf("X: %d    sin(x_rad): %0.3f  (x movement)\n\n", body[i].x, sin(sensor_data.acc.x_rad));
    }
}


//void physics_calculate_velocity(void){
//    for (uint16_t i=0; i<bodies_idx; i++){
//        //body[i].v   ;

//    }
//}


void physics_calculate_direction_collision(uint16_t body1, uint16_t body2){
    printf("body collision management: %d %d\n", body1, body2);
    printf("\tbody %d: ( %d , %d )\n\tbody %d: ( %d , %d )\n", body1, body[body1].x, body[body1].y, body2, body[body2].x, body[body2].y);
    float angle = atan2(body[body2].y - body[body1].y, body[body2].x - body[body1].x) * 180 / PI;
    printf("\tangle: %0.5f\n", angle);
    if (angle*PI/180.0 < 180.0 && angle*PI/180.0 > 0.0){
        if (body[body1].x > body[body2].x){
            if (body[body1].x < 799-BODY_RADIUS-MOVE_DISTANCE){
                body[body1].x += MOVE_DISTANCE*1;
            }
            if (body[body2].x > 1+BODY_RADIUS+MOVE_DISTANCE){
                body[body2].x -= MOVE_DISTANCE*1;
            }
        } else {
            if (body[body1].x > 1+BODY_RADIUS-MOVE_DISTANCE){
                body[body1].x -= MOVE_DISTANCE*1;
            }
            if (body[body2].x < 799-BODY_RADIUS+MOVE_DISTANCE){
                body[body2].x += MOVE_DISTANCE*1;
            }
        }
    } else {
        printf("...\n");
    }
    if (body[body1].x < BODY_RADIUS){
        body[body1].x = BODY_RADIUS;
    }
    if (body[body2].x < BODY_RADIUS){
        body[body2].x = BODY_RADIUS;
    }
        
    printf("\tangle: %0.5f\n", cos(angle));
    physics_erase_bodies();
    physics_draw_bodies();
    physics_new_to_old();
    
    physics_calculate_position();
    //body[body1].y = 
    //body[body2].x = 
    //body[body2].y = 


}


void physics_new_to_old(void){
    for (uint16_t i=0; i<bodies_idx; i++){
        body[i].x_prev = body[i].x;
        body[i].y_prev = body[i].y;
        body[i].v_prev = body[i].v;
    }
}


//void physics_detect_wall_collision(void){
//    for (uint16_t i=0; i<bodies_idx; i++){

//    }
//}

uint16_t physics_detect_body_collision(uint16_t body_src, uint16_t* colliding_body){
    for (uint16_t t=0; t<bodies_idx; t++){
        if (body_src != t){
            float distance = physics_distance(body[body_src].x, body[body_src].y, body[t].x, body[t].y);
            if (distance < 2*BODY_RADIUS){
                *colliding_body = t;
                return 1;
            }
        }
    }
    return 0;
}

void physics_interrupt_operations(void){
    acc_read_xyz();
    acc_calculate_xyz_angles();
    physics_new_to_old();
    physics_calculate_position();
    physics_erase_bodies();
    physics_draw_bodies();
}

void physics_run_app(void){
    // calculate trajectory / position
    // calculate / detect collision
    // bounce?
    
    // get acc data
    // move points
    // draw at some point
    acc_init();
    timer0_init();
    physics_add_body(400,240);
    physics_add_body(415,320);
    //physics_add_body(200,240);
    //physics_add_body(100,140);
    while(1){
        
    }
}
