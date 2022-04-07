#ifndef UNRECOGNISED_TOKEN_H_
#define UNRECOGNISED_TOKEN_H_

#include <exception>
#include <string>

class UnrecognisedTokenException : public std::exception {
 private:
  std::string m_msg;

 public:
  UnrecognisedTokenException();
  [[nodiscard]] const char *what() const noexcept override;
};

#endif //UNRECOGNISED_TOKEN_H_
