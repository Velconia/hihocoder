#include <iostream>
#include <string>
#include <vector>

using namespace std;

// The number symbolize that there is no next number to search
static const int NO_NEXT_NUM = -1;

// The number of SKIP STEP
static const size_t SKIP_STEP = 1;

// The Iterator should support the operation 
// "+", "*", "==", "!=".
template <typename Iterator>
vector<int> createNext(Iterator first, Iterator last) {
    // The result to return
    vector<int> result; 

    // Didn't do anything if the array is empty
    if (first == last) return result;

    // The first one is regarded to be NO_NEXT_NUM
    result.push_back(NO_NEXT_NUM);

    // Begin with the element after the first element 
    Iterator walker = first + SKIP_STEP;

    // Go through all elements
    while (walker != last) {
        // The lastPos is the last position of the result vector
        // Some compiler doesn't support the *vector.back() syntax
        // so we have to use this method to get the last element
        int lastPos = result.size() - 1;

        if (result[lastPos] == NO_NEXT_NUM) {
            if (*walker == *first) result.push_back(0);
            else result.push_back(NO_NEXT_NUM);
        } else {
            // cur should be the iterator to be compared now
            Iterator cur = first + result[lastPos] + SKIP_STEP;
            while (*cur != *walker) {
                // lastPos is set to the new place to compare 
                lastPos = cur - first - SKIP_STEP;

                if (!lastPos) break;
                else cur = first + result[lastPos] + SKIP_STEP;
            }

            if (*walker == *walker) result.push_back(cur - first);
            else result.push_back(NO_NEXT_NUM);
        }

        walker++;
    }

    return result;
}

// Find the first match position
// We got one original string with length N and one pattern string
// with length K then, assume that we should go M times of 
// every search of the pattern array ( 0 <= M <= K).
template <typename Iterator>
Iterator KMP(Iterator firstA, Iterator lastA, Iterator firstB, Iterator lastB) {
    // if pattern string is empty, then return 
    // the first element of the original string.
    if (lastB == firstB) return firstA;

    // Default result is the end of string A.
    Iterator result = lastA;

    // If the original string is empty, the
    // return the default result.
    if (lastA == firstA) return result;
 
    Iterator walker = firstA, pattern = firstB;

    // Prepare the nextArray about O(K * M)
    vector<int> nextArray = createNext(firstB, lastB);

    while (walker != lastA) {
        // Store the walker start place
        Iterator tmpResult = walker;        

        // Match the two strings
        while (*walker == *pattern) { walker++; pattern++; }

        if (pattern == lastB) {
            result = tmpResult; break;
        } else { 
            int nextPos = pattern - firstB - SKIP_STEP;

            // If the value in next array is NEGATIVE or the nextPos is NEGATIVE
            // Besides, the (firstB + nextArray[nextPos]) equals to firstB when nextArray[nextPos] == 0
            if (nextPos < 0 || nextArray[nextPos] == NO_NEXT_NUM) { 
                walker++; // walker moves on
                pattern = firstB; // Reset the pattern pointer 
            } else { 
                pattern = firstB + nextArray[nextPos] + SKIP_STEP;
            }
        }
    }

    return result;
}

// Find all the matching positions
// We got one original string with length N and one pattern string
// with length K then, assume that we should go M times of 
// every search of the pattern array (0 <= M <= K).
template <typename Iterator>
vector<Iterator> allKMP(Iterator firstA, Iterator lastA, Iterator firstB, Iterator lastB) {
    // Default result is the end of string A;
    vector<Iterator> result;

    // If the patten string is empty
    // then we return all iterator in the original string
    if (firstB == lastB) {
        while (firstA != lastA) result.push_back(firstA++);
        return result;
    } 

    // If the original string is empty, 
    // then return the default result
    if (firstA == lastA) {
        return result;
    }

    Iterator walker = firstA, pattern = firstB;

    // Prepare the nextArray
    vector<int> nextArray = createNext(firstB, lastB);

    // Go through all elements in the array
    while (walker != lastA) {
        // Store the walker start place
        Iterator tmpResult = walker;        

        // Match the two strings
        // We could NOT use (*walker++ == *pattern++), cause
        // we could NOT judge wether pattern reach the end
        while (*walker == *pattern) { walker++; pattern++; }

        // if pattern reach the end, then we just return the start place
        if (pattern == lastB) { result.push_back(tmpResult); }

        // the nextPos in NEXT array
        int nextPos = pattern - firstB - SKIP_STEP;

        // If the value in next array is NOT active
        if (nextPos < 0 || nextArray[nextPos] == NO_NEXT_NUM) {
            // Reset the pattern and continue
            walker++; pattern = firstB; continue;
        } else { 
            if (pattern == lastB) {
                pattern = firstB;
                walker -= (nextArray[nextPos] + SKIP_STEP);
            } else {
                pattern = firstB + nextArray[nextPos] + SKIP_STEP;;
            }
        }
    }

    return result;
}

// Find all distinct matching positions
// We got one original string with length N and one pattern string
// with length K then, assume that we should go M times of 
// every search of the pattern array (0 <= M <= K).
template <typename Iterator>
vector<Iterator> allDistinctKMP(Iterator firstA, Iterator lastA, Iterator firstB, Iterator lastB) {
    // Default result is the end of string A;
    vector<Iterator> result;

    // If the patten string is empty
    // then we return all iterator in the original string
    if (firstB == lastB) {
        while (firstA != lastA) result.push_back(firstA++);
        return result;
    } 

    // If the original string is empty, 
    // then return the default result
    if (firstA == lastA) {
        return result;
    }

    // Match process 
    Iterator walker = firstA, pattern = firstB;

    // Prepare the nextArray
    vector<int> nextArray = createNext(firstB, lastB);

    while (walker != lastA) {
        // Store the walker start place
        Iterator tmpResult = walker;        

        // Match the two strings
        while (*walker == *pattern) { walker++; pattern++; }

        if (pattern == lastB) {
            result.push_back(tmpResult);
            pattern = firstB; continue;
        }

        int nextPos = pattern - firstB - 1;

        // If the value in next array is NOT active
        if (nextPos < 0 || nextArray[nextPos] == -1) {
            // Reset the pattern and continue
            walker++; pattern = firstB; continue;
        } else { 
            pattern = firstB + nextArray[nextPos] + 1;
        }
    }

    return result;
}

int main() {
    string str = "abasfabbceababababbabb";
    string ptn = "abab"; 
    vector<string::iterator> p = allKMP(str.begin(), str.end(), ptn.begin(), ptn.end());
    cout << p.size() << endl;
    for (vector<string::iterator>::iterator iter = p.begin(); iter != p.end(); ++iter) {
        int i = 0;
        while (i != ptn.size()) {
            cout << *(*iter + i++);
        }
        cout << endl;
    }

    vector<int> nextArray = createNext(ptn.begin(), ptn.end());
    for (vector<int>::iterator iter = nextArray.begin(); iter != nextArray.end(); ++iter) {
        cout << *iter << " ";
    }
}