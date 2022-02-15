#ifndef SPA_SRC_SPA_SRC_COMPONENTS_QUERY_SUBSYSTEM_PQL_EVALUATOR_TABLE_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_QUERY_SUBSYSTEM_PQL_EVALUATOR_TABLE_H_

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
 public:
  std::unordered_map<std::string, size_t> synonym_to_col_idx_map_;

  Table(std::string synonym, std::unordered_set<std::string> possible_single_values);
  Table(std::string first_synonym, std::string second_synonym,
                     std::unordered_set<std::pair<std::string, std::string>> possible_pair_values);
  void Join(Table& other_table);
  void NaturalJoin(Table& other_table);
  void CrossJoin(Table& other_table);

 private:
  std::vector<std::vector<std::string>> table_;
  size_t next_col_idx_ = 0;

  std::unordered_set<std::string> GetSharedSynonyms(Table& other_table);
  bool HasSharedSynonym(Table& other_table);
};

}

#endif //SPA_SRC_SPA_SRC_COMPONENTS_QUERY_SUBSYSTEM_PQL_EVALUATOR_TABLE_H_
