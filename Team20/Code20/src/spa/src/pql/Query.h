#include <unordered_map>
#include <vector>

typedef std::unordered_map<std::string, TokenType> DECLARATIONS;
typedef std::vector<RelationshipClause> RELATIONSHIPS;
typedef std::vector<PatternClause> PATTERNS;

/** Structure for Relationship clause
 */
struct RelationshipClause {
    PqlToken argFirst;
    PqlToken argSecond;
    PqlToken relationship;
};

/** Structure for Pattern clause
 */
struct PatternClause {
    PqlToken synonym;
    PqlToken argFirst;
    PqlToken argSecond;
};

/** Query structure to be sent to evaluator
 */
struct Query {
    DECLARATIONS declarations;
    RELATIONSHIPS relationships;
    PATTERNS patterns;
};