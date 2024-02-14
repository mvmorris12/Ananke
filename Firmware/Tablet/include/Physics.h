#include <stdio.h>
#include <LPC177x_8x.h>

typedef struct physics_bodies_struct{
    uint16_t x; // x position
    uint16_t y; // y position
    float d;    // direction
    uint16_t v; // velocity
    uint16_t a; // acceleration
    uint16_t x_prev; // x position
    uint16_t y_prev; // y position
    float d_prev;    // direction
    uint16_t v_prev; // velocity
    uint16_t a_prev; // acceleration
    uint32_t color;
} physics_bodies_struct;


#define BODY_RADIUS     30
#define GRAVITY         2
#define MOVE_DISTANCE   2


void physics_run_app(void);
void physics_add_body(uint16_t x, uint16_t y);
void physics_new_to_old(void);
void physics_draw_bodies(void);
void physics_erase_bodies(void);
void physics_calculate_position(void);
void physics_calculate_velocity(void);
void physics_calculate_direction_collision(uint16_t body1, uint16_t body2);
void physics_interrupt_operations(void);
void physics_detect_wall_collision(void);
uint16_t physics_detect_body_collision(uint16_t body_src, uint16_t* colliding_body);
float physics_distance(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);