#include <stdint.h>

#define PF_ROWS 13
#define PF_COLS 20
#define TRUE 1
#define FALSE 0
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
    uint8_t enabled;
} graph_node_t;

typedef struct graph_tag_t{
   graph_node_t *root;
   uint16_t graph_size;
   uint16_t rows;
   uint16_t cols;
} graph_t;

void pathfinder_run_app(void);
void construct_graph(void);
void build_graph(int rows, int cols);

