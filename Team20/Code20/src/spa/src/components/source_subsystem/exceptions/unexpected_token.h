#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_EXCEPTIONS_UNEXPECTED_TOKEN_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_EXCEPTIONS_UNEXPECTED_TOKEN_H_

#include <exception>
#include <string>

namespace source {

class UnexpectedTokenException : public std::exception {
 private:
  std::string m_msg;

 public:
  UnexpectedTokenException();
  [[nodiscard]] virtual const char *what() const noexcept override;
};

}

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_EXCEPTIONS_UNEXPECTED_TOKEN_H_