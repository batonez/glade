#pragma once

#include "GladeException.h"

class GladeFileNotFoundException: public GladeException
{ 
public:
  GladeFileNotFoundException(const char* message):
    GladeException(message)
  {
  }
};