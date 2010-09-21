#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>

#define ERROR_POS std::string ("(") + std::string(__FILE__) + std::string (":") + toString(__LINE__) + std::string (")")
#define THROW_POS std::string ("\n\t@throw: ") + ERROR_POS
#define CATCH_POS std::string ("\n\t@catch: ") + ERROR_POS

class FileNotFoundException : public std::exception
{
public:
  FileNotFoundException (const std:: string &file) : txt (file) {}
  
  virtual ~FileNotFoundException () throw() {}

  const char* what () const throw ();

private:
  std::string txt;
};

class ConvertFromStringException : public std::exception
{
public:
  ConvertFromStringException (const std:: string &str) : txt (str) {}
  
  virtual ~ConvertFromStringException () throw() {}

  const char* what () const throw ();

private:
  std::string txt;
};


#endif // EXCEPTIONS_H
