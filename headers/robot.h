#ifndef WAREHOUSE_HEADERS_ROBOT_H_
#define WAREHOUSE_HEADERS_ROBOT_H_

#include <stdint.h>

#define ROBOT_ID_NONE UINT32_MAX
#define PARCEL_ID_NONE UINT32_MAX

enum parcel_status {
    PARCEL_STATUS_WAITING,
    PARCEL_STATUS_CARRIED,
    PARCEL_STATUS_DELIVERED,
    PARCEL_STATUS_END
};

struct position {
    uint32_t x;
    uint32_t y;
};

/*
 *  A parcel is a package that must be delivered to a position on the board.
 *
 *  Its status is STATUS_WAITING until a robot collects it, STATUS_CARRIED while
 *  a robot is transporting it, and STATUS_DELIVERED once it reaches dest
 *  carried_by identifies the robot carrying the parcel and is relevant only
 *  when the parcel status is STATUS_CARRIED.
 * */
struct parcel {
    struct position dest;
    uint32_t id;
    uint32_t carried_by;
    uint8_t status;
};

/*
 *  A robot is an entity that moves around the board and transports parcels.
 *  A robot is able to carry one parcel at a time, and must deliver it before moving on to the next one.
 *  For a naive implementation, I'll just teleport the next parcel in the queue onto the appropriate robot,
 *  in the future I might implement loading docks.
 *
 *  `carrying` denotes the ID of the parcel the robot is currently transporting.
 *  This value is PARCEL_ID_NONE if the robot is not carrying a parcel.
 * */
struct robot {
    struct position pos;
    uint32_t id;
    uint32_t carrying;
};

int robot_init(struct robot *r, uint32_t id, struct position pos);
void robot_free(struct robot *r);

#endif
