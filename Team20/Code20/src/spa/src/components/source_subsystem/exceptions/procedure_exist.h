#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_EXCEPTIONS_PROCEDURE_EXIST_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_EXCEPTIONS_PROCEDURE_EXIST_H_

#include <exception>
#include <string>

namespace source {

class ProcedureExistException : public std::exception {
 private:
  std::string m_msg;

 public:
  static std::string MESSAGE;

 public:
  ProcedureExistException();
  [[nodiscard]] virtual const char *what() const noexcept override;
};

}

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_EXCEPTIONS_PROCEDURE_EXIST_H_
