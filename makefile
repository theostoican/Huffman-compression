build: Huffmanencoder.cpp Huffmanserialized.cpp ReadAndWriteBits.cpp 
	g++ -Wall -O2 Huffmanencoder.cpp Huffmanserialized.cpp ReadAndWriteBits.cpp -o comp
clean: comp
	rm -f comp
