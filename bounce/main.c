#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

#define NULL ((void*)0)

#define RESTITUTION 0.8f
#define FOREACH(ball, balls) for (Ball *ball = balls.head; ball != NULL; ball = ball->next)

typedef struct Ball Ball;

struct Ball {
    size_t id;
    Vector2 pos, velocity;
    float radius, mass;

    Ball *next;
    Ball *prev;
};

typedef struct {
    Ball *head;
    Ball *tail;
} Balls;

Balls create_balls(void) {
    return (Balls){
        .head = NULL,
        .tail = NULL
    };
}

Ball *add_new_ball(Balls *balls, Vector2 initial_pos, Vector2 initial_velocity, float radius, float mass) {
    static size_t id = 0;

    Ball *ball = malloc(sizeof(Ball));

    if (ball == NULL) {
        perror("By more RAM");
        exit(1);
    }

    ball->id = ++id;
    ball->next = NULL;
    ball->prev = balls->tail;
    ball->velocity = initial_velocity;
    ball->radius = radius;
    ball->mass = mass;
    ball->pos = initial_pos;

    if (balls->head == NULL) {
        balls->tail = balls->head = ball;
    } else {
        balls->tail = balls->tail->next = ball;
    }

    return ball;
}

void resolve_ball_overlap(Ball *A, Ball *B) {
    Vector2 delta = { .x = B->pos.x - A->pos.x, .y = B->pos.y - B->pos.x };

    float distance = sqrtf(delta.x * delta.x + delta.y * delta.y);
    float overlap = (A->radius + B->radius) - distance;

    if (overlap > 0) {
        Vector2 normal = { .x = delta.x / distance, .y = delta.y / distance };

        float correction = overlap / 2;

        A->pos.x -= normal.x * correction;
        A->pos.y -= normal.y * correction;
        B->pos.x += normal.x * correction;
        B->pos.y += normal.y * correction;
    }
}

void handle_ball_collision(Ball *A, Ball *B) {
    // direction between centers
    Vector2 normal = { .x = B->pos.x - A->pos.x, .y = B->pos.y - A->pos.y };
    float dist = sqrtf(normal.x * normal.x + normal.y * normal.y);

    // normalizing vector
    normal.x /= dist;
    normal.y /= dist;

    Vector2 tangent = { .x = -normal.y, .y = normal.x };

    float vAn = A->velocity.x * normal.x + A->velocity.y * normal.y;
    float vAt = A->velocity.x * tangent.x + A->velocity.y * tangent.y;
    float vBt = B->velocity.x * tangent.x + B->velocity.y * tangent.y;

    float vAnAfter = -RESTITUTION * vAn;
    float vBnAfter = (1 + RESTITUTION) * (A->mass / (A->mass + B->mass)) * vAn;

    A->velocity.x = vAnAfter * normal.x + vAt * tangent.x;
    A->velocity.y = vAnAfter * normal.y + vAt * tangent.y;
    B->velocity.x = vBnAfter * normal.x + vBt * tangent.x;
    B->velocity.y = vBnAfter * normal.y + vBt * tangent.y;
}

bool ball_collided(Ball *A, Ball *B) {
    float dx = B->pos.x - A->pos.x;
    float dy = B->pos.y - A->pos.y;
    float distance = sqrtf(dx * dx + dy * dy);

    return distance <= A->radius + B->radius;
}

void delete_ball(Balls *balls, Ball *ball) {
    if (balls->head == NULL || ball == NULL) return;

    if (balls->head == ball) balls->head = ball->next;

    if (balls->head == NULL) balls->tail = NULL;

    if (ball->next != NULL) ball->next->prev = ball->prev;

    if (ball->prev != NULL) ball->prev->next = ball->next;

    free(ball);
}

void free_balls(Balls *balls) {
    Ball *curr = balls->head;

    while (curr != NULL) {
        Ball *next = curr->next;

        free(curr);

        curr = next;
    }

    balls->head = NULL;
    balls->tail = NULL;
}

int main(void) {
    SetConfigFlags(FLAG_FULLSCREEN_MODE | FLAG_MSAA_4X_HINT);

    InitWindow(0, 0, "BOUNCE");

    int width = GetScreenWidth();
    int height = GetScreenHeight();

    int FPS = 240;

    SetTargetFPS(FPS);

    Balls balls = create_balls();

    int pixels_per_meter = 100;
    float gravity = (9.81 * pixels_per_meter) / (FPS * FPS);

    float radius = 50;

    while (!WindowShouldClose()) {
        PollInputEvents();

        BeginDrawing();

        ClearBackground(BLACK);

        FOREACH(ball, balls) {
            ball->velocity.y += gravity;
            ball->pos.x += ball->velocity.x;
            ball->pos.y += ball->velocity.y;

            if (ball->pos.y + ball->radius >= height) {
                ball->velocity.y *= -RESTITUTION;
                ball->pos.y = height - ball->radius;
            }

            if (ball->pos.x + ball->radius <= 0 || ball->pos.x - ball->radius >= width) {
                delete_ball(&balls, ball);
                continue;
            }

            FOREACH(other, balls) {
                if (other->id == ball->id) continue;

                if (ball_collided(ball, other)) {
                    resolve_ball_overlap(ball, other);

                    handle_ball_collision(ball, other);
                }
            }

            DrawCircleV(ball->pos, ball->radius, WHITE);
        }

        EndDrawing();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mouse = GetMousePosition();

            if (mouse.x - radius <= 0) {
                mouse.x = radius;
            } else if (mouse.x + radius >= width) {
                mouse.x = width - radius;
            }

            if (mouse.y - radius <= 0) {
                mouse.y = radius;
            } else if (mouse.y + radius >= height) {
                mouse.y = height - radius;
            }

            add_new_ball(&balls, mouse, (Vector2){0}, radius, 10);
        }

        if (IsKeyPressed(KEY_Q) || IsKeyPressed(KEY_ESCAPE)) break;

        if (IsKeyPressed(KEY_R)) free_balls(&balls);
    }

    CloseWindow();

    free_balls(&balls);

    return 0;
}
