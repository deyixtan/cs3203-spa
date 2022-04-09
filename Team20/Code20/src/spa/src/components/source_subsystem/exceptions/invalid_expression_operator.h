#ifndef INVALID_EXPRESSION_OPERATOR_H
#define INVALID_EXPRESSION_OPERATOR_H

#include "components/source_subsystem/source_declarations.h"

namespace source {

class InvalidExpressionOperatorException : public Exception {
 private:
  String m_msg;

 public:
  InvalidExpressionOperatorException();
  [[nodiscard]] const char *what() const noexcept override;
};

}

#endif //INVALID_EXPRESSION_OPERATOR_H
