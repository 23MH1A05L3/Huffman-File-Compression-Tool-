#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <string>
#include <iomanip> // for file size display

using namespace std;

// ========== Huffman Node ==========
struct HuffmanNode {
    char ch;
    int freq;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

struct Compare {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        return a->freq > b->freq;
    }
};

// ========== Compression Functions ==========

unordered_map<char, int> countFrequencies(const string& filename) {
    unordered_map<char, int> freq;
    ifstream inFile(filename);
    char ch;
    while (inFile.get(ch)) freq[ch]++;
    inFile.close();
    return freq;
}

HuffmanNode* buildHuffmanTree(const unordered_map<char, int>& freqMap) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> pq;
    for (auto& pair : freqMap)
        pq.push(new HuffmanNode(pair.first, pair.second));
    while (pq.size() > 1) {
        HuffmanNode* l = pq.top(); pq.pop();
        HuffmanNode* r = pq.top(); pq.pop();
        HuffmanNode* merged = new HuffmanNode('\0', l->freq + r->freq);
        merged->left = l; merged->right = r;
        pq.push(merged);
    }
    return pq.top();
}

void generateCodes(HuffmanNode* root, string code, unordered_map<char, string>& huffmanCode) {
    if (!root) return;
    if (!root->left && !root->right) huffmanCode[root->ch] = code;
    generateCodes(root->left, code + "0", huffmanCode);
    generateCodes(root->right, code + "1", huffmanCode);
}

string encodeText(const string& filename, const unordered_map<char, string>& huffmanCode) {
    ifstream inFile(filename);
    string encoded = "", line;
    char ch;
    while (inFile.get(ch)) encoded += huffmanCode.at(ch);
    inFile.close();
    return encoded;
}

void writeBinaryFile(const string& binaryStr, const string& filename) {
    ofstream outFile(filename, ios::binary);
    for (int i = 0; i < binaryStr.length(); i += 8) {
        string byteStr = binaryStr.substr(i, 8);
        while (byteStr.length() < 8) byteStr += '0';
        unsigned char byte = static_cast<unsigned char>(stoi(byteStr, nullptr, 2));
        outFile.write(reinterpret_cast<char*>(&byte), 1);
    }
    outFile.close();
}

void saveCodeMap(const unordered_map<char, string>& huffmanCode, const string& filename) {
    ofstream out(filename);
    for (auto& pair : huffmanCode) {
        if (pair.first == '\n') out << "\\n " << pair.second << '\n';
        else if (pair.first == ' ') out << "\\s " << pair.second << '\n';
        else out << pair.first << " " << pair.second << '\n';
    }
    out.close();
}

// ========== Decompression Functions ==========

unordered_map<string, char> loadCodeMap(const string& filename) {
    unordered_map<string, char> codeMap;
    ifstream in(filename);
    string line;
    while (getline(in, line)) {
        string ch = line.substr(0, line.find(' '));
        string code = line.substr(line.find(' ') + 1);
        char actualChar = (ch == "\\n") ? '\n' : (ch == "\\s") ? ' ' : ch[0];
        codeMap[code] = actualChar;
    }
    in.close();
    return codeMap;
}

string readBinaryFile(const string& filename) {
    ifstream in(filename, ios::binary);
    string bitStr = "";
    char byte;
    while (in.get(byte)) {
        for (int i = 7; i >= 0; --i)
            bitStr += ((byte >> i) & 1) ? '1' : '0';
    }
    in.close();
    return bitStr;
}

string decodeBinaryString(const string& binaryStr, const unordered_map<string, char>& codeMap) {
    string decoded = "", current = "";
    for (char bit : binaryStr) {
        current += bit;
        if (codeMap.count(current)) {
            decoded += codeMap.at(current);
            current = "";
        }
    }
    return decoded;
}

void writeDecodedText(const string& decodedText, const string& filename) {
    ofstream out(filename);
    out << decodedText;
    out.close();
}

void decompress(const string& compressedFile, const string& codeMapFile, const string& outputFile) {
    auto codeMap = loadCodeMap(codeMapFile);
    string binary = readBinaryFile(compressedFile);
    string decoded = decodeBinaryString(binary, codeMap);
    writeDecodedText(decoded, outputFile);
    cout << "✅ Decompression complete → " << outputFile << "\n";
}

// ========== Utility ==========

long getFileSize(const string& filename) {
    ifstream in(filename, ios::binary | ios::ate);
    return in.tellg();
}

// ========== Menu CLI ==========
int main() {
    string inputFile = "input.txt";
    string compressedFile = "compressed.bin";
    string codeMapFile = "code_map.txt";
    string outputFile = "decompressed.txt";

    int choice;
    do {
        cout << "\n=========== Huffman Compression Tool ===========\n";
        cout << "1. Compress\n";
        cout << "2. Decompress\n";
        cout << "0. Exit\n";
        cout << "===============================================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            auto freqMap = countFrequencies(inputFile);
            auto root = buildHuffmanTree(freqMap);
            unordered_map<char, string> huffmanCode;
            generateCodes(root, "", huffmanCode);

            string encoded = encodeText(inputFile, huffmanCode);
            writeBinaryFile(encoded, compressedFile);
            saveCodeMap(huffmanCode, codeMapFile);

            cout << "✅ Compression complete.\n";
            cout << "🔹 Original Size: " << getFileSize(inputFile) << " bytes\n";
            cout << "🔹 Compressed Size: " << getFileSize(compressedFile) << " bytes\n";
        }
        else if (choice == 2) {
            decompress(compressedFile, codeMapFile, outputFile);
        }
        else if (choice != 0) {
            cout << "❌ Invalid choice. Try again.\n";
        }

    } while (choice != 0);

    cout << "\n👋 Exiting Huffman Tool. Goodbye!\n";
    return 0;
}
