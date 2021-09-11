#ifndef UTILITIES_H_INCLUDED
#define UTILITIES_H_INCLUDED

int CharHexArrayToHexInt(unsigned char*);
int HexIntToDecInt(int);
int ContainsAscii(unsigned char*);
unsigned char *GetFileName(unsigned char*);
unsigned char *MakeIncrementalFileName(unsigned char*, int);

#endif // UTILITIES_H_INCLUDED
