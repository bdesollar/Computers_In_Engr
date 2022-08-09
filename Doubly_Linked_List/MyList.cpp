//
// Created by Ben DeSollar on 5/3/2021.
//

#include "MyList.h"
#include "iostream"
#include "Node.h"

using namespace std;



// Initializes currpointer to a nullpointer as the default constructor
MyList::MyList(){
    currPointer = nullptr;
};

Node* MyList::getCurrPointer() const{
    return currPointer;
}

void MyList::setCurrPointer(Node* n){
    currPointer = n;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// Function: Prints the nums of each node in the doubly linked list in ascending order
// Input: None
// Output: None
//
//////////////////////////////////////////////////////////////////////////////////////////

void MyList::printAscending(){
    // checks if the current pointer is a nullpointer
    if(currPointer == nullptr){
        cout << "Empty List" << endl;
        return;
    }
    else {

        while (currPointer->getPrevNode() != nullptr) {
            currPointer = currPointer->getPrevNode();
        }

        while (currPointer->getNextNode() != nullptr) {
            cout << currPointer->getNum() << endl;
            currPointer = currPointer->getNextNode();
        }
    }
    cout << currPointer->getNum() << endl;

}

/////////////////////////////////////////////////////////////////////////////////////////
//
// Function: Prints the nums of each node in the doubly linked list in descending order
// Input: None
// Output: None
//
//////////////////////////////////////////////////////////////////////////////////////////

void MyList::printDescending(){
    if(currPointer == nullptr){
        cout << "Empty List" << endl;
        return;
    }
    else {

        while (currPointer->getNextNode() != nullptr) {
            currPointer = currPointer->getNextNode();
        }

        while (currPointer->getPrevNode() != nullptr) {
            cout << currPointer->getNum() << endl;
            currPointer = currPointer->getPrevNode();
        }

        cout << currPointer->getNum() << endl;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// Function: Inserts a node in the list in ascending order
// Input: val - int - value that will be added to the list
// Output: None
//
//////////////////////////////////////////////////////////////////////////////////////////

void MyList::insert(const int val){
    // Creates a new node
    Node* newNode = new Node(val);
    // If the current pointer is null then the prev node and next node will be made nullptr as well
    if(currPointer == nullptr){
        // Assigns the currpointer to the new node
        currPointer = newNode;
        currPointer->setPrevNode(nullptr);
        currPointer->setNextNode(nullptr);
    }
    else {

        while (currPointer->getPrevNode() != nullptr) {
            currPointer = currPointer->getPrevNode();
        }

        while (currPointer->getNextNode() != nullptr && newNode->getNum() > currPointer->getNum()) {
            currPointer = currPointer->getNextNode();
        }

        if (val < currPointer->getNum() && currPointer->getPrevNode() == nullptr) {
            newNode->setPrevNode(nullptr);
            newNode->setNextNode(currPointer);
            currPointer->setPrevNode(newNode);
        } else if (val < currPointer->getNum() && currPointer->getPrevNode() != nullptr) {
            newNode->setPrevNode(currPointer->getPrevNode());
            newNode->setNextNode(currPointer);
            // Sets the next node
            currPointer->getPrevNode()->setNextNode(newNode);
            currPointer->setPrevNode(newNode);
        }
        else{
            newNode->setPrevNode(currPointer);
            newNode->setNextNode(currPointer->getNextNode());
            currPointer->setNextNode(newNode);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// Function: Removes the value that user puts in from the list
// Input: val - int - value to be removed from the list
// Output: None
//
//////////////////////////////////////////////////////////////////////////////////////////

void MyList::remove(int val){
    Node* nodeRemoved = nullptr;

    if(currPointer == nullptr){
        cout << "Empty List" << endl;
        return;
    }

    while(currPointer->getPrevNode() != nullptr){
        currPointer = currPointer->getPrevNode();
    }

    while(currPointer->getNextNode() != nullptr && currPointer->getNum() < val){
        currPointer = currPointer->getNextNode();
    }

    if(currPointer->getNum() == val){
        nodeRemoved = currPointer;
        if(currPointer->getPrevNode() == nullptr) {
            if (currPointer->getNextNode() == nullptr) {
                if (nodeRemoved != nullptr) {
                    // Removes the node
                    delete nodeRemoved;
                }
                currPointer = nullptr;
                return;
            }
            currPointer = currPointer->getNextNode();
            currPointer->setPrevNode(nullptr);
            if(nodeRemoved != nullptr){
                delete nodeRemoved;
            }
        }
        else if(currPointer->getNextNode() == nullptr){
            currPointer = currPointer->getPrevNode();
            currPointer->setNextNode(nullptr);
            if(nodeRemoved != nullptr){
                delete nodeRemoved;
            }
        }
        else{
            currPointer = currPointer->getPrevNode();
            currPointer->setNextNode(nodeRemoved->getNextNode());
            nodeRemoved->getNextNode()->setPrevNode(currPointer);
            if(nodeRemoved != nullptr){
                delete nodeRemoved;
            }
        }
        cout << "Node = " << val << " deleted" << endl;
    }
    else{
        cout << "Val = " << val << "does not exist in list" << endl;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////
//
// Function: Clears all the nodes from the list
// Input: None
// Output: None
//
//////////////////////////////////////////////////////////////////////////////////////////

void MyList::clear() {
    if(currPointer == nullptr){
        cout << "Empty List" << endl;
    }
    else{
        while(currPointer->getPrevNode() != nullptr){
            currPointer = currPointer->getPrevNode();
        }
        while(currPointer->getNextNode() != nullptr){
            currPointer = currPointer->getNextNode();
            cout << "Deleting " << currPointer->getPrevNode()->getNum() << endl;
            delete currPointer->getPrevNode();
        }

        cout << "Deleting  " << currPointer->getNum() << endl;
        delete currPointer;
        currPointer = nullptr;
    }
}
// MyList destructor
MyList::~MyList(){
    clear();
    cout << "Destructor Called" << endl;
}