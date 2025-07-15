#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>
using namespace std;

// Tree node structure
struct Node {
    unsigned char ch;
    int freq;
    Node *left, *right;
    Node(unsigned char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

// Comparator for min-heap
struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

// Recursively build Huffman codes
void buildCodes(Node* root, string str, unordered_map<unsigned char, string> &huffCode) {
    if (!root) return;
    if (!root->left && !root->right)
        huffCode[root->ch] = str;
    buildCodes(root->left, str + "0", huffCode);
    buildCodes(root->right, str + "1", huffCode);
}

// Read byte frequencies from binary file
unordered_map<unsigned char, int> getFrequencies(const string &filename) {
    unordered_map<unsigned char, int> freq;
    ifstream in(filename, ios::binary);
    if (!in.is_open()) {
        cerr << "Error: Cannot open file " << filename << "\n";
        return freq;
    }
    unsigned char ch;
    while (in.read(reinterpret_cast<char*>(&ch), 1)) {
        freq[ch]++;
    }
    in.close();
    return freq;
}

// Serialize Huffman Tree
void serializeTree(Node* root, ofstream &out) {
    if (!root) return;
    if (!root->left && !root->right) {
        out.put('1');
        out.put(root->ch);
    } else {
        out.put('0');
        serializeTree(root->left, out);
        serializeTree(root->right, out);
    }
}

// Deserialize Huffman Tree
Node* deserializeTree(ifstream &in) {
    char flag;
    if (!in.get(flag)) return nullptr;
    if (flag == '1') {
        unsigned char ch;
        if (!in.read(reinterpret_cast<char*>(&ch), 1)) return nullptr;
        return new Node(ch, 0);
    } else {
        Node* node = new Node('\0', 0);
        node->left = deserializeTree(in);
        node->right = deserializeTree(in);
        return node;
    }
}

// Write encoded bitstream to file
void writeBits(ofstream &out, const string &encoded) {
    unsigned char byte = 0;
    int count = 0;
    for (char bit : encoded) {
        byte <<= 1;
        byte |= (bit - '0');
        count++;
        if (count == 8) {
            out.write(reinterpret_cast<char*>(&byte), 1);
            byte = 0;
            count = 0;
        }
    }
    if (count > 0) {
        byte <<= (8 - count);
        out.write(reinterpret_cast<char*>(&byte), 1);
    }
}

// Compress file
void compress(const string &inputFile, const string &outputFile) {
    unordered_map<unsigned char, int> freq = getFrequencies(inputFile);
    if (freq.empty()) {
        cerr << "Error: Input file is empty or unreadable.\n";
        return;
    }

    // Build Huffman Tree
    priority_queue<Node*, vector<Node*>, Compare> pq;
    for (auto &pair : freq)
        pq.push(new Node(pair.first, pair.second));
    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        Node* merged = new Node('\0', left->freq + right->freq);
        merged->left = left;
        merged->right = right;
        pq.push(merged);
    }

    Node* root = pq.top();
    unordered_map<unsigned char, string> huffCode;
    buildCodes(root, "", huffCode);

    ifstream in(inputFile, ios::binary);
    ofstream out(outputFile, ios::binary);
    if (!in.is_open() || !out.is_open()) {
        cerr << "Error: File open failed.\n";
        return;
    }

    // Write serialized tree and delimiter
    serializeTree(root, out);
    out.put('|');

    // Encode file data
    string encoded;
    unsigned char ch;
    while (in.read(reinterpret_cast<char*>(&ch), 1)) {
        encoded += huffCode[ch];
    }
    writeBits(out, encoded);
    in.close();
    out.close();

    // File size report
    ifstream fin(inputFile, ios::binary | ios::ate);
    streamsize origSize = fin.tellg();
    fin.close();

    ifstream fout(outputFile, ios::binary | ios::ate);
    streamsize compSize = fout.tellg();
    fout.close();

    cout << "✅ Compression complete: " << outputFile << "\n";
    cout << "Original size: " << origSize << " bytes\n";
    cout << "Compressed size: " << compSize << " bytes\n";
    cout << "Compression ratio: " << (100.0 * compSize / origSize) << "%\n";
}

// Decompress file
void decompress(const string &inputFile, const string &outputFile) {
    ifstream in(inputFile, ios::binary);
    if (!in.is_open()) {
        cerr << "Error: Cannot open compressed file.\n";
        return;
    }

    // Deserialize tree
    Node* root = deserializeTree(in);
    char delim;
    in.get(delim);
    if (delim != '|') {
        cerr << "Error: Invalid compressed format.\n";
        return;
    }

    // Read encoded bits
    string encodedBits;
    unsigned char byte;
    while (in.read(reinterpret_cast<char*>(&byte), 1)) {
        for (int i = 7; i >= 0; --i) {
            encodedBits += ((byte >> i) & 1) ? '1' : '0';
        }
    }
    in.close();

    // Decode bits
    ofstream out(outputFile, ios::binary);
    if (!out.is_open()) {
        cerr << "Error: Cannot open output file.\n";
        return;
    }

    Node* curr = root;
    for (char bit : encodedBits) {
        curr = (bit == '0') ? curr->left : curr->right;
        if (!curr->left && !curr->right) {
            out.write(reinterpret_cast<char*>(&curr->ch), 1);
            curr = root;
        }
    }

    out.close();
    cout << "Decompression complete: " << outputFile << "\n";
}

// ----------- MAIN --------------
int main() {
    int choice;
    string inputFile, outputFile;

    cout << "\nHuffman Compression Tool\n";
    cout << "1. Compress any file\n";
    cout << "2. Decompress a file\n";
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice == 1) {
        cout << "Enter input file to compress: ";
        cin >> inputFile;
        cout << "Enter output compressed file: ";
        cin >> outputFile;
        compress(inputFile, outputFile);
    } else if (choice == 2) {
        cout << "Enter compressed input file: ";
        cin >> inputFile;
        cout << "Enter output file to save decompressed data: ";
        cin >> outputFile;
        decompress(inputFile, outputFile);
    } else {
        cout << " Invalid choice.\n";
    }

    return 0;
}

