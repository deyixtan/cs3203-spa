#ifndef QUERY_GRAMMAR_ERROR_H
#define QUERY_GRAMMAR_ERROR_H

#include <string>

// Declaration
extern std::string MISSING_TOKEN_IN_DECLARATION;
extern std::string INVALID_DESIGN_ENTITY_IN_DECLARATION;
extern std::string INVALID_DECLARATION_FORMAT;
extern std::string INVALID_DECLARATION_SYNONYM;
extern std::string DUPLICATE_DECLARATION_SYNONYM;

// Select clause
extern std::string INVALID_SELECT_KEYWORD;
extern std::string INVALID_SELECT_RESULT;
extern std::string INVALID_SELECT_CLAUSE_FORMAT;

// Rel Ref
extern std::string INVALID_REL_REF_ARGUMENTS;
extern std::string INVALID_REL_REF_TOKEN;
extern std::string INVALID_REL_REF_FORMAT;

// Pattern clause
extern std::string INVALID_PATTERN_CLAUSE_FORMAT;
extern std::string INVALID_PATTERN_KEYWORD;
extern std::string INVALID_PATTERN_SYNONYM;
extern std::string INVALID_PATTERN_CLAUSE_ARGUMENT;

// With clause
extern std::string INVALID_WITH_CLAUSE_FORMAT;

#endif
