#include <stdio.h> // fprintf, fopen, fread
#include <stdint.h> // uint8_t
#include <stdlib.h> // malloc

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: recover file\n");
        return 1;
    }

    char *infile = argv[1];

    // open memory card file and check return value is not NULL
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // declare array for storing bytes
    BYTE buffer[512];
    // declare counter for naming each jpeg
    int jpeg_name = 0;

    // allocate memory for naming each jpeg
    char *outfile;
    outfile = (char *) malloc(8);

    // declare outptr as FILE
    FILE *outptr = NULL;

    // read 512 bytes to buffer
    while ((fread(buffer, 1, 512, inptr)) == 512)
    {
        /* find beginning of jpeg using header's first four bytes: 0xff 0xd8 0xff 0xe(1-9) 0xf0 */

        // if a new jpeg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // if already found a jpeg
            if (jpeg_name > 0)
            {
                fclose(outptr);
                sprintf(outfile, "%03i.jpg", jpeg_name);
                outptr = fopen(outfile, "w");
                jpeg_name++;
                fwrite(buffer, 512, 1, outptr);

            }
            // if not already found a jpeg
            else
            {
                sprintf(outfile, "%03i.jpg", jpeg_name);
                outptr = fopen(outfile, "w");
                jpeg_name++;
                fwrite(buffer, 512, 1, outptr);
            }

        }
        // if not a new jpeg
        else
        {
            // if already found a jpeg
            if (jpeg_name > 0)
            {
                fwrite(buffer, 512, 1, outptr);
            }
            // if not already found a jpeg
        }
    }

    // free malloc
    free(outfile);

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // happy days!
    return 0;
}
