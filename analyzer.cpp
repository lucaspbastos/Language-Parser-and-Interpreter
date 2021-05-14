/*
 * analyzer.cpp
 *
 * Lucas Bastos
 * CS280
 * Spring 2020
 */
#include "lexeme.cpp"

int main(int argc, char *argv[]) {
    istream *in = &cin;
    int lineNumber = 0, fileCount = 0, tokenCount = 0;
    Tok tok;
    bool printToken = false, integerNumeric = false, stringAlphabetical = false, identifierAlphabetical = false;
    string filename;
    for (int i = 1; i < argc; i++) {
        string flag = argv[i];
        if(flag[0] == '-') {
            if (flag == "-v") { 
                printToken = true;
            } else if (flag == "-sconsts") {
                stringAlphabetical = true;
            } else if (flag == "-iconsts") {
                integerNumeric = true;
            } else if (flag == "-ids") {
                identifierAlphabetical = true;
            } else {
                cout << "UNRECOGNIZED FLAG " << flag << endl;
                return 0;
            }
        } else {
            if (fileCount == 0 && i==argc-1) {
                filename = flag;
                fileCount++;
            } else {
                cout << "ONLY ONE FILENAME ALLOWED" << endl;
                return 0;
            }
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
    map<string, int> ICONSTS;
    map<string, int> SCONSTS;
    map<string, int> IDS;
    while ( (tok = getNextToken(*in, lineNumber)) != DONE && tok != ERR ) {
        if (printToken) cout << tok << endl;
        if (integerNumeric && tok.GetToken()==6) ICONSTS[tok.GetLexeme()]++;
        if (stringAlphabetical && tok.GetToken()==7) SCONSTS[tok.GetLexeme()]++;
        if (identifierAlphabetical && tok.GetToken()==5) IDS[tok.GetLexeme()]++;
        tokenCount++;
        cout << tok.GetToken();
    }
    if (tok.GetToken() == 16) {
        cout << "Error on line " << tok.GetLinenum()+1 << " (" << tok.GetLexeme() << ")" << endl;
    } else if (tok.GetToken() == 17) {
        cout << "Lines: " << lineNumber << endl; 
        if (lineNumber > 0) cout << "Tokens: " << tokenCount << endl;
    }
    endprint(stringAlphabetical, SCONSTS, "STRINGS:");
    endprint(integerNumeric, ICONSTS, "INTEGERS:");
    endprint(identifierAlphabetical, IDS, "IDENTIFIERS:");
}