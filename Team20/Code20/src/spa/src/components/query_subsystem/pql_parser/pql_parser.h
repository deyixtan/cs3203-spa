#ifndef PQL_PARSER_H_
#define PQL_PARSER_H_

#include "parsed_query.h"
#include "../exceptions/invalid_query_format.h"

using namespace pql;

class PqlParser {
 private:
  std::vector<PqlToken> tokens;
  ParsedQuery pq;
  int cursor;
  std::unordered_map<PqlTokenType, std::pair<std::unordered_set<PqlTokenType>, std::unordered_set<PqlTokenType>>>
      rel_ref_arg_map;
  std::unordered_set<PqlTokenType> rel_ref;
  std::unordered_set<PqlTokenType> design_entities;
  std::unordered_set<PqlTokenType> expression_spec;
  std::unordered_set<PqlTokenType> ent_ref;
  std::unordered_set<PqlTokenType> stmt_ref;
  std::unordered_set<PqlTokenType> stmt_ref_and_ent_ref;
  std::unordered_set<PqlTokenType> allowed_synonyms;
  std::unordered_set<PqlTokenType> result_cl;
  std::unordered_set<PqlTokenType> with_clause_ref;
  void MoveCursor(int);
  PqlToken FetchToken();
  PqlToken ValidateToken(std::unordered_set<PqlTokenType>);
  PqlToken ValidateToken(PqlTokenType);
  void RevertToSynonymType();
  int GetEndOfDeclarationCursor();
  void ParseDeclaration();
  void ParseDeclarationVariables(DesignEntityType &, Declaration &);
  void SetDuplicatedSynonyms(Declaration &, std::string);
  bool HasBooleanSynonym();
  void ParseSelectClause();
  void ParseResultClause();
  void ParseSynonymResultClause(ResultClause &, PqlToken &);
  void ParseAttributeResultClause(ResultClause &, PqlToken &);
  void ParseBooleanResultClause(ResultClause &, PqlToken &);
  void ParseTupleResultClause(ResultClause &, PqlToken &);
  void ParseRelationshipClause();
  void ParsePatternClause();
  void ParseWithClause();
 public:
  PqlParser(std::vector<PqlToken>);
  ParsedQuery ParseQuery();
};

#endif //PQL_PARSER_H_
