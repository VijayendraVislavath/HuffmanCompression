#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>
using namespace std;

// Node for Huffman Tree
struct Node {
    char ch;
    int freq;
    Node *left, *right;
    Node(char c, int f) {
        ch = c;
        freq = f;
        left = right = nullptr;
    }
};

// Comparator for priority queue (min-heap)
struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

// Function to generate Huffman codes
void generateCodes(Node* root, string code, unordered_map<char, string>& codes) {
    if (root == nullptr) return;
    if (!root->left && !root->right) {
        codes[root->ch] = code;
    }
    generateCodes(root->left, code + "0", codes);
    generateCodes(root->right, code + "1", codes);
}

// Function to build the Huffman Tree
Node* buildTree(const string& text) {
    unordered_map<char, int> freq;
    for (char c : text) freq[c]++;
    priority_queue<Node*, vector<Node*>, Compare> pq;
    for (auto it : freq) {
        pq.push(new Node(it.first, it.second));
    }
    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        Node* parent = new Node('\0', left->freq + right->freq);
        parent->left = left;
        parent->right = right;
        pq.push(parent);
    }
    return pq.top();
}

// Function to encode the text
string encode(const string& text, unordered_map<char, string>& codes) {
    string res = "";
    for (char c : text) res += codes[c];
    return res;
}

// Function to save encoded text to file
void saveToFile(const string& data) {
    ofstream fout("compressed.txt");
    if (fout.is_open()) {
        fout << data;
        fout.close();
        cout << "Encoded text saved to compressed.txt" << endl;
    } else {
        cout << "Error: Could not write to file." << endl;
    }
}

int main() {
    string text;
    cout << "Enter the text to compress: ";
    getline(cin, text);

    Node* root = buildTree(text);

    unordered_map<char, string> codes;
    generateCodes(root, "", codes);

    cout << "\nHuffman Codes for each character:\n";
    for (auto it : codes) {
        cout << it.first << ": " << it.second << endl;
    }

    string encoded = encode(text, codes);
    cout << "\nEncoded Text: " << encoded << endl;

    saveToFile(encoded);


    // Decoding (for demonstration)
    cout << "Decoded string: ";
    Node* curr = root;
    for (char bit : encoded) {
        if (bit == '0') curr = curr->left;
        else curr = curr->right;
        if (!curr->left && !curr->right) {
            cout << curr->ch;
            curr = root;
        }
    }
    cout << endl;
    
    return 0;
}

