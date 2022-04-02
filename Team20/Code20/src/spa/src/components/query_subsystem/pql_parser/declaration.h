#ifndef DECLARATION_H_
#define DECLARATION_H_

#include "../pql_lexer/pql_token.h"
#include "../utils.h"

class Declaration {
 private:
  std::unordered_map<std::string, DesignEntityType> declarations;
  bool has_duplicated_synonyms;
 public:
  Declaration();
  Declaration(std::unordered_map<std::string, DesignEntityType>);
  std::unordered_map<std::string, DesignEntityType> GetDeclarations();
  bool HasDuplicatedSynonyms();
  void SetDuplicatedSynonyms();
  void AddDeclaration(std::string, DesignEntityType);
};

#endif //DECLARATION_H_
