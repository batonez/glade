#pragma once

class Context;

class State
{
  public:
    virtual void init(Context &context)
    {    
    }
    
    virtual void applyRules(Context &context)
    { 
    }
    
    virtual void shutdown(Context &context)
    {
    }
    
    virtual ~State()
    {
    }
};
