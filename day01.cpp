
#include <bits/stdc++.h>

using namespace std;

void part1();
void part2();

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    freopen("input/day01", "r", stdin);

    // part1();
    part2();

    return 0;
}

void part1() {
    vector<int> left;
    vector<int> right;
    int a, b;
    while (cin >> a >> b) {
        left.push_back(a);
        right.push_back(b);

        // cout << "Left: " << a << "; Right: " << b << "\n";
    }
    sort(left.begin(), left.end());
    sort(right.begin(), right.end());

    int total = 0;
    for (int i = 0; i < left.size(); i++) {
        total += abs(left[i] - right[i]);
    }
    cout << "Total: " << total << "\n";
}

void part2() {
    vector<int> left;
    vector<int> right;
    int a, b;
    while (cin >> a >> b) {
        left.push_back(a);
        right.push_back(b);

        // cout << "Left: " << a << "; Right: " << b << "\n";
    }

    int total = 0;
    for (int i = 0; i < left.size(); i++) {
        int num_c = count(right.begin(), right.end(), left[i]);
        total += (left[i] * num_c);
    }
    cout << "Total: " << total << "\n";
}
