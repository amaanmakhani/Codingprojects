import java.util.ArrayList;
import java.util.NoSuchElementException;
/*
Name: Amaan Makhani
Class: CSC 225
Description: Priority Queue implemented using an arraylist.
*/

public class  PriorityQueue {
    public PriorityQueue() {
        _mArrayList = new ArrayList<Node>();
    }

    public void enqueue(Node tempNode) {
        int index = 0;
        if(!this.isEmpty()) {
            while(index < this.size() && _mArrayList.get(index).GetPriority()> tempNode.GetPriority()) {
                index++;
            }
            _mArrayList.add(index, tempNode);  
        } else {
            _mArrayList.add(0, tempNode);            
        }
    }

    public Node dequeue() {
        if(!this.isEmpty()) {
            Node temp =_mArrayList.get(0);
            _mArrayList.remove(0);
            return temp;
        } else {
           throw new NoSuchElementException("Queue is empty");
        }
    }

    public boolean isEmpty() {
        boolean bRet = this.size() == 0? true: false;
        return bRet;
    }

    public int size() {
        return _mArrayList.size();
    }
    
    private ArrayList<Node> _mArrayList;
}