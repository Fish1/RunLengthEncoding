#include <iostream>

#include <string>

#include <fstream>

void decompress(std::string name);

void compress(std::string name);

void createFile(std::string name, unsigned int bytes);

void error(std::string message, int code);

int main(int argc, char ** argv)
{
	srand(time(NULL));

	if(argc != 3)
	{
		error("incorrect argument count", 1);
	}

	if(std::string(argv[1]).compare("-c") == 0)
	{
		compress(argv[2]);
	}
	else if(std::string(argv[1]).compare("-d") == 0)
	{
		decompress(argv[2]);
	}
	else if(std::string(argv[1]).compare("-cf") == 0)
	{
		createFile(argv[2], 100000);
	}
	else
	{
		error("incorrect flags", 2);
	}

	return 0;
}

void decompress(std::string name)
{
	std::cout << "decompressing file " << name << "..." << std::endl;
	
	std::ifstream input(name, std::ifstream::binary);
	input.seekg(0, std::ifstream::end);
	unsigned int size = input.tellg();
	input.seekg(0, std::ifstream::beg);

	char data[size + 1];
	input.read(data, size);

	std::ofstream output(name + "_decompressed", std::ofstream::binary);

	for(unsigned int index = 0; index < size + 1; ++index)
	{
		uint16_t count = (data[index] << 8) | data[index + 1];
		uint8_t byte = data[index + 2];

		index += 2;

		std::cout << count << std::endl;

		for(unsigned int byteCount = 0; byteCount < count; ++byteCount)
		{
			output.write((char*)&byte, sizeof(byte));
		}
	}

	input.close();
	output.close();
}

void compress(std::string name)
{
	std::cout << "compressing file " << name << "..." << std::endl;

	std::ifstream input(name, std::ifstream::binary);
	input.seekg(0, std::ifstream::end);
	unsigned int size = input.tellg();
	input.seekg(0, std::ifstream::beg);

	uint8_t data[size + 1];
	input.read((char*)data, size);

	std::ofstream output(
	std::string(name) + "_compressed", std::ofstream::binary);

	uint8_t byte = data[0];
	uint8_t last_byte = data[0];
	uint16_t count = 0;

	for(unsigned int index = 0; index < size + 1; ++index)
	{
		byte = data[index];

		if(byte != last_byte)
		{
			output.write((char*)&count, sizeof(uint16_t));
			output.write((char*)&last_byte, sizeof(uint8_t));
			count = 0;
		}

		last_byte = byte;
		++count;
	}

	output << count;
	output << last_byte;

	input.close();
	output.close();
}

void createFile(std::string name, unsigned int bytes)
{
	std::ofstream output(name);

	uint8_t c = 32 + (rand() % 94);

	for(unsigned int index = 0; index < bytes; ++index)
	{
		if(index > 10 && (rand() % 5) == 0)
		{
			c = 32 + (rand() % 94);
		}

		output.write((char*)&c, sizeof(c));
	}

	output.close();
}

void error(std::string message, int code)
{
	std::cout << "Error: " << message << std::endl;

	exit(code);
}
