/*
 * main.cpp
 *
 * Lucas Bastos
 * CS280
 * Spring 2020
 */
#include "lex.h"
#include "parse.h"
#include "pt.h"
#include "lexeme.cpp"
#include "parse.cpp"
#include "val.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
map<string, Value> symbols;

int main(int argc, char *argv[]) {
    istream *in = &cin;
    int lineNumber = 0, fileCount = 0;
    string filename;
    for (int i = 1; i < argc; i++) {
        string flag = argv[i];
        if (fileCount == 0 && i == argc-1) {
            filename = flag;
            fileCount++;
        } else {
            cout << "ONLY ONE FILE NAME ALLOWED" << endl;
            return 0;
        }
    }

	ifstream input(filename);
    if (fileCount == 1) {
        if (!input.is_open()) {
            cout << "CANNOT OPEN " << filename << endl;
            return 0;
        }
        in = &input;
    }

    Pt *tree = Prog(*in, lineNumber);
    if (tree) {
        try {
            tree->Eval(symbols);
        } catch (const std::exception &e) {
            cout << e.what() << endl;
        }
    }
}

