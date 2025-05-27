#include <iostream>
#include "Vector.hpp"
using namespace std;

int main() {

    cout << "Creating vector v1:" << endl;
    Vector v1 = {1, 2, 3};  // User input: 1 2 3
    try {
        cout << v1 << endl;
    }
    catch (const runtime_error& e) {
        cout << e.what();
    }

    cout << "\nCreating vector v2:" << endl;
    Vector v2 = {4, 5, 6};  // User input: 4 5 6
    try {
        cout << v2 << endl;
    }
    catch (const runtime_error& e) {
        cout << e.what();
    }

    cout << "\nTesting multiplication (v1 * v2):" << endl;
    try {
        double dot_result = v1 * v2;
        cout << "v3 = " << dot_result << endl;
    }
    catch (const runtime_error& e) {
        cout << e.what();
    }

    cout << "\nTesting addition (v1 + v2):" << endl;
    try {
        Vector v3 = v1 + v2;
        cout << "v3 = " << v3 << endl;
    }
    catch (const runtime_error& e) {
        cout << e.what();
    }

    cout << "\nTesting subtraction (v1 - v2):" << endl;
    try {
        Vector v4 = v1 - v2;
        cout << "v4 = "  << v4 << endl;
    }
    catch (const runtime_error& e) {
        cout << e.what();
    }

    cout << "\nTesting scalar multiplication (v1 * 2.0):" << endl;
    Vector v5 = v1 * 2.0;
    cout << "v5 = "  << v5 << endl;

    cout << "\nAccessing elements in v1 using 1-based ():" << endl;
    for (int i = 1; i <= 3; ++i) {
        try {
            cout << "v1(" << i << ") = " << v1(i) << endl;
        }
        catch (const out_of_range& e) {
            cout << e.what() << endl;
        }
    }

    cout << "\nAttempting out-of-bounds access v1(0):" << endl;
    try {
        cout << "v1(0) = " << v1(0);  // Should print error and exit
    }
    catch (const out_of_range& e) {
        cout << e.what() << endl;
    }

    return 0;
}