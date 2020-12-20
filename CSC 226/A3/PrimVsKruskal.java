/* 
Amaan Makhani
June 22, 2020
Description: Compare Prim's vs. Kruskals algorithm creation process.
			 Implemented a early detection system to detect if the trees are different.
			 Multiple minimum spanning trees can exist if edge weights are not unique.
*/
//*****************************************IMPORTANT*****************************************
/*
	Both algorithms are built concurrently and tested for uniqueness of MST as each edge is added.
	In addition there are two early detection mechanisms in place which are described below.

	Early detection strategy is comprised of two components:
	1) Determines if any duplicate edges exist prior to processing. If no duplicate edges exist the MST must be unique.
	2) Builds the mst using Prim's and Kruskals at the same time.
	   After a edge is added in both algorithm's a DFS is run to see if the combined MST produces a cycle. 
	   If a cycle exists the algorithms will produce different MST's. 
*/
//*********************************************************************************************

/* PrimVsKruskal.java
   CSC 226 - Summer 2020
   Assignment 3 - Prim MST versus Kruskal MST Template
   
   The file includes the "import edu.princeton.cs.algs4.*;" so that yo can use
   any of the code in the algs4.jar file. You should be able to compile your program
   with the command
   
	javac -cp .;algs4.jar PrimVsKruskal.java
	
   To conveniently test the algorithm with a large input, create a text file
   containing a test graphs (in the format described below) and run
   the program with
   
	java -cp .;algs4.jar PrimVsKruskal file.txt
	
   where file.txt is replaced by the name of the text file.
   
   The input consists of a graph (as an adjacency matrix) in the following format:
   
    <number of vertices>
	<adjacency matrix row 1>
	...
	<adjacency matrix row n>
	
   Entry G[i][j] >= 0.0 of the adjacency matrix gives the weight (as type double) of the edge from 
   vertex i to vertex j (if G[i][j] is 0.0, then the edge does not exist).
   Note that since the graph is undirected, it is assumed that G[i][j]
   is always equal to G[j][i].
*/

import edu.princeton.cs.algs4.*;
import java.util.Arrays;
import java.util.Scanner;
import java.io.File;

//Do not change the name of the PrimVsKruskal class
public class PrimVsKruskal{

	/* PrimVsKruskal(G)
		Given an adjacency matrix for connected graph G, with no self-loops or parallel edges,
		determine if the minimum spanning tree of G found by Prim's algorithm is equal to 
		the minimum spanning tree of G found by Kruskal's algorithm.
		
		If G[i][j] == 0.0, there is no edge between vertex i and vertex j
		If G[i][j] > 0.0, there is an edge between vertices's i and j, and the
		value of G[i][j] gives the weight of the edge.
		No entries of G will be negative.
	*/
	public static Boolean CycleDFS(int[][] G) {
		int n = G.length;
		boolean[] visited = new boolean[n];
		int parent = -1;
		return DepthFirstSearchHelper(0, G, visited, parent);
	}

	public static Boolean DepthFirstSearchHelper(int vertex, int[][] G, boolean[] visited, int parent) {
		visited[vertex] = true;
		int counter = -1;
		boolean flag = false;
		for (int entry : G[vertex]) {
			counter+=1;
			if(entry == 1 && visited[counter] && counter != parent) {
 				return true;
			}else if(entry == 1 && !visited[counter] && entry != parent) {
				flag = DepthFirstSearchHelper(counter, G, visited, vertex);
				if(flag == true){
					return true;
				}
			}	
		}
		return false;
	}

	static boolean PrimVsKruskal(double[][] G){
		int n = G.length;
		if(n == 1){
			return true;
		}

		// Initialization of variables and structures
		int[][] mstGraph = new int[n][n];
		UF uf = new UF(n);
		MinPQ<Edge> pqKruskal = new MinPQ<Edge>();
		Queue<Edge> mstKruskal = new Queue<Edge>();
		Boolean[] marked = new Boolean[n];
		Arrays.fill(marked, false);
        IndexMinPQ<Edge> pqPrim = new IndexMinPQ<Edge>(n);
        Queue<Edge> mstPrim= new Queue<Edge>();
		Boolean containsDuplicates = false;
		SET<Double> uniqueEdgeWeights = new SET<Double>();

		// Read triangular matrix 
		// Insert edges into the priority queue for Kruskal's algorithm
		for(int i = 0; i < n; i++){
			for(int j = i; j < n; j++){
				if(G[i][j]!=0){
					double weight = G[i][j];
					pqKruskal.insert(new Edge( i, j, weight));
					// Keep track of duplicate edges
					if(!uniqueEdgeWeights.contains(weight)){
						uniqueEdgeWeights.add(weight);
					}else{
						containsDuplicates = true;
					}
				}
			}
		}
		// MST must be unique as all edges are unique
		if(!containsDuplicates){
			return true;
		}

		/* Build the MST concurrently*/
		marked[0] = true;
        for(int i = 0; i < n; i++){
			if(G[0][i] != 0){
				pqPrim.insert( i, new Edge(0, i, Double.valueOf(G[0][i])));
			}
        }

        while((!pqPrim.isEmpty()) || (!pqKruskal.isEmpty() && mstKruskal.size() < n)){
			/* Build the MST by Prim's */
			if(!pqPrim.isEmpty()){
				Edge min = pqPrim.minKey();
				pqPrim.delMin();
				int u = min.either();
				int v = min.other(u);
				// Add the next vertexes adjacenct edges into the pq 
				if(!marked[u] || !marked[v]){
					int otherEdg = !marked[u] ? u : v;
					marked[otherEdg] = true;
					mstPrim.enqueue(min);
					mstGraph[u][v] = 1;
					mstGraph[v][u] = 1;
					for(int i = 0; i < n; i++){
						if(G[otherEdg][i] != 0 ){
							if(pqPrim.contains(i)){
								if(pqPrim.keyOf(i).weight() > G[otherEdg][i]){
									pqPrim.changeKey( i, new Edge(otherEdg, i, Double.valueOf(G[otherEdg][i])));
								}
							}else{
								pqPrim.insert( i, new Edge(otherEdg, i, Double.valueOf(G[otherEdg][i])));
							}
						}
					}
				}
			}	

			/* Build the MST by Kruskal's */
			if(!pqKruskal.isEmpty() && mstKruskal.size() < n){
				Edge min = pqKruskal.delMin();
				int p = min.either();
				int q = min.other(p);
				if(uf.find(p) != uf.find(q)){
					uf.union(p, q);
					mstKruskal.enqueue(min);
					mstGraph[p][q] = 1;
					mstGraph[q][p] = 1;
				}
			}

			/*	Check after each time after a edge addition if the output tree
			 will be the same for both Prim's and Kruskals using a modified DFS*/
			if(CycleDFS(mstGraph)){
				return false;
			}
		}
		/* For debugging use only
		// Print out the mst's produced
		System.out.println("Prim Tree:"); 
        double count = 0;
        for (Edge elem : mstPrim) {
            count += elem.weight();
            String v1 = Integer.toString(elem.either());
            String v2 = Integer.toString(elem.other(elem.either()));
			System.out.println( v1 + "-" + v2 + " " + String.format("%.5f", elem.weight()));    
        }
		System.out.println(String.format("%.5f", count));	
		System.out.println("\nKruskal Tree:"); 
		count = 0;
        for (Edge elem : mstKruskal) {
            count += elem.weight();
            String v1 = Integer.toString(elem.either());
			String v2 = Integer.toString(elem.other(elem.either()));
            System.out.println( v1 + "-" + v2 + " " + String.format("%.5f", elem.weight()));            
		}
		System.out.println(String.format("%.5f", count));
		*/
		return !CycleDFS(mstGraph);
	}
		
	/* main()
	   Contains code to test the PrimVsKruskal function. You may modify the
	   testing code if needed, but nothing in this function will be considered
	   during marking, and the testing process used for marking will not
	   execute any of the code below. 
	*/
   public static void main(String[] args) {
		Scanner s;
		if (args.length > 0){
			try{
				s = new Scanner(new File(args[0]));
			} catch(java.io.FileNotFoundException e){
				System.out.printf("Unable to open %s\n",args[0]);
				return;
			}
			System.out.printf("Reading input values from %s.\n",args[0]);
		}else{
			s = new Scanner(System.in);
			System.out.printf("Reading input values from stdin.\n");
		}
		
		int n = s.nextInt();
		double[][] G = new double[n][n];
		int valuesRead = 0;
		for (int i = 0; i < n && s.hasNextDouble(); i++){
			for (int j = 0; j < n && s.hasNextDouble(); j++){
				G[i][j] = s.nextDouble();
				if (i == j && G[i][j] != 0.0) {
					System.out.printf("Adjacency matrix contains self-loops.\n");
					return;
				}
				if (G[i][j] < 0.0) {
					System.out.printf("Adjacency matrix contains negative values.\n");
					return;
				}
				if (j < i && G[i][j] != G[j][i]) {
					System.out.printf("Adjacency matrix is not symmetric.\n");
					return;
				}
				valuesRead++;
			}
		}
		
		if (valuesRead < n*n){
			System.out.printf("Adjacency matrix for the graph contains too few values.\n");
			return;
		}	
		
        boolean pvk = PrimVsKruskal(G);
        System.out.printf("Does Prim MST = Kruskal MST? %b\n", pvk);
    }
}
