#include "table.h"
#include "clause_util.h"

#include <initializer_list>

namespace pql {

using namespace clause_util;

Table::Table() : attributes({}), records({}) {}

Table::Table(const std::string &synonym, std::unordered_set<std::string> &single_constraints) {
  attributes.emplace_back(synonym);
  for (const auto &single_constraint : single_constraints) {
    records.emplace_back(std::initializer_list<std::string>{single_constraint});
  }
  if (single_constraints.empty()) {
    ToggleFalseClause();
  }
}

Table::Table(const std::string &first_synonym,
             const std::string &second_synonym,
             std::unordered_set<std::pair<std::string, std::string>, pair_hash> &pair_constraints) {
  attributes.emplace_back(first_synonym);
  attributes.emplace_back(second_synonym);
  for (const auto &pair_constraint : pair_constraints) {
    records.emplace_back(std::initializer_list<std::string>{pair_constraint.first, pair_constraint.second});
  }
  if (pair_constraints.empty()) {
    ToggleFalseClause();
  }
}

void Table::Merge(Table &other_table) {
  UpdateResultType(other_table);

  if (IsFalseClause()) {
    // short circuit if current table has already is_false_clause
    return;
  }

  if (other_table.IsFalseClause()) {
    // maintain invariant that FALSE CLAUSE TABLE -> no attribute, no records, is_false_clause = true;
    ToggleFalseClause();
    attributes.clear();
    records.clear();
    return;
  }

  auto common_attribute_index_pairs = GetCommonAttributeIndexPairs(other_table.attributes);
  if (!common_attribute_index_pairs.empty()) {
    NaturalJoin(other_table, common_attribute_index_pairs);
  } else {
    CrossJoin(other_table);
  }

  if (!IsAttributesEmpty() && IsRecordsEmpty()) {
    // if merging two tables with attributes causes empty records
    // there are no possible values so we emulate encountering false clause
    ToggleFalseClause();
  }
}

std::vector<std::pair<size_t, size_t>> Table::GetCommonAttributeIndexPairs(const Table::Attributes &other_attributes) {
  std::vector<std::pair<size_t, size_t>> common_attribute_index_pairs;
  std::unordered_map<std::string, size_t> attributes_to_index;
  for (size_t i = 0; i < attributes.size(); ++i) {
    attributes_to_index.emplace(attributes.at(i), i);
  }
  for (size_t i = 0; i < other_attributes.size(); ++i) {
    auto it = attributes_to_index.find(other_attributes.at(i));
    if (it!=attributes_to_index.end()) {
      common_attribute_index_pairs.emplace_back(it->second, i);
    }
  }
  return common_attribute_index_pairs;
}

std::vector<size_t> Table::GetOtherAttributeIndices(const Attributes &other_attributes) {
  std::vector<size_t> other_attribute_indices;
  std::unordered_map<std::string, size_t> attributes_to_index;
  for (size_t i = 0; i < attributes.size(); ++i) {
    attributes_to_index.emplace(attributes.at(i), i);
  }
  for (size_t i = 0; i < other_attributes.size(); ++i) {
    auto it = attributes_to_index.find(other_attributes.at(i));
    if (it==attributes_to_index.end()) {
      other_attribute_indices.emplace_back(i);
    }
  }
  return other_attribute_indices;
}

void Table::NaturalJoin(Table &other_table, std::vector<std::pair<size_t, size_t>> &common_attribute_index_pairs) {
  HashJoin(other_table, common_attribute_index_pairs);
}

void Table::HashJoin(Table &other_table, std::vector<std::pair<size_t, size_t>> &common_attribute_index_pairs) {
  auto other_attribute_indices = GetOtherAttributeIndices(other_table.attributes);
  for (size_t i : other_attribute_indices) {
    attributes.emplace_back(other_table.attributes.at(i));
  }

  std::multimap<Record, Record> common_record_to_other_record;
  for (auto other_table_record : other_table.records) {
    Record common_record;
    common_record.reserve(common_attribute_index_pairs.size());
    for (auto common_attribute_index_pair : common_attribute_index_pairs) {
      common_record.emplace_back(other_table_record.at(common_attribute_index_pair.second));
    }

    Record other_record;
    other_record.reserve(other_attribute_indices.size());
    for (auto other_attribute_index : other_attribute_indices) {
      other_record.emplace_back(other_table_record.at(other_attribute_index));
    }

    common_record_to_other_record.emplace(common_record, other_record);
  }

  Records new_records;
  for (auto record : records) {
    Record common_record;
    common_record.reserve(common_attribute_index_pairs.size());
    for (auto common_attribute_index_pair : common_attribute_index_pairs) {
      common_record.emplace_back(record.at(common_attribute_index_pair.first));
    }

    auto range = common_record_to_other_record.equal_range(common_record);
    for (auto it = range.first; it!=range.second; ++it) {
      Record new_record = record;
      for (auto value : it->second) {
        new_record.emplace_back(value);
      }
      new_records.emplace_back(new_record);
    }
  }

  records = std::move(new_records);
}

void Table::NestedLoopJoin(Table &other_table, std::vector<std::pair<size_t, size_t>> &common_attribute_index_pairs) {
  auto other_attribute_indices = GetOtherAttributeIndices(other_table.attributes);

  for (size_t i : other_attribute_indices) {
    attributes.emplace_back(other_table.attributes.at(i));
  }

  Records new_records;
  for (auto &record : records) {
    for (auto &other_record_it : other_table.records) {
      bool record_match_on_common_attribute_indices = true;
      for (auto common_attribute_index_pair : common_attribute_index_pairs) {
        if (record.at(common_attribute_index_pair.first)!=other_record_it.at(common_attribute_index_pair.second)) {
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
  for (const auto &record : records) {
    for (const auto &other_record : other_table.records) {
      auto new_record = record;
      new_record.reserve(record.size() + other_record.size());
      new_record.insert(new_record.end(), other_record.begin(), other_record.end());
      new_records.emplace_back(new_record);
    }
  }

  records = std::move(new_records);
}

std::unordered_set<std::string> Table::GetResult(const std::string &select_synonym) {
  std::unordered_set<std::string> result({});
  auto itr = std::find(attributes.begin(), attributes.end(), select_synonym);
  if (itr!=attributes.cend()) {
    int index = std::distance(attributes.begin(), itr);
    for (auto row : records) {
      result.insert(row[index]);
    }
  }
  return result;
}

std::unordered_set<std::string> Table::GetTupleResult(const std::vector<PqlToken> &tuple,
                                                      const std::unordered_map<std::string,
                                                                               DesignEntityType> &declarations,
                                                      PKB *pkb) {
  std::unordered_set<std::string> result;
  const std::string WHITESPACE = " ";

  for (auto record : records) {
    Record new_record;
    for (auto elem : tuple) {
      auto idx = GetAttributeIdxFromElem(elem, declarations);
      if (elem.type==PqlTokenType::ATTRIBUTE) {
        auto parsed_attr_ref = Utils::ParseAttributeRef(elem, declarations);
        auto attr_ref_design_entity = parsed_attr_ref.first.first;
        auto attr_ref_attr_name = parsed_attr_ref.second;
        if (Utils::IsConversionNeeded(attr_ref_design_entity, attr_ref_attr_name)) {
          new_record.push_back(ConvertAttrRef(attr_ref_design_entity, record[idx], pkb));
        } else {
          new_record.push_back(record[idx]);
        }
      }
      if (elem.type==PqlTokenType::SYNONYM) {
        new_record.push_back(record[idx]);
      }
    }

    result.insert(JoinRecordBy(new_record, WHITESPACE));
  }

  return result;
}

size_t Table::GetAttributeIdxFromElem(PqlToken &elem,
                                      const std::unordered_map<std::string, DesignEntityType> &declarations) {
  if (elem.type==PqlTokenType::ATTRIBUTE) {
    auto parsed_attr_ref = Utils::ParseAttributeRef(elem, declarations);
    auto attr_ref_synonym = parsed_attr_ref.first.second;
    auto it = std::find(attributes.begin(), attributes.end(), attr_ref_synonym);
    return it - attributes.begin();
  } else { // elem.type == PqlTokenType::SYNONYM
    auto it = std::find(attributes.begin(), attributes.end(), elem.value);
    return it - attributes.begin();
  }
}

std::string Table::ConvertAttrRef(const DesignEntityType &attr_ref_design_entity, std::string value, PKB *pkb) {
  auto new_value = pkb->GetNameByStmt(GetStmtType(attr_ref_design_entity), value);
  return new_value;
}

std::string Table::JoinRecordBy(const Record &record, const std::string &delimiter) {
  std::string result_string;
  for (size_t i = 0; i < record.size(); ++i) {
    result_string += record[i] + (i!=record.size() - 1 ? delimiter : "");
  }
  return result_string;
}

std::ostream &operator<<(std::ostream &os, const Table &table) {
  for (const auto &attribute : table.attributes) {
    os << attribute << "\t";
  }
  os << std::endl;
  os << "_______________________________________________________" << std::endl;

  for (const auto &record : table.records) {
    for (const auto &value : record) {
      os << value << "\t";
    }
    os << std::endl;
  }

  return os;
}

bool Table::IsAttributesEmpty() const {
  return attributes.empty();
}

bool Table::IsRecordsEmpty() const {
  return records.empty();
}

void Table::ToggleFalseClause() {
  is_false_clause = true;
}

bool Table::IsFalseClause() const {
  return is_false_clause;
}

void Table::ToggleBooleanResult() {
  is_boolean_result ^= true;
}

void Table::ToggleSynonymResult() {
  is_synonym_result ^= true;
}

void Table::ToggleAttributeResult() {
  is_attribute_result ^= true;
}

void Table::ToggleTupleResult() {
  is_tuple_result ^= true;
}

bool Table::IsBooleanResult() const {
  return is_boolean_result;
}

bool Table::IsSynonymResult() const {
  return is_synonym_result;
}

bool Table::IsAttributeResult() const {
  return is_attribute_result;
}

bool Table::IsTupleResult() const {
  return is_tuple_result;
}

void Table::UpdateResultType(const Table &other_table) {
  if (!IsBooleanResult() && other_table.IsBooleanResult()) {
    ToggleBooleanResult();
  }

  if (!IsAttributeResult() && other_table.IsAttributeResult()) {
    ToggleAttributeResult();
  }

  if (!IsSynonymResult() && other_table.IsSynonymResult()) {
    ToggleSynonymResult();
  }

  if (!IsTupleResult() && other_table.IsTupleResult()) {
    ToggleTupleResult();
  }
}

}