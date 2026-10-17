#ifndef WAREHOUSE_HEADERS_BOARD_H_
#define WAREHOUSE_HEADERS_BOARD_H_

#include <stddef.h>
#include <stdint.h>

#include "robot.h"

#define BOARD_HEIGHT 9
#define BOARD_WIDTH  9

enum cell_type {
    TYPE_FLOOR = 0,
    TYPE_DROPOFF,
    TYPE_END
};

enum cell_status {
    STATUS_FREE = 0,
    STATUS_OCCUPIED,
    STATUS_END
};

/*
 *  A cell is defined as a position on the board.
 *  Its type may be either 0 (TYPE_FLOOR) denoting a solid space a robot can occupy,
 *  or 1 (TYPE_DROPOFF) denoting a space a robot can drop a parcel into.
 *
 *  A cell's status may be either 0 (STATUS_FREE) denoting a space a robot can move to,
 *  or 1 (STATUS_OCCUPIED) denoting a space a robot is currently occupying.
 *  
 *  The caller is responsible for determining if the status of a given cell is relevant,
 *  for instance a cell of type TYPE_DROPOFF has no concept of being free or occupied,
 *  but a cell of type TYPE_FLOOR and status STATUS_FREE may in the next timestep result in a collision
 *  if two robots are approaching the same cell from the same distance.
 *
 *  |. . R . . . . . .| -> both approaching x -> |. . . . . . . . .| -> Collision
 *  |. R x . . . . . .|                          |. . C . . . . . .|
 *
 * */
struct cell {
    uint32_t robot_id; /* Only meaningful when status == STATUS_OCCUPIED. */
    uint8_t type;
    uint8_t status;
};

/*
 *  The board is a collection of cells.
 *
 *  board(0,0) denotes the bottom-left corner of the board.
 * */
struct board {
    size_t width;
    size_t height;

    struct cell *cells;
};

int board_init(struct board *b);
void board_free(struct board *b);
void board_print(const struct board *b);
int board_can_occupy(const struct board *b, uint32_t x, uint32_t y);
int board_occupy_cell(struct board *b, uint32_t x, uint32_t y, uint32_t robot_id);
int board_vacate_cell(struct board *b, uint32_t x, uint32_t y, uint32_t robot_id);
uint32_t board_robot_at(const struct board *b, uint32_t x, uint32_t y);
int board_random_cell(const struct board *b, struct position *pos);

#endif
