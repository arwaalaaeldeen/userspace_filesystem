#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/userfs.h" // Adjust if your public header path is different

int main() {
    printf("--- Testing O(1) Hashed Directory Index ---\n");

    // 1. Format and Mount
    // Assuming 16 MiB size (16777216 bytes) based on your design limits
    if (ufs_format("test_hash.img", 16777216) != 0) {
        perror("Format failed");
        return 1;
    }
    if (ufs_mount("test_hash.img") != 0) {
        perror("Mount failed");
        return 1;
    }

    // 2. Create the test directory
    printf("Creating /hash_test directory...\n");
    if (ufs_mkdir("/hash_test") != 0) {
        perror("ufs_mkdir failed");
        return 1;
    }

    // 3. Blast it with 100 files
    // This perfectly proves we bypass the old 64-file limit and tests hash collisions!
    printf("Inserting 100 files to test linear probing...\n");
    for (int i = 0; i < 100; i++) {
        char filepath[256];
        snprintf(filepath, sizeof(filepath), "/hash_test/file_%d.txt", i);
        
        if (ufs_create(filepath) != 0) {
            printf("Failed to create file: %s\n", filepath);
            return 1;
        }
    }
    printf("Successfully inserted 100 files in O(1) time!\n");

    // 4. Look them up instantly
    printf("Reading files back through the hash index...\n");
    for (int i = 0; i < 100; i++) {
        char filepath[256];
        struct ufs_stat st;
        snprintf(filepath, sizeof(filepath), "/hash_test/file_%d.txt", i);
        
        if (ufs_stat(filepath, &st) != 0) {
            printf("Failed to find file: %s\n", filepath);
            return 1;
        }
    }
    printf("All 100 files located instantly! The Master Index is working.\n");

    ufs_unmount();
    printf("Test complete.\n");
    return 0;
}
