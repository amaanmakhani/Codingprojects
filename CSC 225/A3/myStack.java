import java.util.ArrayList;
import java.util.NoSuchElementException;
/*
Name: Amaan Makhani
Class: CSC 225
Description: Created a stack using an ArrayList.
*/

public class myStack {
    public myStack() {
        _mArrayList = new ArrayList<Object>();
    }

    public Object pop() {
        if(!this.isEmpty()) {
            Object temp =_mArrayList.get(0);
            _mArrayList.remove(0);
            return temp;
        } else {
            throw new NoSuchElementException("Stack is empty");
        }
    }

    public Object peek() {
       if(!this.isEmpty()) {
            return _mArrayList.get(0);
        } else {
            throw new NoSuchElementException("Stack is empty");
        }
    }

    public void push(Object obj) {
        _mArrayList.add(0, obj);
    }

    public boolean isEmpty() {
        boolean bRet = this.size() == 0? true: false;
        return bRet;
    }

    public int size() {
        return _mArrayList.size();
    }
    
    private ArrayList<Object> _mArrayList;
}