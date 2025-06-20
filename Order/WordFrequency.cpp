/***
* Author: Nathan Tran
* CruzID: ntran48
* 2024 Fall CSE101 PA8
* File: WordFrequency.cpp
* Description: Second top level client
***/

#include <fstream>
#include "Dictionary.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
        return EXIT_FAILURE;
    }

    ifstream in(argv[1]);
    ofstream out(argv[2]);

    if (!in.is_open()) {
        cerr << "Unable to open file " << argv[1] << " for reading" << endl;
        return EXIT_FAILURE;
    }

    if (!out.is_open()) {
        cerr << "Unable to open file " << argv[2] << " for writing" << endl;
        return EXIT_FAILURE;
    }

    string line, key;
    string delim = " \t\"',<.>/?;:[{]}|`~!@#$%^&*()-_=+0123456789";
    Dictionary D;

    while (getline(in, line)) {
        size_t pos = 0, len = line.size();
        while (pos < len) {
            pos = line.find_first_not_of(delim, pos);
            if (pos == string::npos) break;

            size_t next = line.find_first_of(delim, pos);
            if (next == string::npos) next = len;

            key = line.substr(pos, next - pos);

            for (size_t i = 0; i < key.size(); i++) {
                key[i] = tolower(key[i]);
            }

            if (D.contains(key)) {
                D.getValue(key)++;
            } else {
                D.setValue(key, 1);
            }

            pos = next;
        }
    }

    out << D << endl;

    D.clear();
    in.close();
    out.close();

    return EXIT_SUCCESS;
}
