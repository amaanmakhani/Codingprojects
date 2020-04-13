import java.util.Scanner;
import java.util.LinkedList; 
import java.util.Queue;
import java.util.Arrays;
import java.io.File;
/*
Name: Amaan Makhani
Class: CSC 225
Description: Implementation of the Queue based BFS.
	
	The input consists of a series of graphs in the following format:
   
    <number of vertices>
	<adjacency matrix row 1>
	...
	<adjacency matrix row n>
	
  	An input file can contain an unlimited number of graphs; each will be 
	processed separately.
	   
	To conveniently test the algorithm with a large input, create a text file
   	containing one or more test graphs (in the format described above) and run
	the program with:
	   
	java BFS (graph txt file name here)
*/

public class BFS {

	/* BFS(G)
	   Given an adjacency matrix describing a undirected graph, print the
	   listing of vertices encountered by a Queue based breadth-first 
	   search starting at vertex 0.
	*/
	public static void QueueBasedBFS(int[][] G) {
		int n = G.length;
		// Create a Queue and an array to keep track of vivisted flags
		boolean[] visited = new boolean[n];
		Arrays.fill(visited, Boolean.FALSE);
		Queue<Integer> Q = new LinkedList<>();
		// Start search at vertex 0
		int node = 0;
		visited[node] = true;
		Q.add(node);	
		while(!Q.isEmpty()) {
			node = Q.remove();
			System.out.println(node);
			int counter = -1;
			for (Integer entry : G[node]) {
				counter+=1;
				if(entry == 1 && !visited[counter]) {
					visited[counter] = true;
					Q.add(counter);
				}
			}
		}	
	}

	public static void main(String[] args) {
		Scanner s;
		if (args.length > 0) {
			try {
				s = new Scanner(new File(args[0]));
			} catch (java.io.FileNotFoundException e){
				System.out.printf("Unable to open %s\n",args[0]);
				return;
			}
			System.out.printf("Reading input values from %s.\n",args[0]);
		} else {
			s = new Scanner(System.in);
			System.out.printf("Reading input values from stdin.\n");
		}
		int graphNum = 0;
		double totalTimeSeconds = 0;
		// Read graphs until EOF is encountered (or an error occurs)
		while(true) {
			graphNum++;
			if(graphNum != 1 && !s.hasNextInt()) {
				break;
			}
			System.out.printf("Reading graph %d\n",graphNum);
			int n = s.nextInt();
			int[][] G = new int[n][n];
			int valuesRead = 0;
			for (int i = 0; i < n && s.hasNextInt(); i++) {
				for (int j = 0; j < n && s.hasNextInt(); j++) {
					G[i][j] = s.nextInt();
					valuesRead++;
				}
			}
			if (valuesRead < n*n) {
				System.out.printf("Adjacency matrix for graph %d contains too few values.\n",graphNum);
				break;
			}
			long startTime = System.currentTimeMillis();
			QueueBasedBFS(G.clone());
			long endTime = System.currentTimeMillis();
			totalTimeSeconds += (endTime-startTime)/1000.0;	
		}
		graphNum--;
		System.out.printf("Processed %d graph%s.\nAverage Time (seconds): %.2f\n",graphNum,(graphNum != 1)?"s":"",(graphNum>0)?totalTimeSeconds/graphNum:0);
	}
}