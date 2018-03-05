/* Definitions the scanner shares with the parser
    Michael L. Scott, 2008-2017.
*/

//added relational operators and if, do, check
typedef enum {t_read, t_write, t_id, t_literal, t_gets,
                t_add, t_sub, t_mul, t_div, t_lparen, t_rparen, t_eof, 
				t_equal, t_not_equal, t_less_than, t_greater_than,
				t_less_than_equal, t_greater_than_equal,
				t_if, t_fi, t_do, t_od, t_check} token;

extern char token_image[100];

extern token scan();
