#ifndef SAMPLE_TOKEN_H
#define SAMPLE_TOKEN_H

#include "components/query_subsystem/pql_lexer/pql_token.h"

extern PqlToken comma_token;
extern PqlToken semicolon_token;
extern PqlToken open_parenthesis_token;
extern PqlToken closed_parenthesis_token;
extern PqlToken double_quote_token;
extern PqlToken underscore_token;
extern PqlToken plus_token;
extern PqlToken equal_sign_token;

extern PqlToken stmt_token;
extern PqlToken assign_token;
extern PqlToken while_token;
extern PqlToken if_token;
extern PqlToken variable_token;
extern PqlToken procedure_token;
extern PqlToken constant_token;
extern PqlToken read_token;
extern PqlToken print_token;
extern PqlToken call_token;

extern PqlToken s_token;
extern PqlToken s1_token;
extern PqlToken a_token;
extern PqlToken a1_token;
extern PqlToken w_token;
extern PqlToken ifs_token;
extern PqlToken v_token;
extern PqlToken v1_token;
extern PqlToken p_token;
extern PqlToken q_token;
extern PqlToken c_token;
extern PqlToken re_token;
extern PqlToken pn_token;
extern PqlToken cl_token;
extern PqlToken synonym_select_token;
extern PqlToken synonym_uses_token;
extern PqlToken synonym_assign_token;
extern PqlToken synonym_boolean_token;

extern PqlToken number_value_token_1;
extern PqlToken number_value_token_2;
extern PqlToken number_value_token_3;
extern PqlToken number_value_token_4;
extern PqlToken number_value_token_5;
extern PqlToken x_token;
extern PqlToken y_token;
extern PqlToken x_string_token;
extern PqlToken x_plus_y_string_token;
extern PqlToken sub_expression_token;
extern PqlToken valid_tuple_token_1;
extern PqlToken valid_tuple_token_2;
extern PqlToken valid_tuple_token_3;
extern PqlToken valid_tuple_token_4;
extern PqlToken attribute_statement_token;
extern PqlToken attribute_statement_token;
extern PqlToken attribute_assign_token;
extern PqlToken attribute_var_token;

extern PqlToken select_token;
extern PqlToken such_token;
extern PqlToken that_token;
extern PqlToken and_token;
extern PqlToken boolean_token;

extern PqlToken follow_token;
extern PqlToken follow_t_token;
extern PqlToken parent_token;
extern PqlToken parent_t_token;
extern PqlToken use_token;
extern PqlToken modify_token;
extern PqlToken pql_calls_token;
extern PqlToken pql_calls_t_token;
extern PqlToken affects_token;
extern PqlToken affects_t_token;
extern PqlToken next_token;
extern PqlToken next_t_token;
extern PqlToken pattern_token;
extern PqlToken with_token;

#endif
