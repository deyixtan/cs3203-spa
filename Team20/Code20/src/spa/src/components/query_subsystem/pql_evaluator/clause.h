#ifndef CLAUSE_H_
#define CLAUSE_H_

#include "table.h"

namespace pql {

class Clause {
 public:
  virtual Table Execute() = 0;
  virtual ~Clause() = default;
};

}

#endif //CLAUSE_H_
