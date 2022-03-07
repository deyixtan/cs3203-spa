#ifndef CLAUSE_H_
#define CLAUSE_H_

#include "table.h"

namespace pql {

class Clause {
 public:
  virtual Table Execute();
};

}

#endif //CLAUSE_H_
