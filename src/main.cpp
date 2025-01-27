#include <raylib.h>
#include <iostream>
#include <vector>
#include <map> //key-value map

const int TOTAL_ROWS = 20;
const int TOTAL_COLUMNS = 10;
const int CELL_SIZE = 30;

bool isGamePaused;

// need to add an offsetValue to properly show the grid cells.
const int OFFSET = 1;

int grid[TOTAL_ROWS][TOTAL_COLUMNS];

typedef struct
{
    int id;
    std::map<int, std::vector<Vector2>> cells;
    int rotationState;
} Block;

Block lBlock;
Block jBlock;
Block iBlock;
Block oBlock;
Block sBlock;
Block tBlock;
Block zBlock;

void rotateBlock(Block &block)
{
    //the oBlock id 4 doesn't need to rotate.
    if (block.id != 4 && block.rotationState < 3)
    {
        block.rotationState++;
    }
    else
    {
        block.rotationState = 0;
    }
}

void update(float deltaTime)
{
    if (IsKeyPressed(KEY_W))
    {
        rotateBlock(zBlock);
    }

    if (IsKeyDown(KEY_S))
    {
    }

    if (IsKeyDown(KEY_D))
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
    for (int row = 0; row < TOTAL_ROWS; row++)
    {
        for (int column = 0; column < TOTAL_COLUMNS; column++)
        {
            int cellValue = grid[row][column];

            DrawRectangle(column * CELL_SIZE + OFFSET, row * CELL_SIZE + OFFSET, CELL_SIZE - OFFSET, CELL_SIZE - OFFSET, getCellColorByIndex(cellValue));
        }
    }
}

void drawBlock(Block &block)
{
    std::vector<Vector2> blockTiles = block.cells[block.rotationState];

    for (Vector2 blockTile : blockTiles)
    {
        // The y value is for the column and the x value y for the row
        DrawRectangle(blockTile.y * CELL_SIZE + OFFSET, blockTile.x * CELL_SIZE + OFFSET, CELL_SIZE - OFFSET, CELL_SIZE - OFFSET, getCellColorByIndex(block.id));
    }
}

void draw()
{
    BeginDrawing();

    ClearBackground(BLACK);

    drawGrid();

    drawBlock(zBlock);

    if (isGamePaused)
    {
        DrawText("Game Paused", 220, 100, 80, WHITE);
    }

    EndDrawing();
}

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

void initializeBlocks()
{
    // defining Blocks 4 rotations with a map id and vector2 2
    lBlock.id = 1;
    lBlock.cells[0] = {{0, 2}, {1, 0}, {1, 1}, {1, 2}};
    lBlock.cells[1] = {{0, 1}, {1, 1}, {2, 1}, {2, 2}};
    lBlock.cells[2] = {{1, 0}, {1, 1}, {1, 2}, {2, 0}};
    lBlock.cells[3] = {{0, 0}, {0, 1}, {1, 1}, {2, 1}};

    jBlock.id = 2;
    jBlock.cells[0] = {{0, 0}, {1, 0}, {1, 1}, {1, 2}};
    jBlock.cells[1] = {{0, 1}, {0, 2}, {1, 1}, {2, 1}};
    jBlock.cells[2] = {{1, 0}, {1, 1}, {1, 2}, {2, 2}};
    jBlock.cells[3] = {{0, 1}, {1, 1}, {2, 0}, {2, 1}};

    iBlock.id = 3;
    iBlock.cells[0] = {{1, 0}, {1, 1}, {1, 2}, {1, 3}};
    iBlock.cells[1] = {{0, 2}, {1, 2}, {2, 2}, {3, 2}};
    iBlock.cells[2] = {{2, 0}, {2, 1}, {2, 2}, {2, 3}};
    iBlock.cells[3] = {{0, 1}, {1, 1}, {2, 1}, {3, 1}};

    //I don't need roation with this block
    oBlock.id = 4;
    oBlock.cells[0] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};

    sBlock.id = 5;
    sBlock.cells[0] = {{0, 1}, {0, 2}, {1, 0}, {1, 1}};
    sBlock.cells[1] = {{0, 1}, {1, 1}, {1, 2}, {2, 2}};
    sBlock.cells[2] = {{1, 1}, {1, 2}, {2, 0}, {2, 1}};
    sBlock.cells[3] = {{0, 0}, {1, 0}, {1, 1}, {2, 1}};

    tBlock.id = 6;
    tBlock.cells[0] = {{0, 1}, {1, 0}, {1, 1}, {1, 2}};
    tBlock.cells[1] = {{0, 1}, {1, 1}, {1, 2}, {2, 1}};
    tBlock.cells[2] = {{1, 0}, {1, 1}, {1, 2}, {2, 1}};
    tBlock.cells[3] = {{0, 1}, {1, 0}, {1, 1}, {2, 1}};

    zBlock.id = 7;
    zBlock.cells[0] = {{0, 0}, {0, 1}, {1, 1}, {1, 2}};
    zBlock.cells[1] = {{0, 2}, {1, 1}, {1, 2}, {2, 1}};
    zBlock.cells[2] = {{1, 0}, {1, 1}, {2, 1}, {2, 2}};
    zBlock.cells[3] = {{0, 1}, {1, 0}, {1, 1}, {2, 0}};
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

    initializeBlocks();

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