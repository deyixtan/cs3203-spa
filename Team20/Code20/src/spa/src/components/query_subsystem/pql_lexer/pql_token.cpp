#include "pql_token.h"

namespace pql {

PqlToken::PqlToken() {};
PqlToken::PqlToken(PqlTokenType specified_token_type, std::string specified_value) {
  this->type = specified_token_type;
  this->value = specified_value;
}

}

