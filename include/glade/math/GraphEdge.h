#pragma once

#include "GraphNode.h"

class GraphEdge
{
private:
	float weight;
	GraphNode* destination;
	GraphNode* source;
	
public:
  GraphEdge(GraphNode* source, GraphNode* destination, float weight) :
  weight(weight), destination(destination), source(source)
	{
		destination->getWaysIn()->insert(this);
		source->getWaysOut()->insert(this);
	}
	
	float getWeight(void)
	{
		return weight;
	}
	
	GraphNode* getDestination(void)
	{
		return destination;
	}
	
	GraphNode* getSource(void)
	{
		return source;
	}
};
