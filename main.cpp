#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>

Color turquoise = { 64, 224, 208, 255 };
Color snakeColor = { 34, 139, 34, 255 };    
Color foodColor = { 220, 20, 60, 255 };     

const int cellSize = 20;
const int cellCount = 25;
bool gameOver = false;

struct Snake {
    std::deque<Vector2> body;
    Vector2 direction;
    bool addSegment = false;
    
    void Draw() {
        for (const auto& segment : body) {
            DrawRectangle(segment.x * cellSize, 
                         segment.y * cellSize, 
                         cellSize - 1, 
                         cellSize - 1, 
                         snakeColor);
        }
    }
    
    void Update() {
        body.push_front(Vector2Add(body[0], direction));
        if (!addSegment) {
            body.pop_back();
        } else {
            addSegment = false;
        }
    }
    
    void Reset() {
        body = { Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9} };
        direction = { 1, 0 };
        addSegment = false;
    }
};

class Food {
    public:
        Vector2 position;
        void Generate(const std::deque<Vector2>& snakeBody) {
            position = { 
                (float)(GetRandomValue(0, cellCount - 1)),
                (float)(GetRandomValue(0, cellCount - 1))
            };
            
            for (const auto& segment : snakeBody) {
                if (Vector2Equals(position, segment)) {
                    Generate(snakeBody);
                    break;
                }
            }
        }
        
        void Draw() {
            DrawRectangle(position.x * cellSize, 
                         position.y * cellSize, 
                         cellSize - 1, 
                         cellSize - 1, 
                         foodColor);
        }
};

int main() {
    std::cout << "Starting the game\n";
    InitWindow(cellSize * cellCount, cellSize * cellCount, "Vim Snake");
    SetTargetFPS(60);
    
    Snake snake;
    snake.Reset();
    
    Food food;
    food.Generate(snake.body);
    
    double lastUpdateTime = 0;
    
    while (!WindowShouldClose()) {
        double currentTime = GetTime();
        
        if (currentTime - lastUpdateTime > 0.2) {  // Makes the game twice as fast
            lastUpdateTime = currentTime;
            
            if (!gameOver) {
                // Process inputs immediately for responsive controls
                if (IsKeyDown(KEY_H) && snake.direction.x != 1) snake.direction = { -1, 0 };
                if (IsKeyDown(KEY_L) && snake.direction.x != -1) snake.direction = { 1, 0 };
                if (IsKeyDown(KEY_K) && snake.direction.y != 1) snake.direction = { 0, -1 };
                if (IsKeyDown(KEY_J) && snake.direction.y != -1) snake.direction = { 0, 1 };
                
                snake.Update();
                
                if (snake.body[0].x < 0 || snake.body[0].x >= cellCount ||
                    snake.body[0].y < 0 || snake.body[0].y >= cellCount) {
                    gameOver = true;
                }
                
                for (size_t i = 1; i < snake.body.size(); i++) {
                    if (Vector2Equals(snake.body[0], snake.body[i])) {
                        gameOver = true;
                    }
                }
                
                if (Vector2Equals(snake.body[0], food.position)) {
                    food.Generate(snake.body);
                    snake.addSegment = true;
                }
            }
        }
        
        BeginDrawing();
            ClearBackground(turquoise);
            
            snake.Draw();
            food.Draw();
            
            if (gameOver) {
                DrawText("Game Over!", 
                        (cellSize * cellCount) / 2 - 40,
                        (cellSize * cellCount) / 2,
                        20,
                        BLACK);
                DrawText("Press SPACE to restart",
                        (cellSize * cellCount) / 2 - 80,
                        (cellSize * cellCount) / 2 + 30,
                        20,
                        BLACK);
                
                if (IsKeyPressed(KEY_SPACE)) {
                    snake.Reset();
                    food.Generate(snake.body);
                    gameOver = false;
                }
            }
            
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}
