//
//  main.c
//  iOS-pie-remover
//
//  Created by juliangrtz on 07.10.25.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mach-o/loader.h>

void hexlify(unsigned char *data, uint32_t size) {
    while (size--) {
        printf("%02x", *data++);
    }
}

int main(int argc, char *argv[]) {
    FILE *editedFilePtr;
    uint32_t magic = 0;
    int is64bit = 0;

    if (argc < 2) {
        puts("Usage: ./removePIE <mach-o_binary>");
        return EXIT_FAILURE;
    }

    editedFilePtr = fopen(argv[1], "rb+");
    if (!editedFilePtr) {
        printf("Error: Unable to open file %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    if (fread(&magic, sizeof(uint32_t), 1, editedFilePtr) != 1) {
        puts("Error: Unable to read magic number");
        fclose(editedFilePtr);
        return EXIT_FAILURE;
    }

    if (magic == MH_MAGIC) {
        puts("Detected: 32-bit Mach-O");
        is64bit = 0;
    } else if (magic == MH_MAGIC_64) {
        puts("Detected: 64-bit Mach-O");
        is64bit = 1;
    } else if (magic == MH_CIGAM || magic == MH_CIGAM_64) {
        puts("Error: Big-endian Mach-O (not supported)");
        fclose(editedFilePtr);
        return EXIT_FAILURE;
    } else {
        puts("Error: Not a valid Mach-O file. What are you even doing?");
        fclose(editedFilePtr);
        return EXIT_FAILURE;
    }

    fseek(editedFilePtr, 0, SEEK_SET);

    if (is64bit) {
        struct mach_header_64 header;
        if (fread(&header, sizeof(header), 1, editedFilePtr) != 1) {
            puts("Error: Failed to read 64-bit header");
            fclose(editedFilePtr);
            return EXIT_FAILURE;
        }

        printf("Original flags: ");
        hexlify((unsigned char *)&header.flags, sizeof(header.flags));
        puts("");

        char cpCommand[1024];
        snprintf(cpCommand, sizeof(cpCommand), "cp \"%s\" \"%s.bak\"", argv[1], argv[1]);
        system(cpCommand);
        puts("Backup created.");

        puts("Disabling PIE...");
        header.flags &= ~MH_PIE;

        printf("New flags: ");
        hexlify((unsigned char *)&header.flags, sizeof(header.flags));
        puts("");

        fseek(editedFilePtr, 0, SEEK_SET);
        if (fwrite(&header, sizeof(header), 1, editedFilePtr) != 1) {
            puts("Error: Failed to write updated header");
        } else {
            printf("PIE has been disabled for %s (64-bit)\n", argv[1]);
        }
    } else {
        struct mach_header header;
        if (fread(&header, sizeof(header), 1, editedFilePtr) != 1) {
            puts("Error: Failed to read 32-bit header");
            fclose(editedFilePtr);
            return EXIT_FAILURE;
        }

        printf("Original flags: ");
        hexlify((unsigned char *)&header.flags, sizeof(header.flags));
        puts("");

        char cpCommand[1024];
        snprintf(cpCommand, sizeof(cpCommand), "cp \"%s\" \"%s.bak\"", argv[1], argv[1]);
        system(cpCommand);
        puts("Backup created.");

        puts("Disabling PIE...");
        header.flags &= ~MH_PIE;

        printf("New flags: ");
        hexlify((unsigned char *)&header.flags, sizeof(header.flags));
        puts("");

        fseek(editedFilePtr, 0, SEEK_SET);
        if (fwrite(&header, sizeof(header), 1, editedFilePtr) != 1) {
            puts("Error: Failed to write updated header");
        } else {
            printf("PIE has been disabled for %s (32-bit)\n", argv[1]);
        }
    }

    fclose(editedFilePtr);
    return EXIT_SUCCESS;
}
