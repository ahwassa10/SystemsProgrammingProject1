#!/bin/bash

tests=12

inputFiles=("test1.txt"
"test1.txt"
"test1.txt"
"test1.txt"
"test2.txt"
"test2.txt"
"test3.txt"
"test3.txt"
"test3.txt"
"test4.txt"
"test4.txt"
"test4.txt"
"test5.txt"
"test5.txt"
"test5.txt")

lineWidths=(1
10 15 20
20 30 1 10
20 1 5 10
1 5 11)

outputFiles=("test1_1.txt"
"test1_10.txt"
"test1_15.txt"
"test1_20.txt"
"test2_20.txt"
"test2_30.txt"
"test3_1.txt"
"test3_10.txt"
"test3_20.txt"
"test4_1.txt"
"test4_5.txt"
"test4_10.txt"
"test5_1.txt"
"test5_5.txt"
"test5_11.txt")

for i in
{0..14} do
echo
testing
${inputFiles[i]}
against
${outputFiles[i]}
./wrap
${inputFiles[i]}
${lineWidths[i]}
| cmp
${outputFiles[i]}
echo
testing
reflexive
property
./wrap
${inputFiles[i]}
${lineWidths[i]}
> temp
./wrap
temp
${lineWidths[i]}
| cmp
${outputFiles[i]}
valgrind
--leak-check=yes
--error-exitcode=4
--log-file=temp
./wrap
${inputFiles[i]}
${lineWidths[i]}
> temp
ret=$? if
[ $ret -eq
4 ] then
echo
memory
leaks
found else
echo no
leaks
found fi
done
