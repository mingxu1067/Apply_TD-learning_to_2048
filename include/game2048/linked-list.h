#ifndef GAME2048_FRAMEWORK_LINKEDLIST_H
#define GAME2048_FRAMEWORK_LINKEDLIST_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int row;
    int col;
} CheckerboardIndex;

typedef struct LinkedNode {
    CheckerboardIndex index;
    LinkedNode* next;
} LinkedNode;

class LinkedList {
public:
    LinkedList();
    ~LinkedList();

    int size();

    void add(CheckerboardIndex new_cb_index);

    void remove(int index);

    CheckerboardIndex get(int index);

    bool isEmpty();

    void clear();

private:
    LinkedNode* _head;
    LinkedNode* _tail;
    int _length;
    
};

#endif