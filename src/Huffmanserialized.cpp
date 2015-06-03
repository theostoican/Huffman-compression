#include <iostream>
#include <inttypes.h>

#include "HuffNode.h"



//parcurgere in preordine
void HuffCreateVector (Node *a, HuffmanSerializedNode v[], uint16_t& index)
{
	uint16_t current = index;
	if(a->left == NULL && a->right == NULL)
	{
		v[current].isTerminal = 1;
		v[current].value = a->symbol;
		return;
	}
	else 
	{
		v[current].isTerminal = 0;
	}
	if (a->left != NULL)
	{
		v[current].childData.leftChild = index + 1;
		index += 1;
		HuffCreateVector(a->left, v, index);
	}
	if (a->right != NULL)
	{
		v[current].childData.rightChild = index + 1;
		index += 1;
		HuffCreateVector (a->right, v, index);
	}
}