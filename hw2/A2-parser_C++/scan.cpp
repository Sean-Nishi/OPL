/* Simple ad-hoc scanner for the calculator language.
    Michael L. Scott, 2008-2017.
*/

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cctype>

#include "scan.h"

char token_image[100];

token scan() {
  static int c = ' ';
  /* next available char; extra (int) width accommodates EOF */
  int i = 0;              /* index into token_image */


  //need to change getchar() to read from an input string. see parse.cpp main()
  /* skip white space */
  while (isspace(c)) {
    c = getchar();
  }
  if (c == EOF)
    return t_eof;
  if (isalpha(c)) {
    do {
      token_image[i++] = c;
      c = getchar();
    } while (isalpha(c) || isdigit(c) || c == '_');
    token_image[i] = '\0';
    if (!strcmp(token_image, "read")) return t_read;
    else if (!strcmp(token_image, "write")) return t_write;
    
    //added cases for if, fi, do, od, and check
    else if (!strcmp(token_image, "do")) return t_do;
    else if (!strcmp(token_image, "od")) return t_od;
    else if (!strcmp(token_image, "if")) return t_if;
    else if (!strcmp(token_image, "fi")) return t_fi;
    else if (!strcmp(token_image, "check")) return t_check;
    
    else return t_id;
  }
  else if (isdigit(c)) {
    do {
      token_image[i++] = c;
      c = getchar();
    } while (isdigit(c));
    token_image[i] = '\0';
    return t_literal;
  } else switch (c) {
      //use this case as a model for what we need to do
    case ':':
      if ((c = getchar()) != '=') {
	std::cout << "error after ':', expected a '='" << std::endl;
	exit(1);
      } else {
	c = getchar();
	return t_gets;
      }
      break;
      
     
    case '+': c = getchar(); return t_add;
    case '-': c = getchar(); return t_sub;
    case '*': c = getchar(); return t_mul;
    case '/': c = getchar(); return t_div;
    case '(': c = getchar(); return t_lparen;
    case ')': c = getchar(); return t_rparen;

    //added case statements for remaining realtional operators
    //ex. case '<' needs to scan the next char to determine whether we have <= or <>
    case '=':
      //need to remove exit(1)
      if ((c = getchar()) != '=') {
	std::cout << "error after '=', expected a '='" << std::endl;
	exit(1);
      } else {
	c = getchar();
	return t_equal;
      }
      break;
    case '<':
      if ((c = getchar()) != '=' && c != '>' && !isspace(c)) {
	std::cout << "error after '<', expected a ' ', '=', or a '>'" << std::endl;
	exit(1);
      }
      else if (c == '=') {
	c = getchar();
	return t_less_than_equal;
      }
      else if (c == '>') {
	c = getchar();
	return t_not_equal;
      }
      else {
	c = getchar();
	return t_less_than;
      } 
      break;
      
    case '>':
      if ((c = getchar()) != '=' && !isspace(c)) {
	std::cout << "error after '>', expected a '=' or a ' '" << std::endl;
	exit(1);
      }
      else if (c == '=') {
	c = getchar();
	return t_greater_than_equal;
      }
      else {
	c = getchar();
	return t_greater_than;
      }
      break;
    default:
      std::cout << "Default error in scan()" << std::endl;
      exit(1);
    }
}
