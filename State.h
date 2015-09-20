#pragma once

#include "Context.h"

class State
{
	protected:
    Context* context;
	
	public:
    State(Context* context)
    {
      this->context = context;
    }
     
    Context* getContext()
    {
      return context;
    }
    
    virtual void init()
    {
    
    }
    
    virtual void applyRules()
    {
      
    }
};
