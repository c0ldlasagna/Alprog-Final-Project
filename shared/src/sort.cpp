#include "Transaction.hpp"
#include <vector>
#include <string>
#include "sort.hpp"

void swap(Transaction &x, Transaction &y) {
    Transaction temp = x;
    x = y;
    y = temp;
}

bool compare(const Transaction& a, const Transaction& b, SortMode mode, bool descending) {
    switch (mode) {
        case SortMode::AMOUNT:
            return descending ? a.amount > b.amount : a.amount < b.amount;
        case SortMode::DATE:
            return descending ? a.timestamp > b.timestamp : a.timestamp < b.timestamp;
        default:
            return false;
    }
}

int partition(std::vector<Transaction>& array, int start, int end, SortMode mode,bool descending) {
    Transaction pivot = array[end];
    int i = start - 1;
    for (int j = start; j < end; j++) {
        if (compare(array[j], pivot, mode,descending)) {
            i++;
            swap(array[j], array[i]);
        }
    }
    swap(array[i + 1], array[end]);
    return (i + 1);
}

void quickSortTransactions(std::vector<Transaction>& array, int start, int end, SortMode mode, bool descending) {
    if (start < end) {
        int pivot = partition(array, start, end, mode,descending);
        quickSortTransactions(array, start, pivot - 1, mode,descending);
        quickSortTransactions(array, pivot + 1, end, mode,descending);
    }
}
