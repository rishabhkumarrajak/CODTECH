#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
using namespace std;
using namespace chrono;


string RLECompress(const string& data) {
    string result;
    for (size_t i = 0; i < data.size(); i++) {
        int count = 1;
        while (i + 1 < data.size() && data[i] == data[i + 1]) {
            count++;
            i++;
        }
        result += to_string(count) + data[i];
    }
    return result;
}

string RLEDecompress(const string& data) {
    string result;
    for (size_t i = 0; i < data.size();) {
        string countStr;
        while (isdigit(data[i]))
            countStr += data[i++];

        int count = stoi(countStr);
        result += string(count, data[i++]);
    }
    return result;
}

void threadedCompress(const string& data, string& output, int start, int end) {
    string partial = RLECompress(data.substr(start, end - start));
    output = partial;
}

void compressFileMultiThreaded(const string& inputFile, const string& compressedFile) {
    ifstream in(inputFile);
    if (!in) {
        cerr << "Error opening input file.\n";
        return;
    }

    string content((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    in.close();

    int mid = content.size() / 2;
    string part1, part2;

    auto start_time = high_resolution_clock::now();

    thread t1(threadedCompress, cref(content), ref(part1), 0, mid);
    thread t2(threadedCompress, cref(content), ref(part2), mid, content.size());

    t1.join();
    t2.join();

    ofstream out(compressedFile);
    out << part1 + part2;
    out.close();

    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end_time - start_time);
    cout << "✅ Multi-threaded compression completed in " << duration.count() << " ms\n";
}

void compressFileSingleThreaded(const string& inputFile, const string& compressedFile) {
    ifstream in(inputFile);
    if (!in) {
        cerr << "Error opening input file.\n";
        return;
    }

    string content((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    in.close();

    auto start_time = high_resolution_clock::now();
    string compressed = RLECompress(content);
    auto end_time = high_resolution_clock::now();

    ofstream out(compressedFile);
    out << compressed;
    out.close();

    auto duration = duration_cast<milliseconds>(end_time - start_time);
    cout << "✅ Single-threaded compression completed in " << duration.count() << " ms\n";
}

void decompressFile(const string& compressedFile, const string& outputFile) {
    ifstream in(compressedFile);
    if (!in) {
        cerr << "Error opening compressed file.\n";
        return;
    }

    string content((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    in.close();

    string decompressed = RLEDecompress(content);

    ofstream out(outputFile);
    out << decompressed;
    out.close();

    cout << "✅ Decompression completed.\n";
}

int main() {
   
    ofstream test("original.txt");
    for (int i = 0; i < 10000; ++i) {
        test << "aaaaabbbbccccddddddeeeeffffgggghhhhiiiiijjjjj";
    }
    test.close();

    cout << "🔄 Running single-threaded compression...\n";
    compressFileSingleThreaded("original.txt", "compressed_single.txt");

    cout << "\n🔄 Running multi-threaded compression...\n";
    compressFileMultiThreaded("original.txt", "compressed_multi.txt");

    cout << "\n🔄 Running decompression (from multi-threaded)...\n";
    decompressFile("compressed_multi.txt", "decompressed.txt");

    return 0;
}
