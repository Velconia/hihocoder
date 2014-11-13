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
vector<int> createNext(Iterator first, Iterator last) {
    // The result to return
    vector<int> result; 

    // Didn't do anything if the array is empty
    if (first >= last) return result;

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
            // cur should be the Iterator to be compared now
            Iterator cur = first + result[lastPos] + SKIP_STEP;
            while (*cur != *walker) {
                // lastPos is set to the new place to compare 
                lastPos = cur - first - SKIP_STEP;

                if (!lastPos) break;
                else cur = first + result[lastPos] + SKIP_STEP;
            }

            if (*cur == *walker) result.push_back(cur - first);
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
    if (lastB <= firstB) return firstA;

    // Default result is the end of string A.
    Iterator result = lastA;

    // If the original string is empty, the
    // return the default result.
    if (lastA <= firstA || ((lastA - firstA) < (lastB - firstB))) return result;

    Iterator walker = firstA, pattern = firstB;

    // Prepare the nextArray about O(K * M)
    vector<int> nextArray = createNext(firstB, lastB);

    while (walker != lastA && pattern != lastB) {
        if (pattern == firstA - NO_NEXT_NUM) {
            pattern++;
            continue;
        }

        if (*walker == *pattern) {
            walker++;
            pattern++;
        } else {
            if (pattern != firstB) {
                pattern = nextArray[pattern - firstB - 1] + firstB;
            } else {
                walker++;
            }
        }
    }

    // If we get the answer, then set the result to the walker
    if (pattern == lastB) result = walker - (lastB - firstB);

    return result;
}

// Find all the matching positions
 // We got one original string with length N and one pattern string
 // with length K then, assume that we should go M times of 
 // every search of the pattern array (0 <= M <= K).
 vector<Iterator> allKMP(Iterator firstA, Iterator lastA, 
                                      Iterator firstB, Iterator lastB) {
     // Default result is the end of string A;
     vector<Iterator> result;

     // If the patten string is empty
     // then we return all Iterator in the original string
     if (firstB == lastB) {
         while (firstA != lastA) result.push_back(firstA++);
         return result;
     } 

     // If the original string is empty, 
     // then return the default result
     if (firstA >= lastA || ((lastA - firstA) < (lastB - firstB))) {
         return result;
     }

     Iterator walker = firstA, pattern = firstB;

     // Prepare the nextArray
     vector<int> nextArray = createNext(firstB, lastB);

     // Go through all elements in the array
     while (walker != lastA) {
         // Store the walker start place
         Iterator tmpResult = walker;        

         cout << "tmpResult: " << *tmpResult << " " << tmpResult - firstA << endl;

         // Match the two strings
         // We could NOT use (*walker++ == *pattern++), cause
         // we could NOT judge whether pattern reach the end
         while (walker != lastA && pattern != lastB && *walker == *pattern) 
            { walker++; pattern++; }

         // if pattern reach the end, then we just return the start place
         if (pattern == lastB) { result.push_back(tmpResult); }

         if (walker == lastA) { break; }

         // the nextPos in NEXT array
         int nextPos = pattern - firstB - SKIP_STEP;

         // If the value in next array is NOT active
         if (nextPos < 0 || nextArray[nextPos] == NO_NEXT_NUM) {
             // Reset the pattern and continue
             if (pattern != lastB) 
                 walker++;
             pattern = firstB; 
             continue;
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
// template <typename Iterator>
// vector<Iterator> allDistinctKMP(Iterator firstA, Iterator lastA, Iterator firstB, Iterator lastB) {
//     // Default result is the end of string A;
//     vector<Iterator> result;

//     // If the patten string is empty
//     // then we return all Iterator in the original string
//     if (firstB == lastB) {
//         while (firstA != lastA) result.push_back(firstA++);
//         return result;
//     } 

//     // If the original string is empty, 
//     // then return the default result
//     if (firstA == lastA || ((lastA - firstA) < (lastB - firstB))) {
//         return result;
//     }

//     // Match process 
//     Iterator walker = firstA, pattern = firstB;

//     // Prepare the nextArray
//     vector<int> nextArray = createNext(firstB, lastB);
//     for (vector<int>::Iterator iter = nextArray.begin(); iter != nextArray.end(); ++iter) {
//         cout << *iter << " ";
//     }
//     cout << endl;

//     while (walker != lastA) {
//         // Store the walker start place
//         Iterator tmpResult = walker;        

//         cout << "tmpResult: " << *tmpResult << " " << tmpResult - firstA << endl;

//         // Match the two strings.
//         // We could probably reach the end of the A string
//         // while we didn't reach the end of the B string.
//         while (walker != lastA && *walker == *pattern) { 
//             walker++; pattern++;
//         }

//         if (pattern == lastB) {
//             result.push_back(tmpResult);
//             pattern = firstB; continue;
//         }

//         // If we did NOT reach the end of B string
//         // but we reach the end of A string, then
//         // we just return.
//         if (walker == lastA) {
//             return result;
//         }

//         int nextPos = pattern - firstB - SKIP_STEP;

//         // If the value in next array is NOT active
//         if (nextPos < 0 || nextArray[nextPos] == NO_NEXT_NUM) {
//             // Reset the pattern and continue
//             walker++; pattern = firstB; continue;
//         } else { 
//             pattern = firstB + nextArray[nextPos] + SKIP_STEP;
//         }
//     }

//     return result;
// }

// Find the first match position
// We got one original string with length N and one pattern string
// with length K then, assume that we should go M times of 
// every search of the pattern array ( 0 <= M <= K).
template <typename Iterator>
Iterator KMP(Iterator firstA, Iterator lastA, Iterator firstB, Iterator lastB) {
    // if pattern string is empty, then return 
    // the first element of the original string.
    if (lastB <= firstB) return firstA;

    // Default result is the end of string A.
    Iterator result = lastA;

    // If the original string is empty, the
    // return the default result.
    if (lastA <= firstA || ((lastA - firstA) < (lastB - firstB))) return result;

    Iterator walker = firstA, pattern = firstB;

    // Prepare the nextArray about O(K * M)
    vector<int> nextArray = createNext(firstB, lastB);

    while (walker != lastA && pattern != lastB) {
        if (pattern == firstA - NO_NEXT_NUM) {
            pattern++;
            continue;
        }

        if (*walker == *pattern) {
            walker++;
            pattern++;
        } else {
            if (pattern != firstB) {
                pattern = nextArray[pattern - firstB - 1] + firstB;
            } else {
                walker++;
            }
        }
    }

    // If we get the answer, then set the result to the walker
    if (pattern == lastB) result = walker - (lastB - firstB);

    return result;
}

int main() {
    size_t testCaseCount = 0; 
    cin >> testCaseCount;
    while (testCaseCount--) {
        string ptn, str;
        cin >> ptn >> str;
        cout << allDistinctKMP(str.begin(), str.end(), ptn.begin(), ptn.end()).size() << endl;
    }
}

//#include <iostream>
//#include <string>
//#include <vector>
//
//using namespace std;
//
//// The number symbolize that there is no next number to search
//static const int NO_NEXT_NUM = -1;
//
//// The number of SKIP STEP
//static const size_t SKIP_STEP = 1;
//
//// The std::string::iterator should support the operation 
//// "+", "*", "==", "!=".
//vector<int> createNext(string::iterator first, string::iterator last) {
//  // The result to return
//  vector<int> result; 
//
//  // Didn't do anything if the array is empty
//  if (first >= last) return result;
//
//  // The first one is regarded to be NO_NEXT_NUM
//  result.push_back(NO_NEXT_NUM);
//
//  // Begin with the element after the first element 
//  string::iterator walker = first + SKIP_STEP;
//
//  // Go through all elements
//  while (walker != last) {
//      // The lastPos is the last position of the result vector
//      // Some compiler doesn't support the *vector.back() syntax
//      // so we have to use this method to get the last element
//      int lastPos = result.size() - 1;
//
//      if (result[lastPos] == NO_NEXT_NUM) {
//          if (*walker == *first) {
//              result.push_back(0);
//          } else {
//              result.push_back(NO_NEXT_NUM);
//          }
//      } else {
//          // cur should be the std::string::iterator to be compared now
//          string::iterator cur = first + result[lastPos] + SKIP_STEP;
//          while (*cur != *walker) {
//              // lastPos is set to the new place to compare 
//              lastPos = cur - first - SKIP_STEP;
//
//              if (lastPos == NO_NEXT_NUM) break;
//              else cur = first + result[lastPos] + SKIP_STEP;
//          }
//
//          if (*cur == *walker) result.push_back(cur - first);
//          else result.push_back(NO_NEXT_NUM);
//      }
//
//      walker++;
//  }
//
//  return result;
//}
//
// // Find the first match position
// // We got one original string with length N and one pattern string
// // with length K then, assume that we should go M times of 
// // every search of the pattern array ( 0 <= M <= K).
// std::string::iterator KMP(std::string::iterator firstA, std::string::iterator lastA, 
//                         std::string::iterator firstB, std::string::iterator lastB) {
//     // if pattern string is empty, then return 
//     // the first element of the original string.
//     if (lastB <= firstB || ((lastA - firstA) < (lastB - firstB))) return firstA;
//
//     // Default result is the end of string A.
//     std::string::iterator result = lastA;
//
//     // If the original string is empty, the
//     // return the default result.
//     if (lastA <= firstA) return result;
//
//     std::string::iterator walker = firstA, pattern = firstB;
//
//     // Prepare the nextArray about O(K * M)
//     vector<int> nextArray = createNext(firstB, lastB);
//
//     while (walker != lastA) {
//         // Store the walker start place
//         std::string::iterator tmpResult = walker;        
//
//         // Match the two strings
//         while (walker != lastA && pattern != lastB && *walker == *pattern) 
//          { walker++; pattern++; }
//
//         if (pattern == lastB) {
//             result = tmpResult; break;
//         } else { 
//             int nextPos = pattern - firstB - SKIP_STEP;
//
//             // If the value in next array is NEGATIVE or the nextPos is NEGATIVE
//             // Besides, the (firstB + nextArray[nextPos]) equals to firstB when nextArray[nextPos] == 0
//             if (nextPos < 0 || nextArray[nextPos] == NO_NEXT_NUM) { 
//                 walker++; // walker moves on
//                 pattern = firstB; // Reset the pattern pointer 
//             } else { 
//                 pattern = firstB + nextArray[nextPos] + SKIP_STEP;
//             }
//         }
//     }
//
//     return result;
// }
//
// // Find all the matching positions
// // We got one original string with length N and one pattern string
// // with length K then, assume that we should go M times of 
// // every search of the pattern array (0 <= M <= K).
// vector<string::iterator> allKMP(string::iterator firstA, string::iterator lastA, 
//                                    string::iterator firstB, string::iterator lastB) {
//     // Default result is the end of string A;
//     vector<string::iterator> result;
//
//     // If the patten string is empty
//     // then we return all std::string::iterator in the original string
//     if (firstB == lastB) {
//         while (firstA != lastA) result.push_back(firstA++);
//         return result;
//     }
//
//     // If the original string is empty, 
//     // then return the default result
//     if (firstA >= lastA || ((lastA - firstA) < (lastB - firstB))) {
//         return result;
//     }
//
//     string::iterator walker = firstA, pattern = firstB;
//
//     // Prepare the nextArray
//     vector<int> nextArray = createNext(firstB, lastB);
//   for (vector<int>::iterator iter = nextArray.begin(); iter != nextArray.end(); iter++) {
//      cout << *iter << " ";
//   }
//   cout << endl;
//
//     // Go through all elements in the array
//     while (walker != lastA) {
//         // Store the walker start place
//         std::string::iterator tmpResult = walker;        
//
//       cout << "tmpResult: " << *tmpResult << " " << tmpResult - firstA << endl;
//
//         // Match the two strings
//         // We could NOT use (*walker++ == *pattern++), cause
//         // we could NOT judge whether pattern reach the end
//         while (walker != lastA && pattern != lastB && *walker == *pattern) 
//          { walker++; pattern++; }
//
//         // if pattern reach the end, then we just return the start place
//         if (pattern == lastB) { result.push_back(tmpResult); }
//
//       if (walker == lastA) { break; }
//
//         // the nextPos in NEXT array
//         int nextPos = pattern - firstB - SKIP_STEP;
//
//         // If the value in next array is NOT active
//         if (nextPos < 0 || nextArray[nextPos] == NO_NEXT_NUM) {
//             // Reset the pattern and continue
//           if (pattern != lastB) walker++;
//             pattern = firstB; continue;
//         } else { 
//             if (pattern == lastB) {
//                 pattern = firstB;
//                 walker -= (nextArray[nextPos] + SKIP_STEP);
//             } else {
//                 pattern = firstB + nextArray[nextPos] + SKIP_STEP;;
//             }
//         }
//     }
//
//     return result;
// }
//
//// Find all distinct matching positions
//// We got one original string with length N and one pattern string
//// with length K then, assume that we should go M times of 
//// every search of the pattern array (0 <= M <= K).
//vector<std::string::iterator> allDistinctKMP(std::string::iterator firstA, std::string::iterator lastA, 
//                                           std::string::iterator firstB, std::string::iterator lastB) {
//  // Default result is the end of string A;
//  vector<std::string::iterator> result;
//
//  // If the patten string is empty
//  // then we return all std::string::iterator in the original string
//  if (firstB == lastB) {
//      while (firstA != lastA) result.push_back(firstA++);
//      return result;
//  } 
//
//  // If the original string is empty, 
//  // then return the default result
//  if (firstA == lastA || ((lastA - firstA) < (lastB - firstB))) {
//      return result;
//  }
//
//  // Match process 
//  std::string::iterator walker = firstA, pattern = firstB;
//
//  // Prepare the nextArray
//  vector<int> nextArray = createNext(firstB, lastB);
//
//  while (walker != lastA) {
//      // Store the walker start place
//      std::string::iterator tmpResult = walker;        
//
//      //cout << "tmpResult: " << *tmpResult << " " << tmpResult - firstA << endl;
//
//      // Match the two strings.
//      // We could probably reach the end of the A string
//      // while we didn't reach the end of the B string.
//      while (walker != lastA && pattern != lastB && *walker == *pattern) { 
//          walker++; pattern++;
//      }
//
//      if (pattern == lastB) {
//          result.push_back(tmpResult);
//          pattern = firstB; continue;
//      }
//
//      // If we did NOT reach the end of B string
//      // but we reach the end of A string, then
//      // we just return.
//      if (walker == lastA) {
//          return result;
//      }
//
//      int nextPos = pattern - firstB - SKIP_STEP;
//
//      // If the value in next array is NOT active
//      if (nextPos < 0 || nextArray[nextPos] == NO_NEXT_NUM) {
//          // Reset the pattern and continue
//          walker++; pattern = firstB; continue;
//      } else { 
//          pattern = firstB + nextArray[nextPos] + SKIP_STEP;
//      }
//  }
//
//  return result;
//}
//
////int main() {
////    size_t testCaseCount = 0; 
////    cin >> testCaseCount;
////    while (testCaseCount--) {
////        string ptn, str;
////        cin >> ptn >> str;
////        cout << KMP(str.begin(), str.end(), ptn.begin(), ptn.end()) - str.begin() << endl;
////        cout << allKMP(str.begin(), str.end(), ptn.begin(), ptn.end()).size() << endl;
////        cout << allDistinctKMP(str.begin(), str.end(), ptn.begin(), ptn.end()).size() << endl;
////    }
////}

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
    if (first - last >= 0) return result;

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
            // cur should be the Iterator to be compared now
            Iterator cur = first + result[lastPos] + SKIP_STEP;
            while (*cur != *walker) {
                // lastPos is set to the new place to compare 
                lastPos = cur - first - SKIP_STEP;

                if (lastPos == NO_NEXT_NUM) break;
                else cur = first + result[lastPos] + SKIP_STEP;
            }

            if (*cur == *walker) result.push_back(cur - first);
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
int KMP(Iterator firstA, Iterator lastA, Iterator firstB, Iterator lastB) {

    // Default result is the end of string A.
    int result = 0;

    // if pattern string is empty, then return 
    // the first element of the original string.
    if (lastB - firstB <= 0) return result;

    // If the original string is empty, the
    // return the default result.
    if ((lastA - firstA <= 0) || ((lastA - firstA) < (lastB - firstB))) return result;

    Iterator walker = firstA, pattern = firstB;

    // Prepare the nextArray about O(K * M)
    vector<int> nextArray = createNext(firstB, lastB);
    for (vector<int>::iterator iter = nextArray.begin(); iter != nextArray.end(); ++iter) {
        cout << *iter << " ";
    }
    cout << endl;

    while (walker != lastA) {
        if (pattern == firstB + NO_NEXT_NUM) {
            pattern++;
            continue;
        }

        //cout << (walker - firstA) << " " << (pattern - firstB) << endl;

        if (*walker == *pattern) {
            walker++;
            pattern++;
        } else {
            if (pattern != firstB) {
                pattern = nextArray[pattern - firstB - SKIP_STEP] + firstB;
            } else {
                walker++;
            }
        }

        if (pattern == lastB) {
            //result.push_back(walker - (lastB - firstB));    
            result++;
            //cout << "Catch: " << walker - (lastB - firstB) - firstA << endl;
            pattern = nextArray[pattern - firstB - SKIP_STEP] + firstB + SKIP_STEP;
        }
    }

    return result;
}

int main() {
    int testCaseCount = 0;
    cin >> testCaseCount;
    while (testCaseCount--) {
        string str, ptn;
        cin >> ptn >> str;
        cout << KMP(str.begin(), str.end(), ptn.begin(), ptn.end()) << endl;
    }
}