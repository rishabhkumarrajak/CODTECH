#include <bits/stdc++.h>
using namespace std;

int main() {
    string filename = "sample.txt";

    ofstream outFile(filename);
    outFile << "Hello, this is a sample text file.\n";
    outFile.close();

    ofstream appendFile(filename, ios::app);
    appendFile << "Adding more text to the file.\n";
    appendFile.close();

    ifstream inFile(filename);
    string line;
    while (getline(inFile, line)) {
        cout << line << endl;
    }
    inFile.close();

    return 0;
}
