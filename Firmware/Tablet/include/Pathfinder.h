#include <stdint.h>
#include <type.h>
#include "Colors.h"

#define PF_ROWS                 47
#define PF_COLS                 29
#define PF_OFFSET_X             25
#define PF_OFFSET_Y             25
#define PF_SPACING              15
#define PF_RADIUS               3
#define PF_TRUE                 1
#define PF_FALSE                0
#define ON                      1
#define OFF                     0
#define PF_HORIZONTAL           0
#define PF_VERTICAL             1
#define PF_UP                   1
#define PF_DOWN                 2
#define PF_LEFT                 3
#define PF_RIGHT                4
#define PF_BUTTON_SHUFFLE_X1    725
#define PF_BUTTON_SHUFFLE_Y1    395
#define PF_BUTTON_SHUFFLE_X2    790
#define PF_BUTTON_SHUFFLE_Y2    470
#define PF_SHORT_PATH_WIDTH     2
#define PF_SHORT_PATH_COLOR_U   BLUE
#define PF_SHORT_PATH_COLOR_R   BLUE
#define PF_SHORT_PATH_COLOR_D   BLUE
#define PF_SHORT_PATH_COLOR_L   BLUE



typedef struct pf_graph_node_tag_t{
    uint32_t id;
    struct pf_graph_node_tag_t *right; 
    struct pf_graph_node_tag_t *left; 
    struct pf_graph_node_tag_t *up; 
    struct pf_graph_node_tag_t *down; 
    struct pf_coords{
        uint16_t row;
        uint16_t col;
    } pf_coords;
    struct pf_connections{
        uint32_t up;
        uint32_t down;
        uint32_t left;
        uint32_t right;
    } pf_connections;
    uint32_t enabled;
    uint8_t highlight;
    uint32_t visited;
} pf_graph_node_t;

typedef struct pf_graph_tag_t{
   pf_graph_node_t *root;
   uint16_t graph_size;
   uint16_t rows;
   uint16_t cols;
} pf_graph_t;


typedef struct pf_queue_node_tag_t{
    //uint16_t from;
    //uint16_t to;
    uint16_t cost;
    pf_graph_node_t *from_p;
    pf_graph_node_t *to_p;
    struct pf_queue_node_tag_t *next;
} pf_queue_node_t;
  
typedef struct pf_queue_tag_t{
    pf_queue_node_t *front;
    pf_queue_node_t *back;
    pf_graph_node_t *end;
    uint16_t items;
} pf_queue_t;

void pathfinder_run_app(void);
void pf_construct_graph(void);
void pf_highlight_toggle_node(pf_graph_node_t* node, uint32_t border_color, uint32_t fill_color);
void pf_build_graph(int rows, int cols);
void pf_draw_connections(pf_graph_node_t *node, uint8_t orientation);
void pf_highlight_toggle_connection(pf_graph_node_t *node, uint8_t direction);
void pf_find_shortest_path(pf_graph_node_t *start, pf_graph_node_t *end);
uint16_t pf_core(pf_graph_node_t *start, pf_queue_t *queue, uint16_t next_cost);
void pf_draw_shortest_path(pf_graph_node_t *start, pf_graph_node_t *end);
void pf_shuffle_board(void);
void pf_redraw_board(void);
void pf_draw_buttons(void);

