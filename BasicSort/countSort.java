
public class countSort {
    public static int[] CountingSort (int[] unSortArray){

        int min = unSortArray[0];
        int  max = unSortArray[0];
        for (int i=1; i < unSortArray.length;i++){
            if (unSortArray[i]>max){
                max = unSortArray[i];
            }else if (unSortArray[i]<min ){
                min = unSortArray[i];
            }
        }


        // new counting space
        int[] countSpace =  new int [ max+1];
        for (int i =  0; i< unSortArray.length; i++){
            countSpace[unSortArray[i]] += 1;
        }

        // cumulative sum
        for (int i=0; i< countSpace.length-1;i++){
            countSpace[i+1] += countSpace[i];
        }


        // output
        int[] output = new int [unSortArray.length];
        for (int i =unSortArray.length-1; i>=0;i--){
            output[countSpace[unSortArray[i]]-1] = unSortArray[i];
            countSpace[unSortArray[i]] -= 1;
        }
        return output;
    }

    public static void main(String[] args) {
        int[] array = {1,3,12,0,24,24,4,1};
        int[] output = CountingSort(array);
        for (int i=0; i<output.length;i++){
            System.out.println(output[i]);
        }
    }
}
