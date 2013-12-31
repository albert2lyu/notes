/*
 * =========================================================================
 *       Filename:  insertionSort.c
 *    Description:  Insertion Sort(O(n^2))
 *         Author:  Bian Jiang (Border), borderj@gmail.com
 * =========================================================================
 */
/* 
 * http://en.wikipedia.org/wiki/Insertion_sort
 * 插入排序法 O(N^2)
 * 逐一取出元素，在已经排序的元素序列中从后向前扫描，放到适当的位置
 * 直接插入排序是稳定的。算法时间复杂度O(n2)--[n的平方]
 */
void insert_sort(int *array, unsigned int n) {
    int i,j;
    int tmp;
    for (i = 1; i < n; i++) {
        tmp = *(array + i);
        for (j = i; j > 0 && *(array + j - 1) > tmp; j--) {
            *(array + j) = *(array + j - 1);
        }
        *(array +j) = tmp; 
    }
}
