#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define GRID_SIZE 30

#define EMPTY_CELL '-'
#define SENSOR_NODE 'T'
#define FIRE '@'
#define BURNED_AREA '/'

typedef struct {
    int row;
    int col;
} Node;

void display_forest();
void* node_function(void* arg);
void* ignite_fire(void* arg);
void* command_center_function(void* arg);
void notify_adjacent_nodes(int row, int col);

#endif
