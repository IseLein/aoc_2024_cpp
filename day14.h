
#pragma once

#include <cctype>
#include <fstream>
#include <iostream>
#include <vector>

#define WIDTH 101
#define HEIGHT 103

using namespace std;

typedef struct {
    int pos_x;
    int pos_y;
    int vel_x;
    int vel_y;
} Robot;

void parseInput(vector<Robot> &robots, ifstream &file);
void moveRobot(Robot &robot, int times);
void print(Robot robot);
void printSpace(vector<Robot> &robots);
int countRobots(vector<Robot> &robots, int x, int y, int dx, int dy);
