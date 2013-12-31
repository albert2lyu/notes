/*
 * =========================================================================
 *       Filename:  quickSort.c
 *    Description:  Quick Sort O(n*lgn)
 *         Author:  Bian Jiang (Border), borderj@gmail.com
 * =========================================================================
 */
/*
 * 先选择中间值，然后把比它小的放在左边，大的放在右边（具体的实现是从两边找，
 * 找到一对后交换）。然后对两边分别使用这个过程（递归）
 */
void quick_sort(int *array, int begin, int end) {
    if (array == NULL) {
        return;
    }
    int divide = array[begin];
    int i = begin;
    int j = end;
    while (i < j) {
        while (i < j && array[j] > divide) {
            j--;
        }
        array[i] = array[j]
        while (i < j && array[i] < divide) {
            i++;
        }
        array[j] = array[i];
    }
    array[i] = divide;
    if (i - 1 > begin) {
        quick_sort(array, begin, i -1);
    }
    if (i+1 < end) {
        quick_sort(array, i+1, end);
    }
}
