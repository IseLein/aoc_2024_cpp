
#include <bitset>
#include <fstream>
#include <iostream>
#include <ostream>
#include <set>
#include <sstream>
#include <map>
#include <vector>

#define DEBUG true
#define IFD(x) if (DEBUG) { x; }
#define X_MAX_BIT 45
#define Y_MAX_BIT 45
#define Z_MAX_BIT 46

using namespace std;

enum GATE {
    AND,
    OR,
    XOR
};

struct Construction {
    GATE gate;
    string input1;
    string input2;

    friend ostream& operator<<(ostream& os, const Construction& construction);
};

int wire_value(string wire, map<string, int> &wire_values, map<string, Construction> &wire_constructions);
string wire_value(string wire, map<string, string> &wire_string, map<string, Construction> &wire_constructions);
string z_value(map<string, int> &wire_values, map<string, Construction> &wire_constructions, int max_bit);
string left_pad(string s, int length, char pad = '0');
int test_wires(map<string, Construction> &wire_constructions);
int test_given_wires(map<string, int> &wire_values, map<string, Construction> &wire_constructions);
void swap_wires(map<string, Construction> &wire_constructions, string w1, string w2);
