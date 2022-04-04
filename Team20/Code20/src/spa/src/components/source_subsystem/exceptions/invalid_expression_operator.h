#ifndef INVALID_EXPRESSION_OPERATOR_H
#define INVALID_EXPRESSION_OPERATOR_H

#include <exception>
#include <string>

namespace source {

class InvalidExpressionOperatorException : public std::exception {
 private:
  std::string m_msg;

 public:
  static std::string MESSAGE;

 public:
  InvalidExpressionOperatorException();
  [[nodiscard]] virtual const char *what() const noexcept override;
};

}

#endif //INVALID_EXPRESSION_OPERATOR_H
