#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_EXCEPTIONS_INVALID_PARSE_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_EXCEPTIONS_INVALID_PARSE_H_

#include <exception>
#include <string>

namespace source {

class InvalidParseException : public std::exception {
 private:
  std::string m_msg;

 public:
  InvalidParseException(std::string msg);
  [[nodiscard]] virtual const char *what() const noexcept override;
};

}

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_EXCEPTIONS_INVALID_PARSE_H_
