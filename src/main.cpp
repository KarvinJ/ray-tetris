#include <raylib.h>
#include <iostream>
#include <vector>
#include <map> //key-value map

const int TOTAL_ROWS = 20;
const int TOTAL_COLUMNS = 10;
const int CELL_SIZE = 30;

int grid[TOTAL_ROWS][TOTAL_COLUMNS];

// need to add an offsetValue to properly show the grid cells.
const int POSITION_OFFSET = 11;
const int CELL_OFFSET = 1;

bool isGamePaused;
bool isGameOver;

Font font;

typedef struct
{
    int id;
    std::map<int, std::vector<Vector2>> cells;
    int rotationState;
    int columnOffset;
    int rowOffset;
} Block;

Block lBlock;
Block jBlock;
Block iBlock;
Block oBlock;
Block sBlock;
Block tBlock;
Block zBlock;
Block currentBlock;
Block nextBlock;

std::vector<Block> blocks;

double lastUpdateTime = 0;

bool eventTriggered(double interval)
{
    double currentTime = GetTime();

    if (currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;

        return true;
    }

    return false;
}

std::vector<Vector2> getCellPositions(Block &block)
{
    std::vector<Vector2> blockTiles = block.cells[block.rotationState];

    std::vector<Vector2> movedTiles;
    movedTiles.reserve(4);

    for (Vector2 blockTile : blockTiles)
    {
        Vector2 newPosition = {blockTile.x + block.rowOffset, blockTile.y + block.columnOffset};
        movedTiles.push_back(newPosition);
    }

    return movedTiles;
}

bool isCellOutside(int cellRow, int cellColumn)
{
    if (cellRow >= 0 && cellRow < TOTAL_ROWS && cellColumn >= 0 && cellColumn < TOTAL_COLUMNS)
    {
        return false;
    }

    return true;
}

bool isBlockOutside(Block &block)
{
    std::vector<Vector2> blockTiles = getCellPositions(block);

    for (Vector2 blockTile : blockTiles)
    {
        if (isCellOutside(blockTile.x, blockTile.y))
        {
            return true;
        }
    }

    return false;
}

void undoRotation(Block &block)
{
    block.rotationState--;

    if (block.rotationState == -1)
    {
        block.rotationState = block.cells.size() - 1;
    }
}

bool isCellEmpty(int rowToCheck, int columnToCheck)
{
    if (grid[rowToCheck][columnToCheck] == 0)
    {
        return true;
    }

    return false;
}

bool blockFits(Block &block)
{
    auto blockCells = getCellPositions(block);

    // I need to write in the grid the id of the block that I'm going to lock
    for (Vector2 blockCell : blockCells)
    {
        if (!isCellEmpty(blockCell.x, blockCell.y))
        {
            return false;
        }
    }

    return true;
}

void rotateBlock(Block &block)
{
    block.rotationState++;

    if (block.rotationState == (int)block.cells.size())
    {
        block.rotationState = 0;
    }

    if (isBlockOutside(block) || !blockFits(currentBlock))
    {
        undoRotation(block);
    }
}

void moveBlock(Block &block, int rowsToMove, int columnsToMove)
{
    block.rowOffset += rowsToMove;
    block.columnOffset += columnsToMove;
}

Block getRandomBlock()
{
    if (blocks.empty())
    {
        blocks = {lBlock, jBlock, iBlock, oBlock, sBlock, tBlock, zBlock};
    }

    int randomIndex = GetRandomValue(0, blocks.size() - 1);

    Block actualBlock = blocks[randomIndex];
    blocks.erase(blocks.begin() + randomIndex);

    return actualBlock;
}

bool isRowFull(int rowToCheck)
{
    for (int column = 0; column < TOTAL_COLUMNS; column++)
    {
        if (grid[rowToCheck][column] == 0)
        {
            return false;
        }
    }

    return true;
}

void clearRow(int rowToClear)
{
    for (int column = 0; column < TOTAL_COLUMNS; column++)
    {
        grid[rowToClear][column] = 0;
    }
}

void moveRowDown(int row, int totalRows)
{
    for (int column = 0; column < TOTAL_COLUMNS; column++)
    {
        grid[row + totalRows][column] = grid[row][column];
        grid[row][column] = 0;
    }
}

int clearFullRow()
{
    int completedRow = 0;
    for (int row = TOTAL_ROWS - 1; row >= 0; row--)
    {
        if (isRowFull(row))
        {
            clearRow(row);
            completedRow++;
        }
        else if (completedRow > 0)
        {
            moveRowDown(row, completedRow);
        }
    }

    return completedRow;
}

void lockBlock(Block &block)
{
    auto blockCells = getCellPositions(block);

    // I need to write in the grid the id of the block that I'm going to lock
    for (Vector2 blockCell : blockCells)
    {
        grid[(int)blockCell.x][(int)blockCell.y] = block.id;
    }

    // and then update the current and next blocks.
    block = nextBlock;

    if (!blockFits(block))
    {
        isGameOver = true;
    }

    nextBlock = getRandomBlock();

    clearFullRow();
}

void initializeGrid()
{
    for (int row = 0; row < TOTAL_ROWS; row++)
    {
        for (int column = 0; column < TOTAL_COLUMNS; column++)
        {
            grid[row][column] = 0;
        }
    }
}

void update(float deltaTime)
{
    // to check if any key has been pressed.
    int keyPressed = GetKeyPressed();

    if (isGameOver && keyPressed != 0)
    {
        initializeGrid();
        isGameOver = false;
        currentBlock = getRandomBlock();
        nextBlock = getRandomBlock();
    }

    if (!isGameOver && IsKeyPressed(KEY_W))
    {
        rotateBlock(currentBlock);
    }

    if (!isGameOver && IsKeyPressed(KEY_D))
    {
        moveBlock(currentBlock, 0, 1);

        if (isBlockOutside(currentBlock) || !blockFits(currentBlock))
        {
            moveBlock(currentBlock, 0, -1);
        }
    }

    else if (!isGameOver && IsKeyPressed(KEY_A))
    {
        moveBlock(currentBlock, 0, -1);

        if (isBlockOutside(currentBlock) || !blockFits(currentBlock))
        {
            moveBlock(currentBlock, 0, 1);
        }
    }

    if (!isGameOver && IsKeyDown(KEY_S))
    {
        moveBlock(currentBlock, 1, 0);

        if (isBlockOutside(currentBlock) || !blockFits(currentBlock))
        {
            moveBlock(currentBlock, -1, 0);
            lockBlock(currentBlock);
        }
    }

    if (!isGameOver && eventTriggered(0.4))
    {
        moveBlock(currentBlock, 1, 0);

        if (isBlockOutside(currentBlock) || !blockFits(currentBlock))
        {
            moveBlock(currentBlock, -1, 0);
            lockBlock(currentBlock);
        }
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
    // for all the block to start in the midle of the grid, I need to move to the (0, 3)
    moveBlock(lBlock, 0, 3);

    jBlock.id = 2;
    jBlock.cells[0] = {{0, 0}, {1, 0}, {1, 1}, {1, 2}};
    jBlock.cells[1] = {{0, 1}, {0, 2}, {1, 1}, {2, 1}};
    jBlock.cells[2] = {{1, 0}, {1, 1}, {1, 2}, {2, 2}};
    jBlock.cells[3] = {{0, 1}, {1, 1}, {2, 0}, {2, 1}};

    moveBlock(jBlock, 0, 3);

    iBlock.id = 3;
    iBlock.cells[0] = {{1, 0}, {1, 1}, {1, 2}, {1, 3}};
    iBlock.cells[1] = {{0, 2}, {1, 2}, {2, 2}, {3, 2}};
    iBlock.cells[2] = {{2, 0}, {2, 1}, {2, 2}, {2, 3}};
    iBlock.cells[3] = {{0, 1}, {1, 1}, {2, 1}, {3, 1}};

    moveBlock(iBlock, -1, 3);

    // I don't need rotaion with this block
    oBlock.id = 4;
    oBlock.cells[0] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};

    moveBlock(oBlock, 0, 4);

    sBlock.id = 5;
    sBlock.cells[0] = {{0, 1}, {0, 2}, {1, 0}, {1, 1}};
    sBlock.cells[1] = {{0, 1}, {1, 1}, {1, 2}, {2, 2}};
    sBlock.cells[2] = {{1, 1}, {1, 2}, {2, 0}, {2, 1}};
    sBlock.cells[3] = {{0, 0}, {1, 0}, {1, 1}, {2, 1}};

    moveBlock(sBlock, 0, 3);

    tBlock.id = 6;
    tBlock.cells[0] = {{0, 1}, {1, 0}, {1, 1}, {1, 2}};
    tBlock.cells[1] = {{0, 1}, {1, 1}, {1, 2}, {2, 1}};
    tBlock.cells[2] = {{1, 0}, {1, 1}, {1, 2}, {2, 1}};
    tBlock.cells[3] = {{0, 1}, {1, 0}, {1, 1}, {2, 1}};

    moveBlock(tBlock, 0, 3);

    zBlock.id = 7;
    zBlock.cells[0] = {{0, 0}, {0, 1}, {1, 1}, {1, 2}};
    zBlock.cells[1] = {{0, 2}, {1, 1}, {1, 2}, {2, 1}};
    zBlock.cells[2] = {{1, 0}, {1, 1}, {2, 1}, {2, 2}};
    zBlock.cells[3] = {{0, 1}, {1, 0}, {1, 1}, {2, 0}};

    moveBlock(zBlock, 0, 3);

    blocks.reserve(7);
    blocks = {lBlock, jBlock, iBlock, oBlock, sBlock, tBlock, zBlock};

    currentBlock = getRandomBlock();
    nextBlock = getRandomBlock();
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

Color getColorByIndex(int index)
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

            DrawRectangle(column * CELL_SIZE + POSITION_OFFSET, row * CELL_SIZE + POSITION_OFFSET, CELL_SIZE - CELL_OFFSET, CELL_SIZE - CELL_OFFSET, getColorByIndex(cellValue));
        }
    }
}

void drawBlock(Block &block)
{
    std::vector<Vector2> blockTiles = getCellPositions(block);

    for (Vector2 blockTile : blockTiles)
    {
        // The y value is for the column and the x value y for the row
        DrawRectangle(blockTile.y * CELL_SIZE + POSITION_OFFSET, blockTile.x * CELL_SIZE + POSITION_OFFSET, CELL_SIZE - CELL_OFFSET, CELL_SIZE - CELL_OFFSET, getColorByIndex(block.id));
    }
}

// It's always good to have a light and dark version of a color for ui purposes.
const Color lightBlue = {59, 85, 162, 255};
const Color darkBlue = {44, 44, 127, 255};

void draw()
{
    BeginDrawing();
    // we use the dark color version for the background
    ClearBackground(darkBlue);

    drawGrid();

    drawBlock(currentBlock);

    DrawTextEx(font, "Score", {365, 15}, 38, 2, WHITE);
    DrawTextEx(font, "Next", {370, 175}, 38, 2, WHITE);

    if (isGameOver)
    {
        DrawTextEx(font, "Game Over", {320, 450}, 38, 2, WHITE);
    }

    if (isGamePaused)
    {
        DrawTextEx(font, "Game Paused", {320, 450}, 30, 2, WHITE);
    }

    // we use the light color version for the ui elements.
    DrawRectangleRounded({320, 55, 170, 60}, 0.3, 6, lightBlue);
    DrawRectangleRounded({320, 215, 170, 180}, 0.3, 6, lightBlue);

    EndDrawing();
}

int main()
{
    // SCREEN_WIDTH 10 * 30 = 300
    // SCREEN_HEIGHT 20 * 30 = 600
    // need to give a extra offset of 200 width and 20 heigt for the ui
    InitWindow(TOTAL_COLUMNS * CELL_SIZE + 200, TOTAL_ROWS * CELL_SIZE + 20, "Tetris");
    SetTargetFPS(60);

    font = LoadFontEx("assets/fonts/monogram.ttf", 64, 0, 0);

    initializeGrid();

    initializeBlocks();

    printGrid();

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        if (!isGameOver && IsKeyPressed(KEY_SPACE))
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