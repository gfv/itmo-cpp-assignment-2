#include "compress.h"
#include <iostream>

Node::Node () {
    zero = nullptr;
    one = nullptr;
    parent = nullptr;
}

Node::Node (int frequency, string s, Node* zero, Node* one) {
    this->frequency = frequency;
    this->zero = zero;
    this->one = one;
    this->s = s;
    this->parent = nullptr;
}


void HuffmanTree::build_tree() {
    int last_node = 0;
    for (int i = 0; i < 256; i++) {
        Node leaf (freq_table[i], string(1, i), nullptr, nullptr);
        nodes[last_node] = leaf;
        ns.push(&nodes[last_node]);
        last_node++;
    }

    while (ns.size() > 1) {
        Node* L = ns.top(); ns.pop();
        Node* R = ns.top(); ns.pop();
        nodes[last_node] = Node(L->frequency + R->frequency, L->s + R->s, L, R);
        L->parent = &nodes[last_node];
        R->parent = &nodes[last_node];
        ns.push(&nodes[last_node]);
        last_node++;
    }

    root_node = &nodes[last_node - 1];
}

void HuffmanTree::to_binary(unsigned char c, std::function<void (bool)> emit_bit) {
    Node* ptr = root_node;//&nodes[c];
    while (ptr->s.length() != 1) {
        if (ptr->zero->s.find(c) != string::npos) {
            ptr = ptr->zero;
            emit_bit(false);
        } else {
            ptr = ptr->one;
            emit_bit(true);
        }
    }
}

HuffmanTree::HuffmanTree () : ns ([](Node* const& l, Node* const& r){ return l->frequency >= r->frequency; }) {}

HuffmanTree::~HuffmanTree () {
}

void HuffmanTree::encode(ifstream& in, ofstream& out) {
    for (int i = 0; i < 256; i++) {
        freq_table[i] = 0;
    }

    int byte_count = 0;

    while (!in.eof()) {
        char c;
        in.read(&c, sizeof (char));
        freq_table[c] += 1;
        byte_count++;
    }
    in.clear();

    build_tree();
    in.seekg(0, ios_base::beg);

    char out_buffer = 0;
    int out_bits = 0;
    char in_buffer = 0;

    out.write("HUFF", 4);
    out.write((char*)&byte_count, sizeof(int));
    for (int i = 0; i < 256; i++) {
        out.write((char*)&freq_table[i], sizeof(int));
    }
    out.flush();

    while (!in.eof()) {
        in.read(&in_buffer, sizeof(char));
        to_binary(in_buffer, [&out, &out_buffer, &out_bits](bool bit) {
            if (out_bits == 8) {
                out.put(out_buffer);
                out_buffer = 0;
                out_bits = 0;
            }
            out_buffer <<= 1;
            out_buffer |= bit;
            out_bits++;
        });
    }

    out_buffer <<= (8-out_bits);
    out.put(out_buffer);
    out.flush();
}

void HuffmanTree::decode(ifstream& in, ofstream& out) {
    char sign[4];
    in.read(sign, 4);
    int byte_count;
    in.read((char*)&byte_count, sizeof(int));

    for (int i = 0; i < 256; i++) {
        in.read((char*)&freq_table[i], sizeof(int));
    }

    build_tree();
    Node* tr = root_node;
    while (!in.eof()) {
        char nc = 0;
        in.read(&nc, 1);
        for (int i = 0; i < 8; i++) {
            bool b = (nc >> (7 - i)) & 1;

            if (b) {
                tr = tr->one;
            } else {
                tr = tr->zero;
            }

            if (tr->one == nullptr && tr->zero == nullptr) {
                out.put(tr->s.c_str()[0]);
                byte_count--;
                if (byte_count == 0) return;
                tr = root_node;
            }
        }
    }
}
