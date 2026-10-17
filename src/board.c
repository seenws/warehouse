#include <stdio.h>
#include <stdlib.h>

#include "../headers/board.h"
#include "../headers/robot.h"

static inline struct cell *
board_at(struct board *b, size_t x, size_t y)
{
    return &b->cells[y * b->width + x];
}

static inline const struct cell *
board_at_const(const struct board *b, size_t x, size_t y)
{
    return &b->cells[y * b->width + x];
}

static inline int
is_dropoff(size_t x, size_t y)
{
    return (x % 2 != 0) && (y % 2 != 0);
}

/*
 *  Selects any board position. The caller is responsible for checking whether
 *  the selected cell is useful for its purpose.
 * */
int
board_random_cell(const struct board *b, struct position *pos)
{
    if (b == NULL || b->cells == NULL || pos == NULL)
        return 0;

    pos->x = (uint32_t)((size_t)rand() % b->width);
    pos->y = (uint32_t)((size_t)rand() % b->height);

    return 1;
}

/*
 *  Initiates the board's positions.
 *  Drop-off cells are placed evenly through the grid at odd zero-based coordinates.
 *  
 *  Example 9x9 board: . = floor, o = drop-off
 *
 *   1 2 3 4 5 6 7 8 9
 *  |. . . . . . . . .| 1
 *  |. o . o . o . o .| 2
 *  |. . . . . . . . .| 3
 *  |. o . o . o . o .| 4
 *  |. . . . . . . . .| 5
 *  |. o . o . o . o .| 6
 *  |. . . . . . . . .| 7
 *  |. o . o . o . o .| 8
 *  |. . . . . . . . .| 9
 *
 */
int
board_init(struct board *b)
{
    if (!b) return 0;

    b->width  = BOARD_WIDTH;
    b->height = BOARD_HEIGHT;
    b->cells  = malloc(b->width * b->height * sizeof *b->cells);

    if (!b->cells) {
        b->width = 0;
        b->height = 0;

        return 0;
    }

    /* Fill the grid with free floor cells */
    for (size_t x = 0; x < b->width; ++x)
        for (size_t y = 0; y < b->height; ++y) {
            struct cell *c = board_at(b, x, y);

            c->type = TYPE_FLOOR;
            c->status = STATUS_FREE;
            c->robot_id = ROBOT_ID_NONE;
        }

    /* Place drop-off cells */
    for (size_t x = 1; x < b->width - 1; ++x)
        for (size_t y = 1; y < b->height - 1; ++y)
            if (is_dropoff(x, y))
                board_at(b, x, y)->type = TYPE_DROPOFF;

    return 1;
}

void
board_free(struct board *b)
{
    if (!b) return;

    free(b->cells);
    
    b->cells  = NULL;
    b->width  = 0;
    b->height = 0;
}

/*
 *  Returns true when the position is a free floor cell.
 */
int
board_can_occupy(const struct board *b, uint32_t x, uint32_t y)
{
    if (b == NULL || b->cells == NULL || x >= b->width || y >= b->height)
        return 0;

    const struct cell *c = board_at_const(b, x, y);

    return c->type == TYPE_FLOOR && c->status == STATUS_FREE;
}

/*
 *  Marks a floor cell as occupied.
 *  A drop-off cell and an occupied cell cannot contain a robot.
 * */
int
board_occupy_cell(struct board *b, uint32_t x, uint32_t y, uint32_t robot_id)
{
    if (robot_id == ROBOT_ID_NONE || !board_can_occupy(b, x, y))
        return 0;

    struct cell *c = board_at(b, x, y);

    c->status = STATUS_OCCUPIED;
    c->robot_id = robot_id;

    return 1;
}

int
board_vacate_cell(struct board *b, uint32_t x, uint32_t y, uint32_t robot_id)
{
    if (b == NULL || b->cells == NULL || x >= b->width || y >= b->height)
        return 0;

    struct cell *c = board_at(b, x, y);

    if (c->status != STATUS_OCCUPIED || c->robot_id != robot_id)
        return 0;

    c->status = STATUS_FREE;
    c->robot_id = ROBOT_ID_NONE;

    return 1;
}

uint32_t
board_robot_at(const struct board *b, uint32_t x, uint32_t y)
{
    if (b == NULL || b->cells == NULL || x >= b->width || y >= b->height)
        return ROBOT_ID_NONE;

    const struct cell *c = board_at_const(b, x, y);

    if (c->status != STATUS_OCCUPIED)
        return ROBOT_ID_NONE;

    return c->robot_id;
}

void
board_print(const struct board *b)
{
    if (b == NULL || b->cells == NULL) return;

    for (size_t y = b->height; y-- > 0;) {
        for (size_t x = 0; x < b->width; ++x) {
            const struct cell *c = &b->cells[y * b->width + x];

            if (c->status == STATUS_OCCUPIED)
                putchar('R');
            
            else if (c->type == TYPE_DROPOFF)
                putchar('x');
            
            else
                putchar('o');

            putchar(' ');
        }

    putchar('\n');
    
    }
}
