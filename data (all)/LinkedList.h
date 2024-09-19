#pragma once
#include <iostream>
using namespace std;
template <typename T>
class node {
public:
    T data;
    node* next;

    node(T value) {
        data = value;
        next = nullptr;
    }
};

template <typename T>
class LinkedList
{
private:
    node<T>* head;
    int length;
    node<T>* tail;

public:
    LinkedList() {
        head = nullptr;
       tail = nullptr;
        length = 0;
    }

    bool isEmpty() {
        return (head == nullptr);
    }

    int getsize() {
        return length;
    }


    bool insertend(T& value) {
        // Create a new node to hold the value
        node<T>* newNode = new node<T>(value);
        newNode->data = value;
        newNode->next = nullptr;

        // If the list is empty, set the head and tail to the new node
        if (head == nullptr) {
            head = tail = newNode;
            length++;
            return true;
        }

        // Insert the new node at the end
        tail->next = newNode;
        tail = newNode;
        length++;

        return true;
    }
    bool deletefirst(T&value) {
        // If the list is empty, return false
        if (head == nullptr) {
            value = nullptr;
            return false;
        }

        // Remove the first node
        node<T>* temp = head;
        value = head->data;
        head = head->next;
        delete temp;
        length--;

        // If the list is now empty, update the tail as well
        if (head == nullptr) {
            tail = nullptr;
        }

        return true;
    }
    bool remove(T& value) {
        // If the list is empty, return false
        if (head == nullptr) {
            return false;
        }

        // If the first node contains the value, remove it and update the head
        if (head->data == value) {
            node<T>* temp = head;
            head = head->next;
            delete temp;
            length--;

            // If the list is now empty, update the tail as well
            if (head == nullptr) {
                tail = nullptr;
            }

            return true;
        }

        // Traverse the list to find the node that contains the value
        node<T>* current = head;
        while (current->next != nullptr) {
            if (current->next->data == value) {
                node<T>* temp = current->next;
                current->next = temp->next;
                delete temp;
                length--;

                // If the last node was removed, update the tail
                if (current->next == nullptr) {
                    tail = current;
                }

                return true;
            }
            current = current->next;
        }

        // The value was not found in the list
        return false;
    }
    

    void print() {
        if (isEmpty()) {
            return;
        }
        node<T>* curr = head;
        while (curr != nullptr) {
            cout << curr->data << " ";
            curr = curr->next;
        }
    }

 

   T getpointer(int i) {
       node<T>* temp = head;
       int n = 1;
       while (n < i && n<length) {
           if (temp && temp->next != NULL) {
               temp = temp->next;
               n++;
           }
       }
       if (temp != nullptr) {
           return temp->data;
       }
       else return nullptr;
   }

    
};