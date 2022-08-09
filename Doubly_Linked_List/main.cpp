// Programmer: Ben DeSollar
// Date: 5/2/2021
// Filename: main.cpp
// Description: The program implements a doubly linked list class of integers in two classes
//      called Node and MyList


#include <iostream>
using namespace std;
#include "Node.h"
#include "MyList.h"


int main() {

    // Test for Node getters and setters; Also tests the print function
    Node n1;
    n1.setNum(3);
    cout << "Test should print: Integer Stored in Node = 3" << endl;
    n1.print();
    cout << "\n\n";

    // Test Constructor, and setter for the current node in MyList
    cout << "Test Constructor, and setter for the current node in MyList" << endl;
    MyList test;
    Node testNode2;
    testNode2.setNum(2);
    test.setCurrPointer(&testNode2);
    test.printAscending();
    cout << "\n\n";

    //  Test print ascending, and insert
    cout << "Test print ascending and descending" << endl;
    MyList m1;
    m1.insert(1);
    m1.insert(2);
    m1.insert(3);
    m1.insert(4);
    m1.printAscending();
    cout << "\n\n";
    m1.printDescending();
    cout << "\n\n";

    // Test Remove
    cout << "Test Remove" << endl;
    m1.remove(3);
    m1.remove(2);
    m1.printAscending();
    cout << "\n\n";

    // Test clear
    cout << "Test clear" << endl;
    m1.clear();
    cout << "\n\n";

    return 0;
}
