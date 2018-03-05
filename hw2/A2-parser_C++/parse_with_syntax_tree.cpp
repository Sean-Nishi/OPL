/* Complete recursive descent parser for the calculator language.
    Builds on figure 2.16.  Prints a trace of productions predicted and
    tokens matched.  Does no error recovery: prints "syntax error" and
    dies on invalid input.
    Michael L. Scott, 2008-2017.
*/

/*
  THIS IS MY ATTEMPT AT BUILDING A SYNTAX TREE FOR THE OPTIONAL PART OF THIS ASSIGNMENT. HOWEVER IT DOESN'T WORK RIGHT NOW
*/

//converting to c++
#include <iostream>
#include <cstdlib>

#include "scan.h"
#include "node.h"

//added "equal" to "greater than equal" and "if" to "check" to names[]
const char* names[] = {"read", "write", "id", "literal", "gets",
                       "add", "sub", "mul", "div", "lparen", "rparen", "eof", 			       "equal", "not equal", "less than", "greater than", 
		       "less than equal", "greater than equal",
		       "if", "fi", "do", "od", "check"};

static token input_token;

Node* program ();
Node* stmt_list ();
Node* stmt ();
Node* expr ();
Node* term_tail ();
Node* term ();
Node* factor_tail ();
Node* factor ();
Node* add_op ();
Node* mul_op ();

//Function declarations added for assignment. For relation cases and expression tails
Node* relation_op ();
Node* relation ();
Node* expr_tail ();
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
Node* program () {
  Node* program_node = new Node();
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
	program_node->_add_child(stmt_list());
	match (t_eof);
	break;
      default: error (input_token);
    }
    return program_node;
}

//SL -> S SL | e
Node* stmt_list () {
  try {
    Node* stmt_list_node = new Node();
    switch (input_token) {
      case t_id:
      case t_read:
      case t_write:
      //added cases for if, do, and check
      case t_if:
      case t_do:
      case t_check:
	std::cout << "predict stmt_list --> stmt stmt_list" << std::endl;
	stmt_list_node->_add_child(stmt());
	stmt_list_node->_add_child(stmt_list());
	break;
      case t_eof:
	std::cout << "predict stmt_list --> epsilon" <<std:: endl;
	break;          /*  epsilon production */
      default: throw;
    }
    return stmt_list_node;
  }
  catch (...) {
    exception_handler(input_token);
    stmt_list ();
  }
}

//S -> id | read | write | if | do | check
//added if, do, and check
Node* stmt () {
  try {
    Node* stmt_node = new Node();
    switch (input_token) {
      //if case
      case t_if:
	std::cout << "predict stmt --> if relation_op fi" << std::endl;
	match (t_if);
	stmt_node->_add_child(relation());
	stmt_node->_add_child(stmt_list());
	match (t_fi);
	break;
	//do case
      case t_do:
	std::cout << "predict stmt --> do stmt_list od" << std::endl;
	match (t_do);
	stmt_node->_add_child(stmt_list ());
	match (t_od);
	break;
	//check case
      case t_check:
	std::cout << "predict stmt --> check relation" << std::endl;
	match (t_check);
        stmt_node->_add_child(relation());
	break;
	//given cases
      case t_id:
	std::cout << "predict stmt --> id gets relation" << std::endl;
	match (t_id);
	match (t_gets);
        stmt_node->_add_child(relation());
	break;
      case t_read:
	std::cout << "predict stmt --> read id" << std::endl;
	match (t_read);
	match (t_id);
	break;
      case t_write:
	std::cout << "predict stmt --> write expr" << std::endl;
	match (t_write);
        stmt_node->_add_child(relation());
	break;
      default: throw;
    }
    return stmt_node;
  }
  catch (...) {
    exception_handler (input_token);
    stmt ();
  }
}

//R -> E ET
Node* relation () {
  try {
    Node* relation_node = new Node();
    switch (input_token) {
      case t_id:
      case t_literal:
	std::cout << "predict relation --> expr expr_tail" << std::endl;
	relation_node->_add_child(expr());
	relation_node->_add_child(expr_tail());
        break;
      default: throw;
    }
    return relation_node;
  }
  catch (...) {
    exception_handler (input_token);
    relation ();
  }
}

//E -> T TT
Node* expr () {
  try {
    Node* expr_node = new Node();
    switch (input_token) {
      case t_id:
      case t_literal:
      case t_lparen:
	std::cout << "predict expr --> term term_tail" << std::endl;
	expr_node->_add_child(term());
	expr_node->_add_child(term_tail());
	break;
      default: throw;
    }
    return expr_node;
  }
  catch (...) {
    exception_handler (input_token);
    expr ();
  }
}

//T -> F FT
Node* term () {
  try {
    Node* term_node = new Node();
    switch (input_token) {
      case t_id:
      case t_literal:
      case t_lparen:
	std::cout << "predict term --> factor factor_tail" << std::endl;
	term_node->_add_child(factor());
	term_node->_add_child(factor_tail());
	break;
      default: throw;
    }
    return term_node;
  }
  catch (...) {
    exception_handler (input_token);
    term ();
  }
}

//F -> ( R ) | id | lit
Node* factor () {
  try {
    Node* factor_node = new Node();
    switch (input_token) {
      case t_lparen:
	std::cout << "predict factor --> lparen expr rparen" << std::endl;
	match (t_lparen);
	factor_node->_add_child(relation());
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
    return factor_node;
  }
  catch (...) {
    exception_handler (input_token);
    factor ();
  }
}

//ET -> ro E | e
Node* expr_tail () {
  try {
    Node* expr_tail_node = new Node();
    switch (input_token) {
      case t_equal:
      case t_not_equal:
      case t_less_than:
      case t_greater_than:
      case t_less_than_equal:
      case t_greater_than_equal:
	std::cout << "predict expr_tail --> relation_op expr" << std::endl;
	expr_tail_node->_add_child(relation_op());
	expr_tail_node->_add_child(expr());
	break;
      case t_eof:
	std::cout << "predict expr_tail --> epsilon" << std::endl;
	break;
      default: throw;
    }
    return expr_tail_node;
  }
  catch (...) {
    exception_handler (input_token);
    expr_tail ();
  }
}

//TT -> ao T TT | e
Node* term_tail () {
  try {
    Node* term_tail_node = new Node();
    switch (input_token) {
      case t_add:
      case t_sub:
	std::cout << "predict term_tail --> add_op term term_tail" << std::endl;
	term_tail_node->_add_child(add_op());
	term_tail_node->_add_child(term());
	term_tail_node->_add_child(term_tail());
	break;
      case t_rparen:
      case t_id:
      case t_read:
      case t_write:
      case t_eof:
	std::cout << "predict term_tail --> epsilon" << std::endl;
	break;          /*  epsilon production */
      default: throw;
    }
    return term_tail_node;
  }
  catch (...) {
    exception_handler (input_token);
    term_tail ();
  }
}

//FT -> mo F FT | e
Node* factor_tail () {
  try {
    Node* factor_tail_node = new Node();
    switch (input_token) {
      //add cases for equals, etc. and lparen to get 
      
      
      case t_mul:
      case t_div:
	std::cout << "predict factor_tail --> mul_op factor factor_tail" << std::endl;
	factor_tail_node->_add_child(mul_op());
	factor_tail_node->_add_child(factor());
	factor_tail_node->_add_child(factor_tail());
	break;
      case t_add:
      case t_sub:
      case t_rparen:
      case t_id:
      case t_read:
      case t_write:
      case t_eof:
	std::cout << "predict factor_tail --> epsilon" << std::endl;
	break;          /*  epsilon production */
      default: throw;
    }
    return factor_tail_node;
  }
  catch (...) {
    exception_handler (input_token);
    factor_tail ();
  }
}

//ro -> == | <> | < | > | <= | >=
Node* relation_op () {
  try {
    Node* relation_op_node = new Node();
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
    return relation_op_node;
  }
  catch (...) {
    exception_handler (input_token);
    relation_op ();
  }
}

//ao -> + | -
Node* add_op () {
  try {
    Node* add_op_node = new Node();
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
    return add_op_node;
  }
  catch (...) {
    exception_handler (input_token);
    add_op ();
  }
}

//mo -> * | /
Node* mul_op () {
  try {
    Node* mul_op_node = new Node();
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
    return mul_op_node;
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

int main () {
  input_token = scan ();
  program ();
  return 0;
}
