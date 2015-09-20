#pragma once

class Context;

class State
{
  public:
    typedef std::unique_ptr<State> Unique;
    
	protected:
    Context* context;
	
	public:
    State(Context &context):
      context(&context)
    {
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
    
    virtual ~State()
    {
    }
};
