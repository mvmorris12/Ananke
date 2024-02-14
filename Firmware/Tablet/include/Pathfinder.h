#include <stdint.h>
#include <type.h>

#define PF_ROWS 13
#define PF_COLS 20
#define PF_OFFSET_X 25
#define PF_OFFSET_Y 25
#define PF_SPACING 36
#define PF_RADIUS 3
//#define TRUE 1
//#define FALSE 0
#define ON 1
#define OFF 0
#define PF_HORIZONTAL 0
#define PF_VERTICAL 1
#define PF_UP 1
#define PF_DOWN 2
#define PF_LEFT 3
#define PF_RIGHT 4
#define PF_BUTTON_SHUFFLE_X1 725
#define PF_BUTTON_SHUFFLE_Y1 395
#define PF_BUTTON_SHUFFLE_X2 790
#define PF_BUTTON_SHUFFLE_Y2 470


typedef struct graph_node_tag_t{
    uint16_t id;
    struct graph_node_tag_t *right; 
    struct graph_node_tag_t *left; 
    struct graph_node_tag_t *up; 
    struct graph_node_tag_t *down; 
    struct coords{
        uint16_t row;
        uint16_t col;
    } coords;
    struct connections{
        uint8_t up;
        uint8_t down;
        uint8_t left;
        uint8_t right;
    } connections;
    uint8_t enabled;
    uint8_t highlight;
    uint8_t visited;
} graph_node_t;

typedef struct graph_tag_t{
   graph_node_t *root;
   uint16_t graph_size;
   uint16_t rows;
   uint16_t cols;
} graph_t;


typedef struct queue_node_tag_t{
    //uint16_t from;
    //uint16_t to;
    uint16_t cost;
    graph_node_t *from_p;
    graph_node_t *to_p;
    struct queue_node_tag_t *next;
} queue_node_t;
  
typedef struct queue_tag_t{
    queue_node_t *front;
    queue_node_t *back;
    graph_node_t *end;
    uint16_t items;
} queue_t;

void pathfinder_run_app(void);
void pf_construct_graph(void);
void pf_highlight_toggle_node(graph_node_t* node, uint32_t border_color, uint32_t fill_color);
void pf_build_graph(int rows, int cols);
void pf_draw_connections(graph_node_t *node, uint8_t orientation);
void pf_highlight_toggle_connection(graph_node_t *node, uint8_t direction);
void pf_find_shortest_path(graph_node_t *start, graph_node_t *end);
uint16_t pf_core(graph_node_t *start, queue_t *queue, uint16_t next_cost);
void pf_draw_shortest_path(graph_node_t *start, graph_node_t *end);
void pf_shuffle_board(void);
void pf_redraw_board(void);
void pf_draw_buttons(void);

