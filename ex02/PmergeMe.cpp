#include "PmergeMe.hpp"
#include <ctime>


//##################################################################
//                   Constructor && Destructor                     #
//##################################################################
PmergeMe::PmergeMe(void) {}

PmergeMe::PmergeMe(const PmergeMe& other) : _vec(other._vec), _deq(other._deq) {}

PmergeMe::~PmergeMe(void) {}

//##################################################################
//	            	Operateur d'affectation		                   #
//##################################################################
PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
    if (this != &other) {
        _vec = other._vec;
        _deq = other._deq;
    }
    return *this;
}

//##################################################################
//                          Methodes                               #
//##################################################################
bool    PmergeMe::parseInput(int argc, char** argv) {
    if (argc < 2)
        return false;

    for (int i = 1; i < argc; ++i) {
        std::stringstream ss(argv[i]);
        int num;
        
        if (!(ss >> num) || num < 0 || ss.peek() != EOF)
            return false;
            
        _vec.push_back(num);
        _deq.push_back(num);
    }
    return true;
}

void    PmergeMe::mergeSortVector(std::vector<int>& arr, int left, int right) {
    if (right - left <= 10) {
        insertionSortVector(arr, left, right);
        return;
    }
    
    int mid = left + (right - left) / 2;
    mergeSortVector(arr, left, mid);
    mergeSortVector(arr, mid + 1, right);
    mergeVector(arr, left, mid, right);
}

void    PmergeMe::mergeVector(std::vector<int>& arr, int left, int mid, int right) {
    std::vector<int> temp(right - left + 1);
    int i = left;
    int j = mid + 1;
    int k = 0;
    
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j])
            temp[k++] = arr[i++];
        else
            temp[k++] = arr[j++];
    }
    
    while (i <= mid)
        temp[k++] = arr[i++];
    while (j <= right)
        temp[k++] = arr[j++];
        
    for (i = 0; i < k; ++i)
        arr[left + i] = temp[i];
}

void    PmergeMe::insertionSortVector(std::vector<int>& arr, int left, int right) {
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

void    PmergeMe::mergeSortDeque(std::deque<int>& arr, int left, int right) {
    if (right - left <= 10) {
        insertionSortDeque(arr, left, right);
        return;
    }
    
    int mid = left + (right - left) / 2;
    mergeSortDeque(arr, left, mid);
    mergeSortDeque(arr, mid + 1, right);
    mergeDeque(arr, left, mid, right);
}

void    PmergeMe::mergeDeque(std::deque<int>& arr, int left, int mid, int right) {
    std::deque<int> temp;
    int i = left;
    int j = mid + 1;
    
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j])
            temp.push_back(arr[i++]);
        else
            temp.push_back(arr[j++]);
    }
    
    while (i <= mid)
        temp.push_back(arr[i++]);
    while (j <= right)
        temp.push_back(arr[j++]);
        
    for (size_t k = 0; k < temp.size(); ++k)
        arr[left + k] = temp[k];
}

void    PmergeMe::insertionSortDeque(std::deque<int>& arr, int left, int right) {
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

template<typename Container>
double  PmergeMe::sortAndMeasureTime(Container& container) {
    clock_t start = clock();
    
    if (typeid(Container) == typeid(std::vector<int>))
        mergeSortVector(container, 0, container.size() - 1);
    else
        mergeSortDeque(container, 0, container.size() - 1);
        
    clock_t end = clock();
    return static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000;
}

void    PmergeMe::displayBefore(void) const {
    std::cout << "Before: ";
    for (size_t i = 0; i < _vec.size(); ++i)
        std::cout << _vec[i] << " ";
    std::cout << std::endl;
}

void    PmergeMe::displayAfter(void) const {
    std::cout << "After: ";
    for (size_t i = 0; i < _vec.size(); ++i)
        std::cout << _vec[i] << " ";
    std::cout << std::endl;
}

void    PmergeMe::sort(void) {
    displayBefore();

    double vecTime = sortAndMeasureTime(_vec);
    double deqTime = sortAndMeasureTime(_deq);

    displayAfter();

    std::cout << std::fixed << std::setprecision(5);
    std::cout << "Time to process a range of " << _vec.size() 
              << " elements with std::vector : " << vecTime << " us" << std::endl;
    std::cout << "Time to process a range of " << _deq.size() 
              << " elements with std::deque : " << deqTime << " us" << std::endl;
}