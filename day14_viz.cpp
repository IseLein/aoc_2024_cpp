
#include "raylib.h"
#include "day14.h"
#include <chrono>

// 16:9
#define SCREEN_WIDTH 1900
#define SCREEN_HEIGHT 1300
#define CELL_SIZE 11
#define GRID_WIDTH WIDTH
#define GRID_HEIGHT HEIGHT
#define GRID_FONT_SIZE 3
#define FONT_SIZE 40
#define THICKNESS 2

// random colors
const Color colors[] = { RAYWHITE, GREEN, BLUE, PINK, PURPLE, GOLD };

typedef struct {
    char value;
    Color color;
    Color topBorder;
    Color bottomBorder;
    Color leftBorder;
    Color rightBorder;
} Cell;

int update_grid(vector<vector<Cell>> &grid, vector<Robot> &robots);
void draw_grid(vector<vector<Cell>> &grid);
void draw_title(int num_robots);
void draw_time(int seconds);

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Restroom Redoubt");
    SetTargetFPS(60);

    vector<Robot> robots;
    vector<vector<Cell>> grid;
    for (int i = 0; i < HEIGHT; i++) {
        vector<Cell> v;
        for (int j = 0; j < WIDTH; j++) {
            Cell c = { 0, RAYWHITE, RAYWHITE, RAYWHITE, RAYWHITE, RAYWHITE };
            v.push_back(c);
        }
        grid.push_back(v);
    }

    ifstream file("input/day14");
    parseInput(robots, file);

    for (auto &robot: robots) {
        moveRobot(robot, 7310);
    }
    update_grid(grid, robots);

    float i_seconds = 0;
    int display_seconds = 7310;
    auto start = chrono::high_resolution_clock::now();
    float gap = 0.5;
    bool take_screenshot = false;
    while (!WindowShouldClose()) {
        auto curr = chrono::high_resolution_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(curr - start).count();

        i_seconds = (float)elapsed / 1000;
        if (i_seconds >= gap) {
        // if (false) {
            i_seconds = 0;
            display_seconds += 1;
            start = curr;

            for (auto &robot: robots) {
                moveRobot(robot, 1);
            }
            int score = update_grid(grid, robots);

            if (score == robots.size()) {
                take_screenshot = true;
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        draw_title(robots.size());
        draw_time(display_seconds);
        draw_grid(grid);
        EndDrawing();

        if (take_screenshot) {
            string filename = "day14_ss/" + to_string(display_seconds) + ".png";
            TakeScreenshot(filename.c_str());
            gap += 100;
            take_screenshot = false;
        }
    }
}

int update_grid(vector<vector<Cell>> &grid, vector<Robot> &robots) {
    for (auto &row: grid) {
        for (auto &cell: row) {
            cell.value = 0;
        }
    }
    for (auto &robot: robots) {
        grid[robot.pos_y][robot.pos_x].value += 1;
    }
    int num_boxes = 0;
    for (auto &row: grid) {
        for (auto &cell: row) {
            if (cell.value > 4) {
                cell.color = BROWN;
            } else {
                cell.color = colors[cell.value];
            }

            if (cell.value > 0) {
                num_boxes += 1;
            }
        }
    }
    return num_boxes;
}

void _drawCellBorders(Cell c, float x, float y, float thickness) {
    DrawLineEx({ x, y }, { x + CELL_SIZE, y }, thickness, c.topBorder);
    DrawLineEx({ x, y }, { x , y + CELL_SIZE }, thickness, c.leftBorder);
    DrawLineEx({ x + CELL_SIZE - thickness, y }, { x + CELL_SIZE - thickness, y + CELL_SIZE },  thickness, c.rightBorder);
    DrawLineEx({ x , y + CELL_SIZE - thickness }, { x + CELL_SIZE , y + CELL_SIZE - thickness }, thickness, c.bottomBorder);
}

void draw_grid(vector<vector<Cell>> &grid) {
    int grid_offset_x = (SCREEN_WIDTH - (GRID_WIDTH * CELL_SIZE)) / 2;
    int grid_offset_y = (SCREEN_HEIGHT - (GRID_HEIGHT * CELL_SIZE)) / 2;
    for (int x = 0; x < GRID_WIDTH; x++) {
        for (int y = 0; y < GRID_HEIGHT; y++) {
            int pos_x = grid_offset_x + (x * CELL_SIZE);
            int pos_y = grid_offset_y + (y * CELL_SIZE);

            Rectangle cellRect = {
                static_cast<float>(pos_x),
                static_cast<float>(pos_y),
                CELL_SIZE,
                CELL_SIZE
            };

            DrawRectangleRec(cellRect, grid[y][x].color);
            _drawCellBorders(grid[y][x], cellRect.x, cellRect.y, THICKNESS);
        }
    }
    // top border
    DrawLineEx(
        { (float)grid_offset_x, (float)grid_offset_y },
        { (float)grid_offset_x + (GRID_WIDTH * CELL_SIZE), (float)grid_offset_y },
        THICKNESS,
        PINK
    );
    // bottom border
    DrawLineEx(
        { (float)grid_offset_x, (float)grid_offset_y + (GRID_HEIGHT * CELL_SIZE) },
        { (float)grid_offset_x + (GRID_WIDTH * CELL_SIZE), (float)grid_offset_y + (GRID_HEIGHT * CELL_SIZE) },
        THICKNESS,
        PINK
    );
    // left border
    DrawLineEx(
        { (float)grid_offset_x, (float)grid_offset_y },
        { (float)grid_offset_x, (float)grid_offset_y + (GRID_HEIGHT * CELL_SIZE) },
        THICKNESS,
        PINK
    );
    // right border
    DrawLineEx(
        { (float)grid_offset_x + (GRID_WIDTH * CELL_SIZE), (float)grid_offset_y },
        { (float)grid_offset_x + (GRID_WIDTH * CELL_SIZE), (float)grid_offset_y + (GRID_HEIGHT * CELL_SIZE) },
        THICKNESS,
        PINK
    );
}

void draw_time(int seconds) {
    char text[20];
    sprintf(text, "Time %d", seconds);
    Vector2 textPos = {
        (float)SCREEN_WIDTH - (float)MeasureText(text, FONT_SIZE) - 10,
        SCREEN_HEIGHT - FONT_SIZE - 10
    };
    DrawText(text, textPos.x, textPos.y, FONT_SIZE, RED);
}

void draw_title(int num_robots) {
    string text = "Restroom";
    Vector2 textPos = { 10, 10 };
    DrawText(text.c_str(), textPos.x, textPos.y, FONT_SIZE, BLUE);

    text = "Redoubt";
    textPos = {
        10 + (float)MeasureText("R", FONT_SIZE),
        20 + FONT_SIZE
    };
    DrawText(text.c_str(), textPos.x, textPos.y, FONT_SIZE, BLUE);

    text = "Robots: " + to_string(num_robots);
    textPos = {
        (float)SCREEN_WIDTH - (float)MeasureText(text.c_str(), FONT_SIZE) - 10,
        SCREEN_HEIGHT - (2 * FONT_SIZE) - 30
    };
    DrawText(text.c_str(), textPos.x, textPos.y, FONT_SIZE, BLUE);
}
