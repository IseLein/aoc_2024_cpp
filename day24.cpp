
#include "day24.h"

ostream& operator<<(ostream& os, const Construction& construction) {
    string gate;
    switch(construction.gate) {
        case AND:
            gate = "AND";
            break;
        case OR:
            gate = "OR";
            break;
        case XOR:
            gate = "XOR";
            break;
    }
    os << construction.input1 << " " << gate << " " << construction.input2;
    return os;
}

int l_main() {
    ifstream fin("input/day24");

    map<string, int> wire_values;
    map<string, Construction> wire_constructions;
    set<string> output_wires;
    int max_bit = 0;

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
                return 1;
        }
        wire_constructions[wire_out] = construction;

        if (wire_out[0] == 'z') {
            int bit = stoi(wire_out.substr(1, wire_out.size()-1));
            if (bit > max_bit) {
                max_bit = bit;
            }
        }

        output_wires.insert(wire_out);
    }

    // part 1
    string z_out = z_value(wire_values, wire_constructions, max_bit);
    long output = stol(z_out, nullptr, 2);
    cout << "Binary: " << z_out << "\n";
    cout << "Decimal: " << output << "\n";
    cout << "\n";

    // part 2
    int base_diff = test_wires(wire_constructions);
    int g_base_diff = test_given_wires(wire_values, wire_constructions);
    cout << "Base diff (test): " << base_diff << "\n";
    cout << "Base diff (on given wires): " << g_base_diff << "\n";
    cout << "\n";

    map<string, Construction> constructions(wire_constructions);
    // found wires manually with day24_viz
    swap_wires(constructions, "z10", "ggn");
    swap_wires(constructions, "jcb", "ndw");
    swap_wires(constructions, "z39", "twr");
    swap_wires(constructions, "z32", "grm");
    base_diff = test_wires(constructions);
    g_base_diff = test_given_wires(wire_values, constructions);
    cout << "New diff (test): " << base_diff << "\n";
    cout << "New diff (on given wires): " << g_base_diff << "\n";
    cout << "\n";

    set<string> swapped_wires = {"z10", "ggn", "jcb", "ndw", "z39", "twr", "z32", "grm"};
    string p_wires = "";
    for (auto &wire : swapped_wires) {
        p_wires += wire + ",";
    }
    p_wires.pop_back();
    cout << "swapped wires: " << p_wires << "\n";

    return 0;
}

int test_wires(map<string, Construction> &wire_constructions) {
    srand(7);
    int diff = 0;
    for (int i = 0; i < 3; i++) {
        // monstrosity to get a random number that is larger than int
        long x_in = (((long)(std::rand()) << 32) | std::rand()) % ((long)1 << (X_MAX_BIT));
        long y_in = (((long)(std::rand()) << 32) | std::rand()) % ((long)1 << (Y_MAX_BIT));
        long z_expected = x_in + y_in;

        map<string, int> wire_values;
        string b_x_in = bitset<X_MAX_BIT>(x_in).to_string();
        for (int j = 0; j < X_MAX_BIT; j++) {
            string x_wire = 'x' + left_pad(to_string(j), 2);
            wire_values[x_wire] = b_x_in[X_MAX_BIT - j - 1] == '1' ? 1 : 0;
        }
        string b_y_in = bitset<Y_MAX_BIT>(y_in).to_string();
        for (int j = 0; j < Y_MAX_BIT; j++) {
            string y_wire = 'y' + left_pad(to_string(j), 2);
            wire_values[y_wire] = b_y_in[Y_MAX_BIT - j - 1] == '1' ? 1 : 0;
        }
        // IFD(cout << "x_in: " << b_x_in << ", " << x_in << "\n");
        // IFD(cout << "y_in: " << b_y_in << ", " << y_in << "\n");

        map<string, Construction> constructions(wire_constructions);
        string z_out = string(Z_MAX_BIT, '0');
        for (int j = 0; j < Z_MAX_BIT; j++) {
            string z_wire = 'z' + left_pad(to_string(j), 2);
            int value = wire_value(z_wire, wire_values, constructions);
            z_out[Z_MAX_BIT - j - 1] = value ? '1' : '0';
        }
        long output = stol(z_out, nullptr, 2);

        long z_diff = z_expected ^ output;
        string z_diff_str = bitset<Z_MAX_BIT>(z_diff).to_string();

        /*cout << "expected vs actual:\n";*/
        /*cout << bitset<Z_MAX_BIT>(z_expected).to_string() << ": " << z_expected << "\n";*/
        /*cout << z_out << ": " << output << "\n";*/
        /*cout << z_diff_str << "\n";*/

        int t_diff = 0;
        for (int i = 0; i < Z_MAX_BIT; i++) {
            if (z_diff_str[i] == '1') {
                t_diff++;
            }
        }
        if (t_diff > diff) {
            diff = t_diff;
        }
    }
    return diff;
}

int test_given_wires(map<string, int> &wire_values, map<string, Construction> &wire_constructions) {
    int diff = 0;
    int x_max_bit = 0;
    string x_in;
    while(true) {
        string x_wire = 'x' + left_pad(to_string(x_max_bit), 2);
        if (wire_values.count(x_wire) == 0) {
            x_max_bit--;
            break;
        }
        char value = wire_values[x_wire] ? '1' : '0';
        x_in = value + x_in;
        x_max_bit++;
    }
    int y_max_bit = 0;
    string y_in;
    while(true) {
        string y_wire = 'y' + left_pad(to_string(y_max_bit), 2);
        if (wire_values.count(y_wire) == 0) {
            y_max_bit--;
            break;
        }
        char value = wire_values[y_wire] ? '1' : '0';
        y_in = value + y_in;
        y_max_bit++;
    }
    long x_in_ = stol(x_in, nullptr, 2);
    long y_in_ = stol(y_in, nullptr, 2);
    long z_expected = x_in_ + y_in_;


    map<string, int> wire_values_copy(wire_values);
    string z_out = z_value(wire_values_copy, wire_constructions, Z_MAX_BIT - 1);
    long output = stol(z_out, nullptr, 2);

    long z_diff = z_expected ^ output;
    string z_diff_str = bitset<Z_MAX_BIT>(z_diff).to_string();
    for (int i = 0; i < Z_MAX_BIT; i++) {
        if (z_diff_str[i] == '1') {
            diff++;
        }
    }

    /*cout << "expected vs actual\n";*/
    /*cout << z_out << ": " << output << "\n";*/
    /*cout << bitset<Z_MAX_BIT>(z_expected).to_string() << ": " << z_expected << "\n";*/
    /*cout << z_diff_str << "\n";*/

    return diff;
}

void swap_wires(map<string, Construction> &wire_constructions, string w1, string w2) {
    Construction c1 = wire_constructions[w1];
    Construction c2 = wire_constructions[w2];
    wire_constructions[w1] = c2;
    wire_constructions[w2] = c1;
}

string z_value(map<string, int> &wire_values, map<string, Construction> &wire_constructions, int max_bit) {
    map<string, int> wire_values_copy(wire_values);
    string z_out = string(max_bit + 1, '0');
    for (int i = 0; i <= max_bit; i++) {
        string z_wire = 'z' + left_pad(to_string(i), 2);
        int value = wire_value(z_wire, wire_values_copy, wire_constructions);
        z_out[max_bit - i] = value ? '1' : '0';
    }
    return z_out;
}

string wire_value(string wire, map<string, string> &wire_string, map<string, Construction> &wire_constructions) {
    // cout << wire << "\n";
    if (wire_string.count(wire) > 0) {
        return wire_string.at(wire);
    }

    if (wire[0] == 'x') {
        int num = stoi(wire.substr(1));
        string value = "x" + left_pad(to_string(num), 2);
        wire_string[wire] = value;
        return value;
    }
    if (wire[0] == 'y') {
        int num = stoi(wire.substr(1));
        string value = "y" + left_pad(to_string(num), 2);
        wire_string[wire] = value;
        return value;
    }

    Construction construction = wire_constructions[wire];
    string value1 = wire_value(construction.input1, wire_string, wire_constructions);
    string value2 = wire_value(construction.input2, wire_string, wire_constructions);
    string gate;
    switch(construction.gate) {
        case AND:
            gate = "&";
            break;
        case OR:
            gate = "|";
            break;
        case XOR:
            gate = "^";
            break;
        default:
            gate = "/";
    }

    string value = "(" + value1 + ")" + gate + "(" + value2 + ")";
    wire_string[wire] = value;
    return value;
}

int wire_value(string wire, map<string, int> &wire_values, map<string, Construction> &wire_constructions) {
    if (wire_values.count(wire) > 0) {
        return wire_values.at(wire);
    }

    Construction construction = wire_constructions[wire];
    int value1 = wire_value(construction.input1, wire_values, wire_constructions);
    int value2 = wire_value(construction.input2, wire_values, wire_constructions);
    int value;
    switch(construction.gate) {
        case AND:
            value = value1 & value2;
            wire_values.insert({ wire, value });
            return value;
        case OR:
            value = value1 | value2;
            wire_values.insert({ wire, value });
            return value;
        case XOR:
            value = value1 ^ value2;
            wire_values.insert({ wire, value });
            return value;
    }
    return -1;
}

string left_pad(string s, int length, char pad) {
    string result = s;
    while ((int)result.size() < length) {
        result = pad + result;
    }
    return result;
}
