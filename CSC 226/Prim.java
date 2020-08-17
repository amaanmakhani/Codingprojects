/*
Amaan Makhani
CSC 226
Description: A program to implement Lazy Prim's algorithm.
*/

import edu.princeton.cs.algs4.*;
import java.util.Arrays;

public class Prim {
    public static void main(String[] args){
        EdgeWeightedGraph graph = new EdgeWeightedGraph(5);
        Boolean[] marked = new Boolean[5];
        Arrays.fill(marked, false);
        Queue<Edge> mst = new Queue<Edge>();
        MinPQ<Edge> pq = new MinPQ<Edge>();

        Edge edg1 = new Edge(2, 4, .0);
        Edge edg2 = new Edge(4, 3, .7);
        Edge edg3 = new Edge(1, 3, .6);
        Edge edg4 = new Edge(2, 1, .3);
        Edge edg5 = new Edge(0, 1, .5);
        Edge edg6 = new Edge(0, 2, .4);
        Edge edg7 = new Edge(1, 4, .8);

        graph.addEdge(edg1);
        graph.addEdge(edg2);
        graph.addEdge(edg3);
        graph.addEdge(edg4);
        graph.addEdge(edg7);
        graph.addEdge(edg6);
        graph.addEdge(edg7);

        marked[0] = true;
        for(Edge edg : graph.adj(0)){
            pq.insert(edg);
        }

        while(!pq.isEmpty()){
            Edge min = pq.delMin();
            int u = min.either();
            int v = min.other(u);

            if(!marked[u] || !marked[v]){
                int otherEdg = !marked[u] ? u : v;
                mst.enqueue(min);
                marked[otherEdg] = true;
                for(Edge edg : graph.adj(otherEdg)){
                    pq.insert(edg);
                }
            }

        }

        System.out.println("\nVertex connection's for the shortest path:"); 
        double count = 0;
        for (Edge elem : mst) {
            count += elem.weight();
            String v1 = Integer.toString(elem.either());
            String v2 = Integer.toString(elem.other(elem.either()));
            System.out.println( v1 + " - " + v2);            
        }
        System.out.println("\nCalculated weight: " + count); 

        LazyPrimMST sol = new LazyPrimMST(graph);
        System.out.println("  Solution weight: " + sol.weight());
    }
}