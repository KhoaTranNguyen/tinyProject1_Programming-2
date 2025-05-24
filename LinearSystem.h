#include <iostream>
#include <stdexcept> // For out_of_range exception
using namespace std;

class LinearSystem{

};


class Vector:public LinearSystem {
    private:
        int* mData;
        int mSize;
    public:
        Vector(): mSize(0) {}
        Vector(int size): mSize(size) {
            mData = new int[mSize+1];
        }
        Vector(int size, int* data): mSize(size), mData(data){}
        Vector operator=(const Vector& otherVector){
            return Vector(otherVector.mSize, otherVector.mData);
        }
        Vector operator+(const Vector& otherVector){
            if(this->mSize != otherVector.mSize) {
                return NULL;
            }
            for(int i=1;i<this->mSize+1;i++){
                this->mData[i] += otherVector.mData[i];
            }
            return *this;
        }
        Vector operator-(const Vector& otherVector){
            if(this->mSize != otherVector.mSize) {
                return NULL;
            }
            for(int i=1;i<this->mSize+1;i++){
                this->mData[i] -= otherVector.mData[i];
            }
            return *this; 
        }
        Vector operator*(const Vector& otherVector){
            if(this->mSize != otherVector.mSize) {
                return NULL;
            }
            for(int i=1;i<this->mSize+1;i++){
                this->mData[i] *= otherVector.mData[i];
            }
            return *this;  
        }
        Vector operator*(int scalar){
            for(int i=1;i<this->mSize+1;i++){
                this->mData[i] *= scalar;
            }
            return *this;  
        }
        bool operator[] (int index){
            if(index<=0 || index >= this->mSize){
                throw out_of_range("Vector index out of bound");
                return false;
            }
            return true;
        }
        int operator() (int index){
            if(index<=0 || index >= this->mSize){
                throw out_of_range("Vector index out of bound");
                return false;
            }
            return this->mData[index];
        }
        ~Vector(){}

};

