#include "select_boolean_clause.h"

namespace pql {

Table SelectBooleanClause::Execute() {
  Table table;
  table.ToggleBooleanResult();
  return table;
}

}
