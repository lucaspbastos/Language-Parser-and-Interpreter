/*
 * pt.h
 *
 * Lucas Bastos
 * CS280
 * Spring 2020
 */

#ifndef VALUE_H
#define VALUE_H

#include <iostream>
#include <string>
using namespace std;

enum ValType { VINT, VSTR, VERR };

class Value {
    ValType	T;
	int		I;
    string	S;
    
    public:
        Value() : T(VERR), I(0) {}
        Value(int vi) : T(VINT), I(vi) {}
        Value(string vs) : T(VSTR), I(0), S(vs) {}
        
        ValType GetType() const { return T; }
        bool IsErr() const { return T == VERR; }
        bool IsInt() const { return T == VINT; }
        bool IsStr() const { return T == VSTR; }
        
        int GetInt() const { if( IsInt() ) return I; throw "RUNTIME ERROR: Value not an integer"; }
        string GetStr() const { if( IsStr() ) return S; throw "RUNTIME ERROR: Value not a string"; }
            
        // add op to this
        Value operator+(const Value& op) const {
            if (op.IsErr() || IsErr()) return Value();
            if( op.IsInt() && IsInt()) return Value(I + op.I);
            else if( op.IsStr() && IsStr() ) return Value(S + op.S);
            return Value();
        }
        
        // subtract op from this
        Value operator-(const Value& op) const {
            if (op.IsErr() || IsErr()) return Value();
            if( op.IsInt() && IsInt()) return Value(I - op.I);
            else if( op.IsStr() && IsStr()) {
                if (S.find(op.S) != string::npos) {
                    return Value(S.substr(0,S.find(op.S))+S.substr(S.find(op.S)+op.S.length(),S.length()-1));
                } else
                    return Value(S);
            }
            return Value();
        }
        
        // multiply this by op
        Value operator*(const Value& op) const {
            if (op.IsErr() || IsErr()) return Value();
            int count = 0, max = 0;
            string ret = "", add = "";
            if( op.IsInt() && IsInt()) return Value(I * op.I);
            else if( op.IsStr() && IsInt()) {
                ret = add = op.S;
				max = I-1;
            } else if (op.IsInt() && IsStr()) {
                ret = add = S;
                max = op.I-1;
            } else {
                throw "ERROR";
            }
            while(count++ < max) {
                ret += add;
            }
            return Value(ret);
        }
        
        // divide this by op
        Value operator/(const Value& op) const {
            if (op.IsErr() || IsErr()) return Value();
            if( op.IsInt() && IsInt()) return Value(I / op.I);
            return Value();
        }
        
        friend ostream& operator<<(ostream& out, const Value& op) {
            if( op.IsInt() ) out << op.I;
            else if( op.IsStr() ) out << op.S;
            else out << "ERROR";
            return out;
        }
};

#endif