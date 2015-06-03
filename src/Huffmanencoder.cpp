#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <inttypes.h>
#include <algorithm>
#include <stdio.h>
#include <cmath>
#include <cstring>
#include "heap.h"
#include "HuffNode.h"

void HuffCreateVector (Node *a, HuffmanSerializedNode v[], uint16_t& index);

void generateCode(HuffmanSerializedNode vec[], codified coded[], uint32_t code, uint16_t length, int index);

uint8_t reverseBits(uint8_t num);

void BitString(unsigned char* text, codified coded[], uint32_t length, FILE *f);

void generateString(FILE *fin, FILE *fdec, HuffmanSerializedNode Hufftree[], int filedim);

int main(int argc, char *argv[])
{
	FILE *fout, *input;
	uint8_t *buffer;
	uint32_t length;
	uint16_t numofelemHuffman = 0;
	std::vector<Node> check;
	std::vector<Node>::iterator it;
	char* outfile; 

	codified coded[256];
	
	Node left, right, top, *Hufftree;
	unsigned int i;
	if (argc != 3)
	{
		std::cout<<"Argumente insuficiente";
		return -1;
	}

	//Partea de compresie
	if (strcmp(argv[1],"-c")==0)
	{
		Node *aux = new Node();
		int *v = new int[256];

		//Crearea numelui fisierului de iesire 
		input = fopen(argv[2], "rb");
		outfile = new char[strlen(argv[2])+ 5];
		//memset(outfile, sizeof(char), strlen(argv[2]) + 5);
		strcpy(outfile,argv[2]);
		strcat(outfile,".cmp");
		fout = fopen(outfile, "wb");

		//Citirea intregului fisier intr-un vector, calcularea frecventelor 
		//fiecarui caracter si depozitarea lor intr-un vector
		fseek(input, 0, SEEK_END);
		length = ftell(input);
		fseek(input, 0, SEEK_SET);
		buffer = new uint8_t[length];
		memset(v, 0, sizeof(int)*256);
		while (fread(buffer, sizeof(uint8_t)* length, 1, input) == 1);
		for (i = 0; i<= length - 1; i++)
		{
			if(v[(int)buffer[i]] == 0)
			{
				aux->symbol = buffer[i];
				check.push_back(*aux);
			}
			v[(int)buffer[i]]++;
		}

		//depozitarea in heap a fiecarui caracter, impreuna cu frecventa si probabilitatea
		Heap<Node> heap(check.size());
		for (it = check.begin(); it != check.end(); ++it)
		{	
			(*it).freq	= v[(int)(*it).symbol];
			(*it).probability = (double)v[(int)(*it).symbol]/length;
			heap.insert(*it);
		}
		delete[] v;
		//partea de extragere din Heap si de constructie a arborelui
		while (heap.getSize() > 1)
		{
			left = heap.extractMin();
			right = heap.extractMin();
			if (left.left == NULL && left.right == NULL && right.left == NULL && right.right == NULL)
			{
				numofelemHuffman += 3;
			}
			else if (left.left == NULL && left.right == NULL)
			{
				numofelemHuffman += 2;
			}
			else if (right.left == NULL && right.right == NULL)
			{
				numofelemHuffman += 2;
			}
			else numofelemHuffman += 1;
			top.freq = left.freq + right.freq;
			top.probability = left.probability + right.probability;
			top.height = std::max(left.height, right.height) + 1;
			top.symbol = std::min(left.symbol, right.symbol);
			top.left = new Node;
			top.right = new Node;
			*top.left = left;
			*top.right = right;
			heap.insert(top);
		}
		Hufftree = new Node;
		*Hufftree = heap.extractMin();

		//Partea de serializare a arborelui Huffman
		HuffmanSerializedNode vec[numofelemHuffman];
		uint16_t index = 0;
		HuffCreateVector(Hufftree, vec, index);

		//generarea codului pentru fiecare caracter si printarea dimensiunilor si arborelui in fisier
		generateCode(vec, coded, 0, 0, 0);
		fwrite(&numofelemHuffman, sizeof(uint16_t), 1, fout);
		fwrite(vec, sizeof(HuffmanSerializedNode), numofelemHuffman, fout);
		fwrite(&length, sizeof(uint32_t), 1, fout);

		//codificarea textului si printarea
		BitString (buffer, coded, length, fout);

		Hufftree->deallocate();
		delete aux;
		delete[] buffer;
		delete[] outfile;
		fclose(input);
		fclose(fout);
	}
	//Partea de decompresie
	else if (strcmp(argv[1], "-d") == 0)
	{
		input = fopen(argv[2], "rb");
		outfile = new char[strlen(argv[2])+ 14];
		strcpy(outfile,"decompressed_");
		strncat(outfile,argv[2], strlen(argv[2]) - 4 );
		fout = fopen(outfile, "wb");
		if (fout == NULL) {
			std::cout<<"fisierul de out nu se deschide";
			return -1;
		}
		//Folosesc fread asa pentru a evita warning-urile de la compilarea cu -O2
		if (!fread(&numofelemHuffman, sizeof(uint16_t), 1, input))
			return -1;
		HuffmanSerializedNode Huffmantree[numofelemHuffman];
		if (!fread(&Huffmantree, sizeof(HuffmanSerializedNode), numofelemHuffman, input))
			return -1;
		if (!fread(&length, sizeof(uint32_t), 1, input))
			return -1;
		generateString (input, fout, Huffmantree, length);
		delete[] outfile;
		fclose(input);
		fclose(fout);
	}
	return 0;
}