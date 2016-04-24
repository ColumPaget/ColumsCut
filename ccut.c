#include "libUseful-2.0/libUseful.h"
#include "common.h"
#include <ctype.h>

#define FLAG_CHARS  1
#define FLAG_BYTES  2
#define FLAG_FIELDS 8
#define FLAG_REVERSE  16
#define FLAG_SUPPRESS 32
#define FLAG_QUOTED   64
#define FLAG_COMPLEMENT 128
#define FLAG_COMBINE_DELIMS 256
#define FLAG_SETVARS 512
#define FLAG_ZERO_TERM 1024

#define OPT_NAME  1
#define OPT_SHORT 2
#define OPT_LONG  3

#define FIELD_INCLUDED 1
#define FIELD_LAST 2

char *Delim=NULL, *OutputDelim=NULL, *InPath=NULL, *OutPath=NULL, *FieldSpec=NULL;
int StartPos=0, EndPos=0, MaxField=0, DelimLen=0;


//this is defined in common.c
//int Flags=0;

char *Version="1.0";

typedef struct 
{
int Flags;
const char *Start;
const char *End;
char *Var;
} TCutField;


void DisplayHelp()
{
printf("Usage: cut OPTION... [FILE]...\n");
printf("Print selected parts of lines from each FILE to standard output.\n");
printf("Supports: Multiple delimiters. Quoting within the document. Outputing fields in anyspecified order. Using a different delimiter on output than input.\n");
printf("\n");
printf("Mandatory arguments to long options are mandatory for short options too.\n");
printf("  -b, --bytes=[list]      select only these bytes\n");
printf("  -c, --characters=[list] select only these characters\n");
printf("  -d, -t, --delimiter=[list] list of delimiter characters. Default is just the 'tab' character.\n");
printf("  -f, --fields=LIST       select only these fields;  also print any line without delimiter characters, unless the -s option is specified\n");
printf("      --complement        complement the set of selected bytes, characters or fields\n");
printf("  -j, --join-delims       combine runs of delimters and treat them as one delimiter\n");
printf("  -q, --quote             honor quoting within target document using \\ or ' or \"\n");
//printf("  -r, --reverse           cut by counting chars/bytes/fields from end of line, not from start of line\n");
printf("  -s, --only-delimited    do not print lines not containing delimiters\n");
printf("  -T, --output-delimiter=[string] use string as the output delimiter\n");
printf("                            the default is to use the input delimiter\n");
printf("  -z, --zero-terminated   read input where lines are null terminated\n");
printf("      --help     display this help and exit\n");
printf("      --version  output version information and exit\n");
printf("\n");
printf("Use one, and only one of -b, -c or -f.  Each LIST is made up of one range, or many ranges separated by commas.\n");
printf("THIS CUT DOES NOT SUPPORT WIDE CHARACTERS (yet). So '-c' and '-b' are equivalent\n\n");
printf("Multiple characters can be specified as the input delimiter. The following quoted characters are recognized:\n");
printf("	\\e			escape\n");
printf("	\\t			tab\n");
printf("	\\r			carriage-return\n");
printf("	\\n			newline\n");
printf("	\\xnn		where 'nn' is a two-digit hex-code\n\n");

printf("Selected input is written in the SPECIFIED ORDER (unlike gnu cut), and fields can be output multiple times.\n");
printf("However, order has no meaning when cut is run with --complement, so then fields are output in the order they are encountered in the data\n");
printf("\nEach range is one of:\n");
printf("  N     N'th byte, character or field, counted from 1\n");
printf("  N-    from N'th byte, character or field, to end of line\n");
printf("  N-M   from N'th to M'th (included) byte, character or field\n");
printf("  -M    from first to M'th (included) byte, character or field\n");
printf("\n");
printf("With no FILE, or when FILE is -, read standard input.\n");

printf("\n");
printf("Report bugs to colums.projects@gmail.com\n");

exit(0);
}


void DisplayVersion()
{
printf("ccut (Colum's cut) %s\n",Version);
printf("Copyright (C) 2015 Colum Paget\n");
printf("License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n");
printf("This is free software: you are free to change and redistribute it.\n");
printf("There is NO WARRANTY, to the extent permitted by law.\n");

printf("\nWritten by Colum Paget\n");

exit(0);
}






void ParseVars(TCutField *Fields, const char *VarList)
{
const char *start, *ptr;
int i=0;

start=VarList;
ptr=strchr(start, ',');
while (start)
{
	if (! ptr)
	{
		Fields[i].Var=CopyStr(Fields[i].Var,start);
		break;
	}
	else Fields[i].Var=CopyStrLen(Fields[i].Var, start, ptr-start);

	ptr++;
	i++;

	if (i > MaxField) break;
	if (*ptr == '\0') break;

	start=ptr;
	ptr=strchr(start, ',');
}

}




TCutField *ParseCommandLine(int argc, char *argv[])
{
int i, State=OPT_NAME;
char *Tempstr=NULL, *VarList=NULL, *ptr, *arg;
TCutField *CutFields=NULL;
int val;

for (i=1; i < argc; i++)
{
	State=OPT_NAME;
	ptr=argv[i];
	while (*ptr=='-') 
	{
		State++;
		ptr++;
	}

	switch (State)
	{
		case OPT_NAME:
			if (! StrLen(InPath)) InPath=CopyStr(InPath, argv[i]);
			else if (! StrLen(OutPath)) OutPath=CopyStr(OutPath, argv[i]);
		break;


		case OPT_SHORT:
			switch (*ptr)
			{
				case 'c': 
					Flags |= FLAG_CHARS; 
					ParseCommandValue(argc, argv, ++i, 0, &FieldSpec);
					GetMinMaxFields(FieldSpec, &val, &MaxField);
				break;
				case 'b': 
					Flags |= FLAG_BYTES; 
					ParseCommandValue(argc, argv, ++i, 0, &FieldSpec);
					GetMinMaxFields(FieldSpec, &val, &MaxField);
				break;

				//not working yet
				//case 'r': Flags |= FLAG_REVERSE; break;
				case 's': Flags |= FLAG_SUPPRESS; break;
				case 'q': Flags |= FLAG_QUOTED; break;
				case 'j': Flags |= FLAG_COMBINE_DELIMS; break;


				case 'f':
					Flags |= FLAG_FIELDS;
					ParseCommandValue(argc, argv, ++i, 0, &FieldSpec);
					GetMinMaxFields(FieldSpec, &val, &MaxField);
				break;

				case 'd':
				case 't':
					ParseCommandValue(argc, argv, ++i, 0, &Delim);
				break;

				case 'T':
					ParseCommandValue(argc, argv, ++i, 0, &OutputDelim);
				break;

				case 'z': Flags |= FLAG_ZERO_TERM; break;

				case '?':
					DisplayHelp();
				break;
			}
		break;

		case OPT_LONG:
				arg=strchr(ptr,'=');
				if (arg) 
				{
					*arg='\0';
					arg++;
				}

				switch (*ptr)
				{
					case 'b':
						if (strcmp(ptr, "bytes")==0)
						{
							Flags |= FLAG_BYTES;
							FieldSpec=CopyStr(FieldSpec, arg);
							GetMinMaxFields(FieldSpec, &val, &MaxField);
						}
					break;

					case 'c':
						if (strcmp(ptr, "characters")==0) 
						{
							Flags |= FLAG_CHARS;
							FieldSpec=CopyStr(FieldSpec, arg);
							GetMinMaxFields(FieldSpec, &val, &MaxField);
						}
						else if (strcmp(ptr, "complement")==0) Flags |= FLAG_COMPLEMENT;
						else if (strcmp(ptr, "join-delims")==0) Flags |= FLAG_COMBINE_DELIMS;
					break;

					case 'f':
						if (strcmp(ptr, "fields")==0)
						{
							Flags |= FLAG_BYTES;
							FieldSpec=CopyStr(FieldSpec, arg);
							GetMinMaxFields(FieldSpec, &val, &MaxField);
						}
					break;

					case 'o':
						if (strcmp(ptr, "only-delimited")==0) Flags |= FLAG_SUPPRESS;
						if (strcmp(ptr, "output-delimiter")==0) Flags |= FLAG_SUPPRESS;
					break;

					case 'h':
						if (strcmp(ptr,"help")==0) DisplayHelp();
					break;

					/*
					//not working yet
					case 'r':
						if (strcmp(ptr, "reverse")==0) Flags |= FLAG_REVERSE;
					break;
					*/

					case 'v':
						if (strcmp(ptr,"version")==0) DisplayVersion();
						if (strcmp(ptr,"vars")==0) 
						{
							Flags |= FLAG_SETVARS;
							VarList=CopyStr(VarList,arg);
						}
					break;

					case 'z': 
						if (strcmp(ptr,"zero-terminated")==0) Flags |= FLAG_ZERO_TERM; 
					break;
				}
		break;

		default:
		break;
	}
}

if ((Flags & FLAG_FIELDS) && (StrLen(Delim)==0)) Delim=CopyStr(Delim,"	");
CutFields=(TCutField *) calloc(MaxField+1,sizeof(TCutField));
if (StrLen(VarList)) ParseVars(CutFields, VarList);

DelimLen=StrLen(Delim);

if (! (Flags & (FLAG_FIELDS | FLAG_CHARS | FLAG_BYTES))) 
{
	fprintf(stderr,"cut: you must specify a list of bytes, characters, or fields\n");
	exit(3);
}

Destroy(Tempstr);
Destroy(VarList);

return(CutFields);
}




const char *ExtractNextField(const char *Line, const char **Start, const char **End)
{
const char *ptr, *fstart;
char qchar;

fstart=Line;
for (ptr=fstart; /* we have to handle '\0' */ ; ptr++)
{
	//if current character is one of the delimiters
	if (Flags & FLAG_QUOTED)
	{
		switch(*ptr)
		{
		case '\'':
		case '"':
			qchar=*ptr;
			ptr++;
			while ((*ptr != '\0') && (*ptr != qchar)) ptr++;
			if (*ptr==qchar) ptr++;
		break;

		case '\\':
			ptr++;
			if (*ptr !='\0') ptr++;
		break;
		}
	}


	if ((*ptr=='\0') || memchr(Delim,*ptr,DelimLen))
	{
		*Start=fstart;
		*End=ptr; //including delim
		break;
	}

}

return(ptr);
}





int ExtractFields(const char *Line, TCutField *Fields)
{
int fcount=0;
const char *ptr;

ptr=Line;

while (fcount < MaxField)
{
	ptr=ExtractNextField(ptr, &(Fields[fcount].Start), &(Fields[fcount].End));
	fcount++;
	if (*ptr=='\0') break;
	ptr++;

	if (Flags & FLAG_COMBINE_DELIMS)
	{
		while (memchr(Delim,*ptr,DelimLen)) ptr++;
	}
}


//We have to count the remaining fields, even though we might not be outputting them, because the '--reverse' option
//requires us to know the total number of fields
for (; *ptr !='\0' ; ptr++)
{
	//if current character is one of the delimiters
	if (memchr(Delim,*ptr,DelimLen)) fcount++;
	if (Flags & FLAG_COMBINE_DELIMS)
	{
		while (memchr(Delim,*ptr,DelimLen)) ptr++;
	}
}

return(fcount);
}






void OutputField(const char *start, const char *end, int IsLast)
{
const char *ptr;
char *Tempstr=NULL;


if (start)
{
	for (ptr=start; ptr && (ptr < end) ; ptr++) 
	{
		if (*ptr == '\0') break;
	}

	if (Flags & FLAG_SETVARS)
	{
		/*
		Tempstr=MCopyStr(Tempstr,CutFields[FieldNo].Var,"=",NULL);
		Tempstr=CatStrLen(Tempstr,start,ptr-start);
		Tempstr=CatStr(Tempstr,"; ");
		fwrite(Tempstr,StrLen(Tempstr),1,stdout);
		*/
	}
	else
	{
		if (OutputDelim) 
		{
			fwrite(start,ptr-start,1,stdout);
			fputs(OutputDelim, stdout);
		}
		else if (IsLast) fwrite(start,ptr-start,1,stdout);
		else fwrite(start,ptr+1-start,1,stdout);
	}
}


Destroy(Tempstr);
}



void OutputCutField(int FCount, TCutField *CutFields, int FieldNo, int IsLast)
{

if ((FieldNo > 0) && (FieldNo <= FCount))
{
//	if (Flags & FLAG_REVERSE) FieldNo=FCount+1-FieldNo;
	FieldNo--; //Fields are 1 based, so make zero based

	OutputField(CutFields[FieldNo].Start, CutFields[FieldNo].End, IsLast);
}

}



void OutputFieldRange(int FCount, TCutField *CutFields,int Start, int End, int IsLast)
{
int i;

if (Start < 1) Start=1;
for (i=Start; i < End; i++) OutputCutField(FCount,CutFields,i, FALSE);
OutputCutField(FCount,CutFields,i, IsLast);
}



void OutputFieldsAfter(int FCount,TCutField *CutFields, int First, int IsLast)
{
const char *ptr, *Start, *End;

if (First < 1) First=1;
//Field Numbers are 1-based, but need to be converted to zero-base
First--;
ptr=CutFields[First].Start;

while (*ptr !='\0')
{
	ptr=ExtractNextField(ptr, &Start, &End);
	OutputField(Start,End,FALSE);
	if (*ptr !='\0') ptr++;
}

}




void OutputRequestedFields(int FCount, TCutField *CutFields, const char *FieldSpec)
{
char *ptr;
long Start=0, End=0;
int LastField=FALSE;


//ptr has to be 'char *' rather than 'const char *' to keep strtol happy
ptr=(char *) FieldSpec;
while (*ptr != '\0')
{
	switch (*ptr)
	{
		case '-': 
		ptr++;
		if (*ptr == '\0') End=0;
		else End=strtol(ptr,&ptr,10);
		if (*ptr=='\0') LastField=TRUE;
		if (End==0) OutputFieldsAfter(FCount,CutFields,Start,LastField);
		else OutputFieldRange(FCount,CutFields,Start,End,LastField);
		Start=-1;
		break;
	
		case ',':
		if (Start > -1) OutputCutField(FCount,CutFields,Start,FALSE);
		Start=-1;
		ptr++;
		break;
	
		default:
		Start=strtol(ptr,&ptr,10);
		break;
	}
}

if (Start > -1) OutputCutField(FCount,CutFields,Start,TRUE);

putchar('\n');
}



//Bit of a complex function to output the fields NOT specified on the command line.
//This function parses the field specification passed on the command-line, and marks
//which fields are included. It then runs through the input line, and outputs any fields
//that haven't been marked.
void OutputComplementFields(int FCount, TCutField *CutFields, const char *FieldSpec, const char *Line)
{
const char *ptr;
//we set start to -1 to distinguish between 'no field' or 'first field'
long Start=-1, End=-1, i;
const char *fstart, *fend;



//ptr has to be 'char *' rather than 'const char *' to keep strtol happy
ptr=(char *) FieldSpec;
while (*ptr != '\0')
{
	switch (*ptr)
	{
		case '-': 
		ptr++;

		if (*ptr == '\0') End=-1;
		else End=strtol(ptr, (char **) &ptr, 10) - 1;

		if (Start==-1) Start=0;


		if (End==-1) CutFields[Start].Flags=FIELD_INCLUDED | FIELD_LAST;
		else for (i=Start; i <= End; i++) CutFields[i].Flags=FIELD_INCLUDED;
		Start=-1;
		break;
	
		case ',':
		if (Start > -1) CutFields[Start].Flags=FIELD_INCLUDED;
		Start=-1;
		ptr++;
		break;
	
		default:
		Start=strtol(ptr, (char **) &ptr, 10) - 1;
		break;
	}
}


if (Start > -1) CutFields[Start].Flags=FIELD_INCLUDED;

i=0;
ptr=Line;
while (*ptr !='\0')
{
	ptr=ExtractNextField(ptr, &fstart, &fend);
	if (i < MaxField)
	{
		if (! (CutFields[i].Flags & FIELD_INCLUDED)) OutputField(fstart,fend,FALSE);
		if (CutFields[i].Flags & FIELD_LAST) break;
	}
	else OutputField(fstart,fend,FALSE);
	i++;
	if (*ptr !='\0') ptr++;
}


}


void OutputFields(TCutField *CutFields, const char *FieldSpec, const char *Line)
{
int count;

		count=ExtractFields(Line,CutFields);
		if (count==0)
		{
			if (Flags & FLAG_SUPPRESS) /* Do Nothing */ ;
			else fwrite(Line,StrLen(Line),1,stdout);
		}
		else 
		{
			if (Flags & FLAG_COMPLEMENT) OutputComplementFields(count, CutFields, FieldSpec, Line);
			else OutputRequestedFields(count,CutFields,FieldSpec);
		}
}



void OutputBytes(const char *Line, TCutField *Fields)
{
char *ptr;
int len;
long Start=0, End=0;

len=StrLen(Line);
//if (Flags & FLAG_REVERSE) FieldNo=FCount+1-FieldNo;

ptr=FieldSpec;
while (*ptr != '\0')
{
	switch (*ptr)
	{
		case '-': 
			ptr++;
			if (isdigit(*ptr)) End=strtol(ptr,&ptr,10);
			else End=len;
			if ((End < 0) || (End > len)) End=len;
			if (Start < 0) Start=0;
			fwrite(Line+Start,End-Start,1,stdout);
			Start=-1;
		break;
	
		case ',':
			if (Start > -1) fwrite(Line+Start,1,1,stdout);
			Start=-1;
			ptr++;
		break;
	
		default:
			Start=strtol(ptr,&ptr,10)-1;
		break;
	}
}

if (Start > -1) fwrite(Line+Start,1,1,stdout);

putchar('\n');
}




int main(int argc, char *argv[])
{
TCutField *CutFields=NULL;
char *Tempstr=NULL;
FILE *InF;

CutFields=ParseCommandLine(argc, argv);

InF=stdin;
if (Flags & FLAG_ZERO_TERM) Tempstr=FILEReadLine(Tempstr,InF,'\0');
else Tempstr=FILEReadLine(Tempstr,InF,'\n');

while (Tempstr)
{
	StripTrailingWhitespace(Tempstr);
	if (Flags & FLAG_FIELDS) OutputFields(CutFields,FieldSpec,Tempstr);
	else if (Flags & FLAG_BYTES) OutputBytes(Tempstr, CutFields);

if (Flags & FLAG_ZERO_TERM) Tempstr=FILEReadLine(Tempstr,InF,'\0');
else Tempstr=FILEReadLine(Tempstr,InF,'\n');
}

Destroy(Tempstr);

return(0);
}
