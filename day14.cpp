
#include "day14.h"

int l_main() {
    ifstream file("input/day14");

    vector<Robot> robots;
    parseInput(robots, file);

    for (auto &robot: robots) {
        moveRobot(robot, 100);
    }

    int half_w = WIDTH / 2;
    int half_h = HEIGHT / 2;
    int q1 = countRobots(robots, 0, 0, half_w, half_h);
    int q2 = countRobots(robots, half_w + 1, 0, half_w, half_h);
    int q3 = countRobots(robots, 0, half_h + 1, half_w, half_h);
    int q4 = countRobots(robots, half_w + 1, half_h + 1, half_w, half_h);
    cout << "Safety score: " << q1 * q2 * q3 * q4 << "\n";

    return 0;
}

void print(Robot robot) {
    cout << "p: " << robot.pos_x << "," << robot.pos_y << "; ";
    cout << "v: " << robot.vel_x << "," << robot.vel_y << ";\n";
}

void moveRobot(Robot &robot, int times) {
    robot.pos_x += (robot.vel_x * times);
    robot.pos_y += (robot.vel_y * times);

    if (robot.pos_x < 0) {
        robot.pos_x = ((robot.pos_x % WIDTH) + WIDTH) % WIDTH;
    } else if (robot.pos_x >= WIDTH) {
        robot.pos_x = robot.pos_x % WIDTH;
    }
    if (robot.pos_y < 0) {
        robot.pos_y = ((robot.pos_y % HEIGHT) + HEIGHT) % HEIGHT;
    } else if (robot.pos_y >= HEIGHT) {
        robot.pos_y = robot.pos_y % HEIGHT;
    }
}

int countRobots(vector<Robot> &robots, int x, int y, int dx, int dy) {
    int total = 0;
    for (auto &robot: robots) {
        if ((robot.pos_x >= x) && (robot.pos_x < (x + dx))
            && (robot.pos_y >= y) && (robot.pos_y < (y + dy))) {
            total += 1;
        }
    }
    return total;
}

void printSpace(vector<Robot> &robots) {
    vector<vector<int>> grid;
    for (int i = 0; i < HEIGHT; i++) {
        vector<int> v;
        for (int j = 0; j < WIDTH; j++) {
            v.push_back(0);
        }
        grid.push_back(v);
    }

    for (auto robot: robots) {
        if ((robot.pos_x < 0) || (robot.pos_y < 0) || (robot.pos_x >= WIDTH) || (robot.pos_y >= HEIGHT)){
            print(robot);
        }
        grid[robot.pos_y][robot.pos_x] += 1;
    }

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (grid[i][j] == 0) {
                cout << ".";
            } else {
                cout << grid[i][j];
            }
        }
        cout << "\n";
    }
}

int _getNum(string line, int &index) {
    string num = "";
    do {
        num = num += line[index];
        index++;
    } while(isdigit(line[index]));

    return stoi(num);
}

void parseInput(vector<Robot> &robots, ifstream &file) {
    string line;
    while(getline(file, line)) {
        Robot robot;

        int i = 2;
        robot.pos_x = _getNum(line, i);
        i += 1;
        robot.pos_y = _getNum(line, i);
        i += 3;
        robot.vel_x = _getNum(line, i);
        i += 1;
        robot.vel_y = _getNum(line, i);

        robots.push_back(robot);
    }
}

