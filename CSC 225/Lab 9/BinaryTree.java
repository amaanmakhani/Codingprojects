import java.util.ArrayList;
/*
Name: Amaan Makhani
Class: CSC 225
Description: Implementation of binary tree transversals.
             In-order, pre-order, and post-order.
*/

class BinaryTree {
    static public void main(String[] args) {
        BinaryTree obj = new BinaryTree(args[0].split(" "));
        System.out.println("In order:");
        obj.transverseInOrder(obj.GetRoot());
        System.out.println("\nPre-order:");
        obj.transversePreOrder(obj.GetRoot());
        System.out.println("\nPost-order:");
        obj.transversePostOrder(obj.GetRoot());
    }

    public void transverseInOrder(Node node) {
        if (node != null) {
            transverseInOrder(node.GetLeft());
            System.out.print(" " + node.GetValue());
            transverseInOrder(node.GetRight());
        }
    }

    public void transversePreOrder(Node node) {
        if (node != null) {
            System.out.print(" " + node.GetValue());
            transversePreOrder(node.GetLeft());
            transversePreOrder(node.GetRight());
        }
    }

    public void transversePostOrder(Node node) {
        if (node != null) {
            transversePostOrder(node.GetLeft());
            transversePostOrder(node.GetRight());
            System.out.print(" " + node.GetValue());
        }
    }

    BinaryTree(String[] strArray) {
        for (String elem : strArray) {
            add(Integer.parseInt(elem));
        }
    }
    
    public void add(int value) {
        SetRoot(addRecursive(GetRoot(), value));
    }
        
    public Node addRecursive(Node current, int value) {
        // Case 1: Root doesn't exist
        // Case 2: Value is less than node you add it as the left child  
        // Case 3: Value is greater than node you add it as the right child
        // Case 4: Value exists
        if(current == null) {
            return new Node(value);
        } else if(value < current.GetValue()) {
            current.SetLeft(addRecursive(current.GetLeft(), value));
        } else if(value > current.GetValue()) {
            current.SetRight(addRecursive(current.GetRight(), value));
        } else {
            return current;   
        } 
        return current;
    }

    private Node root = null;

    public Node GetRoot() {
        return root;
    }

    public void SetRoot(Node node) {
        root = node;
    }
}