import java.io.*;
import java.util.Scanner;  
/*
Name: Amaan Makhani
Class: CSC 226
Description: Create DFA for patterns and use the DFA to find the patterns in text files.
*/
public class createDFA {
    static public int[][] DFA(String pattern, int alphabet) {
        int[][] dfa = new int[alphabet][pattern.length()];
        dfa[Character.getNumericValue(pattern.charAt(0))-10][0] = 1;
        for(int X = 0, j =1; j < pattern.length(); j++) {
            for(int c = 0; c < alphabet; c++) {
                dfa[c][j] = dfa[c][X];
            }
            dfa[Character.getNumericValue(pattern.charAt(j))-10][j] = j + 1;
            X = dfa[Character.getNumericValue(pattern.charAt(j))-10][X];
        }
        return dfa;
    }

    static public void findPattern(int[][] dfa, String filename) {
        String line = "";
        try{
            FileInputStream fis=new FileInputStream(filename);
            Scanner sc = new Scanner(fis);
            while(sc.hasNextLine()){  
                line = line + sc.nextLine();
            }  
            sc.close();
        } catch(Exception e) {
            System.out.println(e.getMessage());
        }
        int i, j;
        for(i = 0, j= 0; i < line.length() && j < dfa[0].length; i++){
            j = dfa[Character.getNumericValue(line.charAt(i))-10][j];
        }
        if(j == dfa[0].length) {
            System.out.println("Found starting at position " + Integer.toString(i-dfa[0].length));
        }else{
            System.out.println("Not found");
        }
    }

    static public void main(String[] args) {
        int[][] result = DFA("AACAAAB", 3);
        System.out.println("\nP1");
        for (int[] letter: result) {
            for (int elem: letter) {
                System.out.print(Integer.toString(elem) + " ");
            }
            System.out.println();
        }
        findPattern(result, "test1.txt");
        System.out.println("\nP2");
        result = DFA("ABABABAB", 2);
        for (int[] letter: result) {
            for (int elem: letter) {
                System.out.print(Integer.toString(elem) + " ");          
            }
            System.out.println();
        }
        findPattern(result, "test2.txt");
    }
}