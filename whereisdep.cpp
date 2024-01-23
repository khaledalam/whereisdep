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
#include <map>
#include "json.hpp"
#include <getopt.h>
#include <sstream>


using namespace std;

using json = nlohmann::json;


string packageJsonFilePath, searchDir;
json packageJson;
struct FileLine {
    int number;
    string content;
    string path;
};

map<string, vector<FileLine>> targetFilesPaths;

// Filter JavaScript Files
string exts[] = {".js", ".jsx"};

void parsePackageFile(const string &packageFilePath)
{
    try
    {
        ifstream file(packageFilePath);

        if (!file.is_open())
        {
            cerr << "Error: Unable to open package.json file" << endl;
            exit(1);
        }

        // Read the contents of the file into a JSON object
        file >> packageJson;

        // Extract information from the JSON object
        string name = packageJson["name"];
        string version = packageJson["version"];
        string description = packageJson["description"];

        // Print the extracted information
        cout << "Package Name: " << name << endl;
        cout << "Version: " << version << endl;
        cout << "Description: " << description << endl;
    }
    catch (const exception &e)
    {
        cerr << "Error: " << e.what() << endl;
        exit(1);
    }
}

void parseOptions(int &argc, char *argv[], string &packageJsonFilePath, string &searchDir)
{
    const char *short_options = "hp:s:";
    const option long_options[] = {
        {"help", no_argument, nullptr, 'h'},
        {"packageJsonFilePath", required_argument, nullptr, 'p'},
        {"searchDir", required_argument, nullptr, 's'},
        {nullptr, 0, nullptr, 0}};

    int option;

    while ((option = getopt_long(argc, argv, short_options, long_options, nullptr)) != -1)
    {
        switch (option)
        {
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

    if (packageJsonFilePath.empty())
    {
        cout << "package.json file path is required." << endl;
        exit(1);
    }

    if (searchDir.empty())
    {
        cout << "Search Files Path is required." << endl;
        exit(1);
    }
}

void searchPackageUsage(const string &text, ifstream &file)
{
    string line;
    int lineNumber = 0;

    while (getline(file, line))
    {
        lineNumber++;
        size_t pos = line.find(text);
        if (pos != string::npos)
        {
            cout << "Found in line " << lineNumber << ": " << line << endl;
        }
    }
}

string_view trimStringView(string_view str) {
    auto start = str.find_first_not_of(" \t\n\r\f\v");
    if (start == string_view::npos) {
        return "";
    }

    auto end = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(start, end - start + 1);
}

string readFileContent(const string& filePath) {
    ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        return "";
    }

    stringstream buffer;
    buffer << inputFile.rdbuf(); 
    inputFile.close(); 

    return buffer.str(); 
}

void getTargetedFilesByExt(const filesystem::path &directoryPath, const string extensions[])
{
    for (const auto &entry : filesystem::recursive_directory_iterator(directoryPath))
    {
        bool extWanted = find(extensions, extensions + extensions->size(), entry.path().extension()) != extensions + extensions->size();

        if (entry.is_regular_file() && extWanted)
        {
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
    try{
        for(auto &item: packageJson[key].items()) {

            for(pair<const string, vector<FileLine>> &file: targetFilesPaths) {

                // file.second => lines of content vector
                bool fileOk = true;

                for(FileLine &line: file.second) {
                    if (line.content.find(item.key()) != string::npos) {
                        cout<<"Package \"" << item.key() << "\" Found at file: " << line.path << endl;
                        cout<<"Content: " << trimStringView(line.content) << endl;
                        cout<<"Line: " << line.number  << endl << "---------\n";

                        fileOk = false;
                        break;
                    }
                }

                if (!fileOk) break;
            }
        }
    } catch(const exception e) {

    }
}

int main(int argc, char *argv[])
{
    parseOptions(argc, argv, packageJsonFilePath, searchDir);
    parsePackageFile(packageJsonFilePath);

    getTargetedFilesByExt(searchDir, exts);

    cout << "dependencies:\n\n";

    processPackages("dependencies");

    cout << "\n\t--------------------------------\n\n";

    cout << "devDependencies:\n\n";

    processPackages("devDependencies");

    return 0;
}