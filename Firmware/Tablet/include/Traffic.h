#include <stdint.h>
#include <type.h>


#define TR_MAX_CARS_CNT             100
#define TR_VERTICAL_ROADS           2
#define TR_VERTICAL_NODES           41
#define TR_VERTICAL_NODES_SPACING   10
#define TR_HORIZONTAL_ROADS         2
#define TR_HORIZONTAL_NODES         73
#define TR_HORIZONTAL_NODES_SPACING 10
#define TR_LIGHT_RED                0
#define TR_LIGHT_GREEN              1
#define TR_NODES_TOTAL_CNT          TR_VERTICAL_ROADS*TR_VERTICAL_NODES+TR_HORIZONTAL_ROADS*TR_HORIZONTAL_NODES
#define TR_UP                       0
#define TR_DOWN                     1
#define TR_LEFT                     2
#define TR_RIGHT                    3
#define TR_CAR_STOPPED              0
#define TR_CAR_MOVING_START         1
#define TR_CAR_MOVING_HALFWAY       3
#define TR_CAR_MOVING_END           5
#define TR_NO_CAR                   0xFF
#define TR_CAR_PLACEHOLDER          0xDD
#define TR_SHORT_PATH_WIDTH         2
#define TR_ROAD_WIDTH               30
#define TR_ROAD_OFFSET              15
#define TR_SHOULDER_LINE_WIDTH      1
#define TR_CENTER_LINE_SPACING      1
#define TR_CENTER_LINE_WIDTH        1
#define TR_SHORT_PATH_COLOR_U       BLUE
#define TR_SHORT_PATH_COLOR_R       BLUE
#define TR_SHORT_PATH_COLOR_D       BLUE
#define TR_SHORT_PATH_COLOR_L       BLUE
#define TR_LIGHTS_WIDTH             10    
#define TR_LIGHTS_DEPTH             4    
#define TR_LIGHTS_OFFSET            0    
#define TR_INTERSECTION             4
#define TR_PREINTERSECTION          4
#define TR_DIRECTIONS_CNT           TR_HORIZONTAL_ROADS+TR_VERTICAL_ROADS-1
#define TR_ENDPOINTS_CNT            ((TR_VERTICAL_ROADS+TR_HORIZONTAL_ROADS)*2)
#define TR_PATHS_TOTAL_CNT          (TR_ENDPOINTS_CNT*(TR_ENDPOINTS_CNT-1)*4*3*2)


/////???
//struct tr_graph_node_tag_t;
//typedef struct tr_graph_node_tag_t tr_graph_node_t;
///???





typedef struct tr_car_queue_node_t{
    struct tr_car_queue_node_t *next;
    struct tr_car_queue_node_t *prev;
    struct tr_car_t *car;
} tr_car_queue_node_t;


typedef struct tr_car_queue_t{
    struct tr_car_queue_node_t *front;
    struct tr_car_queue_node_t *back;
    uint16_t items;
} tr_car_queue_t;


typedef struct tr_road{
    uint16_t x_start;
    uint16_t y_start;
    uint16_t x_end;
    uint16_t y_end;
} tr_road;


typedef struct tr_road_t{
    tr_road *vertical;
    tr_road *horizontal;
} tr_road_t;


typedef struct tr_lights_t{
    uint32_t up;
    uint32_t down;
    uint32_t left;
    uint32_t right;
} tr_lights_t;



//typedef struct tr_car_spots_t{
//    tr_car_t *up;
//    tr_car_t *down;
//    tr_car_t *left;
//    tr_car_t *right;        
//} tr_car_spots;

typedef struct tr_car_spots{
    uint8_t up;
    uint8_t down;
    uint8_t left;
    uint8_t right;        
} tr_car_spots;

typedef struct tr_graph_node_tag_t{
    uint32_t id;
    struct tr_graph_node_tag_t *right; 
    struct tr_graph_node_tag_t *left; 
    struct tr_graph_node_tag_t *up; 
    struct tr_graph_node_tag_t *down; 
    struct tr_coords{
        uint16_t row;
        uint16_t col;
    } tr_coords;
    tr_car_spots tr_car_spots;
    uint32_t visited;
    uint32_t is_intersection;
    tr_lights_t *lights;
} tr_graph_node_t;


typedef struct tr_car_t{
    uint16_t id;
    uint32_t color;
    uint8_t direction_driving_current;
    int8_t *directions;
    uint8_t start;
    uint8_t destination;
    uint8_t direction_step_current;
    tr_graph_node_t *current_node;
    uint8_t is_moving;
    uint32_t time_active;
} tr_car_t;



typedef struct tr_graph_tag_t{
   tr_graph_node_t *root;
   uint16_t graph_size;
   uint16_t rows;
   uint16_t cols;
} tr_graph_t;


typedef struct tr_queue_node_tag_t{
    uint16_t cost;
    tr_graph_node_t *from_p;
    tr_graph_node_t *to_p;
    struct tr_queue_node_tag_t *next;
} tr_queue_node_t;

  
typedef struct tr_queue_tag_t{
    tr_queue_node_t *front;
    tr_queue_node_t *back;
    tr_graph_node_t *end;
    uint16_t items;
} tr_queue_t;


typedef struct tr_endpoints_t{
    tr_graph_node_t *endpoint;
    uint32_t color;
    int8_t start_direction;
} tr_endpoints_t;



void traffic_run_app(void);
void tr_shuffle_roads(void);
void tr_create_roads(void);
void tr_draw_roads(void);
void tr_draw_nodes(void);
void tr_find_intersections(void);
void tr_clear_intersections(void);
void tr_find_endpoints(void);
void tr_add_endpoint(tr_graph_node_t *node, int *endpoint_cnt_p);
void tr_draw_traffic_lights(void);
void tr_find_shortest_path(tr_graph_node_t *start, tr_graph_node_t *end, uint8_t nav_mode);
void tr_draw_shortest_path(tr_graph_node_t *start, tr_graph_node_t *end);
void tr_highlight_toggle_connection(tr_graph_node_t *node, uint8_t direction);
void tr_reset_nodes_visited(void);
void tr_reset_costs(void);
void tr_process_direction_arr(int i, int j);
void tr_draw_loading_bar(uint16_t paths_completed, uint8_t init);
int8_t tr_build_direction_arr(uint8_t current_dir, tr_graph_node_t *rover);
void tr_car_enqueue(tr_car_queue_t *queue, tr_car_queue_node_t *node);
tr_car_queue_node_t *tr_car_dequeue(tr_car_queue_t *queue, uint16_t idx);
void tr_randomize_car(tr_car_queue_node_t *car);
void tr_car_activate(void);
void tr_car_deactivate(tr_car_queue_node_t *car_p);
uint16_t tr_core(tr_graph_node_t *start, tr_queue_t *queue, uint16_t next_cost, uint8_t nav_mode);