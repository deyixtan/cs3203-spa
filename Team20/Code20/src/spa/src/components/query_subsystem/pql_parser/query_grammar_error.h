#ifndef QUERY_GRAMMAR_ERROR_H
#define QUERY_GRAMMAR_ERROR_H

#include <string>

// Declaration
extern std::string MISSING_TOKEN_IN_DECLARATION;
extern std::string INVALID_DESIGN_ENTITY_IN_DECLARATION;
extern std::string INVALID_DECLARATION_FORMAT;
extern std::string INVALID_DECLARATION_SYNONYM;

// Select Clause
extern std::string INVALID_SELECT_KEYWORD;
extern std::string INVALID_SELECT_SYNONYM;
extern std::string INVALID_SELECT_CLAUSE_FORMAT;

// Rel Ref
extern std::string INVALID_REL_REF_ARGUMENTS;
extern std::string INVALID_REL_REF_TOKEN;
extern std::string INVALID_REL_REF_FORMAT;

// Pattern Clause
extern std::string INVALID_PATTERN_CLAUSE_FORMAT;
extern std::string INVALID_PATTERN_KEYWORD;
extern std::string INVALID_PATTERN_SYNONYM;
extern std::string INVALID_PATTERN_CLAUSE_ARGUMENT;

#endif