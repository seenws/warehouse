#ifndef WAREHOUSE_HEADERS_GRIDVIEW_H_
#define WAREHOUSE_HEADERS_GRIDVIEW_H_

#include <stddef.h>
#include <raylib.h>

struct gridview {
    Vector2 start; /* Grid origin space in the world. */
    size_t n;
    size_t gridsz;
    
    float zoom;
    Vector2 pan;
};

void gridview_init(struct gridview *gv, Vector2 start, size_t n, float gridsz);
void gridview_handle_input(struct gridview *gv);
void gridview_draw(const struct gridview *gv);

#endif /* WAREHOUSE_HEADERS_GRIDVIEW_H_ */
