/*
Name: Amaan Makhani
Class: CSC 225
Description: postfix calculator.
             Input example "3 10 5 + *"
             Resulting example calculation:
             10 + 5 = 15
             15 * 3 = 45
             Example output: 45
*/

public class postfix {
    static public void main(String[] args) {
        myStack operandStack = new myStack();
        boolean bEmpty = true;
        if(args[0].length() > 0) {
            String[]strSequence = args[0].split(" ");
            for (String elem : strSequence) {
                bEmpty = false;
                if(elem.equals("+") || elem.equals("-") || elem.equals("*")|| elem.equals("/")) {
                    // Temporary storage variables
                    if(operandStack.size() < 2) {
                        bEmpty = true;
                        break;
                    }
                    int nOp1 = (int)operandStack.pop();
                    int nOp2 = (int)operandStack.pop();
                    int nResult = 0;  
                    switch(elem) {
                        case "+":
                            nResult= nOp1 + nOp2;
                            break;

                        case "-":
                            nResult= nOp1 - nOp2;
                            break;

                        case "*":
                            nResult= nOp1 * nOp2;
                            break;

                        case "/":
                            nResult= nOp1 / nOp2;
                            break;
                    }
                    operandStack.push(nResult);
                } else {
                    operandStack.push(Integer.parseInt(elem));
                }
            }
        }
        if(!bEmpty) {
            System.out.print(operandStack.pop().toString());
        } else {
            System.out.print("Not enough terms have been entered.");
        }
    }
}