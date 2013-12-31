/*
 * =========================================================================
 *       Filename:  selectSort.c
 *    Description:  Select Sort O(N^2)
 *         Author:  Bian Jiang (Border), borderj@gmail.com
 * =========================================================================
 */

/*
 * Select Sort 选择排序 O(N^2)
 * 首先在未排序序列中找到最小元素，存放到排序序列的起始位置，
 * 然后，再从剩余未排序元素中继续寻找最小元素，然后放到排序序列末尾。以此递归。
 * 选择排序是不稳定的。算法复杂度O(n2)--[n的平方]
 */
void select_sort(int *array, int len) {
    int i, j, min, tmp;
    for (i=0; i<len; i ++) {
        min = i;
        for (j=i+1; j<len; j++) {
            if (*(array + j) < *(array+min)) {
                min = j;
            }
        }
        if (min != i) {
            tmp = *(array + i);
            *(array + i) = *(array+min);
            *(array + min) = tmp;
        }
    }
}
