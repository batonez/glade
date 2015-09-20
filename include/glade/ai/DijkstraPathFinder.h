#pragma once

#include <stack>
#include "glade/math/Graph.h"

class DijkstraPathFinder
{
  private:
    std::stack<GraphNode*> route;
    GraphNode::GraphEdges  frontierEdges;
    Graph::GraphNodes      checkedNodes;
    std::map<GraphNode*,GraphNode*> routeWaypoints;
    std::map<GraphNode*,float> shortestPaths;

  public:
    std::stack<GraphNode*>* findPath(Graph* graph, GraphNode* source, GraphNode* destination)
    {
      if (destination == NULL || source == NULL) {
        return &route;
      }

      frontierEdges.clear();
      checkedNodes.clear();
      shortestPaths.clear();
      routeWaypoints.clear();
      route = std::stack<GraphNode*>();
      shortestPaths.emplace(source, 0.0f);

      if (dijkstra(source, destination)) {
        GraphNode* current = destination;

        while (current != NULL) {
          route.push(current);
          current = routeWaypoints.find(current)->second;
        }

        route.pop();
      }

      return &route;
    }

  private:
    bool dijkstra(GraphNode* source, GraphNode* destination)
    {
      GraphNode* current = source;

      if (current == NULL) {
        return false;
      }

      while (current != destination) {
        checkedNodes.insert(current);

        GraphNode::GraphEdges* waysIn = current->getWaysIn();

        for (GraphNode::GraphEdgesI edge = waysIn->begin(); edge != waysIn->end(); ++edge) {
          if (checkedNodes.find((*edge)->getSource()) != checkedNodes.end()) {
            frontierEdges.erase(*edge);
          }
        }

        GraphNode::GraphEdges* waysOut = current->getWaysOut();

        for (GraphNode::GraphEdgesI edge = waysOut->begin(); edge != waysOut->end(); ++edge) {
          if (checkedNodes.find((*edge)->getDestination()) != checkedNodes.end()) {
            frontierEdges.insert(*edge);
          }
        }

        current = pickNext();

        if (current == NULL) {
          return false;
        }
      }

      return true;
    }

  GraphNode* pickNext()
  {
    float greedyNumber;
    float minGreedyNumber, *minGreedyNumberP = NULL;
    GraphNode *result = NULL;

    GraphNode::GraphEdgesI selectedEdge, edge = frontierEdges.begin();

    while (edge != frontierEdges.end()) {
      greedyNumber = shortestPaths.find((*edge)->getSource())->second + (*edge)->getWeight();

      if (minGreedyNumberP == NULL || greedyNumber < minGreedyNumber) {
        minGreedyNumber = greedyNumber;
        minGreedyNumberP = &greedyNumber;
        result = (*edge)->getDestination();
        selectedEdge = edge;
      }
    }

    if (minGreedyNumberP != NULL) {
      shortestPaths.emplace(result, minGreedyNumber);
      routeWaypoints.emplace(result, (*selectedEdge)->getSource());
    }

    return result;
  }
};
