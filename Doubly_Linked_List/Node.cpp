//
// Created by Ben DeSollar on 5/3/2021.
//
#include "Node.h"

/////////////////////////////////////////////////////
//
// Function: Sets the num in the Node
// Input: number - int - number to be set in node
// Output: None
//
//////////////////////////////////////////////////////

void Node::setNum(int number){
    num = number;
}

/////////////////////////////////////////////////////
//
// Function: Gets the num in the Node
// Input: None
// Output: number - int - number to be set in node
//
//////////////////////////////////////////////////////

int Node::getNum() const {
    return num;
}

void Node::setNextNode(Node *nxtNode){
    nextNode = nxtNode;
}

Node* Node::getNextNode() const{
    return nextNode;
}

void Node::setPrevNode(Node *previousNode){
    prevNode = previousNode;
}

Node* Node::getPrevNode() const{
    return prevNode;
}

void Node::print() const {
// Prints number in node
    cout << "Integer Stored in Node = " << num << endl;
}



