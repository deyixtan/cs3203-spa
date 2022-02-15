#include "table.h"

namespace pql {

Table::Table(std::string synonym, std::unordered_set<std::string> possible_single_values) {
  synonym_to_col_idx_map_[synonym] = next_col_idx_++;
  for (auto possible_single_value : possible_single_values) {
    table_.push_back(std::vector{possible_single_value});
  }
}

Table::Table(std::string first_synonym, std::string second_synonym,
             std::unordered_set<std::pair<std::string, std::string>> possible_pair_values){
  synonym_to_col_idx_map_[first_synonym] = next_col_idx_++;
  synonym_to_col_idx_map_[second_synonym] = next_col_idx_++;
  for (auto possible_pair_value : possible_pair_values) {
    table_.push_back(std::vector{possible_pair_value.first, possible_pair_value.second});
  }
}

}