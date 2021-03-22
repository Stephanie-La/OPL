/* Complete recursive descent parser for the calculator language.
    Builds on figure 2.16.  Prints a trace of productions predicted and
    tokens matched.  Does no error recovery: prints "syntax error" and
    dies on invalid input.
    Michael L. Scott, 2008-2020.
*/

#include <iostream>
#include <exception>
#include <string>
#include <cstdlib>
#include <cstdio>
#include "scan.h"

using namespace std;


string names[21] = {"read", "write", "if", "while", "end", "id", "literal", "gets",
                       "add", "sub", "mul", "div", "eq", "neq", "lt", "gt", "loe", "goe", "lparen", "rparen", "eof"};

static token input_token;

class Syntax_Error : public std::exception{
	public:
		virtual const char * what() const throw(){
			return "syntax error";
	}
};

void error () {
     cout << "syntax error" << endl;
    // exit (1);
}

void match (token expected) {
    if (input_token == expected) {
        cout << "matched " <<  names[input_token];
        if (input_token == t_id || input_token == t_literal)
	    cout << ": " << token_image;
	cout << endl;
	token_image.clear();
        input_token = scan ();
    }
      else{
	  error ();
	  throw Syntax_Error();
	// input_token = expected;
	}
}

void program ();
void stmt_list ();
void stmt ();
void cond();
void expr ();
void term ();
void term_tail ();
void factor ();
void factor_tail ();
void ro_op();
void add_op ();
void mul_op ();

void program () {
	try{
    	switch (input_token) {
        	case t_id:
        	case t_read:
        	case t_write:
		case t_if:
		case t_while:
        	case t_eof:
	    		cout << "predict program --> stmt_list eof" << endl;
            	stmt_list ();
           	 match (t_eof);
           	 break;
       		default:  
			error ();
			throw Syntax_Error();
	}
    }
	catch(Syntax_Error& error){
		while(true){
			switch(input_token){
				case t_id:
        			case t_read:
        			case t_write:
				case t_if:
				case t_while:
        			case t_eof:
					program();
					return;
				default: 
				input_token = scan();
			}
		}
	}
}

void stmt_list () {
	try{
    switch (input_token) {
        case t_id:
        case t_read:
        case t_write:
	case t_if:
	case t_while:
	    cout << "predict stmt_list --> stmt stmt_list" << endl;
            stmt ();
            stmt_list ();
            break;
        case t_eof:
	case t_end:
	    cout << "predict stmt_list --> epsilon" << endl;
            break;          /* epsilon production */
        default:  
		error ();
		throw Syntax_Error();
	}
    }
	catch(Syntax_Error& error){
		while(true){
			switch(input_token){
				case t_id:
        			case t_read:
        			case t_write:
				case t_if:
				case t_while:
					stmt_list();
					return;
				// Follow
				case t_eof:
				case t_end:
					return;
				default:
					input_token = scan();

			}
		}
	}
}

void stmt () {
	try{
    switch (input_token) {
        case t_id:
	    cout << "predict stmt --> id gets expr" << endl;
            match (t_id);
            match (t_gets);
            expr ();
            break;
        case t_read:
	    cout << "predict stmt --> read id" << endl;
            match (t_read);
            match (t_id);
            break;
        case t_write:
	    cout << "predict stmt --> write expr" << endl;
            match (t_write);
            expr ();
            break;
	case t_if:
	    cout << "predict stmt --> if cond stmt_list end" << endl;
	    match(t_if);
	    cond();
	    stmt_list();
	    match(t_end);
	    break;
	case t_while:
	    cout << "predict stmt --> while cond stmt_list end" << endl;
	    match(t_while);
	    cond();
	    stmt_list();
	    match(t_end);
	    break;
        default:  
		error();
		throw Syntax_Error();
	}
    }
	catch(Syntax_Error& error){
		while(true){
			switch (input_token){
				// First
				case t_id:
				case t_read:
				case t_write:
				case t_if:
				case t_while:
					stmt();
					return;
				// follow
				case t_eof:
					return;
				default:
					input_token = scan();
			}
		}	
	}
}

void cond(){	
	try{
	switch(input_token){
		case t_id:
		case t_literal:
		case t_lparen:
			cout << "predict cond --> expr rel_op expr" << endl;
			expr();
			ro_op();
			expr();
			break;
		default:  
			error();
			throw Syntax_Error();
	  	}
	}
	catch(Syntax_Error& error){
		while (true){
			switch (input_token){
				// First
				case t_lparen:
				case t_id:
				case t_literal:
					cond();
					return;
				// Follow
				case t_rparen:
				case t_if:
				case t_while:
				case t_read:
				case t_write:
				case t_eof:
					return;
				default:
					input_token = scan();
			}
		}
	}
}

void expr () {
	try{
    switch (input_token) {
        case t_id:
        case t_literal:
        case t_lparen:
	    cout << "predict expr --> term term_tail" << endl;
            term ();
            term_tail ();
            break;
        default:  
		error ();
		throw Syntax_Error();
    		}
	}
	catch(Syntax_Error& error){
		while(true){
			switch (input_token){
				// First
				case t_lparen:
				case t_id:
				case t_literal:
					expr();
					return;
				// Follow
				case t_rparen:
				case t_read:
				case t_write:
				case t_eof:
				case t_while: 
				case t_if:
				case t_end:
				case t_eq:
				case t_neq:
				case t_lt:
				case t_loe:
				case t_gt:
				case t_goe:
					return;
				default:
					input_token = scan();				
			}
		}
	}
}

void term () {
	try{
    switch (input_token) {
        case t_id:
        case t_literal:
        case t_lparen:
	    cout << "predict term --> factor factor_tail" << endl;
            factor ();
            factor_tail ();
            break;
        default:  
		error ();
		throw Syntax_Error();
	}
    }
	catch(Syntax_Error& error){
		while(true){
			switch(input_token){
				case t_id:
        			case t_literal:
        			case t_lparen:
					term();	
					return;
				// Follow 
				case t_add:
				case t_sub:
				case t_rparen:
				case t_read:
				case t_write:
				case t_eof:
				case t_if:
				case t_while:
				case t_end:
				case t_lt:
				case t_loe:
				case t_gt:
				case t_goe:
				case t_eq:
				case t_neq: 
					return;
				default:
					input_token = scan();

			}
		}	
	}
}

void term_tail () {
	try{
    switch (input_token) {
        case t_add:
        case t_sub:
	    cout << "predict term_tail --> add_op term term_tail" << endl;
            add_op ();
            term ();
            term_tail ();
            break;
        case t_rparen:
        case t_id:
        case t_read:
        case t_write:
	case t_if:
	case t_while:
	case t_end:
	case t_eq:
	case t_neq:
	case t_lt:
	case t_gt:
	case t_loe:
	case t_goe:
        case t_eof:
	    cout << "predict term_tail --> epsilon" << endl;
            break;          /* epsilon production */
        default:  
		error ();
		throw Syntax_Error();
	}
    }
	catch(Syntax_Error& error){
		while (true){
			switch (input_token){
				// First
				case t_add:
				case t_sub:
					term_tail();	
					return;
				// Follow
				case t_rparen:
        			case t_id:
        			case t_read:
        			case t_write:
				case t_if:
				case t_while:
				case t_end:
				case t_lt:
				case t_loe:
				case t_eq:
				case t_neq:
				case t_gt:
				case t_goe:
				case t_eof:
					return;
				default:
					input_token = scan();

			}
		}
	}
}

void factor () {
	try{
    switch (input_token) {
        case t_literal:
	    cout << "predict factor --> literal" << endl;
            match (t_literal);
            break;
        case t_id :
	    cout << "predict factor --> id" << endl;
            match (t_id);
            break;
        case t_lparen:
	    cout << "predict factor --> lparen expr rparen" << endl;
            match (t_lparen);
            expr ();
            match (t_rparen);
            break;
        default: 
		error ();
		throw Syntax_Error();
	}
    }
	catch (Syntax_Error& error){
		while (true){
			switch (input_token){
				// First
				case t_id:
				case t_literal:
				case t_lparen:
					factor();
					return;
				// Follow
				case t_add:
				case t_sub:
				case t_mul:
				case t_div:
				case t_rparen:
				case t_read:
				case t_write:
				case t_if:
				case t_while:
				case t_end:
				case t_gt:
				case t_goe:
				case t_eq:
				case t_neq:
				case t_loe:
				case t_lt: 
				case t_eof:
					return;
				default:
					input_token = scan();
	
			}
		}
	}
}

void factor_tail () {
	try{
    switch (input_token) {
        case t_mul:
        case t_div:
	    cout << "predict factor_tail --> mul_op factor factor_tail" << endl;
            mul_op ();
            factor ();
            factor_tail ();
            break;
        case t_add:
        case t_sub:
        case t_rparen:
        case t_id:
        case t_read:
        case t_write:
	case t_if:
	case t_while:
	case t_end:
	case t_eq:
	case t_neq:
	case t_lt:
	case t_gt:
	case t_loe:
	case t_goe:
        case t_eof:
	    cout << "predict factor_tail --> epsilon" << endl;
            break;          /* epsilon production */
        default:  
		error ();
		throw Syntax_Error();
	}
    }
	catch (Syntax_Error& error){
		while (true){
			switch (input_token){
				// First
				case t_mul:
				case t_div:
					factor_tail();
					return;
				// follow
				case t_add:
        			case t_sub:
        			case t_rparen:
        			case t_id:
        			case t_read:
       				case t_write:
				case t_if:
				case t_while:
				case t_end:
				case t_lt:
				case t_loe:
				case t_eq:
				case t_neq:
				case t_gt:
				case t_goe:
				case t_eof:
					return;
				default:
					input_token = scan();
			}
		}
	}
}

void ro_op(){
	try{
	switch(input_token){
		case t_eq:
			cout << "predict ro_op --> eq" << endl;
			match(t_eq);
			break;
		case t_neq:
			cout << "predict ro_op --> neq" << endl;
                        match(t_neq);
                        break;
		case t_lt:
			cout << "predict ro_op --> lt" << endl;
                        match(t_lt);
                        break;
		case t_gt:
			cout << "predict ro_op --> gt" << endl;
                        match(t_gt);
                        break;
		case t_loe:
			cout << "predict ro_op --> loe" << endl;
                        match(t_loe);
                        break;
		case t_goe:
			cout << "predict ro_op --> goe" << endl;
                        match(t_goe);
                        break;
		default:  
			error();
			throw Syntax_Error();
		}
	}
	catch (Syntax_Error& error){
		while (true){
			switch (input_token){
				// First
				case t_eq:
				case t_neq:
				case t_lt:
				case t_gt:
				case t_loe:
				case t_goe:
					ro_op();
					return;
				// Follow
				case t_lparen:
				case t_id:
				case t_literal:
					return;
				default:
					input_token = scan();	
			}
		}
	}
}




void add_op () {
	try{
    switch (input_token) {
        case t_add:
	    cout << "predict add_op --> add" << endl;
            match (t_add);
            break;
        case t_sub:
	    cout << "predict add_op --> sub" << endl;
            match (t_sub);
            break;
        default:  
		error ();
		throw Syntax_Error();
	}
    }
	catch (Syntax_Error& error){
		while (true){
			switch (input_token){
				// First
				case t_add:
				case t_sub:
					add_op();
					return;
				// Follow
				case t_lparen:
				case t_id:
				case t_literal:
					return;
				default:
					input_token = scan();
			}
		}
	}
}

void mul_op () {
	try{
    switch (input_token) {
        case t_mul:
	    cout << "predict mul_op --> mul" << endl;
            match (t_mul);
            break;
        case t_div:
	    cout << "predict mul_op --> div" << endl;
            match (t_div);
            break;
        default:  
		error ();
		throw Syntax_Error();
	}
    }
	catch (Syntax_Error& error){
		while (true){
			switch (input_token){
				// First
				case t_mul:
				case t_div:
					mul_op();
					return;
				// Follow
				case t_lparen:
				case t_id:
				case t_literal:
					return;
				default:
					input_token = scan();
			}
		}
	}
}

int main () {
    input_token = scan ();
    program ();
    return 0;
}
