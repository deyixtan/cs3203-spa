#ifndef SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_EXCEPTIONS_END_OF_STREAM_H_
#define SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_EXCEPTIONS_END_OF_STREAM_H_

#include <exception>
#include <string>

namespace source {

class EndOfStreamException : public std::exception {
 private:
  std::string m_msg;

 public:
  EndOfStreamException();
  [[nodiscard]] virtual const char *what() const noexcept override;
};

}

#endif //SPA_SRC_COMPONENTS_SOURCE_SUBSYSTEM_EXCEPTIONS_END_OF_STREAM_H_
