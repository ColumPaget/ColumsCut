title: ccut
mansection: 1
date: 2016/05/15


ccut - COLUM'S CUT
------------------

ccut is a unix 'cut' command with a couple of extra features (and a couple of features missing). 

ccut supports:

utf-8 input in -c mode
multiple delimiters   
treating runs of a delimiter as one delimiter   
quoting using either quotes or backslash within the cut document   
outputting fields in any specified order   
outputting a different delimiter than those within the cut document   
setting variables in the shell to values of cut fields   



SYNTAX
------

Usage: cut OPTION... [FILE]...

Mandatory arguments to long options are mandatory for short options too.

*-b, --bytes=[list]*
 : select only these bytes

*-c, --characters=[list]*
 : select only these characters

*-d, -t, --delimiter*=[list] 
 : list of delimiter characters. Default is just the 'tab' character.

*-f, --fields=LIST*
 : select only these fields;  also print any line without delimiter characters, unless the -s option is specified

*--complement*
 : complement the set of selected bytes, characters or fields

*-j, --join-delims*
 : combine runs of delimters and treat them as one delimiter

*-q, --quote*
 : honor quoting within target document using \ or ' or "

*-Q, --quote-strip*
 : honor quoting within target document, but strip quotes off output fields

*-s, --only-delimited*
 : do not print lines not containing delimiters

*-T, --output-delimiter=[string]*
 : use string as the output delimiter the default is to use the input delimiter

*--utf8*
 : honor UTF-8 unicode characters on input. This causes unicode strings to be treated as single characters in both -c and -b opertations

*-V, --vars=[list]*
 : print out bash commands to set variables using the supplied comma-separated list of names

*-z, --zero-terminated*
 : read input where lines are null terminated

*-?, --help*
 : display this help and exit

*-v, --version*
 : output version information and exit


Use one, and only one of -b, -c or -f.  Each LIST is made up of one range, or many ranges separated by commas.
THIS CUT DOES NOT SUPPORT WIDE CHARACTERS (yet). So '-c' and '-b' are equivalent

Multiple characters can be specified as the input delimiter. The following quoted characters are recognized:
```
        \e                      escape
        \t                      tab
        \r                      carriage-return
        \n                      newline
        \xnn            where 'nn' is a two-digit hex-code
```

Selected input is written in the SPECIFIED ORDER (unlike gnu cut), and fields can be output multiple times.
However, order has no meaning when cut is run with --complement, so then fields are output in the order they are encountered in the data

Each range is one of:
```
  N     N'th byte, character or field, counted from 1
  N-    from N'th byte, character or field, to end of line
  N-M   from N'th to M'th (included) byte, character or field
  -M    from first to M'th (included) byte, character or field
```

The '-V' or '--vars' option allows a comma-separated list of variable names to be supplied. Cut will then match output fields to those variable names and print out commands to set those variables in a borne-style shell. This can then be used with the 'eval' command to set variables in the shell.


EXAMPLES
--------

Cut using either [ or ] as the delimiter

`cat file | ccut -d "[]" -f 3`

Cut and output fields in a particular order

`cat file | ccut -f 3,1,6,5`

cut using escape and tab as delimiters

`cat file | ccut -d "\e\t" -f 4`

cut honoring document quoting (quoting can use \\ ' or ")

`echo "field1,"field2 with , in it",field3,field4\,comma,field5 | ccut -d , -q -f 4`

cut using space, comma and semicolon as delimiters, replace delimiters with '-' on output

`echo "field1 field2,field3;field4,field5" | ccut -d " ,;" -f 2,4 -T -`

cut using space as a delimiter, and treating runs of multiple spaces as one delimiter

`echo "field1 field2     field3  field4 field5" | ccut -d " " -j -f 2,4,3 -T -`

set variables in the shell from fields in input

```eval `echo apples,oranges,pears,lemons,lime | ccut -d , -f 2,4,5,1,3 -V citrus1,citrus2,citrus3,poma1,poma2` ```


AUTHOR
------

Written by Colum Paget <colums.projects@gmail.com>

COPYRIGHT
---------

Copyright (c) 2016 Colum Paget. License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.  
This is free software: you are free to change and redistribute it. There is NO WARRANTY, to the extent permitted by law.


