#include <raylib.h>

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 544;

typedef struct
{
    Rectangle bounds;
    Texture2D sprite;
    int speed;
    int score;
} Player;

Player player;

bool isGamePaused;

Sound hitSound;

void update(float deltaTime)
{
    if (IsKeyDown(KEY_W) && player.bounds.y >= 0)
    {
        player.bounds.y -= player.speed * deltaTime;
    }

    else if (IsKeyDown(KEY_S) && player.bounds.y <= SCREEN_HEIGHT - player.bounds.height)
    {
        player.bounds.y += player.speed * deltaTime;
    }

    else if (IsKeyDown(KEY_D) && player.bounds.x <= SCREEN_WIDTH - player.bounds.width)
    {
        player.bounds.x += player.speed * deltaTime;
    }

    else if (IsKeyDown(KEY_A) && player.bounds.x > 0)
    {
        player.bounds.x -= player.speed * deltaTime;
    }
}

void draw()
{
    BeginDrawing();

    ClearBackground(BLACK);

    DrawText(TextFormat("%i", player.score), 230, 20, 80, WHITE);

    DrawTexture(player.sprite, player.bounds.x, player.bounds.y, WHITE);

    if (isGamePaused)
    {
        DrawText("Game Paused", 220, 100, 80, WHITE);
    }

    EndDrawing();
}

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Starter");
    SetTargetFPS(60);

    Texture2D sprite = LoadTexture("assets/img/alien.png");
    player = {{SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, (float)sprite.width, (float)sprite.height}, sprite, 600, 0};

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        if (IsKeyPressed(KEY_SPACE))
        {
            isGamePaused = !isGamePaused;
            PlaySound(hitSound);
        }

        if (!isGamePaused)
        {
            update(deltaTime);
        }

        draw();
    }

    UnloadTexture(player.sprite);

    CloseWindow();
}