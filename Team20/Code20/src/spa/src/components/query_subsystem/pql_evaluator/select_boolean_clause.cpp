#include "select_boolean_clause.h"

namespace pql {

Table SelectBooleanClause::Execute() {
  return clause_util::ConstructEmptyTable();
}

}
