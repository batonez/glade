#pragma once

#include <set>
#include "../exception/GladeException.h"
#include "GraphNode.h"
#include "GraphEdge.h"

class Graph
{
private:
  std::set<GraphNode*> nodes;
	std::set<GraphEdge*> edges;
	
public:
  ~Graph()
  {
    for (nodes::iterator inode = nodes.begin(); inode != nodes.end(); ++inode) {
      delete *inode;
    }
    
    for (edges::iterator iedge = edges.begin(); iedge != edges.end(); ++iedge) {
      delete *inode;
    }
  }
  
  std::set<GraphNode*>* getNodes(void)
	{
		return nodes;
	}
	
	std::set<GraphEdge*>* getEdges(void)
	{
		return edges;
	}
  
  GraphNode* newNode(void)
  {
    GraphNode* node = new GraphNode();
    nodes.insert(node);
    
    return node;
  }
  
  GraphEdge* newEdge(GraphNode* source, GraphNode* destination, float weight)
  {
    if (!source || !destination || nodes.find(source) == nodes.end() || nodes.find(destination) == nodes.end())
      throw GladeException("There are no such nodes in this graph. Call newNode() first");
    
    GraphEdge* edge = new GraphEdge(source, destination, weight);
    edges.insert(edge);
    
    return edge;
  }
};
