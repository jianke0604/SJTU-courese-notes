import java.util.Scanner;
import java.util.concurrent.*;


public class quicksort extends RecursiveAction
{
    private int[] data;
    private int left;
    private int right;

    public quicksort(int[] data, int left, int right) {
        this.data = data;
        this.left = left;
        this.right = right;
    }

    protected void compute() {
        if (left < right) {
            int pivotIndex = left + (right - left) / 2;
            pivotIndex = partition(pivotIndex);
            invokeAll(new quicksort(data, left, pivotIndex - 1),
                      new quicksort(data, pivotIndex + 1, right));
        }
    }

    private int partition(int pivotIndex) {
        int pivotValue = data[pivotIndex];
        swap(pivotIndex, right);
        int storeIndex = left;
        for (int i = left; i < right; i++) {
            if (data[i] < pivotValue) {
                swap(i, storeIndex);
                storeIndex++;
            }
        }
        swap(storeIndex, right);
        return storeIndex;
    }

    private void swap(int i, int j) {
        if (i != j) {
            int iValue = data[i];
            data[i] = data[j];
            data[j] = iValue;
        }
    }

    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        System.out.println("Enter the size of the array:");
        int size = sc.nextInt();
        int[] data = new int[size];
        System.out.println("Enter the elements of the array:");
        for (int i = 0; i < size; i++) {
            data[i] = sc.nextInt();
        }
        sc.close();
        ForkJoinPool pool = new ForkJoinPool();
        quicksort sorter = new quicksort(data, 0, data.length - 1);
        pool.invoke(sorter);
        System.out.println("The sorted array is:");
        for (int i = 0; i < data.length; i++) {
            System.out.print(data[i]);
            System.out.print(" ");
        }
        System.out.println();
    }
}
