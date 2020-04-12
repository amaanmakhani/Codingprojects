import java.io.File;
import java.util.Scanner;
/*
Name: Amaan Makhani
Class: CSC 225
Description: Given two sorted arrays of size n and m.
             Must execute in O(n+m) time.
             Sorted array of size n+m is created.
             Arrays seperated by a semicolon.
             Elements are seperated by commas.
             ex."1,2,7,8;3,4,9,10"
             Must print out sorted array with it's elements seperated by spaces.
             File name is taken as a command line arg.
*/

class Sort
{
    static public void main(String[] args)
    {
        if(args.length > 0 && args[0].length() > 0)
        {
            try
            {
                File ArrayFile = new File(args[0]);
                Scanner scanner = new Scanner(ArrayFile);
               
                // Read line by line
                while(scanner.hasNextLine())
                {
                    String[] temp = scanner.nextLine().split(";");
                    if(temp.length == 2)
                    {
                        String[] Result = Execute(temp[0].split(","), temp[1].split(","));
                        PrintArray(Result);
                    }
                    else
                    {
                        System.out.print("Input is formatted incorrectly");
                    }
                    
                    if(scanner.hasNextLine())
                    {
                        System.out.print("\n");
                    }
                }
                scanner.close();
            }
            catch(Exception e)
            {
                System.out.println("Exception while reading file:");
                System.out.print(e.getMessage());
            }
        }
        else
        {
            System.out.print("No file arguments provided");
        }
    }

    // Runs in O(n+m) time
    static private String[] Execute( String[] strArray1, String[] strArray2 )
    {
        int nIndexArray1 = 0;
        int nIndexArray2 = 0;
        int nIndexArray3 = 0;
        String[] strArray3 = new String[strArray1.length+strArray2.length];

        while(strArray1.length > nIndexArray1 && strArray2.length > nIndexArray2)
        {
            if(Integer.parseInt(strArray1[nIndexArray1]) > Integer.parseInt(strArray2[nIndexArray2]))
            {
                strArray3[nIndexArray3] = strArray2[nIndexArray2];
                nIndexArray2 += 1;
                nIndexArray3 += 1;
            }
            else if(Integer.parseInt(strArray2[nIndexArray2]) > Integer.parseInt(strArray1[nIndexArray1]))
            {
                strArray3[nIndexArray3] = strArray1[nIndexArray1];
                nIndexArray1 += 1;
                nIndexArray3 += 1;
            }
            else
            {
                // Numbers are equal, increment both indexes and put both in array
                strArray3[nIndexArray3] = strArray1[nIndexArray1];
                nIndexArray3 += 1;
                strArray3[nIndexArray3] = strArray2[nIndexArray2];
                nIndexArray1 += 1;
                nIndexArray2 += 1;
                nIndexArray3 += 1;
            }
        }

        while(strArray1.length > nIndexArray1)
        {
            strArray3[nIndexArray3] = strArray1[nIndexArray1];
            nIndexArray1 += 1;
            nIndexArray3 += 1;
        }

        while(strArray2.length > nIndexArray2)
        {
            strArray3[nIndexArray3] = strArray2[nIndexArray2];
            nIndexArray2 += 1;
            nIndexArray3 += 1;
        }

        return strArray3;
    }

    static private void PrintArray(String[] strArray)
    {
        String strOutput = "";
        for (String Elem : strArray) 
        {
            strOutput += Elem;
            strOutput +=", ";
        }
        if(strOutput.length()>2)
        {
            System.out.print(strOutput.substring(0, strOutput.length()-2));
        }
        else
        {
            System.out.print("Unable to print array");
        }
    }
}