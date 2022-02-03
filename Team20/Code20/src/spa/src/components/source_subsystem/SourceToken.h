#ifndef SPA_SOURCETOKEN_H
#define SPA_SOURCETOKEN_H

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


#endif //SPA_SOURCETOKEN_H
