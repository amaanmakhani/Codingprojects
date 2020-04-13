import java.io.File;
import java.util.Scanner;
/*
Name: Amaan Makhani
Class: CSC 225
Description: Given a sequence from 1 to n, find the missing number.
             Must be found in O(n) time, one extra array element can be used.
             File name is taken as a command line arg.
             Each file line contains an array of elements seperated by commas.
             ex."1,2,3,4"
             Missing element should be printed out.
*/

class Find {
    static public void main(String[] args) {
       
        if(args.length > 0 && args[0].length() > 0) {
            try {
                File ArrayFile = new File(args[0]);
                Scanner scanner = new Scanner(ArrayFile);
                // Read line by line
                while(scanner.hasNextLine()) {
                    Execute(scanner.nextLine().split(","));
                    if(scanner.hasNextLine()) {
                        System.out.print("\n");
                    }
                }
                scanner.close();
            } catch(Exception e) {
                System.out.println("Exception while reading file:");
                System.out.println(e.getMessage());
            }
        } else {
            System.out.println("No file arguments provided");
        }
    }

    // Runs in O(n) time
    static private void Execute( String[] strArray ) {
        // Calculate expected sum
        int nSum = ((strArray.length+1) * (strArray.length+2))/2;
        int nCurrentSum = 0;
        // Calculate sum in current array
        for (String elem : strArray) {
            nCurrentSum += Integer.parseInt(elem);
        }
        // The missing element is difference between the sums
        System.out.print(nSum - nCurrentSum);
    }
}