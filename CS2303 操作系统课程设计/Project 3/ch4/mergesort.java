import java.util.concurrent.*;
import java.util.Scanner;

public class mergesort extends RecursiveAction 
{
    private int[] data;
    private int left;
    private int right;
    private int threshold;

    public mergesort(int[] data, int left, int right, int threshold) {
        this.data = data;
        this.left = left;
        this.right = right;
        this.threshold = threshold;
    }

    protected void compute() {
        if (left < right) {
            if (right - left < threshold) {
                insertionsort(data, left, right);
            } else {
                int mid = left + (right - left) / 2;
                invokeAll(new mergesort(data, left, mid, threshold),
                          new mergesort(data, mid + 1, right, threshold));
                merge(data, left, mid, right);
            }
        }
    }

    private void insertionsort(int[] data, int left, int right) {
        for (int i = left + 1; i <= right; i++) {
            int current = data[i];
            int j = i - 1;
            while (j >= left && data[j] > current) {
                data[j + 1] = data[j];
                j--;
            }
            data[j + 1] = current;
        }
    }

    private void merge(int[] data, int left, int mid, int right) {
        int []temp = new int[right - left + 1];
        int i = left, j = mid + 1, k = 0;
        while (i <= mid && j <= right) {
            temp[k++] = data[i] <= data[j] ? data[i++] : data[j++];
        }
        while (i <= mid) {
            temp[k++] = data[i++];
        }
        while (j <= right) {
            temp[k++] = data[j++];
        }
        System.arraycopy(temp, 0, data, left, temp.length);
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
        mergesort sorter = new mergesort(data, 0, data.length - 1, 4);
        pool.invoke(sorter);
        System.out.println("The sorted array is:");
        for (int i = 0; i < size; i++) {
            System.out.print(data[i] + " ");
        }
        System.out.println();
    }
}
