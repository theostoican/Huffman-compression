#ifndef HUFFNODE_H
#define HUFFNODE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <inttypes.h>
#include <algorithm>
#include <stdio.h>
#include <cmath>
#include <cstring>

struct HuffmanSerializedNode{
	uint8_t isTerminal;
	union{
		uint8_t value;
		struct{
			uint16_t leftChild;
			uint16_t rightChild;
		} __attribute__((__packed__)) childData;
	}__attribute__((__packed__));

//constructor pentru a initializa vecinii
//util in cazul in care nodul e terminal
	HuffmanSerializedNode()
	{
		childData.leftChild = 0;
		childData.rightChild = 0;
	}
}__attribute__((__packed__));

//structura in care retin codificarea si lungimea unui caracter
struct codified 
{
    uint32_t code;
    int length;
    codified()
    {
        code = 0;
        length = 0;
    }
};

struct Node
{
    int freq;
    double probability;
    uint8_t symbol;
    int height;
    Node *left;
    Node *right;
    friend std::ostream& operator<<(std::ostream&, const Node&);
    Node()
    {
        freq = 0;
        probability = 0;
        height = 0;
        left = NULL;
        right = NULL;
        symbol = 0;
    }
    ~Node()
    {
    }
    //Functie de dealocare, pe care o aplic doar pe ultimul nod din Heap(adica arborele Huffmann)
    //fiindca folosesc mult shallow copy si nu pot utiliza destructorul
    void deallocate()
    {
        if (left != NULL)
        {
            this->left->deallocate();
        }
        if (right != NULL)
            this->right->deallocate();
        delete this;
    }
    //copy constructor
    Node(const Node& obj)
    {
        freq = obj.freq;
        probability = obj.probability;
        symbol = obj.symbol;
        height = obj.height;
        left = obj.left;
        right = obj.right;
    }
    bool operator>(Node a)
    {
        if (this->freq > a.freq)
            return true;
        else
            return false;
    }
    bool operator<(Node a)
    {
        if (this->freq < a.freq)
            return true;
        else
            return false;
    }
    bool operator==(Node a)
    {
        if (this->freq == a.freq)
            return true;
        else 
            return false;
    }
    //assignment, pentru rule of three
    void operator=(Node a)
    {
        this->probability = a.probability;
        this->symbol = a.symbol;
        this->freq = a.freq;
        this->right = a.right;
        this->left = a.left;
        this->height = a.height;
    }
    int getHeight()
    {
        int hleft = 0, hright = 0;
        if (left != NULL)
            hleft = left->getHeight();
        if (right != NULL)
            hright = right->getHeight();
        return 1 + (hleft < hright ? hright : hleft);
    }  
    void display()
    {
        if(left == NULL && right == NULL)
            std::cout<<(char)symbol<<" ";
        if (left != NULL)
            left->display();
        if (right != NULL)
            right->display();
    }
};

inline std::ostream& operator<<(std::ostream& stream, const Node& node)
{
    std::cout<<node.symbol;
    return stream;
}
 
#endif