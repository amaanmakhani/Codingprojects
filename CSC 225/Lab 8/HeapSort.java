import java.util.ArrayList;
/*
Name: Amaan Makhani
Class: CSC 225
Description: Implementation of heap sort.
*/

class HeapSort
{
    static public void main(String[] args)
    {
        HeapSort obj = new HeapSort(args[0].split(" "));
        obj.Sort();
        System.out.println("Sorted Array:");
        System.out.println(obj.SortedDataArray.toString());
    }

    // Build a max heap and extract the root each time
    public void Sort()
    {
        // Create max heap
        for(int i = (DataArray.size()/2)-1; i>=0; i--)
        {
            Heapify(i, DataArray.size());
            // Enables us to see the algrothims progress
            print();
        }

        // Continue extracting from the max heap
        SortedDataArray = new ArrayList<>();
        SortedDataArray.add(DataArray.get(0));
        DataArray.remove(0);
        while(DataArray.size() > 0)
        {
            Heapify(0, DataArray.size());
            SortedDataArray.add(DataArray.get(0));
            DataArray.remove(0);
        }
    }

    HeapSort(String[] strArray)
    {
        DataArray = new ArrayList<>();
        for (String elem : strArray) 
        {
            DataArray.add(Integer.parseInt(elem));
        }
    }
    
    private void Heapify(int index, int StopIndex)
    {
        int Parent = index;
        int LeftChild = 2*index + 1;
        int RightChild = 2*index + 2;
        
        if(LeftChild < StopIndex && DataArray.get(LeftChild) > DataArray.get(Parent))
        {
            Parent = LeftChild;
        }

        if(RightChild < StopIndex && DataArray.get(RightChild) > DataArray.get(Parent))
        {
            Parent = RightChild;
        }

        if(Parent != index)
        {
            Swap(index,Parent);
            Heapify(Parent, StopIndex);
        }
    }

    private void print()
    {
        System.out.println(DataArray.toString());
    }

    private void Swap(int nIndex1, int nIndex2)
    {
        int temp = DataArray.get(nIndex1);
        DataArray.set(nIndex1, DataArray.get(nIndex2));
        DataArray.set(nIndex2, temp);
    }

    private ArrayList<Integer> DataArray;
    private ArrayList<Integer> SortedDataArray;
}