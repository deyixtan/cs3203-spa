#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_EXCEPTIONS_EMPTY_STATEMENT_LIST_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_EXCEPTIONS_EMPTY_STATEMENT_LIST_H_

#include <exception>
#include <string>

namespace source {

class EmptyStatementListException : public std::exception {
 private:
  std::string m_msg;

 public:
  EmptyStatementListException();
  [[nodiscard]] virtual const char *what() const noexcept override;
};

}

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_EXCEPTIONS_EMPTY_STATEMENT_LIST_H_
