/*
Copyright (c) 2015 Colum Paget <colums.projects@googlemail.com>
* SPDX-License-Identifier: GPL-3.0
*/

#include "common.h"
#include <ctype.h>

int Flags=0;


char *CatStrLen(char *Dest, const char *Src, size_t len)
{
const char *src, *end;
char *dst;
int dstlen;

dstlen=StrLen(Dest);
Dest=(char *)realloc(Dest,dstlen+len+1);
dst=Dest+dstlen;
src=Src;
end=src+len;
while ((src < end) && (*src != '\0'))
{
*dst=*src;
dst++;
src++;
}
*dst='\0';

return(Dest);
}



char *CatStr(char *Dest, const char *Src)
{
return(CatStrLen(Dest, Src, StrLen(Src)));
}


char *CopyStrLen(char *Dest, const char *Src, size_t len)
{
const char *src, *end;
char *dst;

Dest=(char *)realloc(Dest,len+1);
dst=Dest;
src=Src;
end=src+len;
while ((src < end) && (*src != '\0'))
{
*dst=*src;
dst++;
src++;
}
*dst='\0';

return(Dest);
}


char *CopyStr(char *Dest, const char *Src)
{
if (Dest) *Dest=0;
return(CatStr(Dest,Src));
}



char *AddCharToBuffer(char *Dest, size_t DestLen, char Char)
{
char *actb_ptr;

//if (Dest==NULL || ((DestLen % 100)==0)) 
actb_ptr=(char *) realloc((void *) Dest,DestLen +110);
//else actb_ptr=Dest;

actb_ptr[DestLen]=Char;
actb_ptr[DestLen+1]='\0';

return(actb_ptr);
}



//This function looks at a field specifier (list of fields) and extracts the minimum and
//maximum fields
void GetMinMaxFields(const char *FieldSpec, int *MinField, int *MaxField)
{
const char *ptr;
int val;

*MinField=0;
*MaxField=0;
if (StrLen(FieldSpec))
{
	ptr=FieldSpec;
	while (isspace(*ptr)) ptr++;
  if (isdigit(*ptr)) *MinField=strtol(FieldSpec, (char **) &ptr, 10);

	//will be zero if Field spec didn't start with a number
  val=*MinField;
  while (ptr && (*ptr != '0'))
  {
    if (val > *MaxField) *MaxField=val;
    while ((! isdigit(*ptr)) && (*ptr !='\0'))  ptr++;
    if (! StrLen(ptr)) break;
		if (! isdigit(*ptr)) break;
    val=strtol(ptr,(char **) &ptr,10);
		if (val > *MaxField) *MaxField=val;
  }
}


}



void ParseCommandValue(int argc, char *argv[], int pos, int Flag, char **String)
{

if (pos >= argc)
{
	fprintf(stderr,"ERROR: Argument missing after '%s'\n",argv[pos-1]);
	exit(1);
}

Flags |= Flag;
if (String) 
{
	if (Flag & FLAG_QDELIM) *String=DeQuoteStr(*String,argv[pos]);
	else *String=CopyStr(*String,argv[pos]);
}

}



char *FILEReadLine(char *RetStr, FILE *f, char Term)
{
int inchar, len=0;

RetStr=CopyStr(RetStr,"");
inchar=fgetc(f);
while ((inchar != Term) && (inchar != EOF))
{
	RetStr=AddCharToBuffer(RetStr,len++,inchar & 0xFF);
	inchar=fgetc(f);
}

if ((inchar==EOF) && (StrLen(RetStr)==0))
{
	Destroy(RetStr);
	return(NULL);
}

return(RetStr);
}





#define ESC 0x1B

char *DeQuoteStr(char *Buffer, const char *Line)
{
char *out, *in;
size_t olen=0;
char hex[3];

if (Line==NULL) return(NULL);
out=CopyStr(Buffer,"");
in=(char *) Line;

while(in && (*in != '\0') )
{
	if (*in=='\\')
	{
		in++;
		switch (*in)
		{
		  case 'e': 
			out=AddCharToBuffer(out,olen,ESC);
			olen++;
			break;


		  case 'n': 
			out=AddCharToBuffer(out,olen,'\n');
			olen++;
			break;

		  case 'r': 
			out=AddCharToBuffer(out,olen,'\r');
			olen++;
			break;

		  case 't': 
			out=AddCharToBuffer(out,olen,'\t');
			olen++;
			break;

			case 'x':
			in++; hex[0]=*in;
			in++; hex[1]=*in;
			hex[2]='\0';
			out=AddCharToBuffer(out,olen,strtol(hex,NULL,16) & 0xFF);
			olen++;
			break;

		  case '\\': 
		  default:
			out=AddCharToBuffer(out,olen,*in);
			olen++;
			break;

		}
	}
	else 
	{
		out=AddCharToBuffer(out,olen,*in);
		olen++;
	}
	in++;
}

return(out);
}

void StripTrailingWhitespace(char *str)
{
size_t len;
char *ptr;

len=StrLen(str);
if (len==0) return;
for(ptr=str+len-1; (ptr >= str) && isspace(*ptr); ptr--) *ptr='\0';
}
