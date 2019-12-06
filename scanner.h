#ifndef scanner_h
#define scanner_h

#include <stdio.h>
#include "token.h"
#include <iostream>

token scanner(FILE *input_file, int &lineNum);
int getColumnIndex(char lookAhead);
std::string enumToString(int e);


#endif
