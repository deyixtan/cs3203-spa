#include "parsed_query.h"

ParsedQuery::ParsedQuery(PqlToken selected_synonym,
                         std::vector<Declaration> declarations,
                         Relationship relationship,
                         Pattern pattern):
                         selected_synonym(selected_synonym),
                         declarations(declarations),
                         relationship(relationship),
                         pattern(pattern) {}