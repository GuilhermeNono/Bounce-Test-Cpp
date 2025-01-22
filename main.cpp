#include <iostream>
#include <ctime>
#include <raylib.h>
#include <flecs.h>

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;
int bounce = 0;

struct Position {
    float x, y;
};

struct Velocity {
    float dX, dY;
};

void MoveSystem(Position &position, Velocity &velocity) {

    float deltaTime = GetFrameTime();

    position.x += velocity.dX * deltaTime;
    position.y += velocity.dY * deltaTime;

    if (position.x < 0 || position.x > SCREEN_WIDTH) {
        velocity.dX = -velocity.dX;
        bounce++;
    }
    if (position.y < 0 || position.y > SCREEN_HEIGHT) {
        velocity.dY = -velocity.dY;
        bounce++;
    };
}

float randomFloat(float min, float max) {
    return min + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

int main() {
    // Inicializa o Raylib
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib + Flecs");
    SetTargetFPS(165);

    // Inicializa o Flecs
    flecs::world world;

    //Registra os componentes-;
    world.component<Velocity>();

    // Cria o sistema de movimentação
    world.system<Position, Velocity>()
            .each(MoveSystem);

    // Seed para numeros aleatorios
    std::srand(std::time(nullptr));
    for (int i = 0; i < 10; i++) {
        world.entity()
                .set<Position>({
                    static_cast<float>(SCREEN_WIDTH / 2),
                    static_cast<float>(SCREEN_HEIGHT / 2)
                })
                .set<Velocity>({
                    randomFloat(-400.f, 400.f),
                    randomFloat(-400.f, 400.f),
                });
    }

    // Loop principal do jogo
    while (!WindowShouldClose()) {
        world.progress();

        BeginDrawing();
        ClearBackground(BLACK);

        // Desenhando FPS na tela
        DrawText(TextFormat("%i", GetFPS()), 0, 10, 30, WHITE);

        // Desenhando a quantidade de Bounce nas bordas
        DrawText(TextFormat("%i", bounce), SCREEN_WIDTH - 50, 10, 30, WHITE);

        world.query<Position>().each([&](auto &position) {
            DrawCircle(static_cast<int>(position.x), static_cast<int>(position.y), 20, RED);
        });

        EndDrawing();
    }

    CloseWindow();
}
