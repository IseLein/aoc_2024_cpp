
#include <bits/stdc++.h>

using namespace std;

void part1();
void part2();
string get_num(string str, size_t index, char delim);

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    freopen("input/day03", "r", stdin);

    part2();
}

string get_num(string str, size_t index, char delim) {
    string num = "";
    for (size_t i = index; i < index + 3; i++) {
        if (i >= str.size()) {
            return "";
        }
        if (isdigit(str[i])) {
            num += str[i];
        } else {
            if (str[i] == delim && num.size() > 0) {
                return num;
            }
            return "";
        }
    }
    if (str[index + 3] == delim) {
        return num;
    }
    return "";
}


void part1() {
    string line;
    string temp;
    while (getline(cin, temp)) {
        line += temp;
    }

    int total = 0;
    string curr_a = "";
    string curr_b = "";
    string curr_tok = "";
    for (size_t i = 0; i < line.size(); i++) {
        if (curr_tok.empty()) {
            if (line[i] == 'm') {
                curr_tok = "m";
            } else {
                continue;
            }
        } else if (curr_tok == "m") {
            if (line[i] == 'u') {
                curr_tok = "mu";
            } else {
                curr_tok = "";
                continue;
            }
        } else if (curr_tok == "mu") {
            if (line[i] == 'l') {
                curr_tok = "mul";
            } else {
                curr_tok = "";
                continue;
            }
        } else if (curr_tok == "mul") {
            if (line[i] == '(') {
                curr_tok = "mul(";
            } else {
                curr_tok = "";
                continue;
            }
        } else if (curr_tok == "mul(") {
            // first number
            string num = get_num(line, i, ',');
            if (num.empty()) {
                curr_tok = "";
                continue;
            }
            curr_a = num;
            i += num.size() + 1;
            // second number
            num = get_num(line, i, ')');
            if (num.empty()) {
                curr_tok = "";
                continue;
            }
            curr_b = num;
            i += num.size();
            total += stoi(curr_a) * stoi(curr_b);
            curr_tok = "";
            // cout << "A: " << curr_a << "; B: " << curr_b << "; Total: " << total << ".\n";
        }
    }
    cout << "Total: " << total << ".\n";
}

void part2() {
    string line;
    string temp;
    while (getline(cin, temp)) {
        line += temp;
    }

    int total = 0;
    string curr_a = "";
    string curr_b = "";
    string curr_tok = "";
    bool do_mul = true;
    for (size_t i = 0; i < line.size(); i++) {
        if (curr_tok.empty()) {
            if (line[i] == 'm') {
                curr_tok = "m";
            } else if (line[i] == 'd') {
                curr_tok = "d";
            } else {
                continue;
            }
        } else if (curr_tok == "d") {
            if (line[i] == 'o') {
                curr_tok = "do";
            } else {
                curr_tok = "";
                continue;
            }
        } else if (curr_tok == "do") {
            if (line[i] == '(') {
                curr_tok = "do(";
            } else if (line[i] == 'n') {
                curr_tok = "don";
            } else {
                curr_tok = "";
                continue;
            }
        } else if (curr_tok == "do(") {
            if (line[i] == ')') {
                do_mul = true;
                curr_tok = "";
                cout << "do()" << "\n";
            }
            continue;
        } else if (curr_tok == "don") {
            if (i + 3 >= line.size() - 1) {
                curr_tok = "";
                continue;
            }
            if ((line[i] == '\'') && (line[i + 1] == 't') && (line[i + 2] == '(') && (line[i + 3] == ')')) {
                do_mul = false;
                curr_tok = "";
                cout << "don't()" << "\n";
            } else {
                curr_tok = "";
            }
            continue;
        } else if (curr_tok == "m") {
            if (line[i] == 'u') {
                curr_tok = "mu";
            } else {
                curr_tok = "";
                continue;
            }
        } else if (curr_tok == "mu") {
            if (line[i] == 'l') {
                curr_tok = "mul";
            } else {
                curr_tok = "";
                continue;
            }
        } else if (curr_tok == "mul") {
            if (line[i] == '(') {
                curr_tok = "mul(";
            } else {
                curr_tok = "";
                continue;
            }
        } else if (curr_tok == "mul(") {
            // first number
            string num = get_num(line, i, ',');
            if (num.empty()) {
                curr_tok = "";
                continue;
            }
            curr_a = num;
            i += num.size() + 1;
            // second number
            num = get_num(line, i, ')');
            if (num.empty()) {
                curr_tok = "";
                continue;
            }
            curr_b = num;
            i += num.size();
            if (do_mul) {
                total += stoi(curr_a) * stoi(curr_b);
            }
            curr_tok = "";
            if (!do_mul) {
                cout << "No mul == ";
            }
            cout << "A: " << curr_a << "; B: " << curr_b << "; Total: " << total << ".\n";
        }
    }
    cout << "Total: " << total << ".\n";
}

