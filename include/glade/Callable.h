#pragma once

class Callable
{
  public:
    typedef std::set<Callable*>        Callables;
    typedef Callables::iterator        CallablesI;
    typedef std::unique_ptr<Callable> AutoDispose;
    
    virtual void call() = 0;
    virtual ~Callable() {}
};
