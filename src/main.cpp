#include <raylib.h>
#include <iostream>

const int TOTAL_ROWS = 20;
const int TOTAL_COLUMNS = 10;
const int CELL_SIZE = 30;

int grid[TOTAL_ROWS][TOTAL_COLUMNS];

bool isGamePaused;

void update(float deltaTime)
{
    if (IsKeyDown(KEY_S))
    {
    }

    else if (IsKeyDown(KEY_D))
    {
    }

    else if (IsKeyDown(KEY_A))
    {
    }
}

Color getCellColorByIndex(int index)
{
    const Color darkGrey = {26, 31, 40, 255};
    const Color green = {47, 230, 23, 255};
    const Color red = {232, 18, 18, 255};
    const Color orange = {226, 116, 17, 255};
    const Color yellow = {237, 234, 4, 255};
    const Color purple = {166, 0, 247, 255};
    const Color cyan = {21, 204, 209, 255};
    const Color blue = {13, 64, 216, 255};

    Color colors[] = {darkGrey, green, red, orange, yellow, purple, cyan, blue};

    return colors[index];
}

void drawGrid()
{
    // need to add an offsetValue to properly show the grid cells.
    int offset = 1;

    for (int row = 0; row < TOTAL_ROWS; row++)
    {
        for (int column = 0; column < TOTAL_COLUMNS; column++)
        {
            int cellValue = grid[row][column];

            DrawRectangle(column * CELL_SIZE + offset, row * CELL_SIZE + offset, CELL_SIZE - offset, CELL_SIZE - offset, getCellColorByIndex(cellValue));
        }
    }
}

void draw()
{
    BeginDrawing();

    ClearBackground(BLACK);

    drawGrid();

    if (isGamePaused)
    {
        DrawText("Game Paused", 220, 100, 80, WHITE);
    }

    EndDrawing();
}

// this will be our grid with 30 columns, 20 rows
// 0 0 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0 0 0

void printGrid()
{
    for (int row = 0; row < TOTAL_ROWS; row++)
    {
        for (int column = 0; column < TOTAL_COLUMNS; column++)
        {
            std::cout << grid[row][column] << " ";
        }

        std::cout << std::endl;
    }
}

int main()
{
    // SCREEN_WIDTH 10 * 30 = 300
    // SCREEN_HEIGHT 20 * 30 = 600
    InitWindow(TOTAL_COLUMNS * CELL_SIZE, TOTAL_ROWS * CELL_SIZE, "Tetris");
    SetTargetFPS(60);

    // initialize grid
    for (int row = 0; row < TOTAL_ROWS; row++)
    {
        for (int column = 0; column < TOTAL_COLUMNS; column++)
        {
            grid[row][column] = 0;
        }
    }

    // adding some test color values
    grid[0][0] = 1;
    grid[3][5] = 4;
    grid[19][9] = 7;
    grid[19][5] = 2;

    printGrid();

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        if (IsKeyPressed(KEY_SPACE))
        {
            isGamePaused = !isGamePaused;
        }

        if (!isGamePaused)
        {
            update(deltaTime);
        }

        draw();
    }

    CloseWindow();
}