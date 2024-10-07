#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include "functions.h"

extern char forest_matrix[GRID_SIZE][GRID_SIZE];
extern pthread_mutex_t cell_mutex[GRID_SIZE][GRID_SIZE];
extern pthread_cond_t fire_alert;
extern pthread_mutex_t fire_alert_mutex;
extern pthread_mutex_t central_mutex;

int is_boundary_node(int row, int col) {
    return (row == 0 || row == GRID_SIZE - 1 || col == 0 || col == GRID_SIZE - 1);
}

void display_forest() {
    pthread_mutex_lock(&central_mutex);
    printf("Floresta:\n");
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            printf("%c ", forest_matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    pthread_mutex_unlock(&central_mutex);
}

_Bool fire_detected[GRID_SIZE][GRID_SIZE];

void* node_function(void* arg) {
    Node* node = (Node*)arg;

    while (1) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                int adj_row = node->row + i;
                int adj_col = node->col + j;

                if (adj_row >= 0 && adj_row < GRID_SIZE && adj_col >= 0 && adj_col < GRID_SIZE) {
                    pthread_mutex_lock(&cell_mutex[adj_row][adj_col]);
                    if (forest_matrix[adj_row][adj_col] == FIRE && !fire_detected[node->row][node->col]) {
                        printf("O sensor [%d, %d] identificou incendio na posicao [%d, %d]!\n", node->row, node->col, adj_row, adj_col);

                        fire_detected[node->row][node->col] = 1;

                        if (is_boundary_node(node->row, node->col)) {
                            pthread_mutex_lock(&fire_alert_mutex);
                            pthread_cond_signal(&fire_alert);
                            pthread_mutex_unlock(&fire_alert_mutex);
                        } else {
                            notify_adjacent_nodes(node->row, node->col);
                        }
                    }
                    pthread_mutex_unlock(&cell_mutex[adj_row][adj_col]);
                }
            }
        }
        sleep(1);
    }

    return NULL;
}

void notify_adjacent_nodes(int row, int col) {
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int adj_row = row + i;
            int adj_col = col + j;

            if (adj_row >= 0 && adj_row < GRID_SIZE && adj_col >= 0 && adj_col < GRID_SIZE) {
                pthread_mutex_lock(&cell_mutex[adj_row][adj_col]);
                if (forest_matrix[adj_row][adj_col] == SENSOR_NODE && (adj_row != row || adj_col != col)) {
                    printf("Sensor [%d, %d] notifica o sensor [%d, %d] sobre o fogo!\n", row, col, adj_row, adj_col);

                    if (is_boundary_node(adj_row, adj_col)) {
                        pthread_mutex_lock(&fire_alert_mutex);
                        pthread_cond_signal(&fire_alert);
                        pthread_mutex_unlock(&fire_alert_mutex);
                    } else {
                        notify_adjacent_nodes(adj_row, adj_col);
                    }
                }
                pthread_mutex_unlock(&cell_mutex[adj_row][adj_col]);
            }
        }
    }
}

void* ignite_fire(void* arg) {
    while (1) {
        int row = rand() % GRID_SIZE;
        int col = rand() % GRID_SIZE;

        pthread_mutex_lock(&cell_mutex[row][col]);
        if (forest_matrix[row][col] == EMPTY_CELL) {
            forest_matrix[row][col] = FIRE;
            printf("Fogo comecou [%d, %d]!\n", row, col);
            display_forest();
        }
        pthread_mutex_unlock(&cell_mutex[row][col]);

        sleep(3);
    }

    return NULL;
}

void* command_center_function(void* arg) {
    while (1) {
        pthread_mutex_lock(&fire_alert_mutex);
        pthread_cond_wait(&fire_alert, &fire_alert_mutex);
        pthread_mutex_unlock(&fire_alert_mutex);

        printf("Centro de comando notificado do incendio!\n");

        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
                pthread_mutex_lock(&cell_mutex[i][j]);
                if (forest_matrix[i][j] == FIRE) {
                    printf("O centro de comando está apagando o fogo em [%d, %d]!\n", i, j);
                    forest_matrix[i][j] = BURNED_AREA;
                }
                pthread_mutex_unlock(&cell_mutex[i][j]);
            }
        }

        pthread_mutex_lock(&central_mutex);
        display_forest();
        pthread_mutex_unlock(&central_mutex);
    }

    return NULL;
}
