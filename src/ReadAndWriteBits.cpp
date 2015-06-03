#include <iostream>
#include <inttypes.h>


#include "HuffNode.h"

//Functie care genereaza codul corespunzator fiecarui caracter
void generateCode(HuffmanSerializedNode vec[], codified coded[], uint32_t code, uint16_t length, int index)
{
	if (vec[index].isTerminal == 1)
	{
		coded[(int)vec[index].value].code = code;
		if (length == 0)
			coded[(int)vec[index].value].length = 1;
		else
			coded[(int)vec[index].value].length = length;
	}
	else
	{
		generateCode(vec, coded, code<<1, length + 1, vec[index].childData.leftChild);
		generateCode(vec, coded, code<<1 | 1, length + 1, vec[index].childData.rightChild);
	}
}

//Functia care inverseaza bitii dintr-un octet de la dreapta la stanga
uint8_t reverseBits(uint8_t num)
{
    unsigned int count = sizeof(num) * 8 - 1;
    uint8_t reverse_num = num;
     
    num >>= 1;
    while(num)
    {
       reverse_num <<= 1;      
       reverse_num |= num & 1;
       num >>= 1;
       count--;
    }
    reverse_num <<= count;
    return reverse_num;
}

//Functia care codifica textul conform codurilor gasite cu functia generateCode
void BitString(unsigned char* text, codified coded[], uint32_t length, FILE *f)
{
	uint8_t aux = 0, rev,k;
	unsigned int i;
	int count = 0, j, notcomplete = 1;
	for (i = 0; i < length; i++)
	{
		for (j = coded[(int)text[i]].length - 1; j >= 0; j--)
		{
			//k va reprezenta, de fapt, un bit de 1 sau 0 pe care il adaug la sfarsitul octetului aux
			//parcurgand fiecare bit din codificarea aferenta unui caracter
			k = (coded[(int)text[i]].code >> j) & 1;
			aux = (aux<<1) + k;
			++count;
			if (count == 8)
			{
				//trebuie sa inversez ordinea bitilor in octet, pentru a-i putea printa
				//in fisier, conform codificarii Huffman
				rev = reverseBits(aux);
				//pornesc cu urmatorul octet
				count = 0;
				fwrite(&rev, sizeof(uint8_t), 1, f);
			}
		}
	}
	//Cazul in care s-a terminat textul si trebuie sa fac padding
	if (count != 8)
		notcomplete = 0;
	while (count < 8)
	{
		aux = aux<<1;
		++count;
	}
	if (!notcomplete)
	{
		rev = reverseBits(aux);
		fwrite(&rev, sizeof(uint8_t), 1, f);
	}
}

//Functie care genereaza textul din codificarea Huffman, utilizata
//la pasul de decompresie
void generateString(FILE *fin, FILE *fdec, HuffmanSerializedNode Hufftree[], int filedim)
{
	uint8_t symbol;
	int i, numprinted = 0, direction, index = 0;
	while (fread(&symbol, sizeof(uint8_t), 1, fin) == 1)
	{
		for (i = 0; i <= 7; i++)
		{
			if (Hufftree[index].isTerminal == 1)
			{
				fwrite(&Hufftree[index].value, sizeof(uint8_t), 1, fdec);
				++numprinted;
				if (numprinted == filedim)
					break;
				index = 0;
			}
				direction = (symbol >> i) & 1;
				if (direction == 0)
				{
					index = Hufftree[index].childData.leftChild;
				}
				else if (direction == 1)
				{
					index = Hufftree[index].childData.rightChild;
				}
		}
	}
} 
