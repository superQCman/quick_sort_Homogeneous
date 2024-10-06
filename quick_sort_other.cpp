#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include "apis_c.h"

void quicksort(int *arr, int left, int right, int depth);
int partition(int *arr, int left, int right);
int size=30;

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
    // const int MAX_DEPTH = 4; //防止创建过多线程，限制递归深度

    if (right - left <= THRESHOLD) {
        std::cout<<"--------------------------------------------------------------------插入排序----------------------------------------------------------------"<<std::endl;
        std::cout<<"left:"<<left<<" right:"<<right<<std::endl;
        insertion_sort(arr, left, right);
        for (int i=0;i<size;i++){
            std::cout<<arr[i]<<" ";
        }
        std::cout<<std::endl;
    } else if (left < right) {
        int pivot_index = partition(arr, left, right);
        
        if (idX < 2) {
            int pivot_index_left = pivot_index - 1;
            int pivot_index_right = pivot_index + 1;
            depth += 1;
            // 创建左、右部分的线程，但限制递归深度
            if (idY == 0){
                InterChiplet::sendMessage(idX+1, 0, idX, idY, arr, sizeof(int)*size);
                InterChiplet::sendMessage(idX+1, 0, idX, idY, &left, sizeof(left));

                InterChiplet::sendMessage(idX+1, 0, idX, idY, &pivot_index_left, sizeof(pivot_index_left)); //将pivot_index_发送给下一个chiplet
                InterChiplet::sendMessage(idX+1, 0, idX, idY, &depth, sizeof(depth));

                InterChiplet::sendMessage(idX+1, 1, idX, idY, arr, sizeof(int)*size);
                InterChiplet::sendMessage(idX+1, 1, idX, idY, &pivot_index_right, sizeof(pivot_index_right));
                InterChiplet::sendMessage(idX+1, 1, idX, idY, &right, sizeof(right));
                InterChiplet::sendMessage(idX+1, 1, idX, idY, &depth, sizeof(depth));

                int *arr_left = new int [size];
                int *arr_right = new int [size];
                InterChiplet::receiveMessage(idX, idY, idX+1, idY, arr_left, sizeof(int)*size);
                InterChiplet::receiveMessage(idX, idY, idX+1, idY+1, arr_right, sizeof(int)*size);
                std::cout<<"---------------------------------------------------------------"<<std::endl;
                std::cout<<"left: ";
                for (int i=0;i<size;i++){
                    std::cout<<arr_left[i]<<" ";
                }
                std::cout<<std::endl;
                std::cout<<"right: ";
                for (int i=0;i<size;i++){
                    std::cout<<arr_right[i]<<" ";
                }
                std::cout<<std::endl;
                std::cout<<"pivot_index: "<<pivot_index<<std::endl;
                for (int i=left;i <= pivot_index;i++){
                    arr[i] = arr_left[i];
                }
                for (int i = pivot_index + 1; i <= right; ++i) {
                    arr[i] = arr_right[i];
                }
                std::cout<<std::endl;
                delete[] arr_left;
                delete[] arr_right;
            }else{
                InterChiplet::sendMessage(idX+2, 0, idX, idY, arr, sizeof(int)*size);
                InterChiplet::sendMessage(idX+2, 0, idX, idY, &left, sizeof(left));

                InterChiplet::sendMessage(idX+2, 0, idX, idY, &pivot_index_left, sizeof(pivot_index_left)); //将pivot_index_发送给下一个chiplet
                InterChiplet::sendMessage(idX+2, 0, idX, idY, &depth, sizeof(depth));

                InterChiplet::sendMessage(idX+2, 1, idX, idY, arr, sizeof(int)*size);
                InterChiplet::sendMessage(idX+2, 1, idX, idY, &pivot_index_right, sizeof(pivot_index_right));
                InterChiplet::sendMessage(idX+2, 1, idX, idY, &right, sizeof(right));
                InterChiplet::sendMessage(idX+2, 1, idX, idY, &depth, sizeof(depth));

                int *arr_left = new int [size];
                int *arr_right = new int [size];
                InterChiplet::receiveMessage(idX, idY, idX+2, idY-1, arr_left, sizeof(int)*size);
                InterChiplet::receiveMessage(idX, idY, idX+2, idY, arr_right, sizeof(int)*size);
                std::cout<<"---------------------------------------------------------------"<<std::endl;
                std::cout<<"left: ";
                for (int i=0;i<size;i++){
                    std::cout<<arr_left[i]<<" ";
                }
                std::cout<<std::endl;
                std::cout<<"right: ";
                for (int i=0;i<size;i++){
                    std::cout<<arr_right[i]<<" ";
                }
                std::cout<<std::endl;
                std::cout<<"pivot_index: "<<pivot_index<<std::endl;
                for (int i=left;i <= pivot_index;i++){
                    arr[i] = arr_left[i];
                    std::cout<<arr[i]<<" ";
                }
                for (int i = pivot_index + 1; i <= right; ++i) {
                    arr[i] = arr_right[i];
                    std::cout<<arr[i]<<" ";
                }
                std::cout<<std::endl;
                delete[] arr_left;
                delete[] arr_right;
            }
            
        } else {
            quicksort(arr, left, pivot_index - 1, depth + 1, idX, idY);
            quicksort(arr, pivot_index + 1, right, depth + 1, idX, idY);
        }
        
    }
    if (idX !=3 && idY == 0) InterChiplet::sendMessage(idX-1, idY, idX, idY, arr, sizeof(int)*size);
    if (idX !=3 && idY == 1) InterChiplet::sendMessage(idX-1, idY-1, idX, idY, arr, sizeof(int)*size);
    if (idX ==3 && idY == 0) InterChiplet::sendMessage(idX-2, idY+1, idX, idY, arr, sizeof(int)*size);
    if (idX ==3 && idY == 1) InterChiplet::sendMessage(idX-2, idY, idX, idY, arr, sizeof(int)*size);
    
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
    int *depth = new int;
    int *left = new int;
    int *right = new int;
    if(idX != 3 && idY == 0){
        InterChiplet::receiveMessage(idX, idY, idX-1, idY, left, sizeof(int));
        InterChiplet::receiveMessage(idX, idY, idX-1, idY, right, sizeof(int));
        InterChiplet::receiveMessage(idX, idY, idX-1, idY, depth, sizeof(int));
        std::cout<<"------------------------------------接受数据-------------------------------------"<<std::endl;
        std::cout<<"left: "<<*left<<" right: "<<*right<<" depth: "<<*depth<<std::endl;
    }else if(idX != 3){
        InterChiplet::receiveMessage(idX, idY, idX-1, idY-1, left, sizeof(int));
        InterChiplet::receiveMessage(idX, idY, idX-1, idY-1, right, sizeof(int));
        InterChiplet::receiveMessage(idX, idY, idX-1, idY-1, depth, sizeof(int));
        std::cout<<"------------------------------------接受数据-------------------------------------"<<std::endl;
        std::cout<<"left: "<<*left<<" right: "<<*right<<" depth: "<<*depth<<std::endl;
    }
    if (idX == 3 && idY == 0){
        InterChiplet::receiveMessage(idX, idY, idX-2, idY+1, left, sizeof(int));
        InterChiplet::receiveMessage(idX, idY, idX-2, idY+1, right, sizeof(int));
        InterChiplet::receiveMessage(idX, idY, idX-2, idY+1, depth, sizeof(int));
        std::cout<<"------------------------------------接受数据-------------------------------------"<<std::endl;
        std::cout<<"left: "<<*left<<" right: "<<*right<<" depth: "<<*depth<<std::endl;
    }else if (idX == 3 && idY == 1){
        InterChiplet::receiveMessage(idX, idY, idX-2, idY, left, sizeof(int));
        InterChiplet::receiveMessage(idX, idY, idX-2, idY, right, sizeof(int));
        InterChiplet::receiveMessage(idX, idY, idX-2, idY, depth, sizeof(int));
        std::cout<<"------------------------------------接受数据-------------------------------------"<<std::endl;
        std::cout<<"left: "<<*left<<" right: "<<*right<<" depth: "<<*depth<<std::endl;
    }

    quicksort(arr, *left, *right, *depth, idX, idY);
    delete depth;
    delete left;
    delete right;
}

int main(int argc, char **argv) {
    int idX = atoi(argv[1]);
    int idY = atoi(argv[2]);
    int *arr = new int [size];
    if (idY == 0 && idX !=3) InterChiplet::receiveMessage(idX, idY, idX-1, idY, arr, sizeof(int)*size);
    else if (idY == 1 && idX !=3) InterChiplet::receiveMessage(idX, idY, idX-1, idY-1, arr, sizeof(int)*size);
    else if (idX == 3 && idY == 0) InterChiplet::receiveMessage(idX, idY, idX-2, idY+1, arr, sizeof(int)*size);
    else if (idX == 3 && idY == 1) InterChiplet::receiveMessage(idX, idY, idX-2, idY, arr, sizeof(int)*size);
    std::cout << "Original array: ";
    for (int i=0; i<size; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    threaded_quicksort(arr, idX, idY);

    std::cout << "Sorted array: ";
    for (int i=0; i<size; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    delete[] arr;

    return 0;
}