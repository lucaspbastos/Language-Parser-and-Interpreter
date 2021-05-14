/*
 * recursiveParser.cpp
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
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
int pluscount = 0, eqcount = 0, maxdepth = 0;

#include <cxxabi.h>
//Demangles class names when printing them out for debugging.
string demangle(const char* name) {
    int status;
    char* demangledName = abi::__cxa_demangle(name, nullptr, nullptr, &status);
    string retVal = string(demangledName);
    free(demangledName);
    return retVal;
}

// Prints out the parse tree if debug is enabled.
void debugPrint(Pt* tree, int level = 0) {
    static const string typeNames[] = {"ERRTYPE", "INTTYPE", "STRTYPE"};
    const string className = demangle(typeid(*tree).name());
    Pt* currTree = tree->left;
    cout << string(level, '\t') << className << string(8 - level - className.length() / 4, '\t')<<endl; //<< typeNames[tree->getId()] << '\n';
    if (currTree)
        debugPrint(currTree, level + 1);
    currTree = tree->right;
    if (currTree)
        // Fragile code, but it's for debugging anyway.
        debugPrint(currTree, level + (demangle(typeid(*tree).name()) != "Sl"));
}

int getInfo(Pt *root) {
    if (root == 0) return 0;
    if (root->isPlus()) pluscount++;
    if (root->isEq()) eqcount++;
    int leftDepth = getInfo(root->getLeft());
    int rightDepth = getInfo(root->getRight());
    if (leftDepth > rightDepth) {
        return leftDepth + 1;
    } else {
        return rightDepth + 1;
    }
}

int main(int argc, char *argv[]) {
    istream *in = &cin;
    int lineNumber = 0, fileCount = 0;
    string filename;
    for (int i = 1; i < argc; i++) {
        string flag = argv[i];
        if (fileCount == 0 && i==argc-1) {
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
    //debugPrint(tree);
    maxdepth = getInfo(tree);
    if (tree != 0) {
        cout << "PLUS COUNT: " << pluscount << endl;
        cout << "EQ COUNT: " << eqcount << endl;
        cout << "MAX DEPTH: " << maxdepth << endl;
    }
}