#pragma once

class Context;

class State
{
	protected:
    Context* context;
	
	public:
    State(Context* context)
    {
      this->context = context;
    }
    
    virtual void init()
    {    
    }
    
    virtual void applyRules()
    { 
    }
    
    virtual void shutdown()
    {
    }
};
