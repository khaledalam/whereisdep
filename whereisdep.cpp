/**
 * Author: Khaled Alam <khaledalam.net@gmail.com>
 * Created: 22nd Jan 2023
 * 
 * Description: C++ CLI application to search package.json dependencies in given directories.
*/

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include "json.hpp"
#include <getopt.h>

using json = nlohmann::json;

using namespace std;

void parsePackageFile(const string &packageFilePath){
    try {
        ifstream file(packageFilePath);

        if (!file.is_open()) {
            cerr << "Error: Unable to open package.json file" << endl;
            exit(1);
        }

        // Read the contents of the file into a JSON object
        json packageJson;
        file >> packageJson;

        // Extract information from the JSON object
        string name = packageJson["name"];
        string version = packageJson["version"];
        string description = packageJson["description"];

        // Print the extracted information
        cout << "Package Name: " << name << endl;
        cout << "Version: " << version << endl;
        cout << "Description: " << description << endl;

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        exit(1);
    }
}


void parseOptions(int &argc, char* argv[], string &packageJsonFilePath, string &searchDir)
{
    const char* short_options = "hp:s:";
    const option long_options[] = {
        {"help", no_argument, nullptr, 'h'},
        {"packageJsonFilePath", required_argument, nullptr, 'p'},
        {"searchDir", required_argument, nullptr, 's'},
        {nullptr, 0, nullptr, 0}
    };

    int option;

    while ((option = getopt_long(argc, argv, short_options, long_options, nullptr)) != -1) {
        switch (option) {
            case 'h':
                cout << "Help message..." << endl;
                exit(1);
            case 'p':
                packageJsonFilePath = optarg;
                break;
            case 's':
                searchDir = optarg;
                break;
            default:
                return;
        }
    }

    if (packageJsonFilePath.empty()) {
        cout << "package.json file path is required." << endl;
        exit(1);
    }

    if (searchDir.empty()) {
        cout << "Search Files Path is required." << endl;
        exit(1);
    }
}

int main(int argc, char* argv[]) {


    string packageJsonFilePath, searchDir;

    parseOptions(argc, argv, packageJsonFilePath, searchDir);

    parsePackageFile(packageJsonFilePath);
    

    cout<< packageJsonFilePath<<endl;

  

    return 0;
}