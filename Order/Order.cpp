/***
* Author: Nathan Tran
* CruzID: ntran48
* 2024 Fall CSE101 PA8
* File: Order.cpp
* Description: Top client
***/

#include <fstream>
#include "Dictionary.h"

using namespace std;

int main(int argc, char* argv[]) {
    // Check for the correct number of arguments
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input file> <output file>\n" << std::endl;
        return EXIT_FAILURE;
    }

    ifstream infile(argv[1]);
    if (!infile) {  // Check for readable input
        std::cerr << "Not able to open file\n" << argv[1] << std::endl;
        return EXIT_FAILURE;
    }

    ofstream outfile(argv[2]);
    if (!outfile) {     // Check for output writability
        std::cerr << "Not able to open file\n" << argv[2] << std::endl;
        infile.close();
        return EXIT_FAILURE;
    }


    Dictionary D;
    std::string k;
    int v = 1;

    while (infile >> k) D.setValue(k, v++);

    outfile << D.to_string() << '\n' << D.pre_string() << '\n';

    D.clear();
    return EXIT_SUCCESS;

}