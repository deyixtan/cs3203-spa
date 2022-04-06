#include "sample_token.h"
#include "components/query_subsystem/pql_lexer/pql_token.h"

using namespace pql;

PqlToken comma_token = PqlToken(PqlTokenType::COMMA, ",");
PqlToken semicolon_token = PqlToken(PqlTokenType::SEMICOLON, ";");
PqlToken open_parenthesis_token = PqlToken(PqlTokenType::OPEN_PARENTHESIS, "(");
PqlToken closed_parenthesis_token = PqlToken(PqlTokenType::CLOSED_PARENTHESIS, ")");
PqlToken double_quote_token = PqlToken(PqlTokenType::DOUBLE_QUOTE, "\"");
PqlToken underscore_token = PqlToken(PqlTokenType::UNDERSCORE, "_");
PqlToken plus_token = PqlToken(PqlTokenType::PLUS, "+");
PqlToken equal_sign_token = PqlToken(PqlTokenType::EQUAL, "=");

PqlToken stmt_token = PqlToken(PqlTokenType::STMT, "stmt");
PqlToken assign_token = PqlToken(PqlTokenType::ASSIGN, "assign");
PqlToken while_token = PqlToken(PqlTokenType::WHILE, "while");
PqlToken if_token = PqlToken(PqlTokenType::IF, "if");
PqlToken variable_token = PqlToken(PqlTokenType::VARIABLE, "variable");
PqlToken procedure_token = PqlToken(PqlTokenType::PROCEDURE, "procedure");
PqlToken constant_token = PqlToken(PqlTokenType::CONSTANT, "constant");
PqlToken read_token = PqlToken(PqlTokenType::READ, "read");
PqlToken print_token = PqlToken(PqlTokenType::PRINT, "print");
PqlToken call_token = PqlToken(PqlTokenType::CALL, "call");

PqlToken s_token = PqlToken(PqlTokenType::SYNONYM, "s");
PqlToken s1_token = PqlToken(PqlTokenType::SYNONYM, "s1");
PqlToken a_token = PqlToken(PqlTokenType::SYNONYM, "a");
PqlToken a1_token = PqlToken(PqlTokenType::SYNONYM, "a1");
PqlToken w_token = PqlToken(PqlTokenType::SYNONYM, "w");
PqlToken ifs_token = PqlToken(PqlTokenType::SYNONYM, "ifs");
PqlToken v_token = PqlToken(PqlTokenType::SYNONYM, "v");
PqlToken v1_token = PqlToken(PqlTokenType::SYNONYM, "v1");
PqlToken p_token = PqlToken(PqlTokenType::SYNONYM, "p");
PqlToken q_token = PqlToken(PqlTokenType::SYNONYM, "q");
PqlToken c_token = PqlToken(PqlTokenType::SYNONYM, "c");
PqlToken re_token = PqlToken(PqlTokenType::SYNONYM, "re");
PqlToken pn_token = PqlToken(PqlTokenType::SYNONYM, "pn");
PqlToken cl_token = PqlToken(PqlTokenType::SYNONYM, "cl");
PqlToken synonym_select_token = PqlToken(PqlTokenType::SYNONYM, "Select");
PqlToken synonym_uses_token = PqlToken(PqlTokenType::SYNONYM, "Uses");
PqlToken synonym_assign_token = PqlToken(PqlTokenType::SYNONYM, "assign");
PqlToken synonym_boolean_token = PqlToken(PqlTokenType::SYNONYM, "BOOLEAN");

PqlToken number_value_token_1 = PqlToken(PqlTokenType::NUMBER, "1");
PqlToken number_value_token_2 = PqlToken(PqlTokenType::NUMBER, "2");
PqlToken number_value_token_3 = PqlToken(PqlTokenType::NUMBER, "3");
PqlToken number_value_token_4 = PqlToken(PqlTokenType::NUMBER, "4");
PqlToken number_value_token_5 = PqlToken(PqlTokenType::NUMBER, "5");
PqlToken x_token = PqlToken(PqlTokenType::SYNONYM, "x");
PqlToken y_token = PqlToken(PqlTokenType::SYNONYM, "y");
PqlToken x_string_token = PqlToken(PqlTokenType::IDENT_WITH_QUOTES, "x");
PqlToken x_plus_y_string_token = PqlToken(PqlTokenType::EXPR, "x+y");
PqlToken sub_expression_token = PqlToken(PqlTokenType::SUB_EXPRESSION, "x+y");
PqlToken valid_tuple_token_1 = PqlToken(PqlTokenType::TUPLE, "<s,t>");
PqlToken valid_tuple_token_2 = PqlToken(PqlTokenType::TUPLE, "<s,t,v>");
PqlToken valid_tuple_token_3 = PqlToken(PqlTokenType::TUPLE, "<s.stmt#>");
PqlToken valid_tuple_token_4 = PqlToken(PqlTokenType::TUPLE, "<s,t,s.stmt#>");
PqlToken attribute_statement_token = PqlToken(PqlTokenType::ATTRIBUTE, "s.stmt#");
PqlToken attribute_assign_token = PqlToken(PqlTokenType::ATTRIBUTE, "a.stmt#");
PqlToken attribute_var_token = PqlToken(PqlTokenType::ATTRIBUTE, "v.varName");
PqlToken attribute_const_token = PqlToken(PqlTokenType::ATTRIBUTE, "c.value");

PqlToken select_token = PqlToken(PqlTokenType::SELECT, "Select");
PqlToken such_token = PqlToken(PqlTokenType::SUCH, "such");
PqlToken that_token = PqlToken(PqlTokenType::THAT, "that");
PqlToken and_token = PqlToken(PqlTokenType::AND, "and");
PqlToken boolean_token = PqlToken(PqlTokenType::BOOLEAN, "BOOLEAN");


PqlToken follow_token = PqlToken(PqlTokenType::FOLLOWS, "Follows");
PqlToken follow_t_token = PqlToken(PqlTokenType::FOLLOWS_T, "Follows*");
PqlToken parent_token = PqlToken(PqlTokenType::PARENT, "Parent");
PqlToken parent_t_token = PqlToken(PqlTokenType::PARENT_T, "Parent*");
PqlToken use_token = PqlToken(PqlTokenType::USES, "Uses");
PqlToken modify_token = PqlToken(PqlTokenType::MODIFIES, "Modifies");
PqlToken pql_calls_token = PqlToken(PqlTokenType::CALLS, "Calls");
PqlToken pql_calls_t_token = PqlToken(PqlTokenType::CALLS_T, "Calls*");
PqlToken affects_token = PqlToken(PqlTokenType::AFFECTS, "Affects");
PqlToken affects_t_token = PqlToken(PqlTokenType::AFFECTS_T, "Affects*");
PqlToken next_token = PqlToken(PqlTokenType::NEXT, "Next");
PqlToken next_t_token = PqlToken(PqlTokenType::NEXT_T, "Next*");
PqlToken pattern_token = PqlToken(PqlTokenType::PATTERN, "pattern");
PqlToken with_token = PqlToken(PqlTokenType::WITH, "with");

PqlToken v_value_token = PqlToken(PqlTokenType::IDENT_WITH_QUOTES, "\"v\"");
