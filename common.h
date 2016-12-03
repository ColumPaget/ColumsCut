#ifndef COLUMS_COREUTILS_COMMON_H
#define COLUMS_COREUTILS_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int Flags;

#define Destroy(x) (x ? free(x) : 0)

#define FALSE 0
#define TRUE  1

#define StrLen(str) ( str ? strlen(str) : 0 )
#define StrValid(str) ( (str && (*(const char *) str != '\0')) ? TRUE : FALSE )
#define StrEnd(str) ( (str &&  (*(const char *) str != '\0')) ? FALSE : TRUE )

char *CopyStrLen(char *Dest, const char *Src, size_t len);
char *CopyStr(char *Dest, const char *Src);
char *AddCharToBuffer(char *Dest, size_t DestLen, char Char);
char *DeQuoteStr(char *Buffer, const char *Line);

void ParseCommandValue(int argc, char *argv[], int pos, int Flag, char **String);
void GetMinMaxFields(const char *FieldSpec, int *MinField, int *MaxField);
char *FILEReadLine(char *RetStr, FILE *f, char Term);

void StripTrailingWhitespace(char *str);

#endif
