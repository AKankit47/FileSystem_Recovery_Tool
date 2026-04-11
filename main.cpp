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
    }

    void simulateCrash(float intensity) {
        for (int i = 0; i < DISK_SIZE; i++) {
            if (!free_map[i] && ((float)rand() / RAND_MAX < intensity)) {
                disk[i].status = CORRUPT;
            }
        }
        std::cout << "[CRASH] System failure simulated." << std::endl;
    }

    // MANINDER'S MODULE: fsck - File System Consistency Check
    void verifyAndRecover() {
        std::cout << "[RECOVERY] Starting fsck scan..." << std::endl;
        int files_fixed = 0;
        for (auto &file : inodes) {
            for (int index : file.block_indices) {
                if (disk[index].status == CORRUPT) {
                    file.is_corrupt = true;
                    std::cout << "[ALERT] File '" << file.name << "' is damaged at block " << index << "!" << std::endl;
                    // Recovery Logic: In a real system, we'd pull from a journal/backup
                    // Here, we mark it and "heal" the block status for simulation
                    disk[index].status = HEALTHY; 
                    files_fixed++;
                }
            }
        }
        std::cout << "[RECOVERY] Scan complete. Issues addressed: " << files_fixed << std::endl;
    }
};

int main() {
    FileSystem myFS;
    myFS.createFile("important_doc.pdf", 5);
    myFS.simulateCrash(0.20f); // 20% corruption
    myFS.verifyAndRecover();
    return 0;
}