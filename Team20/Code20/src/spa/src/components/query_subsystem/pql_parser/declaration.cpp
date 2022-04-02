#include "declaration.h"

Declaration::Declaration() : declarations(declarations), has_duplicated_synonyms(false) {}
Declaration::Declaration(std::unordered_map<std::string, DesignEntityType> declarations) : declarations(declarations), has_duplicated_synonyms(false) {}
std::unordered_map<std::string, DesignEntityType> Declaration::GetDeclarations() {
  return declarations;
};
bool Declaration::HasDuplicatedSynonyms() {
  return has_duplicated_synonyms;
}

void Declaration::SetDuplicatedSynonyms() {
  has_duplicated_synonyms = true;
}

void Declaration::AddDeclaration(std::string value, DesignEntityType design_entity_type) {
  declarations.insert({value, design_entity_type});
}
