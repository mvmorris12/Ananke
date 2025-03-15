#include "Traffic.h"
#include <stdio.h>
#include <stdlib.h>
#include "LCD.h"
#include "Colors.h"
#include "Interrupts.h"
#include "Timer.h"


tr_road_t tr_roads;
tr_road roads_v[TR_VERTICAL_ROADS];
tr_road roads_h[TR_HORIZONTAL_ROADS];
tr_graph_node_t road_nodes_h[TR_HORIZONTAL_ROADS][TR_HORIZONTAL_NODES];
tr_graph_node_t road_nodes_v[TR_VERTICAL_ROADS][TR_VERTICAL_NODES];
tr_graph_node_t node_start;
tr_graph_node_t node_end;
tr_graph_t road_graph;
uint32_t tr_intersections[TR_VERTICAL_ROADS*TR_HORIZONTAL_ROADS][2];
tr_endpoints_t tr_endpoints_arr[TR_ENDPOINTS_CNT];
tr_lights_t tr_lights[TR_VERTICAL_ROADS*TR_HORIZONTAL_ROADS];
uint16_t tr_cost[2][TR_NODES_TOTAL_CNT];
uint16_t tr_queue_idx = 0;
uint16_t tr_node_idx = 0;
int8_t tr_direction_arr[4*3*2][4] = {-1,-1,-1,-1};
uint8_t skip_directions = FALSE;
uint8_t last_pct = 0;

typedef tr_queue_node_t tr_nodes_arr[TR_NODES_TOTAL_CNT];
tr_nodes_arr* tr_nodes = (tr_nodes_arr*)0xA1482000;
//tr_queue_node_t *tr_nodes[TR_NODES_TOTAL_CNT];
uint8_t directions_temp_cnt = 0;
int8_t directions_temp[TR_DIRECTIONS_CNT] = {-1};
int8_t directions[(TR_HORIZONTAL_ROADS+TR_VERTICAL_ROADS)*2][(TR_HORIZONTAL_ROADS+TR_VERTICAL_ROADS)*2][4*3*2][TR_DIRECTIONS_CNT] = {0};
uint8_t directions_cnt[(TR_HORIZONTAL_ROADS+TR_VERTICAL_ROADS)*2][(TR_HORIZONTAL_ROADS+TR_VERTICAL_ROADS)*2] = {0};
tr_car_queue_t car_queue_active;
tr_car_queue_t *car_queue_active_p = &car_queue_active;
tr_car_queue_t car_queue_free;
tr_car_queue_t *car_queue_free_p = &car_queue_free;
tr_car_queue_node_t car_queue_nodes[TR_MAX_CARS_CNT];
tr_car_t car_arr[TR_MAX_CARS_CNT];

uint32_t endpoint_colors[] = {WHITE, RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, ORANGE, OLIVE, PURPLE, TEAL, NAVY, GOLD, GRAY};

volatile uint8_t tr_ready = 0;
volatile uint8_t tr_current_frame = 0;

extern void delay(uint32_t i);
extern void delay_ms(uint32_t i);


/*
    move pieces
        for all cars
            if car is at dest:
                remove car
                run model calculations
            check if at intersection
                do something...
            check next position - see if can move
                if yes, set is_moving to true
                    --- add placeholder to next spot?
                set move position to 1
            if move position == n/2:
                --- remove placeholder and move car to next node
                increment position
            if move position == n:
                set move position to 0                        

        draw all cars:
            repeat n times:
                set tr_ready flag
                wait for lcd interrupt
                erase cars' old spots
                draw cars' new spots


*/

int8_t tr_is_next_spot_clear(tr_car_queue_node_t *rover){
    int8_t next_direction = rover->car->direction_driving_current;
    if ((rover->car->direction_step_current == 0) && (rover->car->current_node->is_intersection == TRUE)){  // check if car is at intersection
        next_direction = rover->car->directions[rover->car->direction_step_current++];                 // get next direction from list
        //TODO:
        /*  check lights
            set car to turning mode (create UDLR turning defines)
            continue with steps below
        */
    } 

    switch (next_direction){
        case TR_UP:
            if ((rover->car->current_node->up != NULL) && (rover->car->current_node->up->tr_car_spots.up == TR_NO_CAR)){
                rover->car->direction_driving_current = TR_UP;
                rover->car->current_node->up->tr_car_spots.up = TR_CAR_PLACEHOLDER;
                return TRUE;
            }
            break;
        case TR_DOWN:
            if ((rover->car->current_node->down != NULL) && (rover->car->current_node->down->tr_car_spots.down == TR_NO_CAR)){
                rover->car->direction_driving_current = TR_DOWN;
                rover->car->current_node->down->tr_car_spots.down = TR_CAR_PLACEHOLDER;
                return TRUE;
            }
            break;
        case TR_LEFT:
            if ((rover->car->current_node->left != NULL) && (rover->car->current_node->left->tr_car_spots.left == TR_NO_CAR)){
                rover->car->direction_driving_current = TR_LEFT;
                rover->car->current_node->left->tr_car_spots.left = TR_CAR_PLACEHOLDER;
                return TRUE;
            }
            break;
        case TR_RIGHT:
            if ((rover->car->current_node->right != NULL) && (rover->car->current_node->right->tr_car_spots.right == TR_NO_CAR)){
                rover->car->direction_driving_current = TR_RIGHT;
                rover->car->current_node->right->tr_car_spots.right = TR_CAR_PLACEHOLDER;
                return TRUE;
            }
            break;
    }

    return FALSE;
}


void tr_move_cars(void){
    tr_car_queue_node_t *rover = car_queue_active_p->front;
    while (rover != NULL){
        rover->car->time_active++;
        if ((rover->car->is_moving == TR_CAR_STOPPED) && (tr_is_next_spot_clear(rover))){
            rover->car->is_moving = TR_CAR_MOVING_START;
            //TODO clear / draw car function - switch with algorith to move car in each direction + turning
        }
        if (rover->car->current_node->id == tr_endpoints_arr[rover->car->destination].endpoint->id){
            //TODO 
            /*  add car time_active to machine learning model
                clear car model
                dequeue car from queue
                ...?
            */
        }
    }
}



void tr_car_enqueue(tr_car_queue_t *queue, tr_car_queue_node_t *node){
    if (queue->back != NULL){
        queue->back->next = node;
        node->prev = queue->back;
        queue->back = node;
        node->next = NULL;
    } else {
        queue->front = node;
        queue->back = node;
        node->next = NULL;
        node->prev = NULL;
    }
    queue->items++;
}


tr_car_queue_node_t *tr_car_dequeue(tr_car_queue_t *queue, uint16_t idx){
    tr_car_queue_node_t *rover = queue->front;
    if (idx != 0xFFFF){
        while (rover->car->id != idx){
            rover = rover->next;
            if (rover == NULL){
                return NULL;
            }
        }
    }
    if (rover->prev != NULL){
        rover->prev->next = rover->next;
    } else {
        queue->front = rover->next;
    }
    if (rover->next != NULL){
        rover->next->prev = rover->prev;
    } else {
        queue->back = rover->prev;
    }
    queue->items--;
    rover->next = NULL;
    rover->prev = NULL;
    return rover;
}


void tr_car_activate(void){
    if (car_queue_free_p->items){
        tr_car_queue_node_t *car_p = tr_car_dequeue(car_queue_free_p, 0xFFFF);
        tr_randomize_car(car_p);
        tr_car_enqueue(car_queue_active_p, car_p);
    }
}


void tr_car_deactivate(tr_car_queue_node_t *car_p){
    tr_car_enqueue(car_queue_free_p, tr_car_dequeue(car_queue_active_p, car_p->car->id));
}


void tr_randomize_car(tr_car_queue_node_t *car_p){
    uint32_t random_seed = ((LPC_RTC->SEC+1) * (LPC_RTC->MIN+1) * (LPC_RTC->HOUR+1) * (rand() % 1000));
    srand(random_seed);
    uint8_t car_spots_init[TR_ENDPOINTS_CNT] = {0};

    while(1){
        car_p->car->start = rand() % TR_ENDPOINTS_CNT;
        car_p->car->direction_driving_current = tr_endpoints_arr[car_p->car->start].start_direction;
        car_p->car->current_node = tr_endpoints_arr[car_p->car->start].endpoint;
        if (car_spots_init[car_p->car->start] == 0){
            if (car_p->car->direction_driving_current == TR_UP && car_p->car->current_node->tr_car_spots.up == TR_NO_CAR){
                break;
            } else if (car_p->car->direction_driving_current == TR_DOWN && car_p->car->current_node->tr_car_spots.down == TR_NO_CAR){
                break;
            } else if (car_p->car->direction_driving_current == TR_LEFT && car_p->car->current_node->tr_car_spots.left == TR_NO_CAR){
                break;
            } else if (car_p->car->direction_driving_current == TR_RIGHT && car_p->car->current_node->tr_car_spots.right == TR_NO_CAR){
                break;
            }
        }
        car_p->car->directions = &(directions[car_p->car->start][car_p->car->destination][rand() % (4*3*2)][0]);
        car_spots_init[car_p->car->start] = 1;
        //uint8_t car_spots_free = 0;
        //for (int i=0; i<TR_ENDPOINTS_CNT; i++){
        //    if (car_spots_init[i] == 0){
        //        car_spots_free = 1;
        //    }
        //}
        //if (car_spots_free == 0){
        //    return;
        //}
    }

    car_p->car->destination =  car_p->car->start;

    while (car_p->car->destination == car_p->car->start){
        car_p->car->destination = rand() % TR_ENDPOINTS_CNT;
    }

    car_p->car->color = tr_endpoints_arr[car_p->car->destination].color;
}


void tr_initialize_cars(void){
    for (int i=0; i<TR_MAX_CARS_CNT; i++){
        car_arr[i].color = 0;
        car_arr[i].direction_step_current = 0;
        car_arr[i].direction_driving_current = 0;
        car_arr[i].directions = NULL;
        car_arr[i].current_node = NULL;
        car_arr[i].start = 0;
        car_arr[i].destination = 0;
        car_arr[i].time_active = 0;

        car_queue_nodes[i].car = &(car_arr[i]);
        if (i>0){
            car_queue_nodes[i].prev = &(car_queue_nodes[i-1]);
        } else {
            car_queue_nodes[i].prev = NULL;
        }
         
        if (i<TR_MAX_CARS_CNT-1){
            car_queue_nodes[i].next = &(car_queue_nodes[i+1]);
        } else {
            car_queue_nodes[i].next = NULL;
        }

    }

    car_queue_active.front = NULL;
    car_queue_active.back = NULL;
    car_queue_active.items = 0;

    car_queue_free.front = &(car_queue_nodes[0]);
    car_queue_free.back = &(car_queue_nodes[TR_MAX_CARS_CNT-1]);
    car_queue_free.items = TR_MAX_CARS_CNT;
}


void tr_create_roads(void){
    tr_roads.horizontal = &(roads_h[0]);
    tr_roads.vertical = &(roads_v[0]);
}


//TODO check this parameter is working correctly
void tr_generate_direction_arrs(int8_t (*tr_dir_arr)[24][4]){ 
    memset(directions_temp, (-1), TR_DIRECTIONS_CNT*sizeof(char));
    memcpy(tr_direction_arr[0],  (uint8_t [4]){0,1,2,3}, 4*sizeof(uint8_t));
    memcpy(tr_direction_arr[1],  (uint8_t [4]){0,1,3,2}, 4*sizeof(uint8_t));
    memcpy(tr_direction_arr[2],  (uint8_t [4]){0,2,1,3}, 4*sizeof(uint8_t));
    memcpy(tr_direction_arr[3],  (uint8_t [4]){0,2,3,1}, 4*sizeof(uint8_t));
    memcpy(tr_direction_arr[4],  (uint8_t [4]){0,3,1,2}, 4*sizeof(uint8_t));
    memcpy(tr_direction_arr[5],  (uint8_t [4]){0,3,2,1}, 4*sizeof(uint8_t));
    memcpy(tr_direction_arr[6],  (uint8_t [4]){1,0,2,3}, 4*sizeof(uint8_t));
    memcpy(tr_direction_arr[7],  (uint8_t [4]){1,0,3,2}, 4*sizeof(uint8_t));
    memcpy(tr_direction_arr[8],  (uint8_t [4]){1,2,0,3}, 4*sizeof(uint8_t));
    memcpy(tr_direction_arr[9],  (uint8_t [4]){1,2,3,0}, 4*sizeof(uint8_t));
    memcpy(tr_direction_arr[10], (uint8_t [4]){1,3,0,2}, 4*sizeof(uint8_t));
    memcpy(tr_direction_arr[11], (uint8_t [4]){1,3,2,0}, 4*sizeof(uint8_t));
    memcpy(tr_direction_arr[12], (uint8_t [4]){2,0,1,3}, 4*sizeof(uint8_t));
    memcpy(tr_direction_arr[13], (uint8_t [4]){2,0,3,1}, 4*sizeof(uint8_t));
    memcpy(tr_direction_arr[14], (uint8_t [4]){2,1,0,3}, 4*sizeof(uint8_t));
    memcpy(tr_direction_arr[15], (uint8_t [4]){2,1,3,0}, 4*sizeof(uint8_t));
    memcpy(tr_direction_arr[16], (uint8_t [4]){2,3,0,1}, 4*sizeof(uint8_t));
    memcpy(tr_direction_arr[17], (uint8_t [4]){2,3,1,0}, 4*sizeof(uint8_t));
    memcpy(tr_direction_arr[18], (uint8_t [4]){3,0,1,2}, 4*sizeof(uint8_t));
    memcpy(tr_direction_arr[19], (uint8_t [4]){3,0,2,1}, 4*sizeof(uint8_t));
    memcpy(tr_direction_arr[20], (uint8_t [4]){3,1,0,2}, 4*sizeof(uint8_t));
    memcpy(tr_direction_arr[21], (uint8_t [4]){3,1,2,0}, 4*sizeof(uint8_t));
    memcpy(tr_direction_arr[22], (uint8_t [4]){3,2,0,1}, 4*sizeof(uint8_t));
    memcpy(tr_direction_arr[23], (uint8_t [4]){3,2,1,0}, 4*sizeof(uint8_t));

    for (int i=0; i<(TR_HORIZONTAL_ROADS+TR_VERTICAL_ROADS)*2; i++){
        for (int j=0; j<(TR_HORIZONTAL_ROADS+TR_VERTICAL_ROADS)*2; j++){
            for (int k=0; k<4*3*2; k++){
                for (int l=0; l<TR_DIRECTIONS_CNT; l++){
                    directions[i][j][k][l] = -1;
                }
            }
        }
    }
}


void tr_reset_costs(void){
    for (uint16_t i=0; i<TR_NODES_TOTAL_CNT; i++){
        tr_cost[0][i] = 0;
        tr_cost[1][i] = 0xFFFF;
    }
}


void tr_reset_nodes_visited(void){
    for (uint8_t i=0; i<TR_VERTICAL_ROADS; i++){
        for (uint16_t j=0; j<TR_VERTICAL_NODES; j++){
            road_nodes_v[i][j].visited = FALSE;
        }
    }
    for (uint8_t i=0; i<TR_HORIZONTAL_ROADS; i++){
        for (uint16_t j=0; j<TR_HORIZONTAL_NODES; j++){
            road_nodes_h[i][j].visited = FALSE;
        } 
    }  
}


void tr_add_endpoint(tr_graph_node_t *node, int *endpoint_cnt_p){
    tr_endpoints_arr[*endpoint_cnt_p].endpoint = node;
    tr_endpoints_arr[*endpoint_cnt_p].color = endpoint_colors[*endpoint_cnt_p];
    if (node->up != NULL){
        tr_endpoints_arr[*endpoint_cnt_p].start_direction = TR_UP;
    } else if (node->down != NULL){
        tr_endpoints_arr[*endpoint_cnt_p].start_direction = TR_DOWN;
    } else if (node->left != NULL){
        tr_endpoints_arr[*endpoint_cnt_p].start_direction = TR_LEFT;
    } else if (node->right != NULL){
        tr_endpoints_arr[*endpoint_cnt_p].start_direction = TR_RIGHT;
    }
    (*endpoint_cnt_p)++;
}


void tr_find_endpoints(void){
    int endpoint_cnt = 0;
    int *endpoint_cnt_p = &endpoint_cnt;
    for (int i=0; i<TR_HORIZONTAL_ROADS; i++){
        tr_add_endpoint(&(road_nodes_h[i][0]), endpoint_cnt_p);
        tr_add_endpoint(&(road_nodes_h[i][TR_HORIZONTAL_NODES-1]), endpoint_cnt_p);
    }
    for (int i=0; i<TR_VERTICAL_ROADS; i++){
        tr_add_endpoint(&(road_nodes_v[i][0]), endpoint_cnt_p);
        tr_add_endpoint(&(road_nodes_v[i][TR_VERTICAL_NODES-1]), endpoint_cnt_p);
    }
}


void tr_find_intersections(void){
    uint8_t intersection_cnt = 0;
    for (int i=0; i<TR_HORIZONTAL_ROADS; i++){
        for (int j=0; j<TR_VERTICAL_ROADS; j++){
            for (int k=0; k<TR_HORIZONTAL_NODES; k++){
                for (int l=0; l<TR_VERTICAL_NODES; l++){
                    if (road_nodes_h[i][k].tr_coords.col == road_nodes_v[j][l].tr_coords.col &&
                        road_nodes_h[i][k].tr_coords.row == road_nodes_v[j][l].tr_coords.row){

                        //lcd_draw_square(road_nodes_h[i][k].tr_coords.row, road_nodes_h[i][k].tr_coords.col, 5, RED);

                        tr_lights[intersection_cnt].up = RED;
                        tr_lights[intersection_cnt].down = RED;
                        tr_lights[intersection_cnt].left = RED;
                        tr_lights[intersection_cnt].right = RED;

                        road_nodes_h[i][k].is_intersection = TRUE;
                        road_nodes_v[j][l].is_intersection = TRUE;

                        road_nodes_h[i][k].down = &(road_nodes_v[j][l+1]);
                        road_nodes_h[i][k].up = &(road_nodes_v[j][l-1]);
                        
                        road_nodes_v[j][l-1].down = &(road_nodes_h[i][k]);
                        road_nodes_v[j][l+1].up = &(road_nodes_h[i][k]);

                        road_nodes_h[i][k-3].is_intersection = TR_PREINTERSECTION;
                        road_nodes_h[i][k+3].is_intersection = TR_PREINTERSECTION;
                        road_nodes_v[j][l-3].is_intersection = TR_PREINTERSECTION;
                        road_nodes_v[j][l+3].is_intersection = TR_PREINTERSECTION;

                        tr_intersections[intersection_cnt][0] = road_nodes_v[j][l].tr_coords.row;
                        tr_intersections[intersection_cnt][1] = road_nodes_v[j][l].tr_coords.col;
                        intersection_cnt++;
                    }
                }
            }
        }
    }
}


void tr_clear_intersections(void){
    for (int i=0; i<TR_HORIZONTAL_ROADS*TR_VERTICAL_ROADS; i++){
        lcd_draw_rectangle( 
            (uint16_t)tr_intersections[i][1]-(TR_ROAD_WIDTH/2-TR_SHOULDER_LINE_WIDTH),
            (uint16_t)tr_intersections[i][0]-(TR_ROAD_WIDTH/2),
            (uint16_t)tr_intersections[i][1]+(TR_ROAD_WIDTH/2-TR_SHOULDER_LINE_WIDTH), 
            (uint16_t)tr_intersections[i][0]+(TR_ROAD_WIDTH/2),
            1, 1, GRAY_DARKER);
    }
}


void tr_draw_traffic_lights(void){
    for (int i=0; i<TR_VERTICAL_ROADS*TR_HORIZONTAL_ROADS; i++){
        //lcd_draw_circle(100, 100, 5, GREEN_DARK, 1, GREEN);
        int x = tr_intersections[i][0];
        int y = tr_intersections[i][1];
        lcd_draw_rectangle(y-((TR_ROAD_WIDTH/2)+TR_LIGHTS_DEPTH+TR_LIGHTS_OFFSET), x-((TR_ROAD_WIDTH/2)+TR_LIGHTS_WIDTH), y-((TR_ROAD_WIDTH/2)+TR_LIGHTS_OFFSET), x-((TR_ROAD_WIDTH/2)), 1, 1, tr_lights[i].down);
        lcd_draw_rectangle(y+((TR_ROAD_WIDTH/2)+TR_LIGHTS_OFFSET), x+((TR_ROAD_WIDTH/2)), y+((TR_ROAD_WIDTH/2)+TR_LIGHTS_DEPTH+TR_LIGHTS_OFFSET), x+((TR_ROAD_WIDTH/2)+TR_LIGHTS_WIDTH), 1, 1, tr_lights[i].up);
        lcd_draw_rectangle(y+((TR_ROAD_WIDTH/2)), x-((TR_ROAD_WIDTH/2)+TR_LIGHTS_OFFSET+TR_LIGHTS_DEPTH), y+((TR_ROAD_WIDTH/2)+TR_LIGHTS_WIDTH), x-((TR_ROAD_WIDTH/2)+TR_LIGHTS_OFFSET), 1, 1, tr_lights[i].right);
        lcd_draw_rectangle(y-((TR_ROAD_WIDTH/2)+TR_LIGHTS_WIDTH), x+((TR_ROAD_WIDTH/2)+TR_LIGHTS_OFFSET), y-((TR_ROAD_WIDTH/2)), x+((TR_ROAD_WIDTH/2)+TR_LIGHTS_OFFSET+TR_LIGHTS_DEPTH), 1, 1, tr_lights[i].left);   
    }
}


void tr_draw_nodes(void){
    tr_graph_node_t *rover;
    for (int i=0; i<TR_HORIZONTAL_ROADS; i++){
        rover = road_nodes_h[0];
        while (rover != NULL){
            if (rover->is_intersection == TRUE){
                lcd_draw_square(rover->tr_coords.row, rover->tr_coords.col, 4, CYAN);
            } else if (rover->is_intersection == 2){
                lcd_draw_square(rover->tr_coords.row, rover->tr_coords.col, 4, RED);
            } else {
                lcd_draw_square(rover->tr_coords.row, rover->tr_coords.col, 4, WHITE);
            }
            rover = rover->right;
        }
    }
    for (int i=0; i<TR_VERTICAL_ROADS; i++){
        rover = road_nodes_v[0];
        while (rover != NULL){
            if (rover->is_intersection == TRUE){
                lcd_draw_square(rover->tr_coords.row, rover->tr_coords.col, 4, CYAN);
            } else if (rover->is_intersection == 2){
                lcd_draw_square(rover->tr_coords.row, rover->tr_coords.col, 4, RED);
            } else {
                lcd_draw_square(rover->tr_coords.row, rover->tr_coords.col, 4, WHITE);
            }
            rover = rover->down;
        }
    }
}


void tr_shuffle_roads(void){
    //uint32_t random_seed = ((LPC_RTC->SEC+1) * (LPC_RTC->MIN+1) * (LPC_RTC->HOUR+1) * (rand() % 1000));
    //printf("random_seed: %d\n", random_seed);
    //srand(random_seed);
    //for (int row=0; row<PF_ROWS; row++){
    //    for (int col=0; col<PF_COLS; col++){
    //        if (rand() % 100 < 25){
    //            ((pf_graph_nodes)[row][col]).enabled = FALSE;
    //        } else {
    //            ((pf_graph_nodes)[row][col]).enabled = TRUE;
    //        }
    //    }
    //}
    tr_graph_node_t *rover_prev; 
    tr_graph_node_t *rover_curr; 
    uint16_t j = 0;
    for (uint16_t i=0; i<TR_HORIZONTAL_ROADS; i++){
        tr_roads.horizontal[i].x_start = 40;
        tr_roads.horizontal[i].y_start = 140+(i*200);
        tr_roads.horizontal[i].x_end = 760;
        tr_roads.horizontal[i].y_end = 140+(i*200);
        for (uint16_t j=0; j<TR_HORIZONTAL_NODES; j++){
            road_nodes_h[i][j].up = NULL;
            road_nodes_h[i][j].down = NULL;
            if (j==0){
                road_nodes_h[i][j].left = NULL;     
            } else {
                road_nodes_h[i][j].left = &(road_nodes_h[i][j-1]);
            }
            if (j<TR_HORIZONTAL_NODES-1){
                road_nodes_h[i][j].right = &(road_nodes_h[i][j+1]);
            } else {
                road_nodes_h[i][j].right = NULL;
            }
            road_nodes_h[i][j].tr_coords.row=40+j*TR_HORIZONTAL_NODES_SPACING;
            road_nodes_h[i][j].tr_coords.col=140+(i*200);
            road_nodes_h[i][j].is_intersection = FALSE;
            road_nodes_h[i][j].tr_car_spots.up = TR_NO_CAR;
            road_nodes_h[i][j].tr_car_spots.down = TR_NO_CAR;
            road_nodes_h[i][j].tr_car_spots.left = TR_NO_CAR;
            road_nodes_h[i][j].tr_car_spots.right = TR_NO_CAR;
            road_nodes_h[i][j].id = tr_node_idx++;
        }
    }

    for (int i=0; i<TR_VERTICAL_ROADS; i++){
        tr_roads.vertical[i].x_start = 200+(i*360);
        tr_roads.vertical[i].y_start = 40;
        tr_roads.vertical[i].x_end = 200+(i*360);
        tr_roads.vertical[i].y_end = 440;
        for (uint16_t j=0; j<TR_VERTICAL_NODES; j++){
            road_nodes_v[i][j].left = NULL;
            road_nodes_v[i][j].right = NULL;
            if (j==0){
                road_nodes_v[i][j].up = NULL;     
            } else {
                road_nodes_v[i][j].up = &(road_nodes_v[i][j-1]);
            }
            if (j<TR_VERTICAL_NODES-1){
                road_nodes_v[i][j].down = &(road_nodes_v[i][j+1]);
            } else {
                road_nodes_v[i][j].down = NULL;
            }
            road_nodes_v[i][j].tr_coords.row=200+(i*360);
            road_nodes_v[i][j].tr_coords.col=40+j*TR_VERTICAL_NODES_SPACING;
            road_nodes_v[i][j].is_intersection = FALSE;
            road_nodes_v[i][j].tr_car_spots.up = TR_NO_CAR;
            road_nodes_v[i][j].tr_car_spots.down = TR_NO_CAR;
            road_nodes_v[i][j].tr_car_spots.left = TR_NO_CAR;
            road_nodes_v[i][j].tr_car_spots.right = TR_NO_CAR;
            road_nodes_v[i][j].id = tr_node_idx++;
        }
    }
}


void tr_draw_roads(void){
    uint16_t x0,y0,x1,y1;

    for (uint8_t i=0; i<TR_HORIZONTAL_ROADS; i++){
        x0 = tr_roads.horizontal[i].x_start;
        y0 = tr_roads.horizontal[i].y_start;
        x1 = tr_roads.horizontal[i].x_end;
        y1 = tr_roads.horizontal[i].y_end;
        lcd_draw_rectangle(y0-(TR_ROAD_WIDTH/2),x0-TR_ROAD_OFFSET,y1+(TR_ROAD_WIDTH/2),x1+TR_ROAD_OFFSET,1,1,GRAY_DARKER);
        lcd_draw_rectangle(y0-(TR_ROAD_WIDTH/2),x0,y1-(TR_ROAD_WIDTH/2-TR_SHOULDER_LINE_WIDTH),x1,1,1,GRAY_DARK);
        lcd_draw_rectangle(y0-(TR_CENTER_LINE_SPACING+TR_CENTER_LINE_WIDTH),x0,y1-TR_CENTER_LINE_SPACING,x1,1,1,GOLD);
        lcd_draw_rectangle(y0+(TR_CENTER_LINE_SPACING),x0,y1+(TR_CENTER_LINE_SPACING+TR_CENTER_LINE_WIDTH),x1,1,1,GOLD);
        lcd_draw_rectangle(y0+(TR_ROAD_WIDTH/2-TR_SHOULDER_LINE_WIDTH),x0,y1+(TR_ROAD_WIDTH/2),x1,1,1,GRAY_DARK);
    }

    for (uint8_t i=0; i<TR_VERTICAL_ROADS; i++){
        x0 = tr_roads.vertical[i].x_start;
        y0 = tr_roads.vertical[i].y_start;
        x1 = tr_roads.vertical[i].x_end;
        y1 = tr_roads.vertical[i].y_end;
        lcd_draw_rectangle(y0-TR_ROAD_OFFSET,x0-(TR_ROAD_WIDTH/2),y1+TR_ROAD_OFFSET,x1+(TR_ROAD_WIDTH/2),1,1,GRAY_DARKER);
        lcd_draw_rectangle(y0,x0-(TR_ROAD_WIDTH/2),y1,x1-(TR_ROAD_WIDTH/2-TR_SHOULDER_LINE_WIDTH),1,1,GRAY_DARK);
        lcd_draw_rectangle(y0,x0-(TR_CENTER_LINE_SPACING+TR_CENTER_LINE_WIDTH),y1,x1-(TR_CENTER_LINE_SPACING),1,1,GOLD);
        lcd_draw_rectangle(y0,x0+(TR_CENTER_LINE_SPACING),y1,x1+(TR_CENTER_LINE_SPACING+TR_CENTER_LINE_WIDTH),1,1,GOLD);;
        lcd_draw_rectangle(y0,x0+(TR_ROAD_WIDTH/2-TR_SHOULDER_LINE_WIDTH),y1,x1+(TR_ROAD_WIDTH/2),1,1,GRAY_DARK);
    }
}


void tr_enqueue(tr_queue_t *queue, tr_queue_node_t *nodes, uint16_t *idx, tr_graph_node_t *from_p, tr_graph_node_t *to_p,  uint16_t cost){ 
        
    nodes[*idx].from_p = from_p;
    nodes[*idx].to_p = to_p;
    nodes[*idx].cost = cost;
    nodes[*idx].next = NULL;
    
    tr_queue_node_t *rover = queue->front;

    if (rover == NULL){
        queue->front = &nodes[*idx];
    } else {
        while (rover->next != NULL){
            rover = rover->next;
        }
        rover->next = &nodes[*idx];
    }
    queue->items++;
    (*idx)++;
}


uint16_t tr_dequeue(tr_queue_t *queue, uint8_t nav_mode){
    if (queue->front != NULL){

        if (queue->front->cost < tr_cost[1][queue->front->to_p->id]){           //TODO step through / check ->id stuff
            tr_cost[0][queue->front->to_p->id] = queue->front->from_p->id;
            tr_cost[1][queue->front->to_p->id] = queue->front->cost;
            (*(tr_nodes)[queue->front->to_p->id]).cost = queue->front->cost;
        }
        tr_graph_node_t *new_start = queue->front->to_p;
        uint16_t next_cost = queue->front->cost+1;
        queue->front = queue->front->next;
        queue->items--;
        if (new_start->visited == FALSE){
            //nav_mode = 
            if (tr_core(new_start, queue, next_cost, nav_mode)){
                return 1;
            }
        }
    }
    return 0;
}


uint16_t tr_core(tr_graph_node_t *start, tr_queue_t *queue, uint16_t next_cost, uint8_t nav_mode){
    
    start->visited = TRUE;
    
    if (queue->end->visited == FALSE){
        for (uint8_t j=0; j<4; j++){
            switch(tr_direction_arr[nav_mode][j]){
                case TR_LEFT:                  
                    if (start->left != NULL && start->left->visited == FALSE){
                        tr_enqueue(queue, *tr_nodes, &tr_queue_idx, start, start->left, next_cost);
                    }
                    break;
                case TR_RIGHT:
                    if (start->right != NULL && start->right->visited == FALSE){
                        tr_enqueue(queue, *tr_nodes, &tr_queue_idx, start, start->right, next_cost);
                    }
                    break;
                case TR_UP:
                    if (start->up != NULL && start->up->visited == FALSE){
                        tr_enqueue(queue, *tr_nodes, &tr_queue_idx, start, start->up, next_cost);
                    }
                    break;
                case TR_DOWN:
                    if (start->down != NULL && start->down->visited == FALSE){
                        tr_enqueue(queue, *tr_nodes, &tr_queue_idx, start, start->down, next_cost);
                    }
                    break;
            }
        }
    } else {
        //printf("found path!\n");
        while (queue->front != NULL){        
            queue->front = queue->front->next;
            queue->items--;
        }
        return 1;
    }
    return 0;
}


void tr_find_shortest_path(tr_graph_node_t *start, tr_graph_node_t *end, uint8_t nav_mode){
    tr_queue_t queue;
    queue.back = NULL;
    queue.front = NULL;
    queue.end = end;
    tr_queue_t *queue_p = &queue;
    queue.items = 0;
    tr_queue_node_t *rover;
    tr_queue_node_t *node_p = &((*tr_nodes)[0]);
    tr_queue_idx = 0;

    node_start = *start;
    node_end = *end;

    for (int i=0; i<TR_NODES_TOTAL_CNT; i++){
        ((*tr_nodes)[i]).cost = 0;
        ((*tr_nodes)[i]).next = NULL;
        ((*tr_nodes)[i]).from_p = NULL;
        ((*tr_nodes)[i]).to_p = NULL;
    }
    for (uint16_t i=0; i<TR_NODES_TOTAL_CNT; i++){
        tr_cost[0][i] = 0;
        tr_cost[1][i] = 0xFFFF;
    }
    
    uint16_t idx = 0;

    tr_cost[1][start->id] = 0;

    //start->enabled = TRUE;
    //end->enabled = TRUE;
    //pf_highlight_toggle_node(start, CYAN, WHITE);
    //pf_highlight_toggle_node(end, GREEN_DARK, GREEN);
    //printf("going from: %d  to  %d\n", start->id, end->id);
    //printf("going from: (%d, %d)  to  (%d, %d)\n", start->tr_coords.row, start->tr_coords.col, end->tr_coords.row, end->tr_coords.col);

    if (tr_core(start, queue_p, 0, nav_mode)){
        printf("here\n");
    }

    while (queue.items){
        if (tr_dequeue(queue_p, nav_mode)){
            //printf("found shortest path\n");
            tr_draw_shortest_path(start, end);
        }
    }
    //delay(3000);
    //printf("done\n");
}


void tr_process_direction_arr(int i, int j){
    char dir_temp = directions_temp[0];
    uint8_t array_depth = directions_cnt[i][j];

    memcpy((void*)&(directions[i][j][array_depth]), (void*)&(directions_temp), TR_DIRECTIONS_CNT*sizeof(char));
    memset(directions_temp, (-1), TR_DIRECTIONS_CNT*sizeof(char));
    directions_temp_cnt = 0;
    //for (int k=TR_DIRECTIONS_CNT-1; k>= 0; k--){
    //    directions[i][j][array_depth][TR_DIRECTIONS_CNT-1-k] = directions_temp[k];
    //    directions_temp[k] = -1;
    //}
    //directions_temp_cnt = 0;

    //for (int k=0; k<TR_DIRECTIONS_CNT-1; k++){
    //    while(directions[i][j][array_depth][0] == -1){
    //        for (int l=k; l<TR_DIRECTIONS_CNT-1; l++){
    //            directions[i][j][array_depth][l] = directions[i][j][array_depth][l+1];
    //            directions[i][j][array_depth][l+1] = -1;
    //        }
    //    }
    //}
    directions_cnt[i][j]++;
}


int8_t tr_build_direction_arr(uint8_t current_dir, tr_graph_node_t *rover){
    //if (last_dir != current_dir){
    if (rover->is_intersection == TRUE && skip_directions == FALSE){
        skip_directions = TRUE;
        switch(current_dir){
            case TR_UP:
                //printf("U\n");
                directions_temp[directions_temp_cnt++] = TR_UP;
                break;
            case TR_DOWN:
                //printf("D\n");
                directions_temp[directions_temp_cnt++] = TR_DOWN;
                break;
            case TR_LEFT:
                //printf("L\n");
                directions_temp[directions_temp_cnt++] = TR_LEFT;
                break;
            case TR_RIGHT:
                //printf("R\n");
                directions_temp[directions_temp_cnt++] = TR_RIGHT;
                break;
        }
    } else {
        skip_directions = FALSE;
    }
    return current_dir;
}


void tr_draw_shortest_path(tr_graph_node_t *start, tr_graph_node_t *end){
    uint16_t next_id = tr_cost[0][end->id];
    tr_graph_node_t *rover = end;

    int8_t last_dir = -1;


    //lcd_draw_square(start->tr_coords.row, start->tr_coords.col, 4, BLUE);
    //lcd_draw_square(end->tr_coords.row, end->tr_coords.col, 4, GREEN);


    while (rover->id != start->id){
        //printf("%d  %d\n", rover->id, rover->enabled);
        if (rover->left != NULL && rover->left->id == next_id){
            //tr_highlight_toggle_connection(rover, TR_LEFT);
            last_dir = tr_build_direction_arr(TR_LEFT, rover);
            rover = rover->left;
        } else if (rover->up != NULL && rover->up->id == next_id){
            //tr_highlight_toggle_connection(rover, TR_UP);
            last_dir = tr_build_direction_arr(TR_UP, rover);
            rover = rover->up;
        } else if (rover->right != NULL && rover->right->id == next_id){
            //tr_highlight_toggle_connection(rover, TR_RIGHT);
            last_dir = tr_build_direction_arr(TR_RIGHT, rover);
            rover = rover->right;
        } else if (rover->down != NULL && rover->down->id == next_id){
            //tr_highlight_toggle_connection(rover, TR_DOWN);
            last_dir = tr_build_direction_arr(TR_DOWN, rover);
            rover = rover->down;
        } else {
            printf("path error\n");
            return;
        }
        next_id = tr_cost[0][rover->id];        
    }
}


void tr_highlight_toggle_connection(tr_graph_node_t *node, uint8_t direction){
    if (node == NULL){
        return;
    }
    uint32_t color;
    switch(direction){
        case TR_UP:
            if (node->up != NULL){
                lcd_draw_line(node->tr_coords.row, node->tr_coords.col, node->up->tr_coords.row, node->up->tr_coords.col,TR_SHORT_PATH_WIDTH,TR_SHORT_PATH_COLOR_U);
            }
            break;
        
        case TR_DOWN:
            if (node->down != NULL){
                lcd_draw_line(node->tr_coords.row, node->tr_coords.col, node->down->tr_coords.row, node->down->tr_coords.col,TR_SHORT_PATH_WIDTH,TR_SHORT_PATH_COLOR_D);
            }
            break;
        
        case TR_LEFT:
            if (node->left != NULL){
                lcd_draw_line(node->tr_coords.row, node->tr_coords.col, node->left->tr_coords.row, node->left->tr_coords.col,TR_SHORT_PATH_WIDTH,TR_SHORT_PATH_COLOR_L);
            }
            break;

        case TR_RIGHT:
            if (node->right != NULL){
                lcd_draw_line(node->tr_coords.row, node->tr_coords.col, node->right->tr_coords.row, node->right->tr_coords.col,TR_SHORT_PATH_WIDTH,TR_SHORT_PATH_COLOR_R);
            }
            break;
    }
}


void tr_draw_loading_bar(uint16_t paths_completed, uint8_t init){
    char txt_percent[] = "0.0%";
    char *txt_percent_p = &(txt_percent[0]);
    if (init == 2){
        lcd_draw_rectangle(160, 325, 260, 475, 1, 1, BLACK);
        return;
    }
    if (init == 1){
        lcd_draw_rectangle(160, 325, 260, 475, 1, 1, GRAY_DARKER);
        char txt_loading[] = "Loading";
        char *txt_loading_p = &(txt_loading[0]);
        lcd_draw_text(txt_loading_p, 355, 170, CYAN);
        lcd_draw_text(txt_percent_p, 365, 200, CYAN);
    }
    uint8_t pct_current = floor((float)paths_completed/TR_PATHS_TOTAL_CNT*100.0);
    sprintf(txt_percent, "%0.1f%%", (float)paths_completed/TR_PATHS_TOTAL_CNT*100.0);
    lcd_draw_rectangle(200, 360, 220, 440, 1, 1, GRAY_DARKER);
    lcd_draw_text(txt_percent_p, 360, 200, GREEN);
    if (pct_current > last_pct){
        lcd_draw_line(350+pct_current-1, 240, 350+pct_current, 240, 5, GREEN);
        last_pct = pct_current;
    }
}



void print_color_name(uint32_t hex_color){
    switch(hex_color){
        case WHITE:
            printf("White\n");
            break;
        case RED:
            printf("Red\n");
            break;
        case GREEN:
            printf("Green\n");
            break;
        case BLUE:
            printf("Blue\n");
            break;
        case YELLOW:
            printf("Yellow\n");
            break;
        case CYAN:
            printf("Cyan\n");
            break;
        case MAGENTA:
            printf("Magenta\n");
            break;
        case MAROON:
            printf("Maroon\n");
            break;
        case ORANGE:
            printf("Orange\n");
            break;
        case OLIVE:
            printf("Olive\n");
            break;
        case PURPLE:
            printf("Purple\n");
            break;
        case TEAL:
            printf("Teal\n");
            break;
        case NAVY:
            printf("Navy\n");
            break;
        case GOLD:
            printf("Gold\n");
            break;
        case GRAY:
            printf("Gray\n");
            break;
    }
}


char get_direction_letter(int8_t dir){
    switch (dir){
        case TR_UP:
            return 'U';
        case TR_DOWN:
            return 'D';
        case TR_LEFT:
            return 'L';
        case TR_RIGHT:
            return 'R';
        default:
            return '.';
    }
}


void print_car_info(tr_car_t* car){
    printf("------- Car info -------\n");
    printf("      Start: %d\n", car->start);
    printf("        End: %d\n", car->destination);
    printf("    Dir cur: %d\n", car->direction_driving_current);
    printf(" Directions: ");
    for (int i=0; i<TR_DIRECTIONS_CNT; i++){
        printf("%c ", get_direction_letter(car->directions[i]));
    }
    printf("\n");
    printf("   Step cur: %d\n", car->direction_step_current);
    printf("  is moving: %d\n", car->is_moving);
    printf("time active: %d\n", car->time_active);
    printf("      Color: ");
    print_color_name(car->color);
    
    lcd_draw_square(car->current_node->tr_coords.row, car->current_node->tr_coords.col, 5, car->color);
    
}


void draw_endpoint_square(uint8_t endpoint_num){
    lcd_draw_square(tr_endpoints_arr[endpoint_num].endpoint->tr_coords.row, tr_endpoints_arr[endpoint_num].endpoint->tr_coords.col, 16, tr_endpoints_arr[endpoint_num].color);
}


void draw_endpoint_squares(void){
    for (int i=0; i<TR_ENDPOINTS_CNT; i++){
        draw_endpoint_square(i);
    //    uint16_t ep_x = tr_endpoints_arr[i].endpoint->tr_coords.row;
    //    uint16_t ep_y = tr_endpoints_arr[i].endpoint->tr_coords.col;
        
    //    if (ep_y < 100){
    //        lcd_draw_rectangle(ep_y-30, ep_x-30, ep_y-20, ep_x+30, 1, 1, tr_endpoints_arr[i].color);
    //    } else if (ep_y > 700){
    //        lcd_draw_rectangle(ep_y+20, ep_x-30, ep_y+30, ep_x+30, 1, 1, tr_endpoints_arr[i].color);
    //    } else if (ep_x < 50){
    //        lcd_draw_rectangle(ep_y-30, ep_x-30, ep_y+30, ep_x-20, 1, 1, tr_endpoints_arr[i].color);
    //    } else {
    //        lcd_draw_rectangle(ep_y-30, ep_x+20, ep_y+30, ep_x+30, 1, 1, tr_endpoints_arr[i].color);
    //    }
    }
}


void tr_car_activate_testing(uint8_t start, uint8_t end){
    if (car_queue_free_p->items){
        tr_car_queue_node_t *car_p = tr_car_dequeue(car_queue_free_p, 0xFFFF);
        
        car_p->car->start = start;
        car_p->car->direction_driving_current = tr_endpoints_arr[car_p->car->start].start_direction;
        car_p->car->current_node = tr_endpoints_arr[car_p->car->start].endpoint;
        car_p->car->destination = end;
        car_p->car->directions = &(directions[car_p->car->destination][car_p->car->start][0][0]);
        
        car_p->car->color = tr_endpoints_arr[car_p->car->destination].color;

        tr_car_enqueue(car_queue_active_p, car_p);
    }
}


void traffic_run_app(void){
    tr_initialize_cars();

    tr_generate_direction_arrs(&tr_direction_arr);
    tr_create_roads();
    tr_shuffle_roads();
    tr_find_intersections();
    tr_find_endpoints();

    uint16_t path_cnt = 0;
    tr_draw_loading_bar(path_cnt, 1);
    for (int i=0; i<(TR_HORIZONTAL_ROADS+TR_VERTICAL_ROADS)*2; i++){
        for (int j=0; j<(TR_HORIZONTAL_ROADS+TR_VERTICAL_ROADS)*2; j++){
            if (i != j){
                for (int k=0; k<4*3*2; k++){
                    //printf("endpoint %d to %d\n", i, j);
                    tr_find_shortest_path(tr_endpoints_arr[j].endpoint, tr_endpoints_arr[i].endpoint, k);
                    tr_process_direction_arr(j,i);
                    tr_draw_loading_bar(++path_cnt, 0);
                    tr_reset_nodes_visited();
                    //tr_reset_costs();
                    //lcd_draw_square(tr_endpoints_arr[i].endpoint->tr_coords.row, tr_endpoints_arr[i].endpoint->tr_coords.col, 5, BLACK);
                    //lcd_draw_square(tr_endpoints_arr[j].endpoint->tr_coords.row, tr_endpoints_arr[j].endpoint->tr_coords.col, 5, BLACK);
                    //tr_draw_roads();
                    //tr_clear_intersections();
                }
            }
        }
    }
    delay_ms(1000);
    tr_draw_loading_bar(0, 2);
    tr_draw_traffic_lights();
    draw_endpoint_squares();
    tr_draw_roads();
    //tr_draw_nodes();
    tr_clear_intersections();


    /* Testing starts here */
    
    printf("adding new car\n");
    tr_car_activate_testing(0, 2);
    //draw_endpoint_square(2);
    //draw_endpoint_square(0);
    print_car_info(car_queue_active_p->front->car);
    printf("car added\n");
}
