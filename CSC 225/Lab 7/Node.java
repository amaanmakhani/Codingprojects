/*
Name: Amaan Makhani
Class: CSC 225
Description: Node class that stores the priority, and a name.
*/

public class Node
{
    Node(int nVal, String strVal)
    {
        SetPriority(nVal);
        SetName(strVal);
    }

    public int GetPriority()
    {
        return Priority;
    }

    public void SetPriority(int nVal)
    {
        Priority = nVal;
    }

    public String GetName()
    {
        return Name;
    }

    public void SetName(String strVal)
    {
        Name = strVal;
    }

    private int Priority;
    private String Name;
}
