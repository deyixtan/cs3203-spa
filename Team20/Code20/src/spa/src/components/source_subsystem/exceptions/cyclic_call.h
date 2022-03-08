#ifndef SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_EXCEPTIONS_CYCLIC_CALL_H_
#define SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_EXCEPTIONS_CYCLIC_CALL_H_

#include <exception>
#include <string>

namespace source {

class CyclicCallException : public std::exception {
 private:
  std::string m_msg;

 public:
  static std::string MESSAGE;

 public:
  CyclicCallException();
  [[nodiscard]] virtual const char *what() const noexcept override;
};

}

#endif //SPA_SRC_SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_EXCEPTIONS_CYCLIC_CALL_H_
