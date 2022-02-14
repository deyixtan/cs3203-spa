#include "query_condition.h"

QueryCondition::QueryCondition(){};
QueryCondition::QueryCondition(PqlToken first, PqlToken second) : first(first), second(second);