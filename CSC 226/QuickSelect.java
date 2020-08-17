// Template is created by Zhuoli Xiao, on Sept. 19st, 2016.
// Used for Lab 226, 2020 Summer. 
// Amaan Makhani
// Description: Quickslect completed lab code template.

import java.util.Random;
public class QuickSelect {

	// Function to invoke quickSelect
	public static int QS(int[] S, int k){
        if (S.length==1)
        	return S[0];
        return quickSelect(0,S.length-1,S,k);
	}
    
    // Implement quickSelect in a recursive way 
    private static int quickSelect(int left,int right, int[] array, int k){
		// If there is only one element, return the element
    	if(right - left == 0) {
			return array[right];
		}
		// Pick a random pivot
    	int pivotIndex = pickRandomPivot(left, right);
		// Partition based on the pivot and store the new pivot index
		pivotIndex = partition(left, right, array, pivotIndex);
		// Recursive call for quickSelect based on kth position to remove uneeded sections of the array
		if(k-1 < pivotIndex) {
			return quickSelect(left, pivotIndex-1, array, k);
		} else if(k-1 == pivotIndex) {
			return array[pivotIndex];
		} else {
			return quickSelect(pivotIndex + 1, right, array, k);
		}
    }

    // Partition with a pivot
    private static int partition(int left, int right, int[] array, int pivotIndex) {
		// Swap pivot to the end of the array
		swap(array, pivotIndex, right);
		pivotIndex = right;
		int pivot = array[pivotIndex];
		// Start the left value at the 2nd last element
		right--;
		while (left <= right) {
			// Increment left index till a value greater than the pivot is found
			while(left <= right  && array[left] <= pivot) {
				left++;
			}
			// Increment right index till a value lower than the pivot is found
			while(left <= right && array[right] >= pivot) {
				right--;
			}
			if(left < right) {
				swap(array, left, right);
			}
		}
		// Place the pivot in correct location and return it's location
		swap(array, left, pivotIndex);
		return left;
    }

    // Pick a random pivot to do the QuickSelect
	private static int pickRandomPivot(int left, int right){
		int index = 0;
		Random rand = new Random();
		index = left + rand.nextInt(right-left+1);
		return index;  
    }
    
	// Swap two elements in the array
	private static void swap(int[]array, int a, int b){
 		int tmp = array[a];
		array[a] = array[b];
		array[b] = tmp;
	}

	// Our main function to test the algorithm
	public static void main (String[] args){
		// Array one has duplicate elements
  		int[] array1 ={12,13,17,14,21,3,4,9,21,8};  		
  		// Sorted array1 = {3,4,8,9,12,13,14,17,21,21}
  		int[] array2 ={14,8,22,18,6,2,15,84,13,12};
  	    // Sorted array2 = {2,6,8,12,13,14,15,18,22,84}
        int[] array3 ={6,8,14,18,22,2,12,13,15,84};
        // Sorted array3 = {2,6,8,12,13,14,15,18,22,84}
        int[] array4 = {1,2};
        // array4 already sorted
        int[] array5 = {1,1,1,2,2,4};
		// array5 already sorted 
	
		System.out.println("Correct answer is 12 = " + "Your answer: " + QS(array1,5));
  		System.out.println("Correct answer is 21 = " + "Your answer: " + QS(array1,10));
  		System.out.println("Correct answer is 15 = " + "Your answer: " + QS(array2,7));
  		System.out.println("Correct answer is 13 = " + "Your answer: " + QS(array3,5));
  		System.out.println("Correct answer is 1 = " + "Your answer: " + QS(array4,1));
  		System.out.println("Correct answer is 2 = " + "Your answer: " + QS(array5,5));
	}
}

