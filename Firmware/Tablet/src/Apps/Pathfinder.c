#include "Pathfinder.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "LCD.h"
#include "Colors.h"
#include "Interrupts.h"


typedef pf_graph_node_t pf_graph_nodes_arr[PF_COLS];
pf_graph_nodes_arr* pf_graph_nodes = (pf_graph_nodes_arr*)0xA0177000;
//pf_graph_node_t pf_graph_nodes[PF_ROWS][PF_COLS];
pf_graph_t pf_graph;

//typedef uint16_t pf_cost_arr[2];
uint16_t pf_cost[2][PF_ROWS*PF_COLS];

typedef pf_queue_node_t pf_nodes_arr[PF_COLS*PF_ROWS];
pf_nodes_arr* pf_nodes = (pf_nodes_arr*)0xA1482000;
uint16_t pf_queue_idx = 0;
volatile uint8_t pf_buttons_active = 0;
volatile uint8_t pf_run_game = 1;
volatile uint8_t pf_skip = 0;

extern void delay(uint32_t i);
extern void delay_ms(uint32_t i);

void pf_construct_graph(void){
    pf_graph.graph_size = 0;
    pf_graph.root = NULL;
}

void pf_build_graph(int rows, int cols){
    int node_count = 0;
    pf_graph_node_t* rover_col_prev;
    pf_graph_node_t* rover_col_curr;
            
    pf_graph_node_t new_node[rows][cols];
    pf_graph.root = &new_node[0][0];

    for (int row=0; row<rows; row++){
        for (int col=0; col<cols; col++){
      
            ((pf_graph_nodes)[row][col]).pf_coords.row = row*PF_SPACING+PF_OFFSET_Y;
            ((pf_graph_nodes)[row][col]).pf_coords.col = col*PF_SPACING+PF_OFFSET_X;
            ((pf_graph_nodes)[row][col]).id = node_count;
            ((pf_graph_nodes)[row][col]).left = NULL;
            ((pf_graph_nodes)[row][col]).right = NULL;
            ((pf_graph_nodes)[row][col]).up = NULL;
            ((pf_graph_nodes)[row][col]).down = NULL;
            ((pf_graph_nodes)[row][col]).enabled = 56;
            ((pf_graph_nodes)[row][col]).highlight = FALSE;
            ((pf_graph_nodes)[row][col]).pf_connections.up = 0;
            ((pf_graph_nodes)[row][col]).pf_connections.down = 0;
            ((pf_graph_nodes)[row][col]).pf_connections.left = 0;
            ((pf_graph_nodes)[row][col]).pf_connections.right = 0;
            ((pf_graph_nodes)[row][col]).visited = FALSE;

            node_count++;
        }
    }

    for (int row=0; row<rows; row++){
        rover_col_curr = &((pf_graph_nodes)[row][0]);
        for (int col=0; col<cols; col++){
            rover_col_curr = &((pf_graph_nodes)[row][col]);
            if (col < cols-1){
                rover_col_curr->right = &((pf_graph_nodes)[row][col+1]);
            }
            if (col > 0){
                rover_col_curr->left = &((pf_graph_nodes)[row][col-1]);
            }
            if (row < rows-1){
                ((pf_graph_nodes)[row][col]).down = &((pf_graph_nodes)[row+1][col]);
            }
            if (row > 0){
                ((pf_graph_nodes)[row][col]).up = &((pf_graph_nodes)[row-1][col]);
            }
        }
    }

    rover_col_curr = pf_graph.root;

    for (int row=0; row<rows; row++){
        rover_col_curr = &((pf_graph_nodes)[row][0]);
        for (int col=0; col<cols; col++){
            rover_col_curr = rover_col_curr->right;
        }
    }

    pf_graph.graph_size = rows*cols;
    pf_graph.rows = rows;
    pf_graph.cols = cols;
}


void pf_draw_graph(void){
    uint32_t border_color;
    uint32_t fill_color;
    for (int row=0; row<PF_ROWS; row++){
        for (int col=0; col<PF_COLS; col++){
            if (((pf_graph_nodes)[row][col]).enabled == TRUE){
                border_color = MAROON;
                fill_color = ORANGE;
                //printf("filled\n");
            } else {
                //printf("not filled\n");
                //printf("%d %d %d %d\n", row,col, ((pf_graph_nodes)[row][col]).id, ((pf_graph_nodes)[row][col]).enabled);
                border_color = BLACK;
                fill_color = GRAY_DARKER;
            }
            lcd_draw_circle(((pf_graph_nodes)[row][col]).pf_coords.col, ((pf_graph_nodes)[row][col]).pf_coords.row,PF_RADIUS,border_color,1,fill_color);
        }
    }
}


void pf_print_node(pf_graph_node_t* node){
    uint16_t node_id = 0;
    uint16_t node_left = 0;
    uint16_t node_right = 0;
    uint16_t node_up = 0;
    uint16_t node_down = 0;

    //node_id = node->id;
    //node_left = node->left != NULL ? node->left->id : NULL;
    //node_right = node->right != NULL ? node->right->id : NULL;
    //node_up = node->up != NULL ? node->up->id : NULL;
    //node_down = node->down != NULL ? node->down->id : NULL;

    //printf("id: %d\tL: %d\tR: %d\tU: %d\tD: %d\n", node_id,node_left,node_right,node_up,node_down);
}


void pf_highlight_toggle_node(pf_graph_node_t* node, uint32_t border_color, uint32_t fill_color){
    if (node->highlight == TRUE){
        lcd_draw_circle(node->pf_coords.col, node->pf_coords.row,PF_RADIUS,BLACK, 1, GRAY_DARKER);
        node->highlight = FALSE;
    } else {
        //printf("highlight node %d\n", node->id);
        lcd_draw_circle(node->pf_coords.col, node->pf_coords.row,PF_RADIUS,border_color, 1, fill_color);
        node->highlight = TRUE;
    }
}


void pf_draw_connections(pf_graph_node_t *node, uint8_t orientation){
    if (node == NULL){
        return;
    }
    if (orientation==PF_HORIZONTAL){
        pf_draw_connections(node->down, PF_HORIZONTAL);
    } else if (orientation == PF_VERTICAL){
        pf_draw_connections(node->right, PF_VERTICAL);
    }
    while (node != NULL){
        if (orientation == PF_HORIZONTAL){
            if (node->right != NULL){
                lcd_draw_line(node->pf_coords.col+PF_RADIUS-3, node->pf_coords.row, node->right->pf_coords.col-PF_RADIUS+3, node->right->pf_coords.row,1,GRAY_DARKER);
            }
            node = node->right;
        } else if (orientation == PF_VERTICAL){
            if (node->down != NULL){
                lcd_draw_line(node->pf_coords.col, node->pf_coords.row+PF_RADIUS-3, node->down->pf_coords.col, node->down->pf_coords.row-PF_RADIUS+3,1,GRAY_DARKER);
            }
            node = node->down;
        }
    }
}


//void pf_highlight_toggle_connection(pf_graph_node_t *node, uint8_t direction){
//    if (node == NULL){
//        return;
//    }
//    uint32_t color;
//    switch(direction){
//        case PF_UP:
//            if (node->up != NULL){
//                if (node->pf_connections.up == ON){
//                    color = GRAY_DARKER;
//                    node->pf_connections.up = OFF;
//                    node->up->pf_connections.down = OFF;
//                } else {
//                    color = GREEN;
//                    node->pf_connections.up = ON;
//                    node->up->pf_connections.down = ON;
//                }
//                lcd_draw_line(node->pf_coords.col-PF_RADIUS, node->pf_coords.row, node->up->pf_coords.col+PF_RADIUS, node->up->pf_coords.row,2,color);
//            }
//            break;
        
//        case PF_DOWN:
//            if (node->down != NULL){
//                if (node->pf_connections.down == ON){
//                    color = GRAY_DARKER;
//                    node->pf_connections.down = OFF;
//                    node->down->pf_connections.up = OFF;
//                } else {
//                    color = WHITE;
//                    node->pf_connections.down = ON;
//                    node->down->pf_connections.up = ON;
//                }
//                lcd_draw_line(node->pf_coords.col+PF_RADIUS, node->pf_coords.row, node->down->pf_coords.col-PF_RADIUS, node->down->pf_coords.row,2,color);
//            }
//            break;
        
//        case PF_LEFT:
//            if (node->left != NULL){
//                if (node->pf_connections.left == ON){
//                    color = GRAY_DARKER;
//                    node->pf_connections.left = OFF;
//                    node->left->pf_connections.right = OFF;
//                } else {
//                    color = CYAN;
//                    node->pf_connections.left = ON;
//                    node->left->pf_connections.right = ON;
//                }
//                lcd_draw_line(node->pf_coords.col-PF_RADIUS, node->pf_coords.row, node->left->pf_coords.col+PF_RADIUS, node->left->pf_coords.row,2,color);
//            }
//            break;

//        case PF_RIGHT:
//            if (node->right != NULL){
//                if (node->pf_connections.right == ON){
//                    color = GRAY_DARKER;
//                    node->pf_connections.right = OFF;
//                    node->right->pf_connections.left = OFF;
//                } else {
//                    color = CYAN;
//                    node->pf_connections.right = ON;
//                    node->right->pf_connections.left = ON;
//                }
//                lcd_draw_line(node->pf_coords.col, node->pf_coords.row-3, node->right->pf_coords.col, node->right->pf_coords.row+3,2,color);
//            }
//            break;
//    }
//}

void pf_highlight_toggle_connection(pf_graph_node_t *node, uint8_t direction){
    if (node == NULL){
        return;
    }
    uint32_t color;
    switch(direction){
        case PF_UP:
            if (node->up != NULL){
                if (node->pf_connections.up == ON){
                    color = GRAY_DARKER;
                    node->pf_connections.up = OFF;
                    node->up->pf_connections.down = OFF;
                } else {
                    color = BLUE;
                    node->pf_connections.up = ON;
                    node->up->pf_connections.down = ON;
                }
                lcd_draw_line(node->pf_coords.col, node->pf_coords.row, node->up->pf_coords.col, node->up->pf_coords.row,PF_SHORT_PATH_WIDTH,PF_SHORT_PATH_COLOR_U);
            }
            break;
        
        case PF_DOWN:
            if (node->down != NULL){
                if (node->pf_connections.down == ON){
                    color = GRAY_DARKER;
                    node->pf_connections.down = OFF;
                    node->down->pf_connections.up = OFF;
                } else {
                    color = BLUE;
                    node->pf_connections.down = ON;
                    node->down->pf_connections.up = ON;
                }
                lcd_draw_line(node->pf_coords.col, node->pf_coords.row, node->down->pf_coords.col, node->down->pf_coords.row,PF_SHORT_PATH_WIDTH,PF_SHORT_PATH_COLOR_D);
            }
            break;
        
        case PF_LEFT:
            if (node->left != NULL){
                if (node->pf_connections.left == ON){
                    color = GRAY_DARKER;
                    node->pf_connections.left = OFF;
                    node->left->pf_connections.right = OFF;
                } else {
                    color = BLUE;
                    node->pf_connections.left = ON;
                    node->left->pf_connections.right = ON;
                }
                lcd_draw_line(node->pf_coords.col, node->pf_coords.row, node->left->pf_coords.col, node->left->pf_coords.row,PF_SHORT_PATH_WIDTH,PF_SHORT_PATH_COLOR_L);
            }
            break;

        case PF_RIGHT:
            if (node->right != NULL){
                if (node->pf_connections.right == ON){
                    color = GRAY_DARKER;
                    node->pf_connections.right = OFF;
                    node->right->pf_connections.left = OFF;
                } else {
                    color = BLUE;
                    node->pf_connections.right = ON;
                    node->right->pf_connections.left = ON;
                }
                lcd_draw_line(node->pf_coords.col, node->pf_coords.row, node->right->pf_coords.col, node->right->pf_coords.row,PF_SHORT_PATH_WIDTH,PF_SHORT_PATH_COLOR_R);
            }
            break;
    }
}

void pf_enqueue(pf_queue_t *queue, pf_queue_node_t *nodes, uint16_t *idx, pf_graph_node_t *from_p, pf_graph_node_t *to_p,  uint16_t cost){ 
        
    nodes[*idx].from_p = from_p;
    nodes[*idx].to_p = to_p;
    nodes[*idx].cost = cost;
    nodes[*idx].next = NULL;
    
    pf_queue_node_t *rover = queue->front;

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


uint16_t pf_dequeue(pf_queue_t *queue){
    if (queue->front != NULL){

        if (queue->front->cost < pf_cost[1][queue->front->to_p->id]){
            pf_cost[0][queue->front->to_p->id] = queue->front->from_p->id;
            pf_cost[1][queue->front->to_p->id] = queue->front->cost;
            (*(pf_nodes)[queue->front->to_p->id]).cost = queue->front->cost;
        }
        pf_graph_node_t *new_start = queue->front->to_p;
        uint16_t next_cost = queue->front->cost+1;
        queue->front = queue->front->next;
        queue->items--;
        if (new_start->visited == FALSE){
            if (pf_core(new_start, queue, next_cost)){
                return 1;
            }
        }
    }
    return 0;
}



uint16_t pf_core(pf_graph_node_t *start, pf_queue_t *queue, uint16_t next_cost){
    
    start->visited = TRUE;
    
    if (queue->end->visited == FALSE){
        if (start->up != NULL && start->up->enabled && start->up->visited == FALSE){
            pf_enqueue(queue, *pf_nodes, &pf_queue_idx, start, start->up, next_cost);
        }
        if (start->down != NULL && start->down->enabled && start->down->visited == FALSE){
            pf_enqueue(queue, *pf_nodes, &pf_queue_idx, start, start->down, next_cost);
        }
        if (start->left != NULL && start->left->enabled && start->left->visited == FALSE){
            pf_enqueue(queue, *pf_nodes, &pf_queue_idx, start, start->left, next_cost);
        }
        if (start->right != NULL && start->right->enabled && start->right->visited == FALSE){
            pf_enqueue(queue, *pf_nodes, &pf_queue_idx, start, start->right, next_cost);
        }
    } else {
        printf("found path!\n");
        while (queue->front != NULL){        
            queue->front = queue->front->next;
            queue->items--;
        }
        return 1;
    }
    return 0;
}


void pf_draw_shortest_path(pf_graph_node_t *start, pf_graph_node_t *end){
    uint16_t next_id = pf_cost[0][end->id];
    pf_graph_node_t *rover = end;
    while (rover->id != start->id){
        //printf("%d  %d\n", rover->id, rover->enabled);
        if (rover->left != NULL && rover->left->id == next_id){
            pf_highlight_toggle_connection(rover, PF_LEFT);
            rover = rover->left;
        } else if (rover->up != NULL && rover->up->id == next_id){
            pf_highlight_toggle_connection(rover, PF_UP);
            rover = rover->up;
        } else if (rover->right != NULL && rover->right->id == next_id){
            pf_highlight_toggle_connection(rover, PF_RIGHT);
            rover = rover->right;
        } else if (rover->down != NULL && rover->down->id == next_id){
            pf_highlight_toggle_connection(rover, PF_DOWN);
            rover = rover->down;
        } else {
            printf("path error\n");
            return;
        }
        next_id = pf_cost[0][rover->id];        
    }
}

void pf_find_shortest_path(pf_graph_node_t *start, pf_graph_node_t *end){
    pf_queue_t queue;
    queue.back = NULL;
    queue.front = NULL;
    queue.end = end;
    pf_queue_t *queue_p = &queue;
    queue.items = 0;
    //pf_queue_node_t *rover;
    //pf_queue_node_t *node_p = &((*pf_nodes)[0]);
    pf_queue_idx = 0;

    for (int i=0; i<PF_ROWS*PF_COLS; i++){
        ((*pf_nodes)[i]).cost = 0;
        ((*pf_nodes)[i]).next = NULL;
        ((*pf_nodes)[i]).from_p = NULL;
        ((*pf_nodes)[i]).to_p = NULL;
    }
    for (uint16_t i=0; i<PF_ROWS*PF_COLS; i++){
        pf_cost[0][i] = 0;
        pf_cost[1][i] = 0xFFFF;
    }
    
    uint16_t idx = 0;

    pf_cost[1][start->id] = 0;

    start->enabled = TRUE;
    end->enabled = TRUE;
    pf_highlight_toggle_node(start, CYAN, WHITE);
    pf_highlight_toggle_node(end, GREEN_DARK, GREEN);
    printf("going from: %d  to  %d\n", start->id, end->id);

    if (pf_core(start, queue_p, 0)){
        printf("here\n");
    }

    while (queue.items){
        if (pf_dequeue(queue_p)){
            pf_draw_shortest_path(start, end);
        }
    }
    delay(3000);
    printf("done\n");
}


void pf_shuffle_board(void){
    uint32_t random_seed = ((LPC_RTC->SEC+1) * (LPC_RTC->MIN+1) * (LPC_RTC->HOUR+1) * (rand() % 1000));
    printf("random_seed: %d\n", random_seed);
    srand(random_seed);
    for (int row=0; row<PF_ROWS; row++){
        for (int col=0; col<PF_COLS; col++){
            if (rand() % 100 < 25){
                ((pf_graph_nodes)[row][col]).enabled = FALSE;
            } else {
                ((pf_graph_nodes)[row][col]).enabled = TRUE;
            }
        }
    }
}


void pf_draw_buttons(void){
    lcd_draw_rectangle(PF_BUTTON_SHUFFLE_X1, PF_BUTTON_SHUFFLE_Y1, PF_BUTTON_SHUFFLE_X2, PF_BUTTON_SHUFFLE_Y2, 0, 1, WHITE);
    lcd_pf_draw_buttons();
    pf_buttons_active = 1;
}


void pf_clear_board(void){
    for (int row=0; row<PF_ROWS; row++){
        for (int col=0; col<PF_COLS; col++){
            if (row > 0){
                if (((pf_graph_nodes)[row][col]).pf_connections.up == TRUE){
                    pf_highlight_toggle_connection(&((pf_graph_nodes)[row][col]), PF_UP);
                }
            }
            if (row < PF_ROWS-2){
                if (((pf_graph_nodes)[row][col]).pf_connections.down == TRUE){
                    pf_highlight_toggle_connection(&((pf_graph_nodes)[row][col]), PF_DOWN);
                }
            }
            if (col > 0){
                if (((pf_graph_nodes)[row][col]).pf_connections.left == TRUE){
                    pf_highlight_toggle_connection(&((pf_graph_nodes)[row][col]), PF_LEFT);
                }
            }
            if (row < PF_COLS-2){
                if (((pf_graph_nodes)[row][col]).pf_connections.right == TRUE){
                    pf_highlight_toggle_connection(&((pf_graph_nodes)[row][col]), PF_RIGHT);
                }
            }
            if (((pf_graph_nodes)[row][col]).highlight == TRUE){
                pf_highlight_toggle_node(&((pf_graph_nodes)[row][col]), 0, 0);
            }
            ((pf_graph_nodes)[row][col]).visited = FALSE;
        }
    }
}


void pf_run_full_game(void){
    printf("size: %d %d\n", sizeof(pf_graph_node_t), sizeof(int));
    pf_clear_board();
    pf_shuffle_board();
    pf_draw_graph();
    pf_draw_buttons();
    pf_draw_connections(&((pf_graph_nodes)[0][0]), PF_HORIZONTAL);
    pf_draw_connections(&((pf_graph_nodes)[0][0]), PF_VERTICAL);
    pf_find_shortest_path(&((pf_graph_nodes)[1][1]), &((pf_graph_nodes)[PF_ROWS-2][PF_COLS-2]));
}


void pathfinder_run_app(void){
    printf("running pathfinder\n");
    pf_construct_graph();
    pf_build_graph(PF_ROWS, PF_COLS);
    
    int run_count = 0;

    while (1){
        touch_check();
        if (pf_run_game){
            //lcd_touch_interrupt_disable();
            pf_skip = 1;
        //if (run_count < 100){
            pf_run_game = 0;
            pf_buttons_active = 0;
            pf_run_full_game();
            pf_buttons_active = 1;
            run_count++;
            //delay_ms(250);
            pf_skip = 0;
            //lcd_touch_interrupt_enable();
            //delay_ms(300);
        }
    }
      
    //lcd_draw_circle(200,200,5,GRAY_DARK, 1);
    
}

