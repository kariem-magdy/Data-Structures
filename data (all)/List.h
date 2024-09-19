#pragma once
template<class T>
class List
{
public:
    virtual bool isEmpty() const = 0;
    virtual int getLength() = 0;
    virtual bool insert(T& newvalue) = 0;
    virtual bool remove(T& value) = 0;
    virtual void clear() = 0;
    virtual ~List() { }
};