#include <stddef.h>
#include <raylib.h>
#include <raymath.h>
#include <assert.h>

#include "../headers/gridview.h"

#define ZOOMSTEP 1.15f
#define MINZOOM 1.0f
#define MAXZOOM 20.0f

void
gridview_init(struct gridview *gv, Vector2 start, size_t n, float gridsz)
{
    gv->start = start;
    gv->n = n;
    gv->gridsz = gridsz;
    gv->zoom = 1.0f;
    gv->pan = (Vector2){ 0,0 };
}

void
gridview_handle_input(struct gridview *gv)
{
    float wheel = GetMouseWheelMove();

    if (wheel != 0.0f) {
        float before = gv->zoom;
        gv->zoom = Clamp(gv->zoom * powf(ZOOMSTEP, wheel), MINZOOM, MAXZOOM);

        /* Zoom towards the mouse cursor */
        Vector2 m = GetMousePosition();
        Vector2 rel = { m.x - gv->pan.x, m.y - gv->pan.y };
        float ratio = gv->zoom / before;

        gv->pan.x = m.x - rel.x * ratio;
        gv->pan.y = m.y - rel.y * ratio;
    }
}

static Vector2
gridview_world_to_screen(const struct gridview *gv, Vector2 world)
{
    return (Vector2){
        (world.x - gv->start.x) * gv->zoom + gv->start.x + gv->pan.x,
        (world.y - gv->start.y) * gv->zoom + gv->start.y + gv->pan.y,
    };
}

void gridview_draw(const struct gridview *gv)
{
    const float cellsz = (float)gv->gridsz / gv->n;

    /* Drop-zone fills */
    for (size_t row = 1; row < gv->n; row += 2)
        for (size_t col = 1; col < gv->n; col += 2) {
            Vector2 tl = gridview_world_to_screen(gv, (Vector2){
                gv->start.x + col * cellsz,
                gv->start.y + row * cellsz,
            });
            Vector2 br = gridview_world_to_screen(gv, (Vector2){
                gv->start.x + (col + 1) * cellsz,
                gv->start.y + (row + 1) * cellsz,
            });
            DrawRectangle(tl.x, tl.y, br.x - tl.x, br.y - tl.y, DARKGRAY);
        }

    /* Perimeter of the board */
    Vector2 tl = gridview_world_to_screen(gv, gv->start);
    Vector2 br = gridview_world_to_screen(gv, (Vector2){
        gv->start.x + gv->gridsz,
        gv->start.y + gv->gridsz,
    });
    DrawRectangleLinesEx(
        (Rectangle){ tl.x, tl.y, br.x - tl.x, br.y - tl.y },
        1.0f,
        GRAY
    );
}

