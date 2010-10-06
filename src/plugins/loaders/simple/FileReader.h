#ifndef FILE_READER_H
#define FILE_READER_H

#include <sstream>
#include <iostream>
#include <fstream>

/// define the maximum length of an input line
#define MAX_LINE_BUFFER 1024

/*! \brief Wrapper for ifstream using C locale
 */
class FileReader : public std::ifstream
{
private:
  char *locale;

// Intentionally declared as private because not implemented yet
private:
   FileReader           (const FileReader&);
   FileReader& operator=(const FileReader&);

public:
  FileReader () : locale (NULL) {}

  /// destructor closes the file
  virtual ~FileReader () {close ();}

  /*!
  * \param filename the file to open for reading
  * \return true if the file could be opened.
  */
  virtual bool open (const std::string& filename);

  /// closes the currently opened file
  virtual void close ();

};

#endif // FILE_READER_H

