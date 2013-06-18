#ifndef COMPRESS_H
#define COMPRESS_H

#include <queue>
#include <deque>
#include <fstream>
#include <string>
#include <functional>

using namespace std;

struct Node {
    string s;
    int frequency;
    Node* zero;
    Node* one;
    Node* parent;

    Node ();
    Node (int frequency, string s, Node* zero, Node* one);
};

class HuffmanTree {
private:
    priority_queue<Node*, std::deque<Node*>, std::function<bool (Node* const&, Node* const&)> > ns;
    Node nodes[10240];
    Node* root_node;
    ifstream in;
    ofstream out;

    int freq_table[256];

    void build_tree() ;
    void to_binary(unsigned char c, std::function<void (bool)> emit_bit);

public:
    HuffmanTree ();
    ~HuffmanTree ();

    void encode(ifstream& in, ofstream& out);
    void decode(ifstream& in, ofstream& out);

};

#endif // COMPRESS_H
