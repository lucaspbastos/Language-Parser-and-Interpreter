/*
 * pt.h
 *
 * Lucas Bastos
 * CS280
 * Spring 2020
 */

#ifndef PT_H_
#define PT_H_

#include <iostream>
#include <vector>
#include <map>
#include <string>

using std::vector;
using std::map;
using std::string;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;

#include "parse.h"
#include "lex.h"
#include "val.h"

class Pt {
    protected:
        int	linenum;
        Pt	*left;
        Pt	*right;

    public:
        Pt(int linenum, Pt *l = 0, Pt *r = 0)
            : linenum(linenum), left(l), right(r) {}

        virtual ~Pt() {
            delete left;
            delete right;
        }

        int GetLineNumber() const { return linenum; }
        virtual bool isPlus() const { return false; }
        virtual bool isEq() const { return false; }
        virtual bool isIdent() const { return false; }
        virtual bool isVar() const { return false; }
        virtual string getId() const { return ""; }
        virtual Value Eval(map<string, Value>& sysms)=0;
        Pt *getLeft() { return left; }
        Pt *getRight() { return right; }
};

class StmtList : public Pt {
    public:
        StmtList(Pt *l, Pt *r) : Pt(0, l, r) {}
        Value Eval(map<string, Value>& symbols) {
            left->Eval(symbols);
            if (right) right->Eval(symbols);
            return Value();
        }
};

class Print : public Pt {
    public:
        Print(int line, Pt *e) : Pt(line, e) {}
        Value Eval(map<string, Value>& symbols) {
            try {
                cout << left->Eval(symbols);
            } catch (const char* e) {
                cout << e << endl;
            }
            return Value();
        }
};

class Println : public Pt {
    public:
        Println(int line, Pt *e) : Pt(line, e) {}
        Value Eval(map<string, Value>& symbols) {
            try {
                cout << left->Eval(symbols)<<endl;
            } catch (const char* e) {
                cout << e << endl;
            }
            return Value();
        }
};

class Repeat : public Pt {
    public:
        Repeat(int line, Pt *l, Pt *r) : Pt(line, l, r) {}
        Value Eval(map<string, Value>& symbols) {
            Value ex = left->Eval(symbols);
            int count = 0, max = 0;
            try {
                if (ex.IsStr()) throw "RUNTIME ERROR at "+to_string(left->GetLineNumber()+1)+": control expression not an integer";
                if(left->isIdent()) {
                    max = symbols[left->getId()].GetInt();
                } else {
                    max = ex.GetInt();
                }
                while (count++ < max) {
                    right->Eval(symbols);
                }
            } catch (string e) {
                cout << e << endl;
            }
            return Value();
        }

};

class Assign : public Pt {
    public:
        Assign(int line, Pt *l, Pt *r) : Pt(line, l, r) {}
        Value Eval(map<string, Value>& symbols) {
            try {
                if (left->isIdent()) symbols[left->getId()] = right->Eval(symbols);
                return symbols[left->getId()];
            } catch (const char* e) {
                cout << e << endl;
            }
            return Value();
        }
        bool isEq() const { return true; }
};

class PlusExpr : public Pt {
    public:
        PlusExpr(int line, Pt*l, Pt *r) : Pt(line, l, r) {}
        Value Eval(map<string, Value>& symbols) {
            try {
                return left->Eval(symbols)+right->Eval(symbols);
            } catch(const char* e) {
                throw "RUNTIME ERROR at "+to_string(left->GetLineNumber()+1)+": Type error on addition";
            }
            return Value();
        }
        bool isPlus() const { return true; } 
};

class MinusExpr : public Pt {
    public:
        MinusExpr(int line, Pt*l, Pt *r) : Pt(line, l, r) {}
        Value Eval(map<string, Value>& symbols) {
            try {
                return left->Eval(symbols)-right->Eval(symbols);
            } catch(const char* e) {
                throw "RUNTIME ERROR at "+to_string(left->GetLineNumber()+1)+": Type error on subtraction";
            }
            return Value();
        }
};

class TimesExpr : public Pt {
    public:
        TimesExpr(int line, Pt*l, Pt *r) : Pt(line, l, r) {}
        Value Eval(map<string, Value>& symbols) {
            try {
                return left->Eval(symbols)*right->Eval(symbols);
            } catch(const char* e) {
                throw "RUNTIME ERROR at "+to_string(left->GetLineNumber()+1)+": Type error on multiplication";
            }
            return Value();
        }
};

class DivideExpr : public Pt {
    public:
        DivideExpr(int line, Pt*l, Pt *r) : Pt(line, l, r) {}
        Value Eval(map<string, Value>& symbols) {
            try {
                return left->Eval(symbols)/right->Eval(symbols);
            } catch(const char* e) {
                throw "RUNTIME ERROR at "+to_string(left->GetLineNumber()+1)+": Type error on division";
            }
            return Value();
        }
};

class IConst : public Pt {
    int val;
    public:
        IConst(Tok& t) : Pt(t.GetLinenum()) {
            val = stoi(t.GetLexeme());
        }
        Value Eval(map<string, Value>& symbols) {
            return Value(val);
        }
};

class SConst : public Pt {
    string val;
    public:
        SConst(Tok& t) : Pt(t.GetLinenum()) {
            val = t.GetLexeme().substr(1,t.GetLexeme().length()-2);
        }
        Value Eval(map<string, Value>& symbols) {
            return Value(val);
        }
    
};

class Ident : public Pt {
    string val;
    int line;
    public:
        Ident(Tok& t) : Pt(t.GetLinenum()) {
            val = t.GetLexeme();
            line = t.GetLinenum();
        }
        Value Eval(map<string, Value>& symbols) {
            for (auto i = symbols.find(val); i != symbols.end(); i++) {
                return symbols[val];
            }
            throw "RUNTIME ERROR at "+to_string(line+1)+": Variable used but not assigned";
        }
        bool isIdent() const { return true; } 
        bool isVar() const { return true; } 
        string getId() const { return val; }     
};

#endif /* PT_H_ */