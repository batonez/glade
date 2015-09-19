#pragma once

#include <set>
#include "GraphEdge.h"

class GraphNode
{
private:
  std::set<GraphEdge*> waysOut;
  std::set<GraphEdge*> waysIn;

public:
  std::set<GraphEdge*>* getWaysOut(void)
	{
		return &waysOut;
	}
	
	std::set<GraphEdge*>* getWaysIn(void)
	{
		return &waysIn;
	}
};
