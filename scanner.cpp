#include "scanner.h"

using namespace std;

struct table_value{
    int key;
    int value;
};

typedef struct {
    int key;
    tokenID instance;
}tkMap;

std::string token_names[] = {
    "Identifier",
    "Number",
    "Begin keyword",
    "End keyword",
    "Loop keyword",
    "Void keyword",
    "Int keyword",
    "Return keyword",
    "Read keyword",
    "Output keyword",
    "Program keyword",
    "IFF keyword",
    "Then keyword",
    "Let keyword",
    "Operators and Delimiters",
    "End of File",
    "Error"
};

table_value match_instance[18] = {
    {'=', 2},
    {'<', 2},
    {'>', 2},
    {':', 2},
    {'+', 2},
    {'-', 2},
    {'*', 2},
    {'/', 2},
    {'%', 2},
    {'.', 2},
    {'(', 2},
    {')', 2},
    {',', 2},
    {'{', 2},
    {'}', 2},
    {';', 2},
    {'[', 2},
    {']', 2}
};

token keywords[12] = {
    {Begin_tk, "Begin", 0},
    {End_tk, "End", 0},
    {Loop_tk, "Loop", 0},
    {Void_tk, "Void", 0},
    {INT_tk, "INT", 0},
    {Return_tk, "Return", 0},
    {Read_tk, "Read", 0},
    {Output_tk, "Output", 0},
    {Program_tk, "Program", 0},
    {IFF_tk, "IFF", 0},
    {Then_tk, "Then", 0},
    {Let_tk, "Let", 0}
};

string enumToString(int e){
    switch (e) {
        case 0:
            return "Ident_tk";
            break;
        case 1:
            return "Num_tk";
            break;
        case 2:
            return "Begin_tk";
            break;
        case 3:
            return "End_tk";
            break;
        case 4:
            return "Loop_tk";
            break;
        case 5:
            return "Void_tk";
            break;
        case 6:
            return "INT_tk";
            break;
        case 7:
            return "Return_tk";
            break;
        case 8:
            return "Read_tk";
            break;
        case 9:
            return "Output_tk";
            break;
        case 10:
            return "Program_tk";
	    break;
        case 11:
            return "IFF_tk";
            break;
        case 12:
            return "Then_tk";
            break;
        case 13:
            return "Let_tk";
            break;
        case 14:
            return "OPDEL_tk";
            break;
        case 15:
            return "EOF_tk";
            break;
        default:
            break;
    }
    return "";
}

tkMap tokens[4] = {
    {1003,  EOF_tk},
    {1000, IDENT_tk},
    {1001, NUM_tk},
    {1002, OPDEL_tk}
};

int state_table[4][5] = {
    {1,     3,    2,    1003,     0},
    {1,     1,   -1,    -1,       1000},
    {-1,   -1,    -1,    -1,       1002},
    {-1,    3,   -1,    -1,       1001}
};

int getColumnIndex(char lookAhead) {
for (int i = 0; i < 18; i++) {
        if(match_instance[i].key == lookAhead) {
            return match_instance[i].value;
        }
    }
if (isalpha(lookAhead)) {
        return 0;
    } else if (isdigit(lookAhead)) {
        return 1;
    } else if (isspace(lookAhead)) {
        return 4;
    } else if (lookAhead == EOF) {
        return 3;
    } else {
        return 5;
    }
}
token scanner(FILE *input_file, int &lineNum) {

    int lookahead;
    int column_index;
    int current_state = 0;
    int next_state = 0;
    int size_of_identifier = 0;
    std::string instance;
    token the_token;

while (current_state < 1000 && current_state >= 0) {
        lookahead = fgetc(input_file);
        if(lookahead == '\\') {
            while (lookahead != '\n' ){

                lookahead = fgetc(input_file);
            }
            lineNum++;
            lookahead = fgetc(input_file);
        }
        column_index = getColumnIndex(lookahead);
        next_state = state_table[current_state][column_index];

        if (next_state >= 1000) {


            if (next_state == 1003) {
                ungetc(lookahead, input_file);
                the_token.tokenId = EOF_tk;
                the_token.lineNum = lineNum;
                the_token.instance = "EOF";
                return the_token;
            } else if (next_state == 1002) {
                ungetc(lookahead, input_file);
		the_token.tokenId = OPDEL_tk;
                the_token.lineNum = lineNum;
                the_token.instance = instance;
                return the_token;
            } else if (next_state == 1001) {
                ungetc(lookahead, input_file);
                the_token.tokenId = NUM_tk;
                the_token.lineNum = lineNum;
                the_token.instance = instance;
                return the_token;
            } else if (next_state == 1000) {
                ungetc(lookahead, input_file);
                the_token.tokenId = IDENT_tk;
                the_token.lineNum = lineNum;
                the_token.instance = instance;
                for(int i = 0; i < 12; i++){
                    if (keywords[i].instance==the_token.instance) {
                        the_token.tokenId = keywords[i].tokenId;
                    }
                }
                return the_token;
            }
            else {
                std::cout << "Scanner Error: Line " << lineNum << " value: " << static_cast<char>(lookahead) << "\n";
                the_token.tokenId = ERR_tk;
                the_token.lineNum = lineNum;
                the_token.instance = "Error";
		return the_token;
            }
        } else {
        char value = static_cast<char>(lookahead);
        if (!isspace(value)) {
                instance += value;
                size_of_identifier++;
                if(size_of_identifier > 8){
                    the_token.tokenId = ERR_tk;
                    the_token.lineNum = lineNum;
                    the_token.instance = "Scanner Error: Identifier of more than 8 characters.\n";
                    return the_token;
                }
            }
            current_state = next_state;
            if(lookahead==10){
                lineNum++;
            }


        }

    }
    the_token.tokenId = ERR_tk;
    the_token.lineNum = lineNum;
    the_token.instance = "Scanner Error";
    return the_token;
}
