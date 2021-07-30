#ifndef OFS3_H_INCLUDED
#define OFS3_H_INCLUDED

typedef struct OFS3
{
    unsigned char FileSignature[4];
    unsigned char HeaderSize[4];
    unsigned char Unknown1[4];
    unsigned char FileNamesTableStart[4];

    // Need to add 0x10 to the following values to get real value
    unsigned char FileCount[4];

} OFS3;

typedef struct DataFile
{
    char FileStart[4];
    char FileSize[4];

    // Can be null if the file has no filename (such as another OFS3 container for example).
    char FileNamePosition[4];
} DataFile;

#endif // OFS3_H_INCLUDED
