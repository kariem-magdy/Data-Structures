#pragma once
#include <iostream>

template<typename T>
class PriorityQueue{
private:
    struct Node {
        T data;
        float priority;
        Node* next;
    };

    Node* head;
    int length;
public:
    PriorityQueue() : head(nullptr) {}

    bool isEmpty() const {
        return head == nullptr;
    }

    bool peek(T& value) const {
        if (isEmpty()) {
            return false;
        }
        value = head->data;
        return true;
    }

    bool enqueue(const T& value, float priority) {
        Node* newNode = new Node{ value, priority, nullptr };

        if (isEmpty() || priority > head->priority) {
            newNode->next = head;
            head = newNode;
            length++;
            return true;
        }

        Node* current = head;
        while (current->next != nullptr && current->next->priority >= priority) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
        length++;
        return true;
    }

    bool dequeue(T& value) {
        if (isEmpty()) {
            return false;
        }

        Node* temp = head;
        value = head->data;
        head = head->next;
        delete temp;
        length--;
        return true;
    }
    int getsize() {
        return length;
    }
    void print() {
        if (isEmpty()) {
            return;
        }
        Node* curr = head;
        while (curr != nullptr) {
            std::cout << curr-> data << " ";
            curr = curr->next;
        }
    }
    ~PriorityQueue() {
        T temp;

        //Free (Dequeue) all nodes in the queue
        while (dequeue(temp));
    }
};
