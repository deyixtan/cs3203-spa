#include "select_tuple_clause.h"
#include "clause_util.h"

namespace pql {

using namespace clause_util;

SelectTupleClause::SelectTupleClause(std::vector<PqlToken> &tuple,
                                     const std::unordered_map<std::string, DesignEntityType> &declarations,
                                     const PkbPtr &pkb) : tuple(tuple), declarations(declarations), pkb(pkb) {}

Table SelectTupleClause::Execute() {
  Table table;

  for (auto elem : tuple) {
    Table intermediate_table;
    if (elem.type==PqlTokenType::SYNONYM) {
      intermediate_table = HandleSynonymElem(elem);
    } else if (elem.type==PqlTokenType::ATTRIBUTE) {
      intermediate_table = HandleAttrRefElem(elem);
    }

    table.Merge(intermediate_table);
  }

  table.ToggleTupleResult();
  return table;
}

bool SelectTupleClause::ExecuteBool() {
  return false;
}

Table SelectTupleClause::HandleSynonymElem(const PqlToken &synonym) {
  auto single_constraints = pkb->GetStmt(GetStmtType(GetSynonymDesignEntity(synonym, declarations)));
  return {synonym.value, single_constraints};
}

Table SelectTupleClause::HandleAttrRefElem(PqlToken &attr_ref) {
  auto parsed_attr_ref = Utils::ParseAttributeRef(attr_ref, declarations);
  auto attr_ref_design_entity = parsed_attr_ref.first.first;
  auto attr_ref_synonym = parsed_attr_ref.first.second;
  auto single_constraints = pkb->GetStmt(GetStmtType(attr_ref_design_entity));
  return {attr_ref_synonym, single_constraints};
}

std::set<std::string> SelectTupleClause::GetSynonyms() {
  std::set<std::string> synonyms;
  for (auto &elem : tuple) {
    if (IsArgSynonym(elem)) {
      synonyms.emplace(elem.value);
    }
    if (IsArgAttribute(elem)) {
      auto attribute = Utils::ParseAttributeRef(elem, declarations);
      synonyms.emplace(attribute.first.second);
    }
  }

  return synonyms;
}

size_t SelectTupleClause::GetSynonymsSize() {
  size_t size = 0;
  for (auto &elem : tuple) {
    if (IsArgSynonym(elem)) {
      size++;
    }
    if (IsArgAttribute(elem)) {
      size++;
    }
  }

  return size;
}

size_t SelectTupleClause::GetWeight() {
  // select clauses never inside any of the groups that require sorting
  return 0;
}

}
