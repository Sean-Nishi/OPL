/* Complete recursive descent parser for the calculator language.
    Builds on figure 2.16.  Prints a trace of productions predicted and
    tokens matched.  Does no error recovery: prints "syntax error" and
    dies on invalid input.
    Michael L. Scott, 2008-2017.
*/


//converting to c++
#include <iostream>
#include <cstdlib>
#include <fstream>

#include "scan.h"

//added "equal" to "greater than equal" and "if" to "check" to names[]
const char* names[] = {"read", "write", "id", "literal", "gets",
                       "add", "sub", "mul", "div", "lparen", "rparen", "eof", 			       "equal", "not equal", "less than", "greater than", 
		       "less than equal", "greater than equal",
		       "if", "fi", "do", "od", "check"};

static token input_token;

void program ();
void stmt_list ();
void stmt ();
void expr ();
void term_tail ();
void term ();
void factor_tail ();
void factor ();
void add_op ();
void mul_op ();

//Function declarations added for assignment. For relation cases and expression tails
void relation_op ();
void relation ();
void expr_tail ();
void exception_handler (token);

//error calls the exception handler
void error (token expected) {
  std::cout << "Found an error: calling exception handler" << std::endl;
  exception_handler (expected);
}

void match (token expected) {
  if (input_token == expected) {
    std::cout << "matched " << names[input_token] << std::endl;
    if (input_token == t_id || input_token == t_literal)
      std::cout << (": %s", token_image) << std::endl;
    input_token = scan ();
  }
  else error (expected);
}

//---------------------------------------------------------------------------

//need to modify the default
//P -> SL $$
void program () {
    switch (input_token) {
      case t_id:
      case t_read:
      case t_write:
      case t_eof:
      //added cases for if, do, and check
      case t_if:
      case t_do:
      case t_check:
	std::cout << "predict program --> stmt_list eof" << std::endl;
        stmt_list();
	match (t_eof);
	break;
	//cases for an error at the beginning of the file
      default: error (input_token);
    }
}

//SL -> S SL | e
void stmt_list () {
  try {
    switch (input_token) {
      case t_id:
      case t_read:
      case t_write:
      //added cases for if, do, and check
      case t_if:
      case t_do:
      case t_check:
	std::cout << "predict stmt_list --> stmt stmt_list" << std::endl;
        stmt();
        stmt_list();
	break;
      case t_eof:
      //if we are breaking out of an if statement of do statement
      case t_fi:
      case t_od:
	std::cout << "predict stmt_list --> epsilon" <<std::endl;
	break;          /*  epsilon production */
      default: throw;
    }
  }
  catch (...) {
    exception_handler(input_token);
    stmt_list ();
  }
}

//S -> id | read | write | if | do | check
//added if, do, and check
void stmt () {
  try {
    switch (input_token) {
      //if case
      case t_if:
	std::cout << "predict stmt --> if relation stmt_list fi" << std::endl;
	match (t_if);
        relation();
        stmt_list();
	match (t_fi);
	break;
	//do case
      case t_do:
	std::cout << "predict stmt --> do stmt_list od" << std::endl;
	match (t_do);
        stmt_list ();
	match (t_od);
	break;
	//check case
      case t_check:
	std::cout << "predict stmt --> check relation" << std::endl;
	match (t_check);
        relation();
	break;
	//given cases
      case t_id:
	std::cout << "predict stmt --> id gets relation" << std::endl;
	match (t_id);
	match (t_gets);
        relation();
	break;
      case t_read:
	std::cout << "predict stmt --> read id" << std::endl;
	match (t_read);
	match (t_id);
	break;
      case t_write:
	std::cout << "predict stmt --> write expr" << std::endl;
	match (t_write);
        relation();
	break;
      default: throw;
    }
  }
  catch (...) {
    exception_handler (input_token);
    stmt ();
  }
}

//R -> E ET
void relation () {
  try {
    switch (input_token) {
      case t_id:
      case t_literal:
	std::cout << "predict relation --> expr expr_tail" << std::endl;
	expr();
        expr_tail();
        break;
      default: throw;
    }
  }
  catch (...) {
    exception_handler (input_token);
    relation ();
  }
}

//E -> T TT
void expr () {
  try {
    switch (input_token) {
      case t_id:
      case t_literal:
      case t_lparen:
	std::cout << "predict expr --> term term_tail" << std::endl;
        term();
        term_tail();
	break;
      default: throw;
    }
  }
  catch (...) {
    exception_handler (input_token);
    expr ();
  }
}

//T -> F FT
void term () {
  try {
    switch (input_token) {
      case t_id:
      case t_literal:
      case t_lparen:
	std::cout << "predict term --> factor factor_tail" << std::endl;
        factor();
        factor_tail();
	break;
      default: throw;
    }
  }
  catch (...) {
    exception_handler (input_token);
    term ();
  }
}

//F -> ( R ) | id | lit
void factor () {
  try {
    switch (input_token) {
      case t_lparen:
	std::cout << "predict factor --> lparen expr rparen" << std::endl;
	match (t_lparen);
        relation();
	match (t_rparen);
	break;
      case t_id :
	std::cout << "predict factor --> id" << std::endl;
	match (t_id);
	break;
      case t_literal:
	std::cout << "predict factor --> literal" << std::endl;
	match (t_literal);
	break;
      default: throw;
    }
  }
  catch (...) {
    exception_handler (input_token);
    factor ();
  }
}

//ET -> ro E | e
void expr_tail () {
  try {
    switch (input_token) {
      //cases for relation operators
      case t_equal:
      case t_not_equal:
      case t_less_than:
      case t_greater_than:
      case t_less_than_equal:
      case t_greater_than_equal:
	std::cout << "predict expr_tail --> relation_op expr" << std::endl;
	relation_op();
        expr();
	break;
      //case statements for epsilon production
      //added do, if, check, od, and fi
      case t_do:
      case t_if:
      case t_check:
      case t_od:
      case t_fi:
      case t_add:
      case t_sub:
      case t_mul:
      case t_div:
      case t_rparen:
      case t_eof:
	std::cout << "predict expr_tail --> epsilon" << std::endl;
	break;
      default: throw;
    }
  }
  catch (...) {
    exception_handler (input_token);
    expr_tail ();
  }
}

//TT -> ao T TT | e
void term_tail () {
  try {
    switch (input_token) {
      case t_add:
      case t_sub:
	std::cout << "predict term_tail --> add_op term term_tail" << std::endl;
        add_op ();
	term ();
	term_tail ();
	break;
      case t_rparen:
      case t_id:
      case t_read:
      case t_write:
      case t_eof:
      //added statements for do, if, check, od, and fi
      case t_do:
      case t_if:
      case t_check:
      case t_od:
      case t_fi:
      //added cases for relation operators
      case t_equal:
      case t_not_equal:
      case t_less_than:
      case t_greater_than:
      case t_less_than_equal:
      case t_greater_than_equal:
	std::cout << "predict term_tail --> epsilon" << std::endl;
	break;          /*  epsilon production */
      default: throw;
    }
  }
  catch (...) {
    exception_handler (input_token);
    term_tail ();
  }
}

//FT -> mo F FT | e
void factor_tail () {
  try {
    switch (input_token) {
      case t_mul:
      case t_div:
	std::cout << "predict factor_tail --> mul_op factor factor_tail" << std::endl;
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
      case t_eof:
      //added cases for do, if, check, od, and fi
      case t_do:
      case t_if:
      case t_check:
      case t_od:
      case t_fi:
      //added cases for relation operators
      case t_equal:
      case t_not_equal:
      case t_less_than:
      case t_greater_than:
      case t_greater_than_equal:
      case t_less_than_equal:
	std::cout << "predict factor_tail --> epsilon" << std::endl;
	break;          /*  epsilon production */
      default: throw;
    }
  }
  catch (...) {
    exception_handler (input_token);
    factor_tail ();
  }
}

//ro -> == | <> | < | > | <= | >=
void relation_op () {
  try {
    switch (input_token) {
      case t_equal:
	std::cout << "predict relation_op --> equal" << std::endl;
	match (t_equal);
	break;
      case t_not_equal:
	std::cout << "predict relation_op --> not equal" << std::endl;
	match (t_not_equal);
	break;
      case t_less_than:
	std::cout << "predict relation_op --> less than" << std::endl;
	match (t_less_than);
	break;
      case t_greater_than:
	std::cout << "predict relation_op --> greater than" << std::endl;
	match (t_greater_than);
	break;
      case t_less_than_equal:
	std::cout << "predict relation_op --> less than equal" << std::endl;
	match (t_less_than_equal);
	break;
      case t_greater_than_equal:
	std::cout << "predict relation_op --> greater than equal" << std::endl;
	match (t_greater_than_equal);
	break;
      default: throw;
    }
  }
  catch (...) {
    exception_handler (input_token);
    relation_op ();
  }
}

//ao -> + | -
void add_op () {
  try {
    switch (input_token) {
      case t_add:
	std::cout << "predict add_op --> add" << std::endl;
	match (t_add);
	break;
      case t_sub:
	std::cout << "predict add_op --> sub" << std::endl;
	match (t_sub);
	break;
      default: throw;
    }
  }
  catch (...) {
    exception_handler (input_token);
    add_op ();
  }
}

//mo -> * | /
void mul_op () {
  try {
    switch (input_token) {
      case t_mul:
	std::cout << "predict mul_op --> mul" << std::endl;
	match (t_mul);
	break;
      case t_div:
	std::cout << "predict mul_op --> div" << std::endl;
	match (t_div);
	break;
      default: throw;
    }
  }
  catch (...) {
    exception_handler (input_token);
    mul_op();
  }
}

//scan () modifies input_token, try block makes sure we can return safely
void exception_handler(token tok){
  try {
    match (tok);
  }
  catch (...) {
    tok = scan ();
    exception_handler(tok);
  }
}

int main (int argc, char* argv[]) {
  //main still requires an input from the command line
  //similar to the python command line
  input_token = scan ();
  program ();

  /*
  std::string fileString;

  if(argc != 2) {
    std::cout << "ERROR: please enter a file name. ./thisProgram file.txt" << std::endl;
    exit(1);
  }

  //open the file
  std::ifstream inputFile = argv[1];

  //load the contents of the file into the stack

  //modify scan() to read from a file. Read chars? read string? tokenize them?
  //getline() into string, when we finihs reading this line, 

  if (inputFile.is_open()) {
    while (getline (inputFile, fileString) {
      input_token = 
      program ();
    }
  }

  inputFile.close();
  */
    
  return 0;
}
