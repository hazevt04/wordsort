#Wordsort

ws <options> <optional filename>
An options can be:
-c <n>      Print the first n results of the sorted list of words
-r          Print the list of words in sorted in reverse order
-n          Print the list of words sorted as if they were numbers
-l          Print the list of words sorted by lengths
-s          Print the list of words sorted by their Scrabble scores
-a          Print the list of words sorted lexicographically
-u          Print the list of words sorted lexicographically with no duplicates
-h          Print help

Most recently seen option will take precedence. Example:
'ws -r -l file1' will print the words in file1 sorted by their lengths

Each pair of -r options cancels each other out.

Trailing punctuation is considered part of the word.

If no filename is provided then the words to be sorted will be entered
at the command line. Cntrl+D when done.



