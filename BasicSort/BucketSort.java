package BasicSort;

import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Collections;

//Radix Sort
public class BucketSort {


    public static double[] BucketSort (double[] unSortedArray) {
        int n = unSortedArray.length;
        ArrayList<Double>[] bucket = new ArrayList[n];

        // Create empty buckets
        for (int i = 0; i < n; i++)
            bucket[i] = new ArrayList<Double>();

        // Add elements into the buckets
        for (int i = 0; i < n; i++) {
            int bucketIndex = (int) unSortedArray[i] * n;
            bucket[bucketIndex].add(unSortedArray[i]);
        }

        // Sort the elements of each bucket (any)
        for (int i = 0; i < n; i++) {
            Collections.sort((bucket[i]));
        }

        // Get the sorted array
        int index = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0, size = bucket[i].size(); j < size; j++) {
                unSortedArray[index++] = bucket[i].get(j);
            }
        }
        return unSortedArray;
    }

    public static void main(String[] args) {
        double[] test = {0.22,0.4,0.56,0.33,0.78,0.666,0.666,0.22};
        double[] ans = BucketSort(test);
        for (double an : ans) {
            System.out.println(an );
        }
    }

}
