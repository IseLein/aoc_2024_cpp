
#include "raylib.h"
#include "day12.h"
#include <tuple>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 900
#define CELL_SIZE 40
#define GRID_WIDTH 10
#define GRID_HEIGHT 10
#define GRID_FONT_SIZE 20
#define FONT_SIZE 20

// contrast with black
const Color FG_COLORS[] = { SKYBLUE, ORANGE, YELLOW, MAGENTA, PURPLE, GOLD };

using namespace std;

typedef struct {
    char value;
    Color color;
    Color topBorder;
    Color bottomBorder;
    Color leftBorder;
    Color rightBorder;
} Cell;

enum ACTION_DETAIL {
    CELL,
    TOP_BORDER,
    BOTTOM_BORDER,
    LEFT_BORDER,
    RIGHT_BORDER,
    PASS
};

typedef struct {
    float duration;
    int x;
    int y;
    Color color;
    ACTION_DETAIL location;
} Action;

const map<SIDE, ACTION_DETAIL> SIDE_TO_DETAIL = {
    { UP, TOP_BORDER },
    { DOWN, BOTTOM_BORDER },
    { LEFT, LEFT_BORDER },
    { RIGHT, RIGHT_BORDER }
};

void load_grid(vector<vector<Cell>> &grid, vector<vector<char>> &data);
void draw_grid(vector<vector<Cell>> &grid);
void draw_title();
void draw_total(tuple<char, int, int, int, int, int> details);
void handle_action(Action a, vector<vector<Cell>> &grid);

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Garden Groups");
    SetTargetFPS(60);

    vector<vector<Cell>> grid;
    vector<vector<char>> data;
    load_grid(grid, data);

    vector<vector<Cell>> backup(grid);

    vector<Action> actions;
    int total1 = 0;
    int total2 = 0;
    vector<tuple<char, int, int, int, int, int>> totals;
    vector<set<pair<int, int>>> regions = get_regions(data);
    for (int i = 0; i < regions.size(); i++) {
        Color color = FG_COLORS[i % 6];
        set<pair<int, int>> region = regions[i];
        for (auto p: region) {
            actions.push_back({ 0.2, p.second, p.first, color, CELL });
            char plant = data[p.first][p.second];
            totals.push_back({ plant, total1, total2, -1, 0, 0 });
        }
        tuple<int, int, int, vector<vector<Fence>>> region_details = fence_price_viz(region, data);
        int area = get<0>(region_details);
        int num_sides = get<1>(region_details);
        int num_fences = get<2>(region_details);
        vector<vector<Fence>> sides = get<3>(region_details); // no need to be multidimensional but it's ok
        total1 += area * num_fences;
        total2 += area * num_sides;
        for (auto side: sides) {
            for (auto fence: side) {
                actions.push_back({ 0.1, fence.pos.second, fence.pos.first, RED, SIDE_TO_DETAIL.at(fence.side) });
                char plant = data[fence.pos.first][fence.pos.second];
                totals.push_back({ plant, total1, total2, area, num_fences, num_sides });
            }
        }
        for (auto side: sides) {
            for (auto fence: side) {
                actions.push_back({ 0.001, fence.pos.second, fence.pos.first, LIGHTGRAY, SIDE_TO_DETAIL.at(fence.side) });
                char plant = data[fence.pos.first][fence.pos.second];
                totals.push_back({ plant, total1, total2, area, num_fences, num_sides });
            }
        }
    }
    // wait 15s before starting over
    actions.push_back({ 15, 0, 0, RED, PASS });
    totals.push_back({ '=', total1, total2, -2, 0, 0 });

    int i_action = 0;
    float i_seconds;
    auto start = chrono::high_resolution_clock::now();
    while (!WindowShouldClose()) {
        auto curr = chrono::high_resolution_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(curr - start).count();

        i_seconds = (float)elapsed / 1000;
        if (i_seconds >= actions[i_action].duration) {
            i_action += 1;
            start = curr;
        }

        if (i_action >= actions.size()) {
            i_action = 0;
            grid = backup;
            continue;
        }

        BeginDrawing();
        draw_title();
        draw_total(totals[i_action]);
        draw_grid(grid);
        handle_action(actions[i_action], grid);
        EndDrawing();
    }
}

void handle_action(Action a, vector<vector<Cell>> &grid) {
    if (a.location == CELL) {
        grid[a.y][a.x].color = a.color;
    } else if (a.location == TOP_BORDER) {
        grid[a.y][a.x].topBorder = a.color;
    } else if (a.location == BOTTOM_BORDER) {
        grid[a.y][a.x].bottomBorder = a.color;
    } else if (a.location == LEFT_BORDER) {
        grid[a.y][a.x].leftBorder = a.color;
    } else if (a.location == RIGHT_BORDER) {
        grid[a.y][a.x].rightBorder = a.color;
    }
}

void drawCellBorders(Cell c, float x, float y, float thickness) {
    DrawLineEx({ x, y }, { x + CELL_SIZE, y }, thickness, c.topBorder);
    DrawLineEx({ x, y }, { x , y + CELL_SIZE }, thickness, c.leftBorder);
    DrawLineEx({ x + CELL_SIZE - thickness, y }, { x + CELL_SIZE - thickness, y + CELL_SIZE }, 2 * thickness, c.rightBorder);
    DrawLineEx({ x , y + CELL_SIZE - thickness }, { x + CELL_SIZE , y + CELL_SIZE - thickness }, 2 * thickness, c.bottomBorder);
}

void draw_grid(vector<vector<Cell>> &grid) {
    int grid_offset_x = (SCREEN_WIDTH - (GRID_WIDTH * CELL_SIZE)) / 2;
    int grid_offset_y = (SCREEN_HEIGHT - (GRID_HEIGHT * CELL_SIZE)) / 2;
    for (int x = 0; x < GRID_HEIGHT; x++) {
        for (int y = 0; y < GRID_WIDTH; y++) {
            int pos_x = grid_offset_x + (x * CELL_SIZE);
            int pos_y = grid_offset_y + (y * CELL_SIZE);

            Rectangle cellRect = {
                static_cast<float>(pos_x),
                static_cast<float>(pos_y),
                CELL_SIZE,
                CELL_SIZE
            };

            DrawRectangleRec(cellRect, grid[y][x].color);
            drawCellBorders(grid[y][x], cellRect.x, cellRect.y, 1.5);

            char text[2] = { grid[y][x].value, '\0' };
            Vector2 textPos = {
                cellRect.x + (float)CELL_SIZE / 2 - (float)MeasureText(text, GRID_FONT_SIZE) / 2,
                cellRect.y + (float)CELL_SIZE / 2 - (float)GRID_FONT_SIZE / 2
            };
            DrawText(text, textPos.x, textPos.y, GRID_FONT_SIZE, BLACK);
        }
    }
}

void draw_title() {
    char text[] = { 'G', 'A', 'R', 'D', 'E', 'N', ' ', 'G', 'R', 'O', 'U', 'P', 'S', '\0' };
    Vector2 textPos = {
        (float)SCREEN_WIDTH - (float)MeasureText(text, FONT_SIZE) - 10,
        10
    };
    DrawText(text, textPos.x, textPos.y, FONT_SIZE, RED);
}

char *string_to_char(string s) {
    char *c = new char[s.size() + 1];
    strcpy(c, s.c_str());
    return c;
}

string left_pad(string s, int size) {
    if (s.size() >= size) {
        return s;
    }
    string s_padded = s;
    for (int i = 0; i < (size - s.size()); i++) {
        s_padded = "0" + s_padded;
    }
    return s_padded;
}

void draw_total(tuple<char, int, int, int, int, int> details) {
    // clear the area
    int width = MeasureText("TOTAL1: XXXXXXXXXXXXX", FONT_SIZE);
    DrawRectangle(0, 0, width, 60 + 5 * FONT_SIZE, BLACK);

    char plant = get<0>(details);
    int total1 = get<1>(details);
    int total2 = get<2>(details);
    int area = get<3>(details);
    int num_fences = get<4>(details);
    int num_sides = get<5>(details);

    string total_str = "TOTAL1: " + left_pad(to_string(total1), 7);
    Vector2 textPos = { 10, 10 };
    DrawText(string_to_char(total_str), textPos.x, textPos.y, FONT_SIZE, RED);

    total_str = "TOTAL2: " + left_pad(to_string(total2), 7);
    textPos = { 10, 20 + FONT_SIZE };
    DrawText(string_to_char(total_str), textPos.x, textPos.y, FONT_SIZE, RED);

    string plant_str = "PLANT: ";
    textPos = { 10, 30 + 2 * FONT_SIZE };
    char *text_p = string_to_char(plant_str);
    DrawText(text_p, textPos.x, textPos.y, FONT_SIZE, RED);

    char text_c[2] = { get<0>(details), '\0' };
    float pos_x = 10 + MeasureText(text_p, FONT_SIZE);
    textPos = { pos_x, 30 + 2 * FONT_SIZE };
    DrawText(text_c, textPos.x, textPos.y, FONT_SIZE, BLUE);

    if (area < 0) {
        string suffix = area == -1 ? "???" : "===";
        string text_1 = "price1: " + suffix;
        textPos = { 10, 40 + 3 * FONT_SIZE };
        DrawText(string_to_char(text_1), textPos.x, textPos.y, FONT_SIZE, RED);

        text_1 = "price2: " + suffix;
        textPos = { 10, 50 + 4 * FONT_SIZE };
        DrawText(string_to_char(text_1), textPos.x, textPos.y, FONT_SIZE, RED);

        return;
    }

    string text_1 = "price1: " + to_string(area) + " * " + to_string(num_fences) + " = " + to_string(area * num_fences);
    textPos = { 10, 40 + 3 * FONT_SIZE };
    DrawText(string_to_char(text_1), textPos.x, textPos.y, FONT_SIZE, RED);

    string text_2 = "price2: " + to_string(area) + " * " + to_string(num_sides) + " = " + to_string(area * num_sides);
    textPos = { 10, 50 + 4 * FONT_SIZE };
    DrawText(string_to_char(text_2), textPos.x, textPos.y, FONT_SIZE, RED);
}

void load_grid(vector<vector<Cell>> &grid, vector<vector<char>> &data) {
    ifstream file("input/day12_1t");
    string line;

    while(getline(file, line)) {
        vector<Cell> v;
        vector<char> vc;
        for (int x = 0; x < line.size(); x++) {
            Cell c = { line[x], RAYWHITE, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY, LIGHTGRAY };
            v.push_back(c);
            vc.push_back(line[x]);
        }
        grid.push_back(v);
        data.push_back(vc);
    }
    file.close();
}


