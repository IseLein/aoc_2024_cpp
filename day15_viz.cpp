
#include "day15.h"
#include "raylib.h"
#include "raymath.h"
#include <chrono>

#define WIDTH 1800
#define HEIGHT 1200
#define FONT_SIZE 40
#define CELL_SIZE 2

using namespace std;

typedef struct {
    float moveSpeed;
    float rotateSpeed;
    float zoomSpeed;
    bool invertY;

    Vector2 previousMousePos;
    bool isMouseLookActive;

    float distance;    // Distance from target
    float angleY;      // Horizontal orbit angle
    float angleX;      // Vertical orbit angle
} CameraConfig;

void UpdateCamera(Camera3D &camera, CameraConfig &config);
void DrawWarehouse(vector<vector<char>> warehouse);
void DrawMoves(vector<char> moves, int curr_move);

int main() {
    ifstream file("input/day15");
    vector<vector<char>> warehouse;
    vector<char> robotMoves;
    pair<int, int> robotPosition;
    parseInput(file, warehouse, robotMoves, robotPosition);
    file.close();

    InitWindow(WIDTH, HEIGHT, "Warehouse Woes");

    Camera3D camera = { 0 };
    camera.position = (Vector3){ 10.0f, 30.0f, 10.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    CameraConfig config = {
        .moveSpeed = 10.0f,
        .rotateSpeed = 5.0f,
        .zoomSpeed = 10.0f,
        .invertY = true
    };

    Vector3 center = { 0.0f, 0.0f, 0.0f };

    SetTargetFPS(60);

    int curr_move = 0;
    float i_seconds = 0;
    auto start = chrono::high_resolution_clock::now();

    while (!WindowShouldClose()) {
        auto curr = chrono::high_resolution_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(curr - start).count();
        i_seconds = (float)elapsed / 1000;
        if (i_seconds >= 0.05) {
            i_seconds = 0;
            start = curr;
            curr_move += 1;
            if (curr_move < robotMoves.size()) {
                pair<int, int> dir = DIR_MAP.at(robotMoves[curr_move]);
                if(moveRoBox(warehouse, robotPosition, dir)) {
                    robotPosition = make_pair(robotPosition.first + dir.first, robotPosition.second + dir.second);
                }
            }
        }

        UpdateCamera(camera, config);

        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        DrawWarehouse(warehouse);

        // DrawGrid(20, 1.0f);

        EndMode3D();

        DrawText("Warehouse Woes", 10, 10, FONT_SIZE, DARKGRAY);
        DrawMoves(robotMoves, curr_move);
        DrawFPS(10, 20 + FONT_SIZE);

        EndDrawing();
    }

    CloseWindow();
}

void DrawWarehouse(vector<vector<char>> warehouse) {
    float width = warehouse.size() * CELL_SIZE;
    float length = warehouse[0].size() * CELL_SIZE;

    Vector3 pos = {
        0.0f - (float)CELL_SIZE / 2,
        - CELL_SIZE,
        0.0f + (float)CELL_SIZE / 2
    };
    DrawCube(pos, width + CELL_SIZE, CELL_SIZE, length + CELL_SIZE, DARKGRAY);

    float xOffset = - width / 2;
    float yOffset = length / 2;
    for (int x = 0; x < (int)warehouse.size(); x++) {
        for (int y = 0; y < (int)warehouse[0].size(); y++) {
            Vector3 bPos = { xOffset + (x * CELL_SIZE), 0.0f, yOffset - (y * CELL_SIZE) };
            char c = warehouse[x][y];
            if (c == '#') {
                DrawCube(bPos, CELL_SIZE, CELL_SIZE, CELL_SIZE, BROWN);
            } else if (c == 'O') {
                DrawCube(bPos, CELL_SIZE, CELL_SIZE, CELL_SIZE, ORANGE);
                DrawCubeWires(bPos, CELL_SIZE, CELL_SIZE, CELL_SIZE, DARKGRAY);
            } else if (c == '@') {
                DrawSphere(bPos, (float)CELL_SIZE / 2, PINK);
            }
        }
    }
}

void DrawMoves(vector<char> moves, int curr_move) {
    float yPos = HEIGHT - FONT_SIZE - 10;
    float lineLength = MeasureText("--------------------", FONT_SIZE);
    float rightBorder = WIDTH - 10;
    float leftBorder = WIDTH - (float)MeasureText("--------------------", FONT_SIZE) - 10;

    float padding = 5;

    if (curr_move >= moves.size()) {
        DrawLineEx({ leftBorder, yPos }, { rightBorder, yPos }, 2, DARKGRAY);
        return;
    }

    float curr = leftBorder + ((float)curr_move / moves.size()) * lineLength;
    // draw the current move
    char text[2] = { moves[curr_move], '\0' };
    DrawText(text, curr + padding, yPos - (float)FONT_SIZE / 2, FONT_SIZE, DARKGRAY);
    DrawRectangleLines(curr, yPos - (float)FONT_SIZE / 2, MeasureText(text, FONT_SIZE) + padding * 2, FONT_SIZE, DARKGRAY);

    float lCurr = curr;
    float rCurr = curr + MeasureText(text, FONT_SIZE) + padding * 2;

    // draw the previous moves
    for (int i = 0; i < 3; i++) {
        float fontSize = FONT_SIZE * pow(0.7, i);
        float lWidth = MeasureText(text, fontSize) + padding * 2;
        if ((curr_move - i >= 0) && (lCurr - lWidth > leftBorder)) {
            text[0] = moves[curr_move - i];
            lCurr -= lWidth;
            DrawText(text, lCurr + padding, yPos - (float)fontSize / 2, fontSize, DARKGRAY);
        }
    }
    // draw the next moves
    for (int i = 0; i < 3; i++) {
        float fontSize = FONT_SIZE * pow(0.7, i);
        float rWidth = MeasureText(text, fontSize) + padding * 2;
        if ((curr_move + i < moves.size()) && (rCurr + rWidth < rightBorder)) {
            text[0] = moves[curr_move + i];
            DrawText(text, rCurr + padding, yPos - (float)fontSize / 2, fontSize, DARKGRAY);
            rCurr += rWidth;
        }
    }

    DrawLineEx({ leftBorder, yPos }, { lCurr, yPos }, 2, DARKGRAY);
    DrawLineEx({ rCurr, yPos }, { rightBorder, yPos }, 2, DARKGRAY);
}

void UpdateCamera(Camera3D &camera, CameraConfig &config) {
    // Mouse look
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        config.isMouseLookActive = true;
        config.previousMousePos = GetMousePosition();
        DisableCursor();

        Vector3 direction = Vector3Subtract(camera.position, camera.target);
        config.distance = Vector3Length(direction);
        config.angleY = atan2f(direction.x, direction.z);
        config.angleX = atan2f(direction.y, sqrtf(direction.x * direction.x + direction.z * direction.z));
    }
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        config.isMouseLookActive = false;
        EnableCursor();
    }
    if (config.isMouseLookActive) {
        Vector2 currentMousePos = GetMousePosition();
        Vector2 mouseDelta = Vector2Subtract(currentMousePos, config.previousMousePos);
        config.previousMousePos = currentMousePos;

        if (mouseDelta.x != 0 || mouseDelta.y != 0) {
            float deltaY = mouseDelta.x * config.rotateSpeed * GetFrameTime() * (config.invertY ? -1.0f : 1.0f) * DEG2RAD;
            float deltaX = mouseDelta.y * config.rotateSpeed * GetFrameTime() * (config.invertY ? 1.0f : -1.0f) * DEG2RAD;

            // Update angles
            config.angleY += deltaY;
            config.angleX += deltaX;

            // Clamp vertical angle to avoid flipping
            config.angleX = fminf(fmaxf(config.angleX, -PI * 0.4f), PI * 0.4f);

            // Calculate new camera position based on spherical coordinates
            float cosX = cosf(config.angleX);
            camera.position.x = camera.target.x + config.distance * cosX * sinf(config.angleY);
            camera.position.y = camera.target.y + config.distance * sinf(config.angleX);
            camera.position.z = camera.target.z + config.distance * cosX * cosf(config.angleY);
        }
    }

    // zoom
    float wheel = GetMouseWheelMove();
    if (wheel != 0) {
        Vector3 zoomDirection = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
        camera.position = Vector3Add(camera.position, Vector3Scale(zoomDirection, wheel * config.zoomSpeed));
    }

    // move
    Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
    Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, camera.up));
    float moveSpeed = config.moveSpeed * GetFrameTime();

    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) {
        camera.position = Vector3Add(camera.position, Vector3Scale(forward, moveSpeed));
        camera.target = Vector3Add(camera.target, Vector3Scale(forward, moveSpeed));
    }
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
        camera.position = Vector3Subtract(camera.position, Vector3Scale(forward, moveSpeed));
        camera.target = Vector3Subtract(camera.target, Vector3Scale(forward, moveSpeed));
    }
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        camera.position = Vector3Add(camera.position, Vector3Scale(right, moveSpeed));
        camera.target = Vector3Add(camera.target, Vector3Scale(right, moveSpeed));
    }
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        camera.position = Vector3Subtract(camera.position, Vector3Scale(right, moveSpeed));
        camera.target = Vector3Subtract(camera.target, Vector3Scale(right, moveSpeed));
    }
    if (IsKeyDown(KEY_Q)) {
        camera.position.y += moveSpeed;
        camera.target.y += moveSpeed;
    }
    if (IsKeyDown(KEY_E)) {
        camera.position.y -= moveSpeed;
        camera.target.y -= moveSpeed;
    }
}
