#ifndef WAREHOUSE_HEADERS_SIM_H_
#define WAREHOUSE_HEADERS_SIM_H_

#include "board.h"
#include "robot.h"

struct sim {
    struct board board;
    
    struct robot *robots;
    size_t robot_count;
    size_t robot_capacity;

    struct parcel *parcels;
    size_t parcel_count;
    size_t parcel_capacity;
};

int sim_init(struct sim *simp);
void sim_free(struct sim *simp);
int sim_spawn_robot(struct sim *simp, uint32_t id, struct position pos);
int sim_move_robot(struct sim *simp, uint32_t id, struct position pos);
struct robot *sim_robot_at(struct sim *simp, struct position pos);
void sim_step(struct sim *simp);

#endif
