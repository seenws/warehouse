#include <stdio.h>
#include <assert.h>

#include <raylib.h>

#include "../headers/sim.h"
#include "../headers/gridview.h"

#define SCREEN_WIDTH  1200
#define SCREEN_HEIGHT 900

int
main(void)
{
    struct sim sim = {0};
    struct gridview gv = {0};

    if (!sim_init(&sim)) {
        fprintf(stderr, "Unable to initialize board.\n");

        return 1;
    }

    if (!sim_spawn_robot(&sim, 1, (struct position){0, 0})) {
        fprintf(stderr, "Unable to spawn robot.\n");
        sim_free(&sim);

        return 1;
    }

    gridview_init(&gv, (Vector2){ 50,50 }, 51, 800.0f);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Warehouse simulation: demo");

    while (!WindowShouldClose()) {
        gridview_handle_input(&gv);

        BeginDrawing();
        ClearBackground(LIGHTGRAY);
        gridview_draw(&gv);
        EndDrawing();
    }

    CloseWindow();

    sim_free(&sim);

    return 0;
}
