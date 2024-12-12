
#pragma once
#include <bits/stdc++.h>

enum SIDE {
    UP, DOWN, RIGHT, LEFT
};

typedef struct {
    SIDE side;
    std::pair<int, int> pos;
} Fence;

int l_main();
void part1();
void get_region(std::pair<int, int> pos, std::set<std::pair<int, int>> &region, std::vector<std::vector<char>> &grid);
std::vector<std::set<std::pair<int, int>>> get_regions(std::vector<std::vector<char>> &grid);
std::pair<int, int> fence_price(std::set<std::pair<int, int>> &region, std::vector<std::vector<char>> &grid);
std::pair<int, int> fence_price2(std::set<std::pair<int, int>> &region, std::vector<std::vector<char>> &grid);
std::tuple<int, int, int, std::vector<std::vector<Fence>>> fence_price_viz(std::set<std::pair<int, int>> &region, std::vector<std::vector<char>> &grid);

