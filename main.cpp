#include <iostream>
#include "vl_vector.h"
int main() {

    vl_vector<int, 3> vec;
    int arr1[] = {1, 2, 3, 4, 5};
    auto it = vec.insert(vec.static_data,arr1, arr1 + 5);
//    int arr2[] = {11, 22, 33};
//    auto it2 = vec.insert(it,arr2, arr2 + 3);
    auto it2 = vec.erase(it, it + 2);
    auto it3 = vec.erase(it2, it2 + 2);

    return 0;

}