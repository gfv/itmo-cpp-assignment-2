#include <set>
#include <iostream>
#include <cstring>
#include "compress.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cerr << "please provide an appropriate flag (-compress/-decompress) and two file names." << endl;
        return -1;
    }

    HuffmanTree tree;
    ifstream in(argv[2], ios::in | ios::binary);
    ofstream out(argv[3], ios::out | ios::binary);

    if (strcmp(argv[1], "-compress") == 0) {
        tree.encode(in, out);
    } else if (strcmp(argv[1], "-decompress") == 0) {
        tree.decode(in, out);
    }

    return 0;
}
