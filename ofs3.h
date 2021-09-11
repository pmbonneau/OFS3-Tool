#ifndef OFS3_H_INCLUDED
#define OFS3_H_INCLUDED

void Extract(unsigned char*, unsigned char*);
unsigned char** GetFileNameTable(unsigned char*, int);

typedef struct OFS3
{
    // Example of OFS3 header
    // [4F 46 53 33] [10 00 00 00] [00 00 40 00] [F0 9B 7E 00]
    // [04 00 00 00] [30 00 00 00] [C0 9A 7E 00] [F0 9A 7E 00]
    // [1C 00 00 00] [30 9B 7E 00] [28 00 00 00] [70 9B 7E 00]
    // [80 00 00 00] [00 00 00 00] [00 00 00 00] [00 00 00 00]

    unsigned char FileSignature[4]; // [4F 46 53 33]
    unsigned char HeaderSize[4]; // [10 00 00 00]
    unsigned char Unknown1[4]; // [00 00 40 00]
    unsigned char FileSize[4]; // [F0 9B 7E 00], the end of the OFS3 container, also the beginning of the nametable. Need to add 0x+10.

    unsigned char FileCount[4]; // [04 00 00 00], not sure how it works.

} OFS3;

typedef struct DataFile
{
    // Need to add 0x10 to the following values to get real value
    unsigned char FileStart[4]; // [30 00 00 00], relative to OFS3 container

    // FileSize is the start of nametable (name of the first file as well)
    unsigned char FileSize[4]; // [C0 9A 7E 00], no +0x10, be sure to count from the proper file start.

    // Filename from the file names table, max length is 128.
    unsigned char FileName[128];

    // Probably not working
    // Can be null if the file has no filename (such as another OFS3 container for example).
    char NameTableEnd[4]; // [F0 9A 7E 00], End of nametable, +0x10. Probably not true, needs to be confirmed

    char FileNameDifferential[4]; // [1C 00 00 00], substract this to NameTableEnd to get second file name. Probably not true, needs to be confirmed, no +0x10
} DataFile;

#endif // OFS3_H_INCLUDED
