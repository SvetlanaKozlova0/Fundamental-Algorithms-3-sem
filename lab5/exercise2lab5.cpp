#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstddef>

typedef enum statusCode {
	NORMAL,
	INPUT_FILE_OPEN_ERROR,
	OUTPUT_FILE_OPEN_ERROR
} statusCode;

typedef enum constants {
	LENGTH_S_BLOCK = 256
} constants;

class Encoder {
   private:
	std::vector<std::byte> key;

	std::vector<std::byte> initSBlock() {
		std::vector<std::byte> S(LENGTH_S_BLOCK);
		for (size_t i = 0; i < LENGTH_S_BLOCK; i++) {
			S[i] = static_cast<std::byte>(i);
		}
		return S;
	}

	void permutateSBlock(std::vector<std::byte>& S) {
		size_t j = 0;
		for (size_t i = 0; i < LENGTH_S_BLOCK; i++) {
			j = (j + std::to_integer<size_t>(S[i]) + std::to_integer<size_t>(key[i % key.size()])) % LENGTH_S_BLOCK;
			std::swap(S[i], S[j]);
		}
	}

	void partEncoding(std::ifstream& input, std::ofstream& output, std::vector<std::byte>& S) {
		size_t i = 0;
		size_t j = 0;
		char current = '0';
		while (input.get(current)) {
			std::byte currentByte = static_cast<std::byte>(current);
			i = (i + 1) % LENGTH_S_BLOCK;
			j = (j + std::to_integer<size_t>(S[i])) % LENGTH_S_BLOCK;
			std::swap(S[i], S[j]);
			size_t t = (std::to_integer<size_t>(S[i]) + std::to_integer<size_t>(S[j])) % LENGTH_S_BLOCK;
			std::byte k = S[t];
			currentByte = currentByte ^ k;
			output.put(static_cast<char>(currentByte));
		}
	}

   public:
	Encoder(const std::vector<std::byte>& currentKey) : key(currentKey) {}

	statusCode encode(const std::string& inputFile, const std::string& outputFile, bool to_encode) {
		std::ifstream input(inputFile, std::ios::binary);
		if (!input.is_open()) return INPUT_FILE_OPEN_ERROR;
		std::ofstream output(outputFile, std::ios::binary);
		if (!output.is_open()) {
			input.close();
			return OUTPUT_FILE_OPEN_ERROR;
		}
		std::vector<std::byte> S = initSBlock();
		permutateSBlock(S);
		partEncoding(input, output, S);
		input.close();
		output.close();
		return NORMAL;
	}

	void keyMutator(const std::vector<std::byte>& newKey) {
		key = newKey;
	}
};

int main() {
	std::vector<std::byte> key = { std::byte(1), std::byte(2), std::byte(3) };
	Encoder encoder(key);
	std::vector<std::byte> key2 = { std::byte(3), std::byte(4), std::byte(3) };
	encoder.keyMutator(key2);

	std::string inputFile = "input.txt";
	std::string outputFile = "output.txt";
	std::string lastFile = "output2.txt";

	statusCode result = encoder.encode(inputFile, outputFile, true);

	if (result == NORMAL) {
		std::cout << "Everything is successful.\n";
	} else if (result == INPUT_FILE_OPEN_ERROR) {
		std::cout << "An error while opening input file.\n";
		return 1;
	} else {
		std::cout << "An error while opening output file.\n";
		return 1;
	}
	result = encoder.encode(outputFile, lastFile, true);
	if (result == NORMAL) {
		std::cout << "Everything is successful.\n";
	} else if (result == INPUT_FILE_OPEN_ERROR) {
		std::cout << "An error while opening input file.\n";
		return 1;
	} else{
		std::cout << "An error while opening output file.\n";
		return 1;
	}

	return 0;
}