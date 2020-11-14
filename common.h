#ifndef COLUMS_COREUTILS_COMMON_H
#define COLUMS_COREUTILS_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int Flags;

#define Destroy(x) (x ? free(x) : 0)


#define FALSE 0
#define TRUE  1

#define FLAG_CHARS  1
#define FLAG_BYTES  2
#define FLAG_FIELDS 8
#define FLAG_REVERSE  16
#define FLAG_ONLY_DELIM_LINES 32
#define FLAG_QUOTED   64
#define FLAG_QUOTE_STRIP 128
#define FLAG_COMBINE_DELIMS 256
#define FLAG_SETVARS 512
#define FLAG_ZERO_TERM 1024
#define FLAG_COMPLEMENT 2048
#define FLAG_UTF8 4096
#define FLAG_DELIMSTR 8192 //string delimiter (instead of character)
#define FLAG_QDELIM 16384  //handle quoted delimiter
#define FLAG_REPLACE_DELIM 32768

#define StrLen(str) ( str ? strlen(str) : 0 )
#define StrValid(str) ( (str && (*(const char *) str != '\0')) ? TRUE : FALSE )
#define StrEnd(str) ( (str &&  (*(const char *) str != '\0')) ? FALSE : TRUE )

char *CopyStrLen(char *Dest, const char *Src, size_t len);
char *CopyStr(char *Dest, const char *Src);
char *CatStr(char *Dest, const char *Src);
char *AddCharToBuffer(char *Dest, size_t DestLen, char Char);
char *DeQuoteStr(char *Buffer, const char *Line);

void ParseCommandValue(int argc, char *argv[], int pos, int Flag, char **String);
void GetMinMaxFields(const char *FieldSpec, int *MinField, int *MaxField);
char *FILEReadLine(char *RetStr, FILE *f, char Term);

void StripTrailingWhitespace(char *str);

#endif
