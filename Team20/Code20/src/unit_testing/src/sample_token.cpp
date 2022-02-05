#include "sample_token.h"
#include "components/query_subsystem/pql_lexer/pql_token.h"

PqlToken comma_token = PqlToken(TokenType::COMMA, ",");
PqlToken semicolon_token = PqlToken(TokenType::SEMICOLON, ";");
PqlToken open_parenthesis_token = PqlToken(TokenType::OPEN_PARENTHESIS, "(");
PqlToken closed_parenthesis_token = PqlToken(TokenType::CLOSED_PARENTHESIS, ")");
PqlToken double_quote_token = PqlToken(TokenType::DOUBLE_QUOTE, "\"");
PqlToken underscore_token = PqlToken(TokenType::UNDERSCORE, "_");
PqlToken plus_token = PqlToken(TokenType::PLUS, "+");

PqlToken stmt_token = PqlToken(TokenType::STMT, "stmt");
PqlToken assign_token = PqlToken(TokenType::ASSIGN, "assign");
PqlToken while_token = PqlToken(TokenType::WHILE, "while");
PqlToken if_token = PqlToken(TokenType::IF, "if");
PqlToken variable_token = PqlToken(TokenType::VARIABLE, "variable");
PqlToken procedure_token = PqlToken(TokenType::PROCEDURE, "procedure");
PqlToken constant_token = PqlToken(TokenType::CONSTANT, "constant");
PqlToken read_token = PqlToken(TokenType::READ, "read");
PqlToken print_token = PqlToken(TokenType::PRINT, "print");
PqlToken call_token = PqlToken(TokenType::CALL, "call");

PqlToken s_token = PqlToken(TokenType::SYNONYM, "s");
PqlToken s1_token = PqlToken(TokenType::SYNONYM, "s1");
PqlToken a_token = PqlToken(TokenType::SYNONYM, "a");
PqlToken a1_token = PqlToken(TokenType::SYNONYM, "a1");
PqlToken w_token = PqlToken(TokenType::SYNONYM, "w");
PqlToken ifs_token = PqlToken(TokenType::SYNONYM, "ifs");
PqlToken v_token = PqlToken(TokenType::SYNONYM, "v");
PqlToken v1_token = PqlToken(TokenType::SYNONYM, "v1");
PqlToken p_token = PqlToken(TokenType::SYNONYM, "p");
PqlToken q_token = PqlToken(TokenType::SYNONYM, "q");
PqlToken c_token = PqlToken(TokenType::SYNONYM, "c");
PqlToken re_token = PqlToken(TokenType::SYNONYM, "re");
PqlToken pn_token = PqlToken(TokenType::SYNONYM, "pn");
PqlToken cl_token = PqlToken(TokenType::SYNONYM, "cl");

PqlToken number_value_token = PqlToken(TokenType::NUMBER, "1");
PqlToken x_token = PqlToken(TokenType::SYNONYM, "x");
PqlToken y_token = PqlToken(TokenType::SYNONYM, "y");
PqlToken x_string_token = PqlToken(TokenType::IDENT_WITH_QUOTES, "\"x\"");
PqlToken x_plus_y_string_token = PqlToken(TokenType::EXPR, "\"x+y\"");
PqlToken sub_expression_token = PqlToken(TokenType::SUB_EXPRESSION, "_\"x+y\"_");

PqlToken select_token = PqlToken(TokenType::SELECT, "Select");
PqlToken such_token = PqlToken(TokenType::SUCH, "such");
PqlToken that_token = PqlToken(TokenType::THAT, "that");

PqlToken follow_token = PqlToken(TokenType::FOLLOWS, "Follows");
PqlToken follow_t_token = PqlToken(TokenType::FOLLOWS_T, "Follows*");
PqlToken parent_token = PqlToken(TokenType::PARENT, "Parent");
PqlToken parent_t_token = PqlToken(TokenType::PARENT_T, "Parent*");
PqlToken use_token = PqlToken(TokenType::USES, "Uses");
PqlToken modify_token = PqlToken(TokenType::MODIFIES, "Modifies");
PqlToken pattern_token = PqlToken(TokenType::PATTERN, "pattern");
