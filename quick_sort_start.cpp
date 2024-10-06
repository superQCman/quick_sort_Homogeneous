#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include "apis_c.h"

int size=30;
void quicksort(int *arr, int left, int right, int depth);
int partition(int *arr, int left, int right);

void insertion_sort(int *arr, int left, int right) {
    for (int i = left + 1; i <= right; ++i) {
        int key = arr[i];
        int j = i - 1;
        while (j >= left && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

void quicksort(int *arr, int left, int right, int depth, int idX, int idY) {
    const int THRESHOLD = 10; //用于决定在快速排序算法中何时切换到插入排序。通常，当数组的大小小于或等于这个阈值时，插入排序会比快速排序更有效率。
    const int MAX_DEPTH = 4; //防止创建过多线程，限制递归深度

    if (right - left <= THRESHOLD) {
        insertion_sort(arr, left, right);
    } else if (left < right) {
        int pivot_index = partition(arr, left, right);
        
        if (depth < MAX_DEPTH) {
            int pivot_index_left = pivot_index - 1;
            int pivot_index_right = pivot_index + 1;
            depth+=1;
            // 创建左、右部分的线程，但限制递归深度
            InterChiplet::sendMessage(idX+1, 0, idX, idY, arr, sizeof(int)*size);
            InterChiplet::sendMessage(idX+1, 0, idX, idY, &left, sizeof(int));
            InterChiplet::sendMessage(idX+1, 0, idX, idY, &pivot_index_left, sizeof(int)); //将pivot_index_发送给下一个chiplet
            InterChiplet::sendMessage(idX+1, 0, idX, idY, &depth, sizeof(int32_t));

            InterChiplet::sendMessage(idX+1, 1, idX, idY, arr, sizeof(int)*size);
            InterChiplet::sendMessage(idX+1, 1, idX, idY, &pivot_index_right, sizeof(int));
            InterChiplet::sendMessage(idX+1, 1, idX, idY, &right, sizeof(int));
            InterChiplet::sendMessage(idX+1, 1, idX, idY, &depth, sizeof(int));

            int *arr_left = new int [size];
            int *arr_right = new int [size];
            InterChiplet::receiveMessage(idX, idY, idX+1, idY, arr_left, sizeof(int)*size);
            InterChiplet::receiveMessage(idX, idY, idX+1, idY+1, arr_right, sizeof(int)*size);
            for (int i=left;i <= pivot_index;i++){
                arr[i] = arr_left[i];
            }
            for (int i = pivot_index + 1; i <= right; ++i) {
                arr[i] = arr_right[i];
            }
            delete[] arr_left;
            delete[] arr_right;
            // std::thread left_thread(quicksort, std::ref(arr), left, pivot_index - 1, depth + 1);
            // std::thread right_thread(quicksort, std::ref(arr), pivot_index + 1, right, depth + 1);
            
            // 启动线程并等待结束
            // left_thread.join();
            // right_thread.join();
        } else {
            quicksort(arr, left, pivot_index - 1, depth + 1, idX, idY);
            quicksort(arr, pivot_index + 1, right, depth + 1, idX, idY);
        }
    }
}

int partition(int *arr, int left, int right) {
    int mid = left + (right - left) / 2;
    int pivot = std::max(std::min(arr[left], arr[right]), std::min(std::max(arr[left], arr[right]), arr[mid]));
    if (pivot == arr[mid]) {
        std::swap(arr[mid], arr[right]);
    } else if (pivot == arr[left]) {
        std::swap(arr[left], arr[right]);
    }

    int i = left - 1;
    for (int j = left; j < right; ++j) {
        if (arr[j] <= arr[right]) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[right]);
    return i + 1;
}

void threaded_quicksort(int *arr, int idX, int idY) {
    quicksort(arr, 0, size - 1, 0, idX, idY);
}

int main(int argc, char **argv) {
    int idX = atoi(argv[1]);
    int idY = atoi(argv[2]);
    int arr[] = {100, 3, 50, 200, 33, 17, 25, 400, 120, 75, 6, 8, 10, 1, 2, 1, 15, 7, 9, 250, 300, 4, 180, 170, 90, 55, 85, 95, 77, 65};
    size = sizeof(arr) / sizeof(arr[0]);
    std::cout << "Original array: ";
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    threaded_quicksort(arr, idX, idY);

    std::cout << "Sorted array: ";
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}