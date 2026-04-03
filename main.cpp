#include <iostream>
struct Inode {
    int id;
    char name[32];
    int size;
    int blocks[10]; // Pointers to data blocks
};
int main() {
    std::cout << "File System Simulation Tool v1.0" << std::endl;
    std::cout << "Status: Initialized" << std::endl;
    return 0;
}