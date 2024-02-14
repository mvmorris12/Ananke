#include "Pathfinder.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "LCD.h"
#include "Colors.h"


graph_node_t graph_nodes[PF_ROWS][PF_COLS];
graph_t pathfinder_graph;
uint16_t cost[2][PF_ROWS*PF_COLS];
queue_node_t nodes[500];
uint16_t queue_idx = 0;
volatile uint8_t pf_buttons_active = 0;
volatile uint8_t pf_run_game = 1;

void pf_construct_graph(void){
    pathfinder_graph.graph_size = 0;
    pathfinder_graph.root = NULL;
}

void pf_build_graph(int rows, int cols){
    int node_count = 0;
    graph_node_t* rover_col_prev;
    graph_node_t* rover_col_curr;
            
    graph_node_t new_node[rows][cols];
    pathfinder_graph.root = &new_node[0][0];
    for (int row=0; row<rows; row++){
        for (int col=0; col<cols; col++){
      
            graph_nodes[row][col].coords.row = row*PF_SPACING+PF_OFFSET_Y;
            graph_nodes[row][col].coords.col = col*PF_SPACING+PF_OFFSET_X;
            graph_nodes[row][col].id = node_count;
            graph_nodes[row][col].left = NULL;
            graph_nodes[row][col].right = NULL;
            graph_nodes[row][col].up = NULL;
            graph_nodes[row][col].down = NULL;
            graph_nodes[row][col].enabled = TRUE;
            graph_nodes[row][col].highlight = FALSE;
            graph_nodes[row][col].connections.up = 0;
            graph_nodes[row][col].connections.down = 0;
            graph_nodes[row][col].connections.left = 0;
            graph_nodes[row][col].connections.right = 0;
            graph_nodes[row][col].visited = FALSE;

            node_count++;
        }
    }

    for (int row=0; row<rows; row++){
        rover_col_curr = &graph_nodes[row][0];
        for (int col=0; col<cols; col++){
            rover_col_curr = &graph_nodes[row][col];
            if (col < cols-1){
                rover_col_curr->right = &graph_nodes[row][col+1];
            }
            if (col > 0){
                rover_col_curr->left = &graph_nodes[row][col-1];
            }
            if (row < rows-1){
                graph_nodes[row][col].down = &graph_nodes[row+1][col];
            }
            if (row > 0){
                graph_nodes[row][col].up = &graph_nodes[row-1][col];
            }
        }
    }

    
    rover_col_curr = pathfinder_graph.root;
    for (int row=0; row<rows; row++){
        rover_col_curr = &graph_nodes[row][0];
        for (int col=0; col<cols; col++){
            rover_col_curr = rover_col_curr->right;
        }
    }
    pathfinder_graph.graph_size = rows*cols;
    pathfinder_graph.rows = rows;
    pathfinder_graph.cols = cols;
}


void pf_draw_graph(void){
    uint32_t border_color;
    uint32_t fill_color;
    for (int row=0; row<PF_ROWS; row++){
        for (int col=0; col<PF_COLS; col++){
            if (graph_nodes[row][col].enabled == TRUE){
                border_color = MAROON;
                fill_color = ORANGE;
            } else {
                border_color = BLACK;
                fill_color = GRAY_DARKER;
            }
            lcd_draw_circle(graph_nodes[row][col].coords.col, graph_nodes[row][col].coords.row,PF_RADIUS,border_color,1,fill_color);
        }
    }
}


void pf_print_node(graph_node_t* node){
    uint16_t node_id = 0;
    uint16_t node_left = 0;
    uint16_t node_right = 0;
    uint16_t node_up = 0;
    uint16_t node_down = 0;

    node_id = node->id;
    node_left = node->left != NULL ? node->left->id : NULL;
    node_right = node->right != NULL ? node->right->id : NULL;
    node_up = node->up != NULL ? node->up->id : NULL;
    node_down = node->down != NULL ? node->down->id : NULL;

    printf("id: %d\tL: %d\tR: %d\tU: %d\tD: %d\n", node_id,node_left,node_right,node_up,node_down);
}


void pf_highlight_toggle_node(graph_node_t* node, uint32_t border_color, uint32_t fill_color){
    if (node->highlight == TRUE){
        lcd_draw_circle(node->coords.col, node->coords.row,PF_RADIUS,BLACK, 1, GRAY_DARKER);
        node->highlight = FALSE;
    } else {
        //printf("highlight node %d\n", node->id);
        lcd_draw_circle(node->coords.col, node->coords.row,PF_RADIUS,border_color, 1, fill_color);
        node->highlight = TRUE;
    }
}


void pf_draw_connections(graph_node_t *node, uint8_t orientation){
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
                lcd_draw_line(node->coords.col+PF_RADIUS+1, node->coords.row, node->right->coords.col-PF_RADIUS-1, node->right->coords.row,1,GRAY_DARKER);
            }
            node = node->right;
        } else if (orientation == PF_VERTICAL){
            if (node->down != NULL){
                lcd_draw_line(node->coords.col, node->coords.row+PF_RADIUS+1, node->down->coords.col, node->down->coords.row-PF_RADIUS-1,1,GRAY_DARKER);
            }
            node = node->down;
        }
    }
}


void pf_highlight_toggle_connection(graph_node_t *node, uint8_t direction){
    if (node == NULL){
        return;
    }
    uint32_t color;
    switch(direction){
        case PF_UP:
            if (node->up != NULL){
                if (node->connections.up == ON){
                    color = GRAY_DARKER;
                    node->connections.up = OFF;
                    node->up->connections.down = OFF;
                } else {
                    color = RED;
                    node->connections.up = ON;
                    node->up->connections.down = ON;
                }
                lcd_draw_line(node->coords.col, node->coords.row-PF_RADIUS-2, node->up->coords.col, node->up->coords.row+PF_RADIUS+2,1,color);
            }
            break;
        
        case PF_DOWN:
            if (node->down != NULL){
                if (node->connections.down == ON){
                    color = GRAY_DARKER;
                    node->connections.down = OFF;
                    node->down->connections.up = OFF;
                } else {
                    color = RED;
                    node->connections.down = ON;
                    node->down->connections.up = ON;
                }
                lcd_draw_line(node->coords.col, node->coords.row+PF_RADIUS+2, node->down->coords.col, node->down->coords.row-PF_RADIUS-2,1,color);
            }
            break;
        
        case PF_LEFT:
            if (node->left != NULL){
                if (node->connections.left == ON){
                    color = GRAY_DARKER;
                    node->connections.left = OFF;
                    node->left->connections.right = OFF;
                } else {
                    color = RED;
                    node->connections.left = ON;
                    node->left->connections.right = ON;
                }
                lcd_draw_line(node->coords.col-PF_RADIUS-2, node->coords.row, node->left->coords.col+PF_RADIUS+2, node->left->coords.row,1,color);
            }
            break;

        case PF_RIGHT:
            if (node->right != NULL){
                if (node->connections.right == ON){
                    color = GRAY_DARKER;
                    node->connections.right = OFF;
                    node->right->connections.left = OFF;
                } else {
                    color = RED;
                    node->connections.right = ON;
                    node->right->connections.left = ON;
                }
                lcd_draw_line(node->coords.col+PF_RADIUS+2, node->coords.row, node->right->coords.col-PF_RADIUS-2, node->right->coords.row,1,color);
            }
            break;
    }
}


void enqueue(queue_t *queue, queue_node_t *nodes, uint16_t *idx, graph_node_t *from_p, graph_node_t *to_p,  uint16_t cost){ 
        
    nodes[*idx].from_p = from_p;
    nodes[*idx].to_p = to_p;
    nodes[*idx].cost = cost;
    nodes[*idx].next = NULL;
    
    queue_node_t *rover = queue->front;

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


uint16_t dequeue(queue_t *queue){
    if (queue->front != NULL){

        if (queue->front->cost < cost[1][queue->front->to_p->id]){
            cost[0][queue->front->to_p->id] = queue->front->from_p->id;
            cost[1][queue->front->to_p->id] = queue->front->cost;
            nodes[queue->front->to_p->id].cost = queue->front->cost;
        }
        graph_node_t *new_start = queue->front->to_p;
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



uint16_t pf_core(graph_node_t *start, queue_t *queue, uint16_t next_cost){
    
    start->visited = TRUE;
    
    if (queue->end->visited == FALSE){
        if (start->up != NULL && start->up->enabled && start->up->visited == FALSE){
            enqueue(queue, nodes, &queue_idx, start, start->up, next_cost);
        }
        if (start->down != NULL && start->down->enabled && start->down->visited == FALSE){
            enqueue(queue, nodes, &queue_idx, start, start->down, next_cost);
        }
        if (start->left != NULL && start->left->enabled && start->left->visited == FALSE){
            enqueue(queue, nodes, &queue_idx, start, start->left, next_cost);
        }
        if (start->right != NULL && start->right->enabled && start->right->visited == FALSE){
            enqueue(queue, nodes, &queue_idx, start, start->right, next_cost);
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


void pf_draw_shortest_path(graph_node_t *start, graph_node_t *end){
    uint16_t next_id = cost[0][end->id];
    graph_node_t *rover = end;
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
        next_id = cost[0][rover->id];        
    }
}

void pf_find_shortest_path(graph_node_t *start, graph_node_t *end){
    queue_t queue;
    queue.back = NULL;
    queue.front = NULL;
    queue.end = end;
    queue_t *queue_p = &queue;
    queue.items = 0;
    queue_node_t *rover;
    queue_node_t *node_p = &nodes[0];
    queue_idx = 0;

    for (int i=0; i<500; i++){
        nodes[i].cost = 0;
        nodes[i].next = NULL;
        nodes[i].from_p = NULL;
        nodes[i].to_p = NULL;
    }
    for (uint16_t i=0; i<PF_ROWS*PF_COLS; i++){
        cost[0][i] = 0;
        cost[1][i] = 0xFFFF;
    }
    
    uint16_t idx = 0;

    cost[1][start->id] = 0;

    start->enabled = TRUE;
    end->enabled = TRUE;
    pf_highlight_toggle_node(start, CYAN, WHITE);
    pf_highlight_toggle_node(end, GREEN_DARK, GREEN);
    printf("going from: %d  to  %d\n", start->id, end->id);

    if (pf_core(start, queue_p, 0)){
        printf("here\n");
    }

    while (queue.items){
        if (dequeue(queue_p)){
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
                graph_nodes[row][col].enabled = FALSE;
            } else {
                graph_nodes[row][col].enabled = TRUE;
            }
        }
    }
}


void pf_draw_buttons(void){
    //lcd_draw_rectangle(PF_BUTTON_SHUFFLE_X1, PF_BUTTON_SHUFFLE_Y1, PF_BUTTON_SHUFFLE_X2, PF_BUTTON_SHUFFLE_Y2, 0, 1, WHITE);
    lcd_pf_draw_buttons();
    pf_buttons_active = 1;
}


void pf_clear_board(void){
    for (int row=0; row<PF_ROWS; row++){
        for (int col=0; col<PF_COLS; col++){
            if (row > 0){
                if (graph_nodes[row][col].connections.up == TRUE){
                    pf_highlight_toggle_connection(&graph_nodes[row][col], PF_UP);
                }
            }
            if (row < PF_ROWS-2){
                if (graph_nodes[row][col].connections.down == TRUE){
                    pf_highlight_toggle_connection(&graph_nodes[row][col], PF_DOWN);
                }
            }
            if (col > 0){
                if (graph_nodes[row][col].connections.left == TRUE){
                    pf_highlight_toggle_connection(&graph_nodes[row][col], PF_LEFT);
                }
            }
            if (row < PF_COLS-2){
                if (graph_nodes[row][col].connections.right == TRUE){
                    pf_highlight_toggle_connection(&graph_nodes[row][col], PF_RIGHT);
                }
            }
            if (graph_nodes[row][col].highlight == TRUE){
                pf_highlight_toggle_node(&graph_nodes[row][col], 0, 0);
            }
            graph_nodes[row][col].visited = FALSE;
        }
    }
}


void pf_run_full_game(void){
    pf_clear_board();
    pf_shuffle_board();
    pf_draw_graph();
    pf_draw_buttons();
    pf_draw_connections(&graph_nodes[0][0], PF_HORIZONTAL);
    pf_draw_connections(&graph_nodes[0][0], PF_VERTICAL);
    pf_find_shortest_path(&graph_nodes[1][1], &graph_nodes[PF_ROWS-2][PF_COLS-2]);
}


void pathfinder_run_app(void){
    printf("running pathfinder\n");
    pf_construct_graph();
    pf_build_graph(PF_ROWS, PF_COLS);
    
    int run_count = 0;

    while (1){
        touch_check();
        if (pf_run_game){
        //if (run_count < 100){
            pf_run_game = 0;
            pf_buttons_active = 0;
            pf_run_full_game();
            //delay_short();
            pf_buttons_active = 1;
            run_count++;
        }
    }
      
    //lcd_draw_circle(200,200,5,GRAY_DARK, 1);
    
}

