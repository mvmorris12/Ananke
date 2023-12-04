#include "Pathfinder.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "LCD.h"
#include "Colors.h"

graph_node_t graph_nodes[PF_ROWS][PF_COLS];
graph_t pathfinder_graph;

void construct_graph(void){
    pathfinder_graph.graph_size = 0;
    pathfinder_graph.root = NULL;
}

void build_graph(int rows, int cols){
    int node_count = 0;
    graph_node_t* rover_col_prev;
    graph_node_t* rover_col_curr;
            
    graph_node_t new_node[rows][cols];
    pathfinder_graph.root = &new_node[0][0];
    for (int row=0; row<rows; row++){
        for (int col=0; col<cols; col++){
      
            graph_nodes[row][col].coords.row = row;
            graph_nodes[row][col].coords.col = col;
            graph_nodes[row][col].id = node_count;
            graph_nodes[row][col].left = NULL;
            graph_nodes[row][col].right = NULL;
            graph_nodes[row][col].up = NULL;
            graph_nodes[row][col].down = NULL;
            graph_nodes[row][col].enabled = TRUE;

            node_count++;
        }
    }

    for (int row=0; row<rows; row++){
        rover_col_curr = &graph_nodes[row][0];
        //rover_col_prev = &graph_nodes[row][0];
        for (int col=0; col<cols; col++){
            rover_col_curr = &graph_nodes[row][col];
            if (col < cols-1){
                rover_col_curr->right = &graph_nodes[row][col+1];
            }
            //rover_col_prev = rover_col_curr;
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
            printf("%d %d %d\n", rover_col_curr->id, rover_col_curr->coords.row, rover_col_curr->coords.col);
            rover_col_curr = rover_col_curr->right;
        }
    }
    printf("%d %d %d\n", rows, cols, rows*cols);
    pathfinder_graph.graph_size = rows*cols;
    pathfinder_graph.rows = rows;
    pathfinder_graph.cols = cols;
    printf("before graph size: %d\n", pathfinder_graph.graph_size);
}


void draw_graph(void){
    int offset_x = 25;
    int offset_y = 25;
    int spacing = 35;
    for (int row=0; row<PF_ROWS; row++){
        for (int col=0; col<PF_COLS; col++){
            //printf("id: %d  row: %d   col: %d\n", graph_nodes[row][col].id, graph_nodes[row][col].coords.row, graph_nodes[row][col].coords.col);
            lcd_draw_circle(graph_nodes[row][col].coords.col*spacing+offset_x, graph_nodes[row][col].coords.row*spacing+offset_y,4,GRAY_LIGHT);
        }
    }
}


void print_node(graph_node_t* node){
    printf("id: %d\n", node->id);
    if (node->left!=NULL){
      printf("L: %d\n", node->left->id);
    }
    if (node->right!=NULL){
      printf("R: %d\n", node->right->id);
    }
    if (node->up!=NULL){
      printf("U: %d\n", node->up->id);
    }
    if (node->down!=NULL){
      printf("D: %d\n", node->down->id);
    }
    printf("\n");
}


void pathfinder_run_app(void){
    printf("running pathfinder\n");
    construct_graph();
    build_graph(PF_ROWS, PF_COLS);
    //pathfinder_graph->graph_size = PF_;
    draw_graph();
    for (int row=0; row<PF_ROWS; row++){
        for (int col=0; col<PF_COLS; col++){
            print_node(&graph_nodes[row][col]);
        }
    }
    printf("pathfinder done %d x %d x %d\n", pathfinder_graph.rows, pathfinder_graph.cols, pathfinder_graph.graph_size);
}

