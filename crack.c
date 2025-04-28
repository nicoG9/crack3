#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "md5.h"

#if __has_include("fileutil.h")
#include "fileutil.h"
#endif

#define PASS_LEN 50     // Maximum length any password will be.
#define HASH_LEN 33     // Length of hash plus one for null.

char ** loadFileAA(char *filename, int *size);
char * substringSearchAA(char *target, char **lines, int size);
int arraySort(const void * a, const void * b);

int main(int argc, char *argv[])
{
    if (argc < 3) 
    {
        printf("Usage: %s hash_file dictionary_file\n", argv[0]);
        exit(1);
    }

    // TODO: Read the hashes file into an array.
    //   Use either a 2D array or an array of arrays.
    //   Use the loadFile function from fileutil.c
    //   Uncomment the appropriate statement.
    int size;
    //char (*hashes)[HASH_LEN] = loadFile(argv[1], &size);
    char **hashes = loadFileAA(argv[1], &size);
    
    // CHALLENGE1: Sort the hashes using qsort. Sort the array of arrays
    qsort(hashes, size, sizeof(char *), arraySort);
    
    // TODO
    // Open the password file for reading.
    FILE *password_file = fopen(argv[2], "r");
    if (!password_file)
    {
        printf("Could not open file %s for reading.\n", argv[2]);
    }

    int matches = 0;
    char pass[PASS_LEN];
    while (fgets(pass, PASS_LEN, password_file) != NULL)
    {
        // Trim newline
		char *nl = strchr(pass, '\n');
		if (nl) *nl = '\0';
        char *hash = md5(pass, strlen(pass));

        for (int i = 0; i < size; i++)
        {
            //printf("Password: %s\tHash: %s\n", hash, hashes[i]);
            if (strcmp(hashes[i], hash) == 0)
            {
                printf("Found a match: %s %s\n", hashes[i], pass);
                matches++;
                break;
            }
        }
        free(hash);
    }

    // TODO
    // For each password, hash it, then use the array search
    // function from fileutil.h to find the hash.
    // If you find it, display the password and the hash.
    // Keep track of how many hashes were found.
    // CHALLENGE1: Use binary search instead of linear search.

    printf("Found %d matches.\n", matches);
    // TODO
    // When done with the file:
    //   Close the file
    //   Display the number of hashes found.
    //   Free up memory.
    fclose(password_file);
}

int arraySort(const void * a, const void * b)
{
    char ** aa = (char **)a; // Cast a and b to pointer to pointer
    char ** bb = (char **)b;
    return strcmp(*aa, *bb);
}

char ** loadFileAA(char *filename, int *size)
{
	FILE *in = fopen(filename, "r");
	if (!in)
	{
	    perror("Can't open file");
	    exit(1);
	}
	// File is opened for reading.

	size_t CAPACITY = 100; // Unsigned integer because we aren't gonna be using negative numbers
	char ** arr = malloc(CAPACITY * sizeof(char *)); // Create an array of arrays, aka array of pointers, aka pointer to pointers.
	size_t arrSize = 0; // Unsigned integer, keeps track of where in the array we are
	
	size_t buffSize = 999; 
	char *buff = malloc((buffSize + 1) * sizeof(char));
	while (fgets(buff, buffSize, in) != NULL)
	{
		// Trim newline using cool way I found on stack overflow
		buff[strcspn(buff, "\n")] = 0; // Finds the first \n in the buff, then sets that index to null terminator \0 = 0

		// Check if the array needs to be extended using realloc
		if (arrSize == CAPACITY)
		{
			printf("Reallocating memory.\n");
			CAPACITY = CAPACITY * 1.5; // Reallocated by +50%
			arr = realloc(arr, CAPACITY * sizeof(char *));
		}
		size_t lineSize = strlen(buff);
		arr[arrSize] = malloc(lineSize + 1); // For null terminator
		strcpy(arr[arrSize], buff);
		arrSize = arrSize + 1;
	}

	// Free the buffer memory, and close the file
	free(buff);
	fclose(in); 
	
	// Set size to array size
	*size = arrSize;
	// Return pointer to the array of strings.
	return arr;
}
