/***
* Author: Nathan Tran
* CruzID: ntran48
* 2024 Fall CSE101 PA6
* File: Arithmetic.cpp
* Description: Test client
***/

#include <fstream>
#include <iostream>

#include "BigInteger.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input file> <output file>\n" << endl;
        return EXIT_FAILURE;
    }

    // Open files
    ifstream in(argv[1]);
    ofstream out(argv[2]);
    if (!in || !out) {
        cerr << "Error: Unable to open one of the files.\n" << endl;
        return EXIT_FAILURE;
    }

    // Read in strings
    string sA, sB;
    in >> sA >> sB;

    // Objects
    BigInteger A(sA), B(sB);
    BigInteger two("2"), three("3"), nine("9"), sixteen("16");

    out << A << endl << endl;   // A

    out << B << endl << endl;   // B

    BigInteger sum = A + B; // A + B
    out << sum << endl << endl;

    BigInteger diff = A - B;    // A - B
    out << diff << endl << endl;

    BigInteger zero = A - A;    // A - A
    out << zero << endl << endl;

    BigInteger expr1 = (three * A) - (two * B); // 3A - 2B
    out << expr1 << endl << endl;

    BigInteger prod = A * B;    // AB
    out << prod << endl << endl;

    BigInteger A_squared = A * A;   // A^2
    out << A_squared << endl << endl;

    BigInteger B_squared = B * B;       // B^2
    out << B_squared << endl << endl;

    // 9A^4 + 16B^5
    BigInteger termA = nine * (A * A * A * A);  // A^4
    BigInteger termB = sixteen * (B * B * B * B * B);   // B^5
    out << termA + termB << endl << endl;

    in.close();
    out.close();

    return EXIT_SUCCESS;
}