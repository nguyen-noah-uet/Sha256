#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include "sha256.h"
namespace fs = std::filesystem;

using std::cout; using std::cin;
using std::endl; using std::string;
int main(int argc, char** argv) {

	// Encrypt
	auto inputFilePath = fs::current_path();
	auto outputFilePath = fs::current_path();
	inputFilePath.append("UnencryptedFiles");
	outputFilePath.append("EncryptedFiles");
	try {
		cout << "Encrypting..." << endl;
		for (const auto& entry : fs::directory_iterator(inputFilePath))
		{
			string text;
			std::ifstream MyReadFile;
			MyReadFile.open(entry.path());
			string line;
			while (getline(MyReadFile, line)) {
				text += line + "\n";
			}
			text.pop_back();
			MyReadFile.close();
			fs::path p(entry.path());
			std::ofstream MyWriteFile;
			MyWriteFile.open(outputFilePath.append(p.filename().string()));
			SHA256 sha;
			MyWriteFile << sha(text) << endl;
			outputFilePath = outputFilePath.parent_path();
			MyWriteFile.close();
			text.clear();
		}
		cout << "Encrypt successfully." << endl;
	} catch (std::exception &e)
	{
		std::cerr << "An error occurred while trying to encrypt files." << endl;
		return -1;
	}


	// Search
	string searchTerm;
	while (true)
	{
		cout << "Type hashcode you want to find (type 'exit' to quit): " << endl;
		std::getline(cin, searchTerm);
		if (searchTerm == "exit")
		{
			cout << "Exited." << endl;
			return 0;
		}
		int found = 0;
		for (const auto& entry : fs::directory_iterator(outputFilePath))
		{
			string text;
			std::ifstream MyReadFile;
			MyReadFile.open(entry.path());
			string line;
			while (getline(MyReadFile, line)) {
				text += line + "\n";
			}
			text.pop_back();
			if(text.find(searchTerm) != string::npos)
			{
				cout << "Found \'" + entry.path().filename().string() + "\' contains the hashcode." << endl;
				found++;
			}
			text.clear();
			MyReadFile.close();
		}
		if(found == 0)
		{
			cout << "There is no file containing this hashcode." << endl;
		}
	}
}