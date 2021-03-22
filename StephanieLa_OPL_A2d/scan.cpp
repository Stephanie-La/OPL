/* Simple ad-hoc scanner for the calculator language.
    Michael L. Scott, 2008-2020.
*/

#include <iostream>	// std::cout, std::end
#include <cstdlib>
#include <cctype>
#include <cstdio>
#include <string>
#include "scan.h"

using namespace std;


// char token_image[MAX_TOKEN_LEN];
 string token_image;
// std::string token_image[MAX_TOKEN_LEN];


token scan() {
    static int c = ' ';
        /* next available char; extra (int) width accommodates EOF */
    int i = 0;              /* index into token_image */

    /* skip white space */
    while (isspace(c)) {
	c = cin.get();
    }
    if (c == EOF)
        return t_eof;
    if (isalpha(c)) {
        do {
		token_image.push_back(c);
            if (i >= MAX_TOKEN_LEN) {
		cout << "max token length exceeded" << endl;
               // exit(1);
             }
	    c = cin.get();
        } while (isalpha(c) || isdigit(c) || c == '_');
	if (!(token_image.compare("read"))) return t_read;
        else if (!(token_image.compare("write"))) return t_write;
	else if (!(token_image.compare("if"))) return t_if;
	else if(!(token_image.compare("while"))) return t_while;
	else if (!(token_image.compare("end"))) return t_end;
        else return t_id;
    }
    else if (isdigit(c)) {
        do {
	 token_image.push_back(c);
		c = cin.get();
        } while (isdigit(c));
        return t_literal;
    } else switch (c) {
        case ':':
            if ((c = cin.get()) != '=') {
		cerr << "syntax error" << endl;
               // exit(1);
		 c = '=';
		c = cin.get();
		return t_gets;
            } else {
		c = cin.get();
                return t_gets;
            }
            break;
	case '(': c = cin.get(); return t_lparen;
        case ')': c = cin.get(); return t_rparen;
        case '+': c = cin.get(); return t_add;
        case '-': c = cin.get(); return t_sub;
        case '*': c = cin.get(); return t_mul;
        case '/': c = cin.get(); return t_div;
        case '=': c = cin.get(); return t_eq;
	case '<': c = cin.get();
		if(c == '>'){
			c = cin.get(); return t_neq;
		}
		else if(c == '='){
			c = cin.get(); return t_loe;
		}
		else {
			return t_lt;
		}
	case '>': c = cin.get();
		if(c == '='){
			return t_goe;
		}
		else{
			return t_gt;
		}
        default:
            cout << "error" << endl;
           // exit(1);
    }
}
