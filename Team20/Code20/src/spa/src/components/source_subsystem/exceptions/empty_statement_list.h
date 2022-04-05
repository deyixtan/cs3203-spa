#ifndef EMPTY_STATEMENT_LIST_H
#define EMPTY_STATEMENT_LIST_H

#include "components/source_subsystem/source_declarations.h"

namespace source {

class EmptyStatementListException : public Exception {
 private:
  String m_msg;

 public:
  EmptyStatementListException();
  [[nodiscard]] const char *what() const noexcept override;
};

}

#endif //EMPTY_STATEMENT_LIST_H
