#ifndef INVALID_TERM_OPERATOR_H
#define INVALID_TERM_OPERATOR_H

#include <exception>
#include <string>

namespace source {

class InvalidTermOperatorException : public std::exception {
 private:
  std::string m_msg;

 public:
  static std::string MESSAGE;

 public:
  InvalidTermOperatorException();
  [[nodiscard]] virtual const char *what() const noexcept override;
};

}

#endif //INVALID_TERM_OPERATOR_H
