//
// Created by wdbob on 5/3/2021.
//

#ifndef HW8_MYLIST_H
#define HW8_MYLIST_H
#include "iostream"
#include "Node.h"

using namespace std;

class MyList {

public:
    MyList();

    Node* getCurrPointer() const;

    void setCurrPointer(Node* n);

    void printAscending();


    void printDescending();

    void insert(const int val);

    void remove(int val);

    void clear();

    ~MyList();

private:
    Node* currPointer;
};

#endif //HW8_MYLIST_H
