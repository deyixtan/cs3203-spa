#ifndef TABLE_H_
#define TABLE_H_

#include "utils/pair_hash.h"
#include "components/query_subsystem/pql_lexer/pql_token.h"
#include "components/query_subsystem/utils.h"
#include "components/pkb/pkb.h"
#include <iostream>
#include <utility>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace pql {

/**
 * Table is an abstraction of the possible results that a relation of synonyms can take.
 * A synonym maps to a certain column in the table.
 *
 * In general we only construct tables with a max of two synonyms since we have
 * a maximum of two synonyms for clause arguments.
 *
 */
class Table {
  using Attribute = std::string;
  using Attributes = std::vector<Attribute>;
  using Record = std::vector<std::string>;
  using Records = std::vector<Record>;

 public:
  Attributes attributes;
  Records records;

  Table();
  Table(const std::string &synonym, std::unordered_set<std::string> &single_constraints);
  Table(const std::string &synonym, std::vector<std::string> &single_constraints);
  Table(const std::string &first_synonym, const std::string &second_synonym,
        std::unordered_set<std::pair<std::string, std::string>, pair_hash> &pair_constraints);
  Table(const std::string &first_synonym, const std::string &second_synonym,
        std::vector<std::pair<std::string, std::string>> &pair_constraints);
  void Merge(Table &other_table);
  void ToggleFalseClause();
  void ToggleBooleanResult();
  void ToggleSynonymResult();
  void ToggleAttributeResult();
  void ToggleTupleResult();
  [[nodiscard]] bool IsAttributesEmpty() const;
  [[nodiscard]] bool IsRecordsEmpty() const;
  [[nodiscard]] bool IsFalseClause() const;
  [[nodiscard]] bool IsBooleanResult() const;
  [[nodiscard]] bool IsSynonymResult() const;
  [[nodiscard]] bool IsAttributeResult() const;
  [[nodiscard]] bool IsTupleResult() const;
  std::unordered_set<std::string> GetResult(const std::string &select_synonym);
  std::unordered_set<std::string> GetTupleResult(const std::vector<PqlToken> &tuple,
                                                 const std::unordered_map<std::string, DesignEntityType> &declarations,
                                                 const PkbPtr &pkb);
  friend std::ostream &operator<<(std::ostream &os, const Table &table);

 private:
  bool is_boolean_result = false;
  bool is_synonym_result = false;
  bool is_attribute_result = false;
  bool is_tuple_result = false;
  bool is_false_clause = false;
  std::vector<std::pair<size_t, size_t>> GetCommonAttributeIndexPairs(const Attributes &other_attributes);
  std::vector<size_t> GetOtherAttributeIndices(const Attributes &other_attributes);
  void NaturalJoin(Table &other_table, std::vector<std::pair<size_t, size_t>> &common_attribute_index_pairs);
  void HashJoin(Table &other_table, std::vector<std::pair<size_t, size_t>> &common_attribute_index_pairs);
  void NestedLoopJoin(Table &other_table, std::vector<std::pair<size_t, size_t>> &common_attribute_index_pairs);
  void CrossJoin(Table &other_table);
  void UpdateResultType(const Table &other_table);
  static std::string JoinRecordBy(const Record &record, const std::string &delimiter);
  size_t GetAttributeIdxFromElem(PqlToken &elem, const std::unordered_map<std::string, DesignEntityType> &declarations);
  static std::string ConvertAttrRef(const DesignEntityType &attr_ref, std::string value, const PkbPtr &pkb);
};

}

#endif //TABLE_H_
