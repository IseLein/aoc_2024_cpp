
#include "day24.h"
#include "raylib.h"
#include "raymath.h"

#define WIDTH 1200
#define HEIGHT 600
#define FONT_SIZE 40
#define BALL_SIZE 1
#define GAP_SIZE 4

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

void parseInput(ifstream &fin, map<string, int> &wire_values, map<string, Construction> &wire_constructions);
void UpdateCamera(Camera3D &camera, CameraConfig &config);
void DrawWire(string wire, map<string, Vector3> &wire_positions, map<string, Construction> &wire_constructions, set<string> &drawn_wires);

int main() {
    ifstream file("input/day24");
    map<string, int> wire_values;
    map<string, Construction> wire_constructions;
    parseInput(file, wire_values, wire_constructions);
    file.close();

    InitWindow(WIDTH, HEIGHT, "Crossed Wires");

    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 0.0f, 30.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    CameraConfig config = {
        .moveSpeed = 30.0f,
        .rotateSpeed = 2.0f,
        .zoomSpeed = 10.0f,
        .invertY = true
    };

    Vector3 center = { 0.0f, 0.0f, 0.0f };

    SetTargetFPS(60);

    map<string, Vector3> wire_positions;
    /*swap_wires(wire_constructions, "z10", "ggn");*/
    /*swap_wires(wire_constructions, "jcb", "ndw");*/
    /*swap_wires(wire_constructions, "z39", "twr");*/
    /*swap_wires(wire_constructions, "z32", "grm");*/

    while (!WindowShouldClose()) {
        UpdateCamera(camera, config);

        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);

        set<string> drawn_wires;
        for (int i = 0; i < 46; i++) {
            DrawWire('z' + left_pad(to_string(i), 2), wire_positions, wire_constructions, drawn_wires);
        }

        // DrawGrid(100, GAP_SIZE + BALL_SIZE);

        EndMode3D();

        DrawText("Crossed Wires", 10, 10, FONT_SIZE, DARKGRAY);
        DrawFPS(10, 20 + FONT_SIZE);

        EndDrawing();
    }

    CloseWindow();
}

void DrawGate(string wire, map<string, Vector3> &wire_positions, map<string, Construction> &wire_constructions) {
    Color color;
    switch(wire_constructions[wire].gate) {
        case AND:
            color = BLACK;
            break;
        case OR:
            color = RED;
            break;
        case XOR:
            color = GREEN;
            break;
        default:
            color = DARKGRAY;
    }
    Vector3 pos1 = wire_positions[wire_constructions[wire].input1];
    Vector3 pos2 = wire_positions[wire_constructions[wire].input2];
    Vector3 pos = wire_positions[wire];
    DrawLine3D(pos, pos1, color);
    DrawLine3D(pos, pos2, color);
}

void DrawWire(string wire, map<string, Vector3> &wire_positions, map<string, Construction> &wire_constructions, set<string> &drawn_wires) {
    if (drawn_wires.count(wire) > 0) {
        return;
    }
    Color color;
    switch(wire[0]) {
        case 'x':
            color = BLUE;
            break;
        case 'y':
            color = GREEN;
            break;
        case 'z':
            color = BROWN;
            break;
        default:
            color = DARKGRAY;
    }

    if (wire_positions.count(wire) > 0) {
        DrawSphere(wire_positions[wire], BALL_SIZE, color);
        if (wire_constructions.count(wire) > 0) {
            DrawWire(wire_constructions[wire].input1, wire_positions, wire_constructions, drawn_wires);
            drawn_wires.insert(wire_constructions[wire].input1);
            DrawWire(wire_constructions[wire].input2, wire_positions, wire_constructions, drawn_wires);
            drawn_wires.insert(wire_constructions[wire].input2);
            DrawGate(wire, wire_positions, wire_constructions);
        }
        drawn_wires.insert(wire);
        return;
    }
    if (wire[0] == 'x') {
        int num = stoi(wire.substr(1));
        Vector3 pos = {
            (float)num * (GAP_SIZE + BALL_SIZE),
            0.0f,
            0.0f
        };
        wire_positions[wire] = pos;
        DrawSphere(pos, BALL_SIZE, color);
        drawn_wires.insert(wire);
        return;
    }
    if (wire[0] == 'y') {
        int num = stoi(wire.substr(1));
        Vector3 pos = {
            (float)num * (GAP_SIZE + BALL_SIZE),
            (float)(GAP_SIZE + BALL_SIZE),
            0.0f
        };
        wire_positions[wire] = pos;
        DrawSphere(pos, BALL_SIZE, color);
        drawn_wires.insert(wire);
        return;
    }
    if (wire[0] == 'z') {
        int num = stoi(wire.substr(1));
        Vector3 pos = {
            (float)num * (GAP_SIZE + BALL_SIZE),
            (float)GAP_SIZE / 2 + BALL_SIZE,
            35.0f
        };
        wire_positions[wire] = pos;
        DrawSphere(pos, BALL_SIZE, color);
    }

    DrawWire(wire_constructions[wire].input1, wire_positions, wire_constructions, drawn_wires);
    drawn_wires.insert(wire_constructions[wire].input1);
    DrawWire(wire_constructions[wire].input2, wire_positions, wire_constructions, drawn_wires);
    drawn_wires.insert(wire_constructions[wire].input2);

    float z_factor;
    switch(wire_constructions[wire].gate) {
        case AND:
            z_factor = 2.0f;
            break;
        case OR:
            z_factor = 0.5f;
            break;
        case XOR:
            z_factor = 1.0f;
            break;
        default:
            z_factor = 0.0f;
    }

    Vector3 pos1 = wire_positions[wire_constructions[wire].input1];
    Vector3 pos2 = wire_positions[wire_constructions[wire].input2];
    Vector3 pos = {
        (pos1.x + pos2.x) / 2.0f,
        (pos1.y + pos2.y) / 2.0f + GAP_SIZE + BALL_SIZE,
        (pos1.z + pos2.z) / 2.0f + (GAP_SIZE + BALL_SIZE) * z_factor
    };
    if (wire[0] != 'z') {
        wire_positions[wire] = pos;
        DrawSphere(pos, BALL_SIZE, color);
    }

    DrawGate(wire, wire_positions, wire_constructions);
    drawn_wires.insert(wire);
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
    float moveSpeed = - config.moveSpeed * GetFrameTime();

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

void parseInput(ifstream &fin, map<string, int> &wire_values, map<string, Construction> &wire_constructions) {
    string line;
    while(getline(fin, line)) {
        if(line.empty()) {
            break;
        }
        string wire;
        int value;
        stringstream ss(line);
        ss >> wire >> value;
        wire_values[wire.substr(0, wire.size()-1)] = value;
    }

    while(getline(fin, line)) {
        string wire1;
        string wire2;
        string gate;
        string wire_out;
        string dummy;
        stringstream ss(line);
        ss >> wire1 >> gate >> wire2 >> dummy >> wire_out;

        Construction construction;
        construction.input1 = wire1;
        construction.input2 = wire2;
        switch(gate[0]) {
            case 'A':
                construction.gate = AND;
                break;
            case 'O':
                construction.gate = OR;
                break;
            case 'X':
                construction.gate = XOR;
                break;
            default:
                cout << "Invalid gate: " << gate << "\n";
        }
        wire_constructions[wire_out] = construction;
    }
}
