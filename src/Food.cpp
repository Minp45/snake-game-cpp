#include "Food.h"
#include <raylib.h>  // For Raylib functions
#include <deque>
#include <raymath.h>
#include "Utilities.h" 

// External variables
extern int cellSize;
extern int cellCount;
extern int offset;
extern bool ElementInDeque(Vector2 element, std::deque<Vector2> deque);

Food::Food(std::deque<Vector2> snakeBody) {
    Image image = LoadImage("graphics/food.png");
    texture = LoadTextureFromImage(image);
    UnloadImage(image);
    position = GenerateRandomPos(snakeBody);
}

Food::~Food() {
    UnloadTexture(texture);
}

void Food::Draw() {
    Color white = {255, 255, 255, 255};
    DrawTexture(texture, offset + position.x * cellSize, offset + position.y * cellSize, white);
}

Vector2 Food::GenerateRandomCell() {
    float x = GetRandomValue(0, cellCount - 1);
    float y = GetRandomValue(0, cellCount - 1);
    return Vector2{x, y};
}

Vector2 Food::GenerateRandomPos(std::deque<Vector2> snakeBody) {
    Vector2 pos = GenerateRandomCell();
    while (ElementInDeque(pos, snakeBody)) {
        pos = GenerateRandomCell();
    }
    return pos;
}
