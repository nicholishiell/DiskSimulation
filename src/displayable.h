#ifndef DISP
#define DISP

//#include "xHandle.h"
class xHandle;

class Displayable{ 
public:
  virtual void paint(xHandle* xHandler) = 0;
};    

#endif
