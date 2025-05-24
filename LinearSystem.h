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

        Vector(const Vector& otherVector){
            if(otherVector.mSize == 0 || otherVector.mData == 0) {
                this->mSize = 0;
                this->mData = NULL;
            }
            else{
                this->mSize = otherVector.mSize;
                this->mData = new int[otherVector.mSize+1];
                for(int i=1;i<otherVector.mSize+1;i++){
                    this->mData[i] = otherVector.mData[i];
                }
            }
        }

        //unary operators
        Vector operator+(){
            return *this;
        }

        Vector operator-(){
            if(this->mData == NULL || this->mSize == 0) return NULL;
            for(int i=1;i<this->mSize+1;i++){
                this->mData[i] *= -1;
            }
            return *this;
        }

        //binary operators
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

