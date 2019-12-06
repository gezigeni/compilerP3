#include <iostream>
#include <string.h>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <ctype.h>
#include "scanner.h"
#include "parser.h"
#include "staticSem.h"

using namespace std;

int main(int argc, const char * argv[]) {

    char ch;
    string str;
    int char1;
    FILE *fp;
    fp = fopen ("test.txt" , "r");

    if( argc > 1) {
        ofstream myfile("test.txt");
        string fName = argv[1];
        fName.append(".input1");
        ifstream inFile(fName.c_str());
        string temp;
        FILE * fpFile = fopen(fName.c_str(),"r");
        while((char1 = fgetc(fpFile)) != EOF){
                myfile << static_cast <char>(char1);
        }
        inFile.close();
        myfile.close();
	}
        if(argc == 1) {
                cout << "Please enter strings" << endl;
                while ((ch = getchar()) != EOF) {
                       str = str + ch;
                }
                ofstream myfile("test.txt");
                myfile << str;
                myfile.close();
        }

        node_t* root = parser(fp);
	Initialize();
        checkSem(root); 
	printf("\nFinished semantic check, no issues.\n");	

        return 0;
}
