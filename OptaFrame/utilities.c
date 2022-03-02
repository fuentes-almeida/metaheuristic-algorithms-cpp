#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utilities.h"

int splitStringByDelimiter(String line, String delim, int *num_tokens, int *start_idx, int *end_idx){

    char *start_ptr = line;
    char *end_ptr = NULL, *iter = NULL;
    int tokens_found=0;

    while ((end_ptr = strstr(start_ptr, delim))){   //strtr() will return NULL or a pointer to 1st occurrence of DELIMITER
                                                         //Thus [start,end] will be moving through each piece of data
        start_idx[tokens_found] = start_ptr - line;    //Store the Index of current token
        end_idx[tokens_found] = end_ptr - line;        //Store Index of first character that will be replaced with
                                                         //'\0'. Example: "arg1||arg2||end" -> "arg1\0|arg2\0|end"
        tokens_found++;                                  //Accumulate the count of tokens.
        start_ptr = end_ptr + 1;                         //Set pointer to the next c-string within the line
    }

    for (iter=start_ptr; (*iter!='\0'); iter++);
    start_idx[tokens_found] = start_ptr - line;        //Store the Index of current token: of last token here.
    end_idx[tokens_found] = iter - line;               //and the last element that will be replaced with \0
    tokens_found++;

    *num_tokens = tokens_found;
    int line_size = iter - line;    //Saving CPU cycles: Indirectly Count the size of *line without using strlen();

    return line_size;
}
