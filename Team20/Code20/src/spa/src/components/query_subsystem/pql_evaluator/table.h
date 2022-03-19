#ifndef TABLE_H_
#define TABLE_H_

#include "utils/pair_hash.h"
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
  using Attributes = std::vector<std::string>;
  using Records = std::vector<std::vector<std::string>>;

 public:
  Attributes attributes;
  Records records;

  Table();
  Table(const std::string& synonym, std::unordered_set<std::string>& single_constraint);
  Table(const std::string& first_synonym, const std::string& second_synonym,
        std::unordered_set<std::pair<std::string, std::string>, pair_hash>& pair_constraints);
  [[nodiscard]] bool IsAttributesEmpty() const;
  [[nodiscard]] bool IsRecordsEmpty() const;
  void Merge(Table& other_table);
  void EncounteredFalseClause();
  void ToggleBooleanResult();
  [[nodiscard]] bool HasEncounteredFalseClause() const;
  [[nodiscard]] bool IsBooleanResult() const;
  std::unordered_set<std::string> GetResult(const std::string& select_synonym);
  friend std::ostream& operator<<(std::ostream& os, const Table& table);

 private:
  bool is_boolean_result = false;
  bool encountered_false_clause = false;
  std::vector<std::pair<size_t, size_t>> GetCommonAttributeIndexPairs(const Attributes& other_attributes);
  std::vector<size_t> GetOtherAttributeIndices(const Attributes& other_attributes);
  void NaturalJoin(Table& other_table, std::vector<std::pair<size_t, size_t>>& common_attribute_index_pairs);
  void CrossJoin(Table& other_table);
};

}

#endif //TABLE_H_
