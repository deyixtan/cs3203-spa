#ifndef CLAUSE_H_
#define CLAUSE_H_

#include "table.h"
#include <set>
#include <string>

namespace pql {

class Clause {
 public:
  virtual Table Execute() = 0;
  virtual bool ExecuteBool() = 0;
  virtual ~Clause() = default;
  virtual std::set<std::string> GetSynonyms() = 0;
  virtual size_t GetSynonymsSize() = 0;
//  virtual int GetWeight() = 0;
};

}

#endif //CLAUSE_H_
