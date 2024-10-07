#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <locale.h>
#include "functions.h"

char forest_matrix[GRID_SIZE][GRID_SIZE];
pthread_mutex_t cell_mutex[GRID_SIZE][GRID_SIZE];
pthread_cond_t fire_alert = PTHREAD_COND_INITIALIZER;
pthread_mutex_t fire_alert_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t central_mutex = PTHREAD_MUTEX_INITIALIZER;

int main() {
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL));

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            forest_matrix[i][j] = (rand() % 3 == 0) ? SENSOR_NODE : EMPTY_CELL;
            pthread_mutex_init(&cell_mutex[i][j], NULL);
        }
    }

    display_forest();

    pthread_t node_threads[GRID_SIZE][GRID_SIZE];
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (forest_matrix[i][j] == SENSOR_NODE) {
                Node* new_node = (Node*)malloc(sizeof(Node));
                new_node->row = i;
                new_node->col = j;
                pthread_create(&node_threads[i][j], NULL, node_function, (void*)new_node);
            }
        }
    }

    pthread_t fire_starter_thread;
    pthread_create(&fire_starter_thread, NULL, ignite_fire, NULL);

    pthread_t main_control_thread;
    pthread_create(&main_control_thread, NULL, command_center_function, NULL);

    pthread_join(fire_starter_thread, NULL);
    pthread_join(main_control_thread, NULL);

    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if (forest_matrix[i][j] == SENSOR_NODE) {
                pthread_join(node_threads[i][j], NULL);
            }
        }
    }

    return 0;
}
