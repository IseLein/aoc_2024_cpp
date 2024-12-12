
#include <bits/stdc++.h>

using namespace std;

unordered_map<string, char> NUMS = {
    {"one",   '1'},
    {"two",   '2'},
    {"three", '3'},
    {"four",  '4'},
    {"five",  '5'},
    {"six",   '6'},
    {"seven", '7'},
    {"eight", '8'},
    {"nine",  '9'}
};

char get_num(string line, int index) {
    for (auto pair : NUMS) {
        if (((line.size() - index) >= pair.first.size()) && (line.substr(index, pair.first.size()) == pair.first)) {
            return pair.second;
        }
    }
    return '0';
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    freopen("input/day00", "r", stdin);

    int total = 0;
    int num_lines = 0;
    string line;
    while (cin >> line) {
        string num = "00";
        for (int i = 0; i < line.length(); i++) {
            if (isdigit(line[i])) {
                num[0] = line[i];
                break;
            }
            char str_num = get_num(line, i);
            if (str_num != '0') {
                num[0] = str_num;
                break;
            }
        }
        for (int i = line.length() - 1; i >= 0; i--) {
            if (isdigit(line[i])) {
                num[1] = line[i];
                break;
            }
            int str_num = get_num(line, i);
            if (str_num != '0') {
                num[1] = str_num;
                break;
            }
        }
        total += stoi(num);
        num_lines++;
        // cout << "Line " << num_lines << ": " << num << "\n";
    }
    cout << "Total: " << total << "; " << "Numbers: " << num_lines << ".\n";
}
