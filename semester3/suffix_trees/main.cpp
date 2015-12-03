#include <string>
#include <iostream>
#include "ukkonen.h"

int main() {
    std::string a, b;
    std::cin >> a >> b;
    SuffixTree tree(a);
    tree.build();
    std::cout << tree.gcs(b);
}
    
