#ifndef GRAPHIC_CONTEXT_H
#define GRAPHIC_CONTEXT_H

/* EFL */
#include <evasxx/Evasxx.h>

class GraphicContext
{
public:
  static GraphicContext& instance ();
  
  void init (Evasxx::Canvas &evas);
  
  Evasxx::Canvas &getCanvas ();
  
private:
  GraphicContext () {}
  GraphicContext (const GraphicContext&);
  virtual ~GraphicContext ();
  
  Evasxx::Canvas *mEvas;
};

#endif // GRAPHIC_CONTEXT_H
