#ifndef INVALID_QUERY_FORMAT_H_
#define INVALID_QUERY_FORMAT_H_

#include <exception>
#include <string>

 class InvalidQueryFormatException : public std::exception {
  private:
   std::string m_msg;

  public:
   InvalidQueryFormatException();
   [[nodiscard]] const char *what() const noexcept override;
};

#endif //INVALID_QUERY_FORMAT_H_
