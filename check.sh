#! /bin/sh

EXIT=0

Title()
{
echo -e "\n\033[01m ############### $* \033[00;39m"
}


OkayMessage()
{
echo -e "  \033[01;32mOKAY\033[00;39m    $1"
}

FailMessage()
{
echo -e "  \033[01;31mFAIL\033[00;39m    $1"
EXIT=3    # Bash says 1 and 2 are reserved for specific errors
}


TestCut()
{
RESULT=`cat $2 | ./ccut $1`
EXPECTED="$3"

if [ "$RESULT" = "$EXPECTED" ]
then
	OkayMessage "$4 works"
else
	FailMessage "$4 FAILED [$RESULT]"
fi

}



Title "Testing 'ccut'"
TestCut "-f 3" tests/cut.1 field3 "Cut a single field"
TestCut "-f 2-" tests/cut.1 "field2	field3	field4	field5	field6	field7" "Cut from a field to end of line"
TestCut "-f 2-5" tests/cut.1 "field2	field3	field4	field5" "Cut range of fields"
TestCut "-f 2,5,7" tests/cut.1 "field2	field5	field7" "Cut multiple discontinuous fields"
TestCut "-f 3,1,6,5" tests/cut.1 "field3	field1	field6	field5" "Cut multiple fields, rearranging their order"
TestCut "-f 3,1,7,5" tests/cut.1 "field3	field1	field7	field5" "Cut multiple fields, rearranging their order, with final field included"
TestCut "-T , -f 1-4" tests/cut.1 "field1,field2,field3,field4" "Cut with different delimiter for output than input"
TestCut "-T , -d '\t;,' -f 1-4" tests/cut.2 "field1,field2,field3,field4" "Cut using multiple delimiters"
TestCut "-T , -d '\t;,' -q -f 1-6" tests/cut.2 "field1,field2,field3,field4,field5\,field5.5,\"field6 with a comma , in it\"" "Cut honoring quotes"
TestCut "-Q -d ',' -f 3,6" tests/cut.5 "field3,field6" "Cut honoring quotes but stripping them from output"
TestCut "-d , -j -f 3" tests/cut.3 "field3" "Cut combining runs of the same delimiter"
TestCut "-d ,;: -j -f 6" tests/cut.3 "field6" "Cut combining runs of different delimiters"

eval `cat tests/cut.1 | ./ccut -f 7,5,2 -V arg1,arg2,arg3`
if [ "$arg1" = "field7" -a "$arg2" = "field5" -a "$arg3" = "field2" ]
then
	OkayMessage "Cut fields into variables using eval works"
else
	FailMessage "Cut fields into variables using eval FAILED"
fi 

exit $EXIT
