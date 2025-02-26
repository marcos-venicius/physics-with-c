#include <math.h>
#include <stdlib.h>
#include <raylib.h>
#include <stdio.h>

float vector2_length(Vector2 v) {
    return sqrtf(v.x * v.x + v.y * v.y);
}

Vector2 scale(Vector2 v, float by) {
    return (Vector2){
        .x = v.x * by,
        .y = v.y * by,
    };
}

Vector2 scalev(Vector2 v, Vector2 s) {
    return (Vector2){
        .x = v.x * s.x,
        .y = v.y * s.y,
    };
}

Vector2 norm(Vector2 v) {
    float length = vector2_length(v);

    return (Vector2){
        .x = v.x / length,
        .y = v.y / length,
    };
}

Vector2 add(Vector2 a, Vector2 b) {
    return (Vector2){
        .x = a.x + b.x,
        .y = a.y + b.y,
    };
}

Vector2 sub(Vector2 a, Vector2 b) {
    return (Vector2){
        .x = a.x - b.x,
        .y = a.y - b.y,
    };
}

float mabs(float n) {
    if (n < 0) return n * -1;

    return n;
}

float dist(Vector2 a, Vector2 b) {
    float x = (b.x - a.x);
    float y = (b.y - a.y);

    return sqrtf(x * x + y * y);
}

Vector2 rotate_90_clockwise(Vector2 v) {
    return (Vector2){
        .x = v.y,
        .y = -v.x
    };
}

Vector2 rotate_90_counterclockwise(Vector2 v) {
    return (Vector2){
        .x = -v.y,
        .y = v.x
    };
}

int circle_size(void) {
    static int size = 5;

    return (size += 5);
}

int main(void) {
    SetConfigFlags(FLAG_FULLSCREEN_MODE);  

    InitWindow(0, 0, "SPACIAL");

    DisableCursor();

    int width = GetScreenWidth();
    int height = GetScreenHeight();

    SetTargetFPS(120);

    float gravity = 0.003;
    float xvel = (double)rand() / RAND_MAX;
    float yvel = (double)rand() / RAND_MAX;
    float cxvel = (double)rand() / RAND_MAX;
    float cyvel = (double)rand() / RAND_MAX;

    Vector2 third = {
        .x = width / 2.f,
        .y = height / 2.f
    };

    int default_a = circle_size();
    int default_b = circle_size();
    int default_c = circle_size();
    int default_d = circle_size();
    int default_e = circle_size();

    Vector2 second = third;
    size_t frame = 0;

    Color a_color = RED;
    Color b_color = GREEN;
    Color c_color = BLUE;
    Color d_color = YELLOW;
    Color e_color = MAGENTA;

    while (!WindowShouldClose()) {
        frame++;

        xvel = xvel + gravity;
        yvel = yvel + gravity;
        cxvel = cxvel + gravity;
        cyvel = cyvel + gravity;
        
        PollInputEvents();

        BeginDrawing();

        ClearBackground(BLACK);

        float nscale = dist(third, second) * 0.2;

        Vector2 first = add(scale(norm(sub(second, third)), nscale), second);
        Vector2 fourth = add(scale(norm(sub(third, second)), nscale * 2), second);
        Vector2 fifth = add(scale(norm(sub(fourth, second)), nscale), fourth);

        Vector2 first_rotated = add(scale(norm(rotate_90_clockwise(first)), nscale), first);
        Vector2 second_rotated = add(scale(norm(rotate_90_clockwise(second)), nscale), second);
        Vector2 third_rotated = add(scale(norm(rotate_90_clockwise(third)), nscale), third);
        Vector2 fourth_rotated = add(scale(norm(rotate_90_clockwise(fourth)), nscale), fourth);
        Vector2 fifth_rotated = add(scale(norm(rotate_90_clockwise(fifth)), nscale), fifth);

        Vector2 first_unrotated = add(scale(norm(rotate_90_counterclockwise(first)), nscale), first);
        Vector2 second_unrotated = add(scale(norm(rotate_90_counterclockwise(second)), nscale), second);
        Vector2 third_unrotated = add(scale(norm(rotate_90_counterclockwise(third)), nscale), third);
        Vector2 fourth_unrotated = add(scale(norm(rotate_90_counterclockwise(fourth)), nscale), fourth);
        Vector2 fifth_unrotated = add(scale(norm(rotate_90_counterclockwise(fifth)), nscale), fifth);

        float coef = sqrtf(nscale) * 0.18;

        int a = default_a * coef;
        int b = default_b * coef;
        int c = default_c * coef;
        int d = default_d * coef;
        int e = default_e * coef;

        DrawLineV(first, second, a_color);
        DrawLineV(second, third, b_color);
        DrawLineV(third, fourth, c_color);
        DrawLineV(fourth, fifth, d_color);

        DrawLineV(first, first_rotated, a_color);
        DrawLineV(second, second_rotated, b_color);
        DrawLineV(third, third_rotated, c_color);
        DrawLineV(fourth, fourth_rotated, d_color);
        DrawLineV(fifth, fifth_rotated, e_color);

        DrawLineV(first_rotated, first_unrotated, a_color);
        DrawLineV(second_rotated, second_unrotated, b_color);
        DrawLineV(third_rotated, third_unrotated, c_color);
        DrawLineV(fourth_rotated, fourth_unrotated, d_color);
        DrawLineV(fifth_rotated, fifth_unrotated, e_color);

        DrawCircleV(first_rotated, a, a_color);
        DrawCircleV(second_rotated, b, b_color);
        DrawCircleV(third_rotated, c, c_color);
        DrawCircleV(fourth_rotated, d, d_color);
        DrawCircleV(fifth_rotated, e, e_color);

        DrawCircleV(first_unrotated, a, a_color);
        DrawCircleV(second_unrotated, b, b_color);
        DrawCircleV(third_unrotated, c, c_color);
        DrawCircleV(fourth_unrotated, d, d_color);
        DrawCircleV(fifth_unrotated, e, e_color);

        DrawCircleV(first, a, a_color);
        DrawCircleV(second, b, b_color);
        DrawCircleV(third, c, c_color);
        DrawCircleV(fourth, d, d_color);
        DrawCircleV(fifth, e, e_color);

        EndDrawing();

        second.x += xvel;
        second.y += yvel;
        third.x += cxvel;
        third.y += cyvel;

        if (first.x >= width - a || first.x <= a) {
            xvel = -xvel;
            second.x += xvel * 2;
        }

        if (first.y >= height - a || first.y <= a) {
            yvel = -yvel;

            second.y += yvel * 2;
        }

        if (third.x >= width - c || third.x <= c) {
            cxvel = -cxvel;
            third.x += cxvel * 2;
        }

        if (third.y >= height - c || third.y <= c) {
            cyvel = -cyvel;
            third.y += cyvel * 2;
        }

        if (frame % 2000 == 0) {
            frame = 0;
            gravity = -gravity;
        }
    }

    CloseWindow();

    return 0;
}
