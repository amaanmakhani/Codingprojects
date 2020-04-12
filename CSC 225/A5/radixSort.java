import java.util.ArrayList;

/*
Name: Amaan Makhani
Class: CSC 225
Description: Least Significant digit radix-sort.
             LSD radix-sort starts from the LSD to the MSD.
             Input is a string containing number seperated by spaces.
*/

public class radixSort
{
    static public void main(String[] args)
    {
        radixSort obj = new radixSort();
        if(args.length > 0 && args[0].length() > 0)
        {
            // Find maximum number of digits
            String[] strArray= args[0].split(" ");
            ArrayList<Integer> nArray = new ArrayList<Integer>();
            int nSize = 0;
            for (String elem : strArray) 
            {
                if(elem.length() > nSize)
                {
                    nSize = elem.length();
                }
                nArray.add(Integer.parseInt(elem));
            }

            // Use bucket sort based on the maximum number of digits
            for(int index = 0; index < nSize; index++)
            {
                obj.Execute(nArray, index);
                obj.PrintArray(nArray);
            }
        }
        else
        {
            System.out.println("Incorrect arguments provided");
        }
    }

    private void Execute(ArrayList<Integer> Array, int index)
    {
        // Create bucket of buckets
        ArrayList<ArrayList<Integer> > Bucket =  new ArrayList<ArrayList<Integer>>();
        for(int i = 0; i < 10; i++)
        {
            Bucket.add(new ArrayList<Integer>());
        }
        
        // Place elements in their buckets
        // If number position doesn't exist it's index will be 0
        for (Integer elem : Array) 
        {
            String strelem = elem.toString();
            if(strelem.length() > index)
            {
                int nbucket = Character.getNumericValue(strelem.charAt(strelem.length() - 1 - index));    
                Bucket.get(nbucket).add(elem);
            }
            else
            {
                Bucket.get(0).add(elem);
            }
        }

        // Combine buckets
        Array.clear();
        for (ArrayList<Integer> arrayList : Bucket) 
        {
            for (Integer elem : arrayList)
            {
                Array.add(elem);
            }
        }
    }

    private void PrintArray(ArrayList<Integer> Array)
    {
        // Create output string
        String strOutput = "";
        for (Integer elem : Array) 
        {
            strOutput += elem.toString();
            strOutput += " ";
        }

        // Remove extra space and output the output string
        if(strOutput.length() > 1)
        {
            System.out.println(strOutput.substring(0, strOutput.length()-1));
        }
        else
        {
            System.out.print("Unable to print array");
        }
    }
}