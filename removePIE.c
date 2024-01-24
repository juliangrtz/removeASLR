#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "xnu-definitions.h"

void hexify(unsigned char *data, uint32_t size)
{
	while (size--)
	{
		printf("%02x", *data++);
	}
}

void fcopy(FILE *f1, FILE *f2)
{
	char buffer[BUFSIZ];
	size_t n;

	while ((n = fread(buffer, sizeof(char), sizeof(buffer), f1)) > 0)
	{
		if (fwrite(buffer, sizeof(char), n, f2) != n)
			puts("Error copying backup");
	}
}

int main(int argc, char *argv[])
{
	struct mach_header currentHeader;
	FILE *fp; // edited file pointer
	FILE *fw; // backup file pointer
	char fwName[80];
	char fwPrefix[4] = ".bak";

	if (argc < 1)
	{
		puts("please enter the filename binary: in the format ./removePIE filename");
		return EXIT_FAILURE;
	}
	
	if ((fp = fopen(argv[1], "rb+")) == NULL)
	{
		puts("error, unable to open file");
		return EXIT_FAILURE;
	}

	if ((fw = fopen(fwName, "wb")) == NULL)
	{
		return EXIT_FAILURE;
	}

	if ((fread(&currentHeader.magic, sizeof(int32_t), 1, fp)) == 0)
	{
		puts("error reading magic constant in file");
		return EXIT_FAILURE;
	}

	if (currentHeader.magic == MH_MAGIC || currentHeader.magic == MH_MAGIC_64)
	{
		// create backup
		strlcpy(fwName, argv[1], strlen(argv[1]) + 1);
		strlcat(fwName, fwPrefix, strlen(fwPrefix) + 1);

		puts("loading header");
		fseek(fp, 0, SEEK_SET);
		if ((fread(&currentHeader, sizeof(currentHeader), 1, fp)) == 0)
		{
			printf("error reading mach-o header");
			return EXIT_FAILURE;
		}

		fseek(fp, 0, SEEK_SET); // set fp back to 0 to get full copy
		puts("backing up application binary...");

		fcopy(fp, fw);
		fclose(fw);

		printf("binary backed up to %s\n", fwName);
		printf("mach_header: ");
		hexify((unsigned char *)&currentHeader, sizeof(currentHeader));
		printf("\noriginal flags:\t");
		hexify((unsigned char *)&currentHeader.flags, sizeof(currentHeader.flags));
		printf("\ndisabling ASLR/PIE ...\n");
		currentHeader.flags &= ~MH_PIE;
		puts("new flags:\t");
		hexify((unsigned char *)&currentHeader.flags, sizeof(currentHeader.flags));

		fseek(fp, 0, SEEK_SET);
		if ((fwrite(&currentHeader, sizeof(char), 28, fp)) == 0)
		{
			printf("Error writing to application file %s\n", fwName);
		}
		printf("\nASLR has been disabled for %s\n", argv[1]);
		// exit and close memory
		fclose(fp);
		return EXIT_SUCCESS;
	}
	else if (currentHeader.magic == MH_CIGAM || currentHeader.magic == MH_CIGAM_64) // big endian
	{
		puts("File is big-endian, not an iOS binary!");
		return EXIT_FAILURE;
	}
	else
	{
		puts("File is not a Mach-O binary!");
		return EXIT_FAILURE;
	}

	return EXIT_FAILURE;
}
