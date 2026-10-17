#include "../headers/robot.h"

int
robot_init(struct robot *r, uint32_t id, struct position pos)
{
    if (!r || id == ROBOT_ID_NONE)
        return 0;

    r->pos = pos;
    r->id = id;
    r->carrying = PARCEL_ID_NONE;

    return 1;
}

void
robot_free(struct robot *r)
{
    if (!r)
        return;

    r->pos.x = 0;
    r->pos.y = 0;
    r->id = ROBOT_ID_NONE;
    r->carrying = PARCEL_ID_NONE;
}

