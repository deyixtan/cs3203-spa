#include "table.h"

#include <initializer_list>

namespace pql {

Table::Table(): attributes({}), records({}) {}

Table::Table(const std::string& synonym, std::unordered_set<std::string>& single_constraints) {
  attributes.emplace_back(synonym);
  for (const auto& single_constraint : single_constraints) {
    records.emplace_back(std::initializer_list<std::string>{single_constraint});
  }
  if (single_constraints.empty()) {
    EncounteredFalseClause();
  }
}

Table::Table(const std::string &first_synonym,
             const std::string &second_synonym,
             std::unordered_set<std::pair<std::string, std::string>, pair_hash> &pair_constraints) {
  attributes.emplace_back(first_synonym);
  attributes.emplace_back(second_synonym);
  for (const auto& pair_constraint : pair_constraints) {
    records.emplace_back(std::initializer_list<std::string>{pair_constraint.first, pair_constraint.second});
  }
  if (pair_constraints.empty()) {
    EncounteredFalseClause();
  }
}

bool Table::IsRecordsEmpty() const {
  return records.empty();
}

void Table::Merge(Table &other_table) {
  if (!IsBooleanResult() && other_table.IsBooleanResult()) {
    ToggleBooleanResult();
  }

  if (HasEncounteredFalseClause()) {
    return;
  }

  if (other_table.HasEncounteredFalseClause()) {
    EncounteredFalseClause();
    return;
  }

  auto common_attribute_index_pairs = GetCommonAttributeIndexPairs(other_table.attributes);
  if (!common_attribute_index_pairs.empty()) {
    NaturalJoin(other_table, common_attribute_index_pairs);
  } else {
    CrossJoin(other_table);
  }
}

std::vector<std::pair<size_t, size_t>> Table::GetCommonAttributeIndexPairs(const Table::Attributes &other_attributes) {
  std::vector<std::pair<size_t, size_t>> common_attribute_index_pairs;
  std::unordered_map<std::string, size_t> attributes_to_index;
  for (size_t i = 0; i < attributes.size(); ++i) {
    attributes_to_index.insert({attributes.at(i), i});
  }
  for (size_t i = 0; i < other_attributes.size(); ++i) {
    auto it = attributes_to_index.find(other_attributes.at(i));
    if (it != attributes_to_index.end()) {
      common_attribute_index_pairs.emplace_back(it->second, i);
    }
  }
  return common_attribute_index_pairs;
}

std::vector<size_t> Table::GetOtherAttributeIndices(const Attributes &other_attributes) {
  std::vector<size_t> other_attribute_indices;
  std::unordered_map<std::string, size_t> attributes_to_index;
  for (size_t i = 0; i < attributes.size(); ++i) {
    attributes_to_index.insert({attributes.at(i), i});
  }
  for (size_t i = 0; i < other_attributes.size(); ++i) {
    auto it = attributes_to_index.find(other_attributes.at(i));
    if (it == attributes_to_index.end()) {
      other_attribute_indices.emplace_back(i);
    }
  }
  return other_attribute_indices;
}

//TODO: Replace with Hash Join
void Table::NaturalJoin(Table &other_table, std::vector<std::pair<size_t, size_t>> &common_attribute_index_pairs) {
  auto other_attribute_indices = GetOtherAttributeIndices(other_table.attributes);

  for (size_t i : other_attribute_indices) {
    attributes.emplace_back(other_table.attributes.at(i));
  }

  Records new_records;
  for (auto & record : records) {
    for (auto & other_record_it : other_table.records) {
      bool record_match_on_common_attribute_indices = true;
      for (auto common_attribute_index_pair : common_attribute_index_pairs) {
        if (record.at(common_attribute_index_pair.first) != other_record_it.at(common_attribute_index_pair.second)){
          record_match_on_common_attribute_indices = false;
          break;
        }
      }

      if (record_match_on_common_attribute_indices) {
        auto new_record = record;
        for (auto other_attribute_index : other_attribute_indices) {
          new_record.emplace_back(other_record_it.at(other_attribute_index));
        }
        new_records.emplace_back(new_record);
      }
    }
  }
  records = std::move(new_records);
}
void Table::CrossJoin(Table &other_table) {
  attributes.insert(attributes.end(), other_table.attributes.begin(), other_table.attributes.end());
  if (other_table.IsRecordsEmpty()) {
    return;
  } else if (IsRecordsEmpty()) {
    records = std::move(other_table.records);
    return;
  }

  Records new_records;
  for (const auto& record : records) {
    for (const auto& other_record : other_table.records) {
      auto new_record = record;
      new_record.reserve(record.size() + other_record.size());
      new_record.insert(new_record.end(), other_record.begin(), other_record.end());
      new_records.emplace_back(new_record);
    }
  }

  records = std::move(new_records);
}

std::unordered_set<std::string> Table::GetResult(const std::string& select_synonym) {
  std::unordered_set<std::string> result({});
  auto itr = std::find(attributes.begin(), attributes.end(), select_synonym);
  if (itr != attributes.cend()) {
    int index = std::distance(attributes.begin(), itr);
    for (auto row : records) {
      result.insert(row[index]);
    }
  }
  return result;
}

std::ostream& operator<<(std::ostream& os, const Table& table) {
  for (const auto& attribute : table.attributes) {
    os << attribute << "\t";
  }
  os << std::endl;
  os << "_______________________________________________________" << std::endl;

  for (const auto& record : table.records) {
    for (const auto& value : record) {
      os << value << "\t";
    }
    os << std::endl;
  }

  return os;
}

void Table::EncounteredFalseClause() {
  encountered_false_clause = true;
}

bool Table::HasEncounteredFalseClause() const {
  return encountered_false_clause;
}

void Table::ToggleBooleanResult() {
  is_boolean_result ^= true;
}

bool Table::IsBooleanResult() const {
  return is_boolean_result;
}

}