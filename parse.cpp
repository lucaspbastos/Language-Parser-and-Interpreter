/*
 * parse.cpp
 *
 * Lucas Bastos
 * CS280
 * Spring 2020
 */
#include "parse.h"

namespace Parser {
	bool pushed_back = false;
	Tok	pushed_token;

	static Tok GetNextToken(istream& in, int& line) {
		if( pushed_back ) {
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(Tok& t) {
		if( pushed_back ) {
			abort();
		}
		pushed_back = true;
		pushed_token = t;
	}

}

static int error_count = 0;

void ParseError(int line, string msg) {
	++error_count;
	cout << line << ": " << msg << endl;
}

Pt *Prog(istream& in, int& line) {
	Pt *sl = Sl(in, line);
	if( sl == 0 ) {
		ParseError(line, "No statements in program");
		return 0;
	}
	if( error_count )
		return 0;
	return sl;
}

Pt *Sl(istream& in, int& line) {
	Tok t1 = Parser::GetNextToken(in, line);
	if ( t1 == SC) {
		return new StmtList(0,Sl(in,line));
	} 
	Parser::PushBackToken(t1);
    int l = line;
	Pt *s = Stmt(in, line);
	if( s == 0 ) {
		delete s;
		return 0;
	}
	Tok t2 = Parser::GetNextToken(in, line);
	if( t2 != SC ) {
		ParseError(l, "Missing semicolon");
		delete s;
		return 0;
	}
	return new StmtList(s, Sl(in,line));
}

Pt *Stmt(istream& in, int& line) {
	int l = line;
	Tok t = Parser::GetNextToken(in, line);
	if (t == PRINT) {
		Pt *p = PrintStmt(in, line);
		if (p == 0) {
			ParseError(l, "Missing print");
			delete p;
			return 0;
		}
		return p;
	} else if (t == PRINTLN) {
		Pt *pl = PrintlnStmt(in, line);
		if (pl == 0) {
			ParseError(l, "Missing println");
			delete pl;
			return 0;
		}
		return pl;
	} else if (t == REPEAT) {
		Pt *r = RepeatStmt(in, line);
		if (r == 0) {
			ParseError(l, "Missing statement for repeat");
			delete r;
			return 0;
		}
		return r;
	} else if (t == IDENT) {
		Parser::PushBackToken(t);
		Pt *ex = Expr(in, line);
		if (ex == 0) {
			ParseError(l, "Missing expr");
			delete ex;
			return 0;
		}
		return ex;
	} else if (t == DONE) {
		return 0;
	} else {
		cout <<"invalid"<<endl;
		Parser::PushBackToken(t);
		ParseError(l, "Invalid Statement");
		return 0;
	}
}

Pt *PrintStmt(istream& in, int& line) {
	int l = line;
    Pt *ex = Expr(in, line);
	if( ex == 0 ) {
		ParseError(line, "Missing expression after print");
		return 0;
	}
	return new Print(l, ex);
}

Pt *PrintlnStmt(istream& in, int& line) {
    int l = line;
	Pt *ex = Expr(in, line);
	if( ex == 0 ) {
		ParseError(line, "Missing expression after println");
        delete ex;
		return 0;
	}
	return new Println(l, ex);
}

Pt *RepeatStmt(istream& in, int& line) {
	Pt *ex = Expr(in, line);
    if( ex == 0 ) {
		ParseError(line, "Missing expression");
		return 0;
	}
	Tok t1 = Parser::GetNextToken(in, line);
	if( t1 != BEGIN) {
		Parser::PushBackToken(t1);
		ParseError(line, "Missing begin after expression");
		return 0;
	}
	Pt *s = Stmt(in, line);
	if( s == 0 ) {
		ParseError(line, "Missing statement after begin");
		return 0;
	}
	Tok t2 = Parser::GetNextToken(in, line);
	if( t2 != END) {
		Parser::PushBackToken(t2);
		ParseError(line, "Missing end after statement");
		return 0;
	}
    return new Repeat(line, ex, s);
}

Pt *Expr(istream& in, int& line) {
	Pt *s1 = Sum(in, line);
	if (s1 == 0 ) return 0;
	Tok t = Parser::GetNextToken(in, line);
	if( t != EQ) {
		Parser::PushBackToken(t);
		return s1;
	}
	Pt *e = Expr(in, line);
	if(e==0) return 0;
	s1 = new Assign(t.GetLinenum(), s1, e);
	return s1;
}

Pt *Sum(istream& in, int& line) {
	Pt *p1 = Prod(in, line);
	if( p1 == 0 ) return 0;
	while ( true ) {
		Tok t = Parser::GetNextToken(in, line);
		if( t != PLUS && t != MINUS ) {
			Parser::PushBackToken(t);
			return p1;
		}
		Pt *p2 = Prod(in, line);
		if( p2 == 0 ) {
			ParseError(line, "Missing expression after operator");
            delete p2;
			return 0;
		}
		if( t == PLUS ) {
			p1 = new PlusExpr(t.GetLinenum(), p1, p2);
		} else {
			p1 = new MinusExpr(t.GetLinenum(), p1, p2);
		}
	}
	return 0;
}

Pt *Prod(istream& in, int& line) {
	Pt *p1 = Primary(in, line);
	if (p1 == 0) return 0;
	while ( true ) {
		Tok t = Parser::GetNextToken(in, line);
		if( t != STAR && t != SLASH ) {
			Parser::PushBackToken(t);
			return p1;
		}
		Pt *p2 = Primary(in, line);
		if( p2 == 0 ) {
			ParseError(line, "Missing primary after operator");
            delete p2;
			return 0;
		}
		if( t == STAR )
			p1 = new TimesExpr(t.GetLinenum(), p1, p2);
		else
			p1 = new DivideExpr(t.GetLinenum(), p1, p2);
    }
	return 0;
}

Pt *Primary(istream& in, int& line) {
	Tok t = Parser::GetNextToken(in, line);
	if( t == IDENT ) {
		return new Ident(t);
	} else if (t == ICONST) {
		return new IConst(t);
	} else if (t == SCONST) {
		return new SConst(t);
	} else if (t == LPAREN) {
		Pt *e = Expr(in, line);
		if (e == 0) {
            ParseError(line, "Missing expression after left parenthesis");
			Parser::PushBackToken(t);
            delete e;
			return 0;
		}
		Tok t2 = Parser::GetNextToken(in, line);
		if (t2 != RPAREN) {
			Parser::PushBackToken(t2);
			ParseError(line, "Missing parenthesis after expression");
			return 0;
		}
		return e;
	} else {
		ParseError(line, "Invalid primary");
		return 0;
	}
}