/* Simple ad-hoc scanner for the calculator language.
    Michael L. Scott, 2008-2020.
*/

//#include "stdio.h"
//#include "stdlib.h"
//#include "string.h"
//#include "ctype.h"

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
        // c = getchar();
	c = cin.get();
    }
    if (c == EOF)
        return t_eof;
    if (isalpha(c)) {
        do {
            // token_image[i++] = c;
		token_image.push_back(c);
            if (i >= MAX_TOKEN_LEN) {
                // printf("max token length exceeded\n");
		cout << "max token length exceeded" << endl;
                exit(1);
             }
            // c = getchar();
	    c = cin.get();
        } while (isalpha(c) || isdigit(c) || c == '_');
        // token_image[i] = '\0';
	if (!(token_image.compare("read"))) return t_read;
        else if (!(token_image.compare("write"))) return t_write;
        else return t_id;
    }
    else if (isdigit(c)) {
        do {
            // token_image[i++] = c;
	 token_image.push_back(c);
		c = cin.get();
           // c = getchar();
        } while (isdigit(c));
        // token_image[i] = '\0';
        return t_literal;
    } else switch (c) {
        case ':':
            if ((c = cin.get()) != '=') {
		cerr << "error" << endl;
               // fprintf(stderr, "error\n");
                exit(1);
            } else {
		c = cin.get();
                // c = getchar();
                return t_gets;
            }
            break;
	case '(': c = cin.get(); return t_lparen;
        case ')': c = cin.get(); return t_rparen;
        case '+': c = cin.get(); return t_add;
        case '-': c = cin.get(); return t_sub;
        case '*': c = cin.get(); return t_mul;
        case '/': c = cin.get(); return t_div;

       // case '(': c = getchar(); return t_lparen;
       // case ')': c = getchar(); return t_rparen;
       // case '+': c = getchar(); return t_add;
       // case '-': c = getchar(); return t_sub;
       // case '*': c = getchar(); return t_mul;
       // case '/': c = getchar(); return t_div;
        default:
            cout << "error" << endl;
            // printf("error\n");
            exit(1);
    }
}
