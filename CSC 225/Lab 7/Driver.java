/*
Name: Amaan Makhani
Class: CSC 225
Description: Created a tester for the the Priority Queue class.
             Output should be:
             Fourth
             First
             Third
             Second
             Fifth
             The Priority Queue is empty
*/

public class Driver {

    static public void main(String[] args) {
        // Add to Stack then Queue
        PriorityQueue mainQueue = new PriorityQueue();
        mainQueue.enqueue(new Node(10, "First"));
        mainQueue.enqueue(new Node(5, "Second"));
        mainQueue.enqueue(new Node(8, "Third"));
        mainQueue.enqueue(new Node(30, "Fourth"));
        mainQueue.enqueue(new Node(0, "Fifth"));
        // Remove till queue is empty
        while(!mainQueue.isEmpty()) {
            System.out.println(mainQueue.dequeue().GetName());
        }
        System.out.print("The priority queue is empty");
    }
}