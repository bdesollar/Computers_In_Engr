//
// Created by wdbob on 5/3/2021.
//

#ifndef HW8_NODE_H
#define HW8_NODE_H

#include <iostream>


using namespace std;

class Node {
public:
    explicit Node(int num1 = 0, Node* prevNode1 = nullptr, Node* nextNode1 = nullptr){
        num = num1;
        prevNode = prevNode1;
        nextNode = nextNode1;
    };

    void setNum(int number);

    int getNum() const;

    void setNextNode(Node* nxtNode);

    Node* getNextNode() const;

    void setPrevNode(Node* previousNode);

    Node* getPrevNode() const;

    void print() const;

private:
    int num = 0;
    Node* prevNode;
    Node* nextNode;

};


#endif //HW8_NODE_H
