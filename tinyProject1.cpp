#include <iostream>
using namespace std;

class Vector {
    private:
        int* arr;
        int size;
    public:
        Vector(): size(0) {}
        Vector(int s): size(s) {
            int* arr = new int[size];
            for (int i = 0; i < size; i++) {
                cout << "a" << i << ": ";
                cin >> arr[i];
            }
            for (int i = 0; i < size; i++) {
                
            }
        }
};