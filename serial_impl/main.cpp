#include <iostream>
#include <ctime>

void merge_sort(int *a, int left, int right);

void merge(int *a, int left, int center, int right);

void print_array(int *a, int len);

void merge_sort(int *a, int left, int right) {
    int center;
    if (left < right) {
        center = (left + right) / 2;
        merge_sort(a, left, center);
        merge_sort(a, center + 1, right);
        merge(a, left, center, right);
    }
}

void merge(int *a, int left, int center, int right) {
    int i = left;
    int j = center + 1;
    int k = 0;
    int b[right - left + 1];

    while (i <= center && j <= right) {
        if (a[i] <= a[j]) {
            b[k] = a[i];
            i++;
        } else {
            b[k] = a[j];
            j++;
        }
        k++;
    }

    while (i <= center) {
        b[k] = a[i];
        k++;
        i++;
    }

    while (j <= right) {
        b[k] = a[j];
        k++;
        j++;
    }

    for (k = left; k <= right; k++) {
        a[k] = b[k - left];
    }
}


void print_array(int *a, int len) {
    std::string end_char;
    int i;

    end_char = ", ";
    for (i = 0; i < len; i++) {
        if (i == len - 1) {
            end_char = ".";
        }
        std::cout << a[i] << end_char;
    }
    std::cout << std::endl;
}


int main() {
    int *a, len, i;
    double elapsed_time;

    len = 50;

    a = (int *) malloc(len * sizeof(int));
    for (i = 0; i < len; i++) {
        a[i] = rand() % len;
    }

//    std::cout << "Original array" << std::endl;
//    print_array(a, len);

    clock_t start = clock();
    merge_sort(a, 0, len);
    clock_t end = clock();

//    std::cout << "Ordered array" << std::endl;
//    print_array(a, len);

    elapsed_time = double(end - start) / CLOCKS_PER_SEC;
    std::cout << "elapsed_time: " << elapsed_time << " sec" << std::endl;

    return 0;
}