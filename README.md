# ALG-Minimum-Edit-Distance
An implementation of String1 to String2 conversion algorithm. This algorithm finds the minimal length sequence of operations (insert, remove, replace) to transform one string to another.

# Setup
Program can be run via Unix CLI. First run `make` to create the binary file `s2s`.
```
$ make
gcc -Wall -pedantic -std=c99 -c main.c
gcc -Wall -pedantic -std=c99 -o s2s main.o
$
```
## Input
Requires a file that contains two lines separated by Unix newline character.
eg.
```
String1\n
String2
```
### CLI usage
```
Usage: s2s inputfile outputfile
```
Where
- `s2s`: is the program name.
- `inputfile`: is the name of the file containing the two strings.
- `outputfile`: is the name of the output file for the list of operations.

# Reading the Output
The program will dump the list of operations required to transform String1 to String2 in the specified output file. The operations are represented as:
- `d:position`: Delete character at position (eg d:0  would delete the first character)
- `i:position:char`: Insert char before position.  (eg i:0:B      would turn "AC" into "BAC")
- `c:position:char`: Change the character at position to char  (eg: c:1:Z would turn "ABC" into "AZC")
