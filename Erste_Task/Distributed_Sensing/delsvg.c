/**
 * C program to delete a word from file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define BUFFER_SIZE 1000

void removeAll(char * str, const char * toRemove);


int main()
{
    FILE * fPtr;
    FILE * fPtr1;
    FILE * fPtr2;
    FILE * fTemp;
    FILE * fTemp1;
    FILE * fTemp2;
    char buffer[10000],buffer1[10000],buffer2[10000];
    char toRemove[100];
    gets(toRemove);
    /*  Open files */
    fPtr  = fopen("tempWerteFahrenheit.svg", "w+");
    fTemp = fopen("delete.tmp", "w+"); 
    fPtr1  = fopen("tempWerteCelsius.svg", "w+");
    fPtr2  = fopen("pressWerte.svg", "w+");
    fTemp1 = fopen("delete1.tmp", "w+"); 
    fTemp2 = fopen("delete2.tmp", "w+"); 

    if (fPtr == NULL || fTemp == NULL || fPtr1 == NULL || fTemp1 == NULL || fPtr2 == NULL || fTemp2 == NULL )
    {
        /* Unable to open file hence exit */
        printf("\nUnable to open file.\n");
        printf("Please check whether file exists and you have read/write privilege.\n");
        exit(EXIT_SUCCESS);
    }


    /*
     * Read line from source file and write to destination 
     * file after removing given word.
     */
    while ((fgets(buffer, BUFFER_SIZE, fPtr)) != NULL || fgets(buffer1,BUFFER_SIZE,fPtr1)!=NULL || fgets(buffer2,BUFFER_SIZE,fPtr2)!=NULL)
    {
        // Remove all occurrence of word from current line
        removeAll(buffer, toRemove);
	removeAll(buffer1,toRemove);
	removeAll(buffer2,toRemove);
        // Write to temp file
        fputs(buffer, fTemp);
	fputs(buffer1,fTemp1);
	fputs(buffer2,fTemp2);
    }


    /* Close all files to release resource */
    fclose(fPtr);
    fclose(fPtr1);
    fclose(fPtr2);
    fclose(fTemp);
    fclose(fTemp1);
    fclose(fTemp2);


    /* Delete original source file */
    remove("tempWerteFahrenheit.svg");

    /* Rename temp file as original file */
    rename("delete.tmp", "tempWerteFahrenheit.svg");

    remove("tempWerteCelsius.svg");

    /* Rename temp file as original file */
    rename("delete1.tmp", "tempWerteCelsius.svg");

    remove("pressWerte.svg");

    /* Rename temp file as original file */
    rename("delete2.tmp", "pressWerte.svg");
    

    printf("\nAll occurrence of '%s' removed successfully.", toRemove);
    return 0;
    exit(1);
}



/**
 * Remove first occurrence of a given word in string.
 */
void removeAll(char * str, const char * toRemove)
{
    int i, j, stringLen, toRemoveLen;
    int found=0;

    stringLen   = strlen(str);      // Length of string
    toRemoveLen = strlen(toRemove); // Length of word to remove


    for(i=0; i <= stringLen - toRemoveLen; i++)
    {
        /* Match word with string */
        found = 1;
        for(j=0; j < toRemoveLen; j++)
        {
            if(str[i + j] != toRemove[j])
            {
                found = 0;
                break;
            }
        }

        /* If it is not a word */
        if(str[i + j] != ' ' && str[i + j] != '\t' && str[i+j] != ' ' && str[i + j] != '\n' && str[i + j] != '\0') 
        {
            found = 0;
        }

        /*
         * If word is found then shift all characters to left
         * and decrement the string length
         */
        if(found == 1)
        {
            for(j=i; j <= stringLen - toRemoveLen; j++)
            {
                str[j] = str[j + toRemoveLen];
            }
		break;
        }
    }
}
