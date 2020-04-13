/*
Name: Amaan Makhani
Class: CSC 225
Description: Node class used in implementation of a binary tree.
*/

class Node {
    Node(int nVal) {
        SetValue(nVal);
        SetLeft(null);
        SetRight(null);
    }
    
    Node(int nVal, Node left, Node right) {
        SetValue(nVal);
        SetLeft(left);
        SetRight(right);
    }

    public int GetValue() {
        return value;
    }

    public void SetValue(int nVal) {
        value = nVal;
    }

    public Node GetLeft() {
        return left;
    }

    public Node GetRight() {
        return right;
    }

    public void SetLeft(Node node) {
        left = node;
    }

    public void SetRight(Node node) {
        right = node;
    }

    private int value;
    private Node left;
    private Node right;
}