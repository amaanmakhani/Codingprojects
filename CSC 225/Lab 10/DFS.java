import java.util.Scanner;
import java.io.File;

/*
Name: Amaan Makhani
Class: CSC 225
Description: Implementation of a recursive DFS.
	
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
	   
	java DFS (graph txt file name here)
*/

public class DFS {

	/* DFS(G)
	   Given an adjacency matrix describing a undirected graph, print the
	   listing of vertices encountered by a depth-first search starting at 
	   vertex 0.
	*/
	public static void DFS(int[][] G) {
		int n = G.length;
		boolean[] visited = new boolean[n];
		DepthFirstSearchHelper(0, G, visited);
	}

	public static void DepthFirstSearchHelper(int vertex, int[][] G, boolean[] visited) {
		visited[vertex] = true;
		System.out.println(vertex);
		int counter = -1;
		for (Integer entry : G[vertex]) 
		{
			counter+=1;
			if(entry == 1 && !visited[counter])
			{
				DepthFirstSearchHelper(counter, G, visited);
			}
		}
	}
	
	public static void main(String[] args) {
		Scanner s;
		if (args.length > 0) {
			try{
				s = new Scanner(new File(args[0]));
			} catch (java.io.FileNotFoundException e){
				System.out.printf("Unable to open %s\n",args[0]);
				return;
			}
			System.out.printf("Reading input values from %s.\n",args[0]);
		}else {
			s = new Scanner(System.in);
			System.out.printf("Reading input values from stdin.\n");
		}
		
		int graphNum = 0;
		double totalTimeSeconds = 0;
		
		//Read graphs until EOF is encountered (or an error occurs)
		while(true) {
			graphNum++;
			if(graphNum != 1 && !s.hasNextInt())
				break;
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
			if (valuesRead < n*n){
				System.out.printf("Adjacency matrix for graph %d contains too few values.\n",graphNum);
				break;
			}
			long startTime = System.currentTimeMillis();
			DFS(G.clone());
			long endTime = System.currentTimeMillis();
			totalTimeSeconds += (endTime-startTime)/1000.0;
				
		}
		graphNum--;
		System.out.printf("Processed %d graph%s.\nAverage Time (seconds): %.2f\n",graphNum,(graphNum != 1)?"s":"",(graphNum>0)?totalTimeSeconds/graphNum:0);
	}
}