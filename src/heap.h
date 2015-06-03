#ifndef HEAP_H
#define HEAP_H

#include <iostream>
#include <algorithm>
#include <cmath>

template <typename T>
class Heap
{ 
private:
    T* values;
    int dimVect;
    int capVect;
public:
    Heap(int capVect);
    ~Heap();

    bool compare (int poz, int pozParent);
 
    int parent(int poz);
 
    int leftSubtree(int poz);
 
    int rightSubtree(int poz);
 
    void pushUp(int poz);
 
    void pushDown(int poz);
 
    void insert(T x);

    int getHeight(int);

    T peek();
 
    T extractMin();

    void display();

    int getSize();
};

template <typename T>
Heap<T>::Heap(int capVect)
{
    values = new T[capVect];
    memset(values, 0, sizeof(T)*capVect);
    this->capVect = capVect;
    this->dimVect = 0;
}

template <typename T>
Heap<T>::~Heap()
{
    delete[] values;
}

template <typename T>
int Heap<T>::parent(int poz)
{
    if (poz < 1) return -1;
    return (poz - 1)/2;
}

template <typename T>
int Heap<T>::leftSubtree(int poz)
{
    if (2 * poz + 1 >= dimVect) return -1;
    else return 2 * poz + 1; 
}

template <typename T>
int Heap<T>::rightSubtree (int poz)
{
    if (2 * poz + 2 >= dimVect) 
        return -1;
    else 
        return 2 * poz + 2;
}

template <typename T>
bool Heap<T>::compare(int poz, int pozParent)
{
    if (values[poz] < values[pozParent])
        return true;
    else if (values[poz] == values[pozParent] && values[poz].height < values[pozParent].height)
        return true;
    else if (values[poz] == values[pozParent] && values[poz].height == values[pozParent].height && \
        values[poz].symbol < values[pozParent].symbol)
        return true;
    else 
        return false;
}

template <typename T>
void Heap<T>::pushUp(int poz)
{
    int pozParent = parent(poz);
    if (pozParent != -1 && compare(poz, pozParent) == true)
    {
        T aux = values[poz];
        values[poz] = values[pozParent];
        values[pozParent] = aux;
        pushUp(pozParent);
    }
}

//Final form, caused severe brain damage at debugging...
template<typename T>
void Heap<T>::pushDown(int poz)
{
    int left = leftSubtree(poz);
    int right = rightSubtree(poz);
    T aux;
    if (left != -1 && right != -1)
    {
        if (compare(left,right) == true)
        {
            if (compare(left, poz) == true)
            {
                aux = values[left];
                values[left] = values[poz];
                values[poz] = aux;
                pushDown(left);
            }
        }
        else
        {
            if (compare(right, poz) == true)
            {
                aux = values[right];
                values[right] = values[poz];
                values[poz] = aux;
                pushDown(right);
            }
        }
    }
    else if (left != -1)
    {
        if (compare(left, poz) == true)
        {
            aux = values[left];
            values[left] = values[poz];
            values[poz] = aux;
            pushDown(left);
        }
    }
    else if(right != -1)
    {
        if (compare(right, poz) == true)
        {
            aux = values[right];
            values[right] = values[poz];
            values[poz] = aux;
            pushDown(right);
        }
    }
}

template <typename T>
int Heap<T>::getHeight(int poz)
{
    int auxpoz = (poz - 1)/2;
    if (poz < 1) 
        return 0;
    else
        return 1 + getHeight(auxpoz);
}

template <typename T>
void Heap<T>::insert(T x)
{
    if (capVect == dimVect) 
    {
        std::cout<<"full heap"<<std::endl;
        return;
    }
    values[dimVect] = x;
    ++dimVect;
    pushUp(dimVect - 1);
}

template <typename T>
T Heap<T>::peek()
{
    if (dimVect >= 1)
        return values[0];
    else 
    {
        std::cout<<"empty heap"<<std::endl;
        return -1;
    }
}

template <typename T>
T Heap<T>::extractMin()
{
    if (dimVect >= 1)
    {
        T aux;
        aux = values[dimVect - 1];
        values[dimVect - 1] = values[0];
        values[0] = aux;
        --dimVect;
        pushDown(0);
        return values[dimVect];
    }
    else
    {
        std::cout<<"empty heap"<<std::endl;
        return T();
    }
}

template <typename T>
void Heap<T>::display()
{
    int poz = 0;
    while (poz <= dimVect - 1)
    {
        std::cout<<values[poz]<<" ";
        ++poz;
    }
    std::cout<<std::endl;
}

template <typename T>
int Heap<T>::getSize()
{
    return dimVect;
}

#endif // HEAP_H
