#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>

const int BLOCK_SIZE = 4096;
const int DISK_SIZE = 1024;

enum BlockStatus { HEALTHY, CORRUPT };

struct Block {
    char data[BLOCK_SIZE];
    BlockStatus status = HEALTHY;
};

struct Inode {
    int id;
    std::string name;
    bool is_corrupt = false;
    std::vector<int> block_indices; 
};

class FileSystem {
public:
    std::vector<Block> disk;
    std::vector<bool> free_map;
    std::vector<Inode> inodes;

    FileSystem() {
        disk.resize(DISK_SIZE);
        free_map.resize(DISK_SIZE, true);
        srand(time(0)); 
    }

    void createFile(std::string name, int num_blocks) {
        Inode new_file;
        new_file.name = name;
        int found = 0;
        for (int i = 0; i < DISK_SIZE && found < num_blocks; i++) {
            if (free_map[i]) {
                new_file.block_indices.push_back(i);
                free_map[i] = false;
                found++;
            }
        }
        inodes.push_back(new_file);
        std::cout << "[System] Allocated " << name << " (" << num_blocks << " blocks)." << std::endl;
    }

    void simulateCrash(float intensity) {
        for (int i = 0; i < DISK_SIZE; i++) {
            if (!free_map[i] && ((float)rand() / RAND_MAX < intensity)) {
                disk[i].status = CORRUPT;
            }
        }
        std::cout << "[CRASH] System failure simulated." << std::endl;
    }

    // PRITAM'S MODULE: Optimization & Defragmentation
    void defragment() {
        std::cout << "[OPTIMIZE] Starting Defragmentation..." << std::endl;
        int write_ptr = 0;
        for (auto &file : inodes) {
            std::vector<int> new_indices;
            for (size_t j = 0; j < file.block_indices.size(); j++) {
                // In a real system, we'd move the actual data in disk[write_ptr]
                new_indices.push_back(write_ptr);
                free_map[write_ptr] = false;
                write_ptr++;
            }
            file.block_indices = new_indices;
        }
        // Mark remaining disk as free
        for (int i = write_ptr; i < DISK_SIZE; i++) free_map[i] = true;
        std::cout << "[OPTIMIZE] Disk compacted. Sequential access optimized." << std::endl;
    }
};

int main() {
    FileSystem myFS;
    myFS.createFile("system.sys", 10);
    myFS.createFile("user_data.dat", 15);
    myFS.defragment();
    return 0;
}