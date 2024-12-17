
#include <cctype>
#include <cmath>
#include <fstream>
#include <ios>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

long combo(int operand, long &reg_a, long &reg_b, long &reg_c) {
    if ((operand >= 0) && (operand <= 3)) {
        return operand;
    }
    if (operand == 4) {
        return reg_a;
    }
    if (operand == 5) {
        return reg_b;
    }
    if (operand == 6) {
        return reg_c;
    }
    return -1;
}

void adv(long &reg_a, long &reg_b, long &reg_c, int &iPointer, int operand, string &out) {
    long denom = pow(2, combo(operand, reg_a, reg_b, reg_c));
    reg_a /= denom;
    iPointer += 2;
}

void bxl(long &reg_a, long &reg_b, long &reg_c, int &iPointer, int operand, string &out) {
    reg_b ^= operand;
    iPointer += 2;
}

void bst(long &reg_a, long &reg_b, long &reg_c, int &iPointer, int operand, string &out) {
    reg_b = combo(operand, reg_a, reg_b, reg_c) % 8;
    iPointer += 2;
}

void jnz(long &reg_a, long &reg_b, long &reg_c, int &iPointer, int operand, string &out) {
    if (reg_a == 0) {
        iPointer += 2;
        return;
    }
    iPointer = operand;
}

void bxc(long &reg_a, long &reg_b, long &reg_c, int &iPointer, int operand, string &out) {
    reg_b ^= reg_c;
    iPointer += 2;
}

void out(long &reg_a, long &reg_b, long &reg_c, int &iPointer, int operand, string &out) {
    long l = combo(operand, reg_a, reg_b, reg_c) % 8;
    if (out == "") {
        out += to_string(l);
    } else {
        out += ("," + to_string(l));
    }
    iPointer += 2;
}

void bdv(long &reg_a, long &reg_b, long &reg_c, int &iPointer, int operand, string &out) {
    long denom = pow(2, combo(operand, reg_a, reg_b, reg_c));
    reg_b = reg_a / denom;
    iPointer += 2;
}

void cdv(long &reg_a, long &reg_b, long &reg_c, int &iPointer, int operand, string &out) {
    long denom = pow(2, combo(operand, reg_a, reg_b, reg_c));
    reg_c = reg_a / denom;
    iPointer += 2;
}

using InstructionType = void(*) (long &reg_a, long &reg_b, long &reg_c, int &iPointer, int operand, string &out);

void parseInput(ifstream &fin, long &reg_a, long &reg_b, long &reg_c, vector<int> &instructions);

int main() {
    map<int, InstructionType> iMap;
    iMap[0] = adv;
    iMap[1] = bxl;
    iMap[2] = bst;
    iMap[3] = jnz;
    iMap[4] = bxc;
    iMap[5] = out;
    iMap[6] = bdv;
    iMap[7] = cdv;

    ifstream fin("input/day17");
    long reg_a;
    long reg_b;
    long reg_c;
    vector<int> instructions;
    parseInput(fin, reg_a, reg_b, reg_c, instructions);

    string in = "";
    for (int i = 0; i < (int)instructions.size(); i++) {
        if (i != 0) {
            in += ",";
        }
        in += to_string(instructions[i]);
    }
    cout << in << "\n";

    // long tReg_a = 247839000000002 - 100000000;
    // local brute force search after manually checking for first 9 digits
    string octNum = "7026420340000000";
    long tReg_a = stol(octNum, nullptr, 8);
    cout << oct << tReg_a << "\n";
    int i = 0;
    string iOut;
    while (in != iOut) {
        tReg_a += 1;
        i += 1;
        int iPointer = 0;
        reg_a = tReg_a;
        iOut = "";
        while (iPointer < (int)instructions.size()) {
            int operand = instructions[iPointer + 1];
            iMap[instructions[iPointer]](reg_a, reg_b, reg_c, iPointer, operand, iOut);
        }
        // dec to oct
        cout << dec << tReg_a << "|";
        long octNum = tReg_a;
        cout << oct << octNum << "-->" << iOut << "\n";
        if (i > pow(8, 8)) {
            break;
        }
    }
}

void parseInput(ifstream &fin, long &reg_a, long &reg_b, long &reg_c, vector<int> &instructions) {
    string line;
    string s;

    getline(fin, line);
    stringstream ss_a(line);
    ss_a >> s >> s >> reg_a;

    getline(fin, line);
    stringstream ss_b(line);
    ss_b >> s >> s >> reg_b;

    getline(fin, line);
    stringstream ss_c(line);
    ss_c >> s >> s >> reg_c;

    getline(fin, line);
    getline(fin, line);
    stringstream ss_p(line);
    ss_p >> s;
    ss_p >> s;

    for (char &c: s) {
        if (isdigit(c)) {
            instructions.push_back(c - '0');
        }
    }
}
