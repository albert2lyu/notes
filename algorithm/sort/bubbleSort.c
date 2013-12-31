/*
 * =========================================================================
 *       Filename:  bubbleSort.c
 *    Description:  Bubble Sort O(N^2)
 *    Author:  Bian Jiang (Border), borderj@gmail.com
 * =========================================================================
 */
/*
 * Bubble Sort 冒泡排序法O(n^2)
 * 将待排序的元素看作是竖着排列的“气泡”，较小的元素比较轻，从而要往上浮
 * 在要排序的一组数中，对当前还未排好序的范围内的全部数，自上
 * 而下对相邻的两个数依次进行比较和调整，让较大的数往下沉，较
 * 小的往上冒。即：每当两相邻的数比较后发现它们的排序与排序要
 * 求相反时，就将它们互换。
 * 冒泡排序是稳定的。算法时间复杂度O(n2)--[n的平方]
 */
void bubble_sort(int *array, int len) {
    int i, j , tmp;
    for (i=(len -1); i > 0; i--) {
        for (j = 0, j < i; j++) {
            if (array[j] > array[j+1]) {
                tmp = array[j+1];
                array[j+1] = array[j];
                array[j] = tmp;
            }
        }
    }
}
