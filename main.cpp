#include <iostream>
#include <vector>

const int BLOCK_SIZE = 4096; // 4KB
const int DISK_SIZE = 1024;  // 1024 Blocks (4MB total)

struct Block {
    char data[BLOCK_SIZE];
};

class VirtualDisk {
public:
    std::vector<Block> blocks;
    VirtualDisk() {
        blocks.resize(DISK_SIZE);
        std::cout << "Disk Storage Layer: 4MB allocated." << std::endl;
    }
};

struct Inode {
    int id;
    char name[32];
    int size;
    int block_indices[10]; 
};

int main() {
    VirtualDisk myDisk;
    std::cout << "File System Simulation Tool v1.1" << std::endl;
    return 0;
}