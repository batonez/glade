package ru.hatchery.glade.ai;

import java.util.ArrayDeque;
import java.util.Deque;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Map;
import java.util.NoSuchElementException;
import java.util.Set;

import ru.hatchery.glade.model.Graph;
import ru.hatchery.glade.model.GraphEdge;
import ru.hatchery.glade.model.GraphNode;

public class DijkstraPathFinder
{
	private Deque<GraphNode> route		 			= new ArrayDeque<GraphNode>();
	private Set<GraphEdge> frontierEdges 			= new HashSet<GraphEdge>();
	private Set<GraphNode> checkedNodes 			= new HashSet<GraphNode>();
	private Map<GraphNode,GraphNode> routeWaypoints = new HashMap<GraphNode,GraphNode>();
	private Map<GraphNode,Float> shortestPaths 		= new HashMap<GraphNode,Float>();
	
	public Deque<GraphNode> findPath(Graph graph, GraphNode source, GraphNode destination)
	{
		if (destination == null || source == null) {
			return route;
		}
		
		frontierEdges.clear();
		checkedNodes.clear();
		shortestPaths.clear();
		routeWaypoints.clear();
		route.clear();
		shortestPaths.put(source, 0f);
		
		if (dijkstra(source, destination)) {
			GraphNode current = destination;
			
			while (current != null) {
				route.push(current);
				current = routeWaypoints.get(current);
			}
			
			try {
				route.pop();
			} catch (NoSuchElementException e) { }
		}
		
		return route;
	}
	
	private boolean dijkstra(GraphNode source, GraphNode destination)
	{
		GraphNode current = source;
		
		if (current == null) {
			return false;
		}
		
		while (!current.equals(destination)) {
			checkedNodes.add(current);
			
			for (GraphEdge edge: current.getWaysIn()) {
				if (checkedNodes.contains(edge.getSource())) {
					frontierEdges.remove(edge);
				}
			}
			
			for (GraphEdge edge: current.getWaysOut()) {
				if (!checkedNodes.contains(edge.getDestination())) {
					frontierEdges.add(edge);
				}
			}
			
			current = pickNext();
			
			if (current == null) {
				return false;
			}
		}
		
		return true;
	}
	
	private GraphNode pickNext()
	{
		float greedyNumber;
		Float minGreedyNumber 		 = null;
		GraphNode result	 		 = null;
		GraphEdge edge, selectedEdge = null;
		
		Iterator<GraphEdge> i = frontierEdges.iterator();
		
		while (i.hasNext()) {
			edge = i.next();
			greedyNumber = shortestPaths.get(edge.getSource()) + edge.getWeight();
			
			if (minGreedyNumber == null || greedyNumber < minGreedyNumber) {
				minGreedyNumber = greedyNumber;
				result = edge.getDestination();
				selectedEdge = edge;
			}
		}
		
		if (minGreedyNumber != null) {
			shortestPaths.put(result, minGreedyNumber);
			routeWaypoints.put(result, selectedEdge.getSource());
		}
		
		return result;
	}
}
