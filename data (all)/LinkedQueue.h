#pragma once
#include "Node.h"
#include "QueueADT.h"
#include <vector>
using namespace std;


template <typename T>
class LinkedQueue :public QueueADT<T>
{
private:
	int length;
	Node<T>* backPtr;
	Node<T>* frontPtr;
public:
	LinkedQueue();
	bool isEmpty() const;
	bool enqueue(const T& newEntry);
	bool dequeue(T& frntEntry);
	bool peek(T& frntEntry)  const;
	int getsize();
	void print();
	~LinkedQueue();
};
template <typename T>
LinkedQueue<T>::LinkedQueue()
{
	length = 0;
	backPtr = nullptr;
	frontPtr = nullptr;
}
template <typename T>
bool LinkedQueue<T>::isEmpty() const
{
	return (frontPtr == nullptr);
}
template <typename T>
bool LinkedQueue<T>::enqueue(const T& newEntry)
{
	Node<T>* newNodePtr = new Node<T>(newEntry);
	// Insert the new node
	if (isEmpty())	//special case if this is the first node to insert
		frontPtr = newNodePtr; // The queue is empty
	else
		backPtr->setNext(newNodePtr); // The queue was not empty

	backPtr = newNodePtr; // New node is the last node now
	length++;
	return true;
}
template <typename T>
bool LinkedQueue<T>:: dequeue(T& value) {
	if (frontPtr == nullptr)
		return false;

	// Store previous front and
	// move front one node ahead
	Node<T>* temp = frontPtr;
	value = frontPtr->getItem();
	frontPtr = frontPtr->getNext();

	// If front becomes NULL, then
	// change rear also as NULL
	if (frontPtr == nullptr)
		backPtr = nullptr;

	delete (temp);
	length--;
	return true;
}

	
template <typename T>
bool LinkedQueue<T>::peek(T& frntEntry) const
{
	if (isEmpty())
		return false;

	frntEntry = frontPtr->getItem();
	return true;

}
template <typename T>
int LinkedQueue<T>::getsize() {
	return length;
}
template <typename T>
void LinkedQueue<T>::print() {
	if (isEmpty()) {
		return;
	}
	Node<T>* curr = frontPtr;
	while (curr != nullptr) {
		std::cout << curr->getItem() << " ";
		curr = curr->getNext();
	}
}
template <typename T>
LinkedQueue<T>::~LinkedQueue()
{
	T temp;

	//Free (Dequeue) all nodes in the queue
	while (dequeue(temp));
}