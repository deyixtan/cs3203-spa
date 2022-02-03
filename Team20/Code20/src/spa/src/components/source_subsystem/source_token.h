#ifndef SOURCETOKEN_H
#define SOURCETOKEN_H

#include <string>

enum TokenType {

};

class SourceToken {
 private:
  TokenType type;
  std::string value;

 public:
  SourceToken(TokenType type, std::string value);
  TokenType GetType();
  std::string GetValue();
};

#endif //SOURCETOKEN_H
