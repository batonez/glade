#pragma once

#include <string.h>

class GladeException
{
private:
  char message[512];
  
public:
  GladeException(const char* message)
  {
    int messageLength = strlen(message);
    int sizeToCopy = messageLength < 511 ? messageLength : 511;
    memcpy(this->message, message, sizeof(char) * sizeToCopy);
    this->message[sizeToCopy] = '\0';
  }
  
  const char* getMessage()
  {
    return message;
  }
};