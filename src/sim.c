#include <stdio.h>
#include <stdlib.h>

#include "../headers/board.h"
#include "../headers/robot.h"
#include "../headers/sim.h"

static struct robot *
sim_find_robot(struct sim *simp, uint32_t id)
{
    if (!simp) return NULL;

    for (size_t i = 0; i < simp->robot_count; ++i)
        if (simp->robots[i].id == id)
            return &simp->robots[i];

    return NULL;
}

static int
sim_reserve_robots(struct sim *simp, size_t required)
{
    if (required <= simp->robot_capacity)
        return 1;

    size_t new_capacity = simp->robot_capacity == 0 ? 4 : simp->robot_capacity;

    while (new_capacity < required) {
        if (new_capacity > SIZE_MAX / 2)
            return 0;

        new_capacity *= 2;
    }

    if (new_capacity > SIZE_MAX / sizeof *simp->robots)
        return 0;

    struct robot *robots = realloc(simp->robots,
                                   new_capacity * sizeof *simp->robots);
    if (robots == NULL)
        return 0;

    simp->robots = robots;
    simp->robot_capacity = new_capacity;

    return 1;
}

int
sim_init(struct sim *simp)
{
    if (simp == NULL) return 0;

    simp->robots = NULL;
    simp->robot_count = 0;
    simp->robot_capacity = 0;

    simp->parcels = NULL;
    simp->parcel_count = 0;
    simp->parcel_capacity = 0;

    if (!board_init(&simp->board)) {
        fprintf(stderr, "sim_init: failure to initialize board.\n");
        
        return 0;
    }

    return 1;
}

void
sim_free(struct sim *simp)
{
    if (simp == NULL)
        return;

    for (size_t i = 0; i < simp->robot_count; ++i)
        robot_free(&simp->robots[i]);

    free(simp->robots);
    free(simp->parcels);
    board_free(&simp->board);

    simp->robots = NULL;
    simp->robot_count = 0;
    simp->robot_capacity = 0;

    simp->parcels = NULL;
    simp->parcel_count = 0;
    simp->parcel_capacity = 0;
}

/*
 *  A robot belongs in the simulation's robots array. The board only tracks its location.
 *  Placing a robot on the board means updating both.
 *
 *  robot.pos = pos;
 *  cell_at(pos)->status = STATUS_OCCUPIED;
 *
 *  Moving a robot follows the same philosophy.
 *
 *  robot.pos = new_pos;
 *  cell_at(pos)->status = STATUS_FREE;
 *  cell_at(new_pos)->status = STATUS_OCCUPIED;
 */
int
sim_spawn_robot(struct sim *simp, uint32_t id, struct position pos)
{
    if (simp == NULL || id == ROBOT_ID_NONE)
        return 0;

    if (!board_can_occupy(&simp->board, pos.x, pos.y)) {
        fprintf(stderr, "sim_spawn_robot: position is unavailable.\n");

        return 0;
    }

    if (sim_find_robot(simp, id) != NULL) {
        fprintf(stderr, "sim_spawn_robot: duplicate robot ID.\n");
        return 0;
    }

    if (!sim_reserve_robots(simp, simp->robot_count + 1))
        return 0;

    if (!robot_init(&simp->robots[simp->robot_count], id, pos))
        return 0;

    if (!board_occupy_cell(&simp->board, pos.x, pos.y, id)) {
        robot_free(&simp->robots[simp->robot_count]);
        return 0;
    }

    ++simp->robot_count;

    return 1;
}

int
sim_move_robot(struct sim *simp, uint32_t id, struct position pos)
{
    struct robot *r = sim_find_robot(simp, id);

    if (r == NULL || !board_can_occupy(&simp->board, pos.x, pos.y))
        return 0;

    struct position old_pos = r->pos;

    if (!board_vacate_cell(&simp->board, old_pos.x, old_pos.y, id))
        return 0;

    if (!board_occupy_cell(&simp->board, pos.x, pos.y, id)) {
        (void)board_occupy_cell(&simp->board, old_pos.x, old_pos.y, id);
        return 0;
    }

    r->pos = pos;

    return 1;
}

struct robot *
sim_robot_at(struct sim *simp, struct position pos)
{
    uint32_t id;

    if (simp == NULL)
        return NULL;

    id = board_robot_at(&simp->board, pos.x, pos.y);
    if (id == ROBOT_ID_NONE)
        return NULL;

    return sim_find_robot(simp, id);
}

void
sim_step(struct sim *simp)
{
    (void)simp;

    return;
}
