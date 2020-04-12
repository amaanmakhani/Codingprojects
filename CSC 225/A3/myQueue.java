import java.util.ArrayList;
import java.util.NoSuchElementException;

/*
Name: Amaan Makhani
Class: CSC 225
Description: Created a Queue using an ArrayList.
*/

public class myQueue
{
    public myQueue()
    {
        _mArrayList = new ArrayList<Object>();
    }

    public void enqueue(Object obj)
    {
        _mArrayList.add(obj);
    }

    public Object front()
    {
       if(!this.isEmpty())
        {
            return _mArrayList.get(0); 
        }
        else
        {
            throw new NoSuchElementException("Queue is empty");
        }
        
    }

    public Object dequeue()
    {
        if(!this.isEmpty())
        {
            Object temp =_mArrayList.get(0);
            _mArrayList.remove(0);
            return temp;
        }
        else
        {
           throw new NoSuchElementException("Queue is empty");
        }
    }

    public boolean isEmpty()
    {
        boolean bRet = this.size() == 0? true: false;
        return bRet;
    }

    public int size()
    {
        return _mArrayList.size();
    }
    
    private ArrayList<Object> _mArrayList;
}