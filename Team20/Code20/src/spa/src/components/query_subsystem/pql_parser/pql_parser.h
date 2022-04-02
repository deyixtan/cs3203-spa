#ifndef PQL_PARSER_H_
#define PQL_PARSER_H_

#include "parsed_query.h"

class PqlParser {
 private:
  std::vector<PqlToken> tokens;
  ParsedQuery pq = ParseQuery();
  int cursor;
  void MoveCursor(int);
  PqlToken FetchToken();
  PqlToken FetchNextToken();
  PqlToken ValidateToken(std::unordered_set<PqlTokenType>);
  PqlToken ValidateToken(PqlTokenType);
  int GetEndOfDeclarationCursor();
  void ParseDeclaration();
  void ParseDeclarationVariables(DesignEntityType&, Declaration&);
  void SetDuplicatedSynonyms(Declaration&, std::string);
  bool HasBooleanSynonym();
  void ParseSelectClause();
  void ParseResultClause();
  void ParseSynonymResultClause(ResultClause&, PqlToken&);
  void ParseBooleanResultClause(ResultClause&, PqlToken&);
  void ParseTupleResultClause(ResultClause&, PqlToken&);
  void ParseRelationshipClause();
  void ParsePatternClause();
  void ParseWithClause();
  std::string BOOLEAN_SYNONYM_VALUE = "BOOLEAN";
  std::string INVALID_QUERY_FORMAT = "Invalid Query Format! \n";
 public:
  PqlParser(std::vector<PqlToken>);
  ParsedQuery ParseQuery();
};

#endif //PQL_PARSER_H_
