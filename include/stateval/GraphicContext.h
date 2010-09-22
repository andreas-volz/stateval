#ifndef GRAPHIC_CONTEXT_H
#define GRAPHIC_CONTEXT_H

/* EFL */
#include <evasxx/Evasxx.h>

class GraphicContext
{
public:
  static GraphicContext& instance ();
  
  void init (Evasxx::Canvas &evas);

  void setResolution (const Eflxx::Size &resolution);

  const Eflxx::Size getResolution ();
  
  Evasxx::Canvas &getCanvas ();
  
private:
  GraphicContext () {}
  GraphicContext (const GraphicContext&);
  virtual ~GraphicContext ();
  
  Evasxx::Canvas *mEvas;
  Eflxx::Size mResolution;
};

#endif // GRAPHIC_CONTEXT_H
