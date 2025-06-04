#include "Transaction.hpp"
#include <vector>

enum class SortMode {
    AMOUNT,
    DATE
};

void quickSortTransactions(std::vector<Transaction>& array, int start, int end, SortMode mode, bool descending);