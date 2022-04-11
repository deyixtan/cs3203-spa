#ifndef SELECT_TUPLE_CLAUSE_H_
#define SELECT_TUPLE_CLAUSE_H_

#include "components/query_subsystem/pql_evaluator/clauses/clause.h"
#include "components/pkb/pkb.h"
#include "components/query_subsystem/utils.h"

namespace pql {

class SelectTupleClause : public Clause {
 public:
  SelectTupleClause(std::vector<PqlToken> &tuple,
                    const std::unordered_map<std::string, DesignEntityType> &declarations,
                    const pkb::PkbPtr &pkb);
  SelectTupleClause(const std::unordered_set<std::string> &table_synonyms,
                    std::vector<PqlToken> &tuple,
                    const std::unordered_map<std::string, DesignEntityType> &declarations,
                    const pkb::PkbPtr &pkb);
  Table Execute() override;
  bool ExecuteBool() override;
  std::set<std::string> GetSynonyms() override;
  size_t GetSynonymsSize() override;
  size_t GetWeight() override;
 private:
  std::unordered_set<std::string> table_synonyms;
  std::vector<PqlToken> tuple;
  std::unordered_map<std::string, DesignEntityType> declarations;
  const pkb::PkbPtr &pkb;
  Table HandleSynonymElem(const PqlToken &synonym);
  Table HandleAttrRefElem(PqlToken &attr_ref);
};

}

#endif //SELECT_TUPLE_CLAUSE_H_
