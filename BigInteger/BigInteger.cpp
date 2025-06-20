/***
* Author: Nathan Tran
* CruzID: ntran48
* 2024 Fall CSE101 PA6
* File: BigInteger.cpp
* Description: BigInteger ADT
***/

#include "BigInteger.h"
#include "List.h"

#include <cctype>
#include <cmath>
#include <stdexcept>

using namespace std;

const ListElement BASE  = 1000000000;   // 10^(power)
const int         POWER = 9;    // log_10(base)

void delZero(List* L) {
    if (L->length() == 0) {
        return; 
    }

    L->moveFront();
    while (L->position() < L->length()) {
        int currentElement = L->peekNext();
        if (currentElement != 0) {
            break;
        }
        L->eraseAfter();
    }
}

List tempMult(long s, List *b, int* ctr) {
    List resultList;
    long carryOver = 0;

    for (b->moveBack(); b->position() > 0; b->movePrev()) {
        long currentElement = b->peekPrev();
        long product = currentElement * s + carryOver;

        carryOver = product / BASE;
        resultList.insertAfter(product % BASE);
    }

    if (carryOver > 0) {
        resultList.insertAfter(carryOver);
    }

    if (*ctr > 0) {
        resultList.moveBack();
        for (int i = 0; i < *ctr; i++) {
            resultList.insertAfter(0);
        }
    }

    return resultList;
}

BigInteger::BigInteger() {
    digits = List();
    signum = 0;
}

BigInteger::BigInteger(long value) {
    if (value == 0) {
        signum = 0;
        digits.insertAfter(0);
        return;
    }

    signum = (value < 0) ? -1 : 1;
    long absValue = (value < 0) ? -value : value;

    do {
        long currentDigit = absValue % BASE;
        digits.insertBefore(currentDigit);
        absValue /= BASE;
    } while (absValue > 0);
}

BigInteger::BigInteger(std::string input) {
    if (input.empty()) {
        throw std::invalid_argument("BigInteger error: calling BigInteger on empty string\n");
    }

    signum = (input[0] == '-') ? -1 : 1;
    size_t startIndex = (input[0] == '+' || input[0] == '-') ? 1 : 0;

    for (size_t i = startIndex; i < input.length(); ++i) {
        if (!std::isdigit(input[i])) {
            throw std::invalid_argument("BigInteger error: calling BigInteger on non-numeric number\n");
        }
    }

    size_t totalLength = input.length();
    size_t currentPos = totalLength;

    while (currentPos > startIndex) {
        size_t chunkSize = (currentPos < POWER) ? currentPos - startIndex : POWER;
        std::string chunk = input.substr(currentPos - chunkSize, chunkSize);
        digits.insertAfter(std::stol(chunk));
        currentPos -= chunkSize;
    }

    delZero(&digits);
    if (digits.length() == 1 && digits.peekNext() == 0) {
        signum = 0;
    }
}


BigInteger::BigInteger(const BigInteger& N) {
    this->digits = N.digits;
    this->signum = N.signum;
}

// Access Functions
int BigInteger::sign() const{
    return this->signum;
}

int BigInteger::compare(const BigInteger& N) const {
    if (signum == 0 && N.signum == 0) {
        return 0;
    }

    if (signum != N.signum) {
        return (signum > N.signum) ? 1 : -1;
    }

    const List& thisDigits = digits;
    const List& otherDigits = N.digits;

    if (thisDigits.length() != otherDigits.length()) {
        return (thisDigits.length() > otherDigits.length()) ? signum : -signum;
    }

    List thisCopy = thisDigits;
    List otherCopy = otherDigits;

    thisCopy.moveFront();
    otherCopy.moveFront();

    while (thisCopy.position() < thisCopy.length()) {
        long thisDigit = thisCopy.peekNext();
        long otherDigit = otherCopy.peekNext();

        if (thisDigit != otherDigit) {
            return (thisDigit > otherDigit) ? signum : -signum;
        }

        thisCopy.moveNext();
        otherCopy.moveNext();
    }

    return 0;
}


// Manipulation Procedures
void BigInteger::makeZero() {
    digits.clear();
    signum = 0;
}

void BigInteger::negate() {
    signum *= -1;
}

BigInteger BigInteger::add(const BigInteger& N) const {
    BigInteger result;
    if (this->sign() != N.sign()) {
        BigInteger tempN = N;
        BigInteger tempThis = *this;
        
        if (this->sign() > 0) {
            tempN.negate();
            return this->sub(tempN);
        } else {
            tempThis.negate();
            return N.sub(tempThis);
        }
    }

    List xList = this->digits;
    List yList = N.digits;
    List& resultList = result.digits;

    long carry = 0;
    long tempSum = 0;

    xList.moveBack();
    yList.moveBack();

    while (xList.position() > 0 || yList.position() > 0) {
        long xDigit = (xList.position() > 0) ? xList.peekPrev() : 0;
        long yDigit = (yList.position() > 0) ? yList.peekPrev() : 0;

        tempSum = xDigit + yDigit + carry;
        carry = tempSum / BASE;
        tempSum %= BASE;

        resultList.insertAfter(tempSum);

        if (xList.position() > 0) xList.movePrev();
        if (yList.position() > 0) yList.movePrev();
    }

    if (carry > 0) {
        resultList.insertAfter(carry);
    }

    result.signum = this->sign();
    delZero(&resultList);

    return result;
}

BigInteger BigInteger::sub(const BigInteger& N) const {
    BigInteger A = *this;
    BigInteger B = N;
    BigInteger C;
    List& a = A.digits;
    List& b = B.digits;
    List& c = C.digits;

    if (A == B) {
        return C;
    }

    if (A.sign() > 0 && B.sign() < 0) {
        B.negate();
        return A.add(B);  // Convert to A + (-B)
    } else if (A.sign() < 0 && B.sign() > 0) {
        A.negate();
        C = A.add(B);
        C.negate();
        return C;  // Convert to -A + B
    }

    bool negateResult = false;
    if (A.compare(B) < 0) {
        std::swap(A, B);
        negateResult = true;
    }

    a.moveBack();
    b.moveBack();

    long borrow = 0;
    long temp = 0;

    while (a.position() > 0 || b.position() > 0) {
        long aDigit = 0;
        long bDigit = 0;

        if (a.position() > 0) {
            aDigit = a.peekPrev();
            a.movePrev();
        }

        if (b.position() > 0) {
            bDigit = b.peekPrev();
            b.movePrev();
        }

        temp = aDigit - bDigit - borrow;
        if (temp < 0) {
            temp += BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }

        c.insertAfter(temp);
    }

    delZero(&c);

    C.signum = negateResult ? -1 : 1;

    return C;
}

BigInteger BigInteger::mult(const BigInteger& N) const {
    BigInteger A = *this;
    BigInteger B = N;
    BigInteger C;
    List& a = A.digits;
    List& b = B.digits;

    a.moveBack();
    b.moveBack();
    
    int ctr = 0;

    while (a.position() > 0) {
        List temp = tempMult(a.peekPrev(), &b, &ctr);

        BigInteger T;
        T.signum = 1;
        T.digits = temp;
        C = C.add(T);

        a.movePrev();
        ctr++;
    }

    C.signum = A.signum * B.signum;

    return C;
}


// Other Functions

std::string BigInteger::to_string() {
    if (signum == 0) return "0";

    std::string result;
    if (signum == -1) {
        result += "-";
    }

    List tempList = digits;
    tempList.moveFront();
    
    bool leadingZero = true;
    while (tempList.position() < tempList.length()) {
        std::string A = std::to_string(tempList.peekNext());
        std::string B = "";
        
        while ((int)(B.length() + A.length()) < POWER && tempList.position() != 0) {
            B += '0';
        }
        
        if (A != "0" || !leadingZero) {
            result += (B + A);
            leadingZero = false;
        }

        tempList.moveNext();
    }

    return result;
}

// Overriden Operators

std::ostream& operator<<( std::ostream& stream, BigInteger N ) {
    return stream<<N.BigInteger::to_string();
}
 
bool operator==(const BigInteger& A, const BigInteger& B) {
    return A.compare(B) == 0;
}

bool operator<(const BigInteger& A, const BigInteger& B) {
    return A.compare(B) == -1;
}

bool operator<=(const BigInteger& A, const BigInteger& B) {
    return A.compare(B) <= 0;
}

bool operator>(const BigInteger& A, const BigInteger& B) {
    return A.compare(B) == 1;
}

bool operator>=(const BigInteger& A, const BigInteger& B) {
    return A.compare(B) >= 0;
}

BigInteger operator+(const BigInteger& A, const BigInteger& B) {
    return A.add(B);
}



BigInteger operator+=(BigInteger& A, const BigInteger& B) {
    A = A + B;
    return A;
}

BigInteger operator-(const BigInteger& A, const BigInteger& B) {
    return A.sub(B);
}

BigInteger operator-=(BigInteger& A, const BigInteger& B) {
    A = A - B;
    return A;
}

BigInteger operator*(const BigInteger& A, const BigInteger& B) {
    return A.mult(B);
}

BigInteger operator*=(BigInteger& A, const BigInteger& B) {
    A = A * B;
    return A;
}
