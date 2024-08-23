#include <stdio.h>
#include <stdlib.h>
#include "xnu-definitions.h"

void hexlify(unsigned char *data, uint32_t size) {
    while (size--) {
        printf("%02x", *data++);
    }
}

int main(int argc, char *argv[]) {
    FILE *editedFilePtr;
    struct mach_header currentHeader;
    
    // error cases
    if (argc < 1) {
        puts("please enter the filename binary in the format ./removeASLR filename");
        return EXIT_FAILURE;
    }

    if ((editedFilePtr = fopen(argv[1], "rb+")) == NULL) {
        printf("error, unable to open file %s\n", argv[1]);
        return EXIT_FAILURE;
    }



    if ((fread(&currentHeader.magic, sizeof(int32_t), 1, editedFilePtr)) == 0) {
        puts("error reading magic constant in file");
        return EXIT_FAILURE;
    }

    // check magic
    if (currentHeader.magic == MH_MAGIC || currentHeader.magic == MH_MAGIC_64) {
        puts("loading header...");
        fseek(editedFilePtr, 0, SEEK_SET);
        if ((fread(&currentHeader, sizeof(currentHeader), 1, editedFilePtr)) == 0) {
            printf("error reading mach-o header");
            return EXIT_FAILURE;
        }
        puts("looks ok");
        printf("mach-o header: ");
        hexlify((unsigned char *) &currentHeader, sizeof(currentHeader));

        // everything ok. create a backup
        puts("\n\nbacking up application binary...");
        char cpCommand[1000];
        sprintf(cpCommand, "cp %s %s.bak", argv[1], argv[1]);
        system(cpCommand);
        puts("done");

        // modify mach-o header
        printf("\noriginal flags:\t");
        hexlify((unsigned char *) &currentHeader.flags, sizeof(currentHeader.flags));
        printf("\ndisabling ASLR...\n");
        currentHeader.flags &= ~MH_PIE;
        printf("new flags:\t");
        hexlify((unsigned char *) &currentHeader.flags, sizeof(currentHeader.flags));

        fseek(editedFilePtr, 0, SEEK_SET);
        if ((fwrite(&currentHeader, sizeof(char), 28, editedFilePtr)) == 0) {
            printf("error writing to application file");
        }

        printf("\n\nASLR has been disabled for %s!\n", argv[1]);

        // exit and close memory
        fclose(editedFilePtr);
        return EXIT_SUCCESS;
    } else if (currentHeader.magic == MH_CIGAM || currentHeader.magic == MH_CIGAM_64) // big endian
    {
        puts("file is big endian, not an iOS binary!");
        return EXIT_FAILURE;
    } else {
        puts("file is not a Mach-O binary!");
        return EXIT_FAILURE;
    }

}
