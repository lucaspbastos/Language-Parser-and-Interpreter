/*
 * lexeme.cpp
 *
 * Lucas Bastos
 * CS280
 * Spring 2020
 */
#include "lex.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
using namespace std;

string tokens[] = {"PRINT","PRINTLN","REPEAT", "BEGIN", "END", "IDENT", "ICONST", "SCONST", "PLUS", "MINUS", "STAR", "SLASH", "EQ", "LPAREN", "RPAREN", "SC", "ERR", "DONE"};
void endprint(bool checker, map<string, int> &map, string out) {
    if (checker && map.size() > 0) {
        cout << out;
        int i = 0;
        for (auto it=map.begin();it!=map.end();it++) {
            cout << " " << it->first;
            if (++i < (int)map.size()) cout << ",";
        }
        cout << endl;   
    }
}

Tok establish(string &lexeme, int linenum) {
    string lower = lexeme;
    transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    if (lower == "print") {
        return Tok(PRINT, lexeme, linenum);
    } else if (lower == "println") {
        return Tok(PRINTLN, lexeme, linenum);
    } else if (lower == "repeat") {
        return Tok(REPEAT, lexeme, linenum);
    } else if (lower == "begin") {
        return Tok(Token::BEGIN, lexeme, linenum);
    }  else if (lower == "end") {
        return Tok(END, lexeme, linenum);
    } else if (isalpha(lexeme[lexeme.length()-1]) || isdigit(lexeme[lexeme.length()-1])){
        return Tok(IDENT, lexeme, linenum);
    } else {
        return Tok(ERR, lexeme, linenum);
    }
}

ostream& operator<<(ostream& out, const Tok& tok) {
    out << tokens[tok.GetToken()];
    return out;
}

Tok getNextToken(istream& in, int& linenum) {
    enum TokState {BEGIN, INID, INSTRING, ININT, INCOMMENT};
    TokState lexstate = BEGIN;
    string lexeme;
    char ch;
    bool escape = false;
    while(in.get(ch)) {
        switch(lexstate) { 
            case BEGIN:
                if( ch == '\n' ) { 
                    linenum++;
                    continue;
                }
                if(isspace(ch)) continue;
                lexeme = ch;
                if(isalpha(ch)) { 
                    if (isalpha((char)in.peek()) || isdigit((char)in.peek())) {
                        lexstate = INID;
                        continue;
                    }
                    return Tok(IDENT, lexeme, linenum);
                } else if (isdigit(ch)) {
                    if (isdigit((char)in.peek())) {
                        lexstate = ININT;
                        continue;
                    }
                    return Tok(ICONST, lexeme, linenum);
                } else if (ch == '"') {
                    lexstate = INSTRING;
                } else if (ch == '/') {
                    if ((char)in.peek()=='/') {
                        lexstate = INCOMMENT;
                        continue;
                    }
                    return Tok(SLASH, lexeme, linenum);
                } else if (ch == '-') {
                    return Tok(MINUS, lexeme, linenum);
                } else if (ch == '+') {
                    return Tok(PLUS, lexeme, linenum);
                } else if (ch == '*') {
                    return Tok(STAR, lexeme, linenum);
                } else if (ch == '=') {
                    return Tok(EQ, lexeme, linenum);
                } else if (ch == '(') {
                    return Tok(LPAREN, lexeme, linenum);
                } else if (ch == ')') {
                    return Tok(RPAREN, lexeme, linenum);
                } else if (ch == ';') {
                    return Tok(SC, lexeme, linenum);
                } else {
                    return Tok(ERR, lexeme, linenum);
                }
                break;
            case INID:
                if ((isdigit(ch) || isalpha(ch))) lexeme += ch;
                if (!isdigit((char)in.peek()) && !isalpha((char)in.peek())) {
                    lexstate = BEGIN;
                    return establish(lexeme, linenum);
                }
                break;
            case INSTRING:
                if (!escape) {
                    if (ch == '\\') {
                        escape = true;
                        continue;
                    }
                    lexeme += ch;
                    if (ch == '"') {
                        lexstate = BEGIN;
                        return Tok(SCONST, lexeme, linenum);
                    }
                    if (ch == '\n') return Tok(ERR, lexeme, linenum);
                } else {
                    if (ch == 'n') {
                        lexeme += '\n';
                    } else {
                        lexeme += ch;
                    }
                    escape = false;
                }
                break;
            case ININT:
                if ((isdigit(ch))) lexeme += ch;
                if (!isdigit((char)in.peek())) {
                    lexstate = BEGIN;
                    return Tok(ICONST, lexeme, linenum);
                }
                break;
            case INCOMMENT:
                if (ch == '\n') {
                    lexstate = BEGIN;
                    linenum++;
                } else {
                    lexeme = "";
                }
                break;
        }
    }
    if (in.eof()) return Tok(DONE, lexeme, linenum);
    return establish(lexeme, linenum);
}