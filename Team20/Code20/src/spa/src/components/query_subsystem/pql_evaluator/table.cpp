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

std::unordered_set<std::string> Table::GetSharedSynonyms(Table& other_table) {
  std::unordered_set<std::string> synonyms;
  std::unordered_set<std::string> other_synonyms;
  for (auto kv : synonym_to_col_idx_map_) {
    synonyms.insert(kv.first);
  }
  for (auto kv : other_table.synonym_to_col_idx_map_) {
    other_synonyms.insert(kv.first);
  }

  std::unordered_set<std::string> common_synonyms;
  if (synonyms.size() < other_synonyms.size()) {
    for (auto synonym : synonyms) {
      if (other_synonyms.find(synonym) != other_synonyms.end()) {
        common_synonyms.insert(synonym);
      }
    }
  } else {
    for (auto other_synonym : other_synonyms) {
      if (synonyms.find(other_synonym) != synonyms.end()) {
        common_synonyms.insert(other_synonym);

      }
    }
  }

  return common_synonyms;
}

bool Table::HasSharedSynonym(Table &other_table) {
  return !GetSharedSynonyms(other_table).empty();
}

void Table::Join(Table &other_table) {
  if (HasSharedSynonym(other_table)) {
    NaturalJoin(other_table);
  } else {
    CrossJoin(other_table);
  }
}

void NaturalJoin(Table &other_table) {

}

void CrossJoin(Table& other_table) {

}

}