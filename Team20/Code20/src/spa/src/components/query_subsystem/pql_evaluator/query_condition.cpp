#include "query_condition.h"

QueryCondition::QueryCondition(){}
QueryCondition::QueryCondition(PqlToken first_arg, PqlToken second_arg) : first_arg(first_arg), second_arg(second_arg) {}