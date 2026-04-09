#include <iostream>
#include <vector>
#include <string>
#include <ctime>

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
        std::cout << "[System] Created file: " << name << std::endl;
    }

    // Simulate unexpected hardware failure
    void simulateCrash(float intensity) {
        int corruption_count = 0;
        for (int i = 0; i < DISK_SIZE; i++) {
            if (!free_map[i]) { // Only corrupt blocks that actually have data
                if ((float)rand() / RAND_MAX < intensity) {
                    disk[i].status = CORRUPT;
                    corruption_count++;
                }
            }
        }
        std::cout << "[CRASH] System failure! " << corruption_count << " blocks corrupted." << std::endl;
    }
};

int main() {
    FileSystem myFS;
    myFS.createFile("database.db", 10);
    myFS.createFile("photos.zip", 20);
    
    // Simulating a crash with 10% intensity
    myFS.simulateCrash(0.10f);
    
    return 0;
}