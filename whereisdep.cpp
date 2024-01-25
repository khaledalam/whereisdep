/**
 * Author: Khaled Alam <khaledalam.net@gmail.com>
 * Created: 22nd Jan 2023
 *
 * Description: C++ CLI application to search package.json dependencies in given directories.
 *
 * Version: 1.0.0
 */

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <map>
#include "json.hpp"
#include <getopt.h>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

using json = nlohmann::json;

string packageJsonFilePath, searchDir, extentions;
const string version = "1.0.0";

json packageJson;
struct FileLine {
	int number;
	string content;
	string path;
};

map<string, vector<FileLine>> targetFilesPaths;

// Filter JavaScript Files
string exts[] = { ".js", ".jsx" };

void parsePackageFile(const string &packageFilePath) {
	try {
		ifstream file(packageFilePath);

		if (!file.is_open()) {
			cerr << "Error: Unable to open package.json file" << endl;
			exit(1);
		}

		// Read the contents of the file into a JSON object
		file >> packageJson;

	} catch (const exception &e) {
		cerr << "Error: " << e.what() << endl;
		exit(1);
	}
}

void helpMessage() {

	cout
			<< "whereisdep\nC++ CLI application to search package.json dependencies in given directories.\n\nAllowed options:\n\n"
			<< "-h | --help\tDisplay this help menu\n"
			<< "-p | --packageJsonFilePath\tSet the path of the package.json file\n"
			<< "-v | --version\tGet the version of the tool\n"
			<< "-s | --searchDir\tSet the searching directory of JS files\n"
			<< "-e | --extensions\tSet the target search files extensions\n\n";
}

void parseOptions(int &argc, char *argv[], string &packageJsonFilePath,
		string &searchDir) {
	const char *short_options = "hvp:s:e:";
	const option long_options[] = { { "help", no_argument, nullptr, 'h' }, {
			"version", no_argument, nullptr, 'v' }, { "packageJsonFilePath",
			required_argument, nullptr, 'p' }, { "searchDir", required_argument,
			nullptr, 's' }, { "extensions", required_argument, nullptr, 'e' }, {
			nullptr, 0, nullptr, 0 } };

	int option;

	while ((option = getopt_long(argc, argv, short_options, long_options,
			nullptr)) != -1) {
		switch (option) {
		case 'v':
			cout << "whereisdep\nVersion: " << version << "\n";
			exit(0);
		case 'e':
			extentions = optarg;
			exit(1);
		case 'p':
			packageJsonFilePath = optarg;
			break;
		case 's':
			searchDir = optarg;
			break;
		default:
		case 'h':
			helpMessage();
			exit(0);
		}
	}

	if (packageJsonFilePath.empty()) {
		cout << "package.json file path is required." << endl;
		exit(1);
	}

	if (searchDir.empty()) {
		cout << "Searching Path is required." << endl;
		exit(1);
	}
}

void searchPackageUsage(const string &text, ifstream &file) {
	string line;
	int lineNumber = 0;

	while (getline(file, line)) {
		lineNumber++;
		size_t pos = line.find(text);
		if (pos != string::npos) {
			cout << "Found in line " << lineNumber << ": " << line << endl;
		}
	}
}

void coloringSubString(string &original, string &subString) {

	int findValue = original.find(subString);

	for (int i = 0; i < (int) original.size(); i++) {
		if (i >= findValue && i < findValue + subString.length()) {
			cout << "\x1b[42m";
		} else {
			cout << "\x1b[0m";
		}
		cout << original[i];

	}

}

string trimStringView(string str) {
	auto start = str.find_first_not_of(" \t\n\r\f\v");
	if (start == string::npos) {
		return "";
	}

	auto end = str.find_last_not_of(" \t\n\r\f\v");
	return str.substr(start, end - start + 1);
}

string readFileContent(const string &filePath) {
	ifstream inputFile(filePath);
	if (!inputFile.is_open()) {
		return "";
	}

	stringstream buffer;
	buffer << inputFile.rdbuf();
	inputFile.close();

	return buffer.str();
}

void getTargetedFilesByExt(const filesystem::path &directoryPath,
		const string extensions[]) {
	for (const auto &entry : filesystem::recursive_directory_iterator(
			directoryPath)) {
		bool extWanted = find(extensions, extensions + extensions->size(),
				entry.path().extension()) != extensions + extensions->size();

		if (entry.is_regular_file() && extWanted) {
			vector<FileLine> lines;
			FileLine line;
			line.content = readFileContent(entry.path());
			line.path = entry.path();
			istringstream iss(line.content);
			int lineCount = 1;
			while (lineCount++ && getline(iss, line.content)) {
				line.number = lineCount;
				lines.push_back(line);
			}
			targetFilesPaths[entry.path()] = lines;
		}
	}
}

void processPackages(const string &key) {
	int founds = 0;
	try {
		for (auto &item : packageJson[key].items()) {
			for (pair<const string, vector<FileLine>> &file : targetFilesPaths) {
				// file.second => lines of content vector
				bool fileOk = true;

				for (FileLine &line : file.second) {
					auto findValue = line.content.find(item.key());

					if (findValue != string::npos) {
						cout << "Package \"" << item.key()
								<< "\" Found at file: " << line.path << endl;
						cout << "Content: ";

						string trimedContent = trimStringView(line.content);
						string key = item.key();

						coloringSubString(trimedContent, key);

						cout << "Line: " << line.number << endl
								<< "---------\n";

						fileOk = false;
						founds++;
						break;
					}
				}

				if (!fileOk) {
					break;
				}
			}
		}
	} catch (const exception e) {

	}

	cout << "\n[" << founds << "] used packages founds!\n\n";
}

int main(int argc, char *argv[]) {
	parseOptions(argc, argv, packageJsonFilePath, searchDir);

	parsePackageFile(packageJsonFilePath);

	getTargetedFilesByExt(searchDir, exts);

	vector < string > keys = { "dependencies", "devDependencies" };

	for (auto &key : keys) {
		cout << key << ":\n";
		processPackages(key);
	}

	return 0;
}
