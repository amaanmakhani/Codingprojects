/*
Name: Amaan Makhani
Class: CSC 225
Description: Created a tester for the myQueue and myStack classes.
             Input is a string of numbers e.x "5 4 3 2 1"
             Numbers are removed from the stack and queue till they are empty.
             Once empty print "The "data type" is empty"
*/

public class test {

    static public void main(String[] args) {
        // Add to Stack then Queue
        myQueue mainQueue = new myQueue();
        myStack mainStack = new myStack();
        boolean bNotEmpty = false;
        String[] strSequence = args[0].split(" ");
        for (String elem : strSequence) {
            mainStack.push(elem);
            mainQueue.enqueue(elem);  
        }
        // Remove till stack is empty
        while(!mainStack.isEmpty()) {
            System.out.print(mainStack.pop());
            System.out.print(" ");
            
            if(args[0].length() > 0) {
                bNotEmpty = true;
            }
        }
        if(bNotEmpty) {
            System.out.print("\n");
        }
        System.out.print("The Stack is empty\n");
        // Remove till queue is empty
        while(!mainQueue.isEmpty()) {
            System.out.print(mainQueue.dequeue());
            System.out.print(" ");
        }
        if(bNotEmpty) {
            System.out.print("\n");
        }
        System.out.print("The Queue is empty");
    }
}