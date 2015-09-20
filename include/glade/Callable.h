#pragma once

class Callable
{
  public:
    typedef std::set<Callable*> Callables;
    typedef Callables::iterator CallablesI;
    
    virtual void call() = 0;
};
