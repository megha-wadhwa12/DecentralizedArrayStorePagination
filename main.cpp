#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <unordered_map>

using namespace std;

// Function to calculate the total number of pages based on the page size
int getTotalNumberOfPages(vector<int>& filteredObjects, int pageSize) {
    int n = filteredObjects.size();
    int result = (n + pageSize - 1) / pageSize;
    return result;
}

// Function to get a subset of objects for a specific page
vector<int> getObjects(vector<int>& filteredObjects, int pageNumber, int pageSize) {
    int n = filteredObjects.size();
    int startIdx = (pageNumber - 1) * pageSize;
    int endIdx = min(startIdx + pageSize, n);
    
    // Return an empty vector if the starting index is beyond the size of the filtered objects
    if (startIdx >= n) {
        return {};
    }
    
    return vector<int>(filteredObjects.begin() + startIdx, filteredObjects.begin() + endIdx);
}

int main() {
    int N;
    cout << "Enter the number of decentralized array stores: ";
    cin >> N;

    // Validate that the number of stores is positive
    if (N <= 0) {
        cout << "Number of decentralized stores should be positive." << endl;
        return 0;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input buffers up to the new line character

    vector<vector<int>> store(N);

    // Input data for each decentralized array store
    for (int i = 0; i < N; i++) {
        cout << "Enter Array " << i + 1 << " objects: ";
        int num;
        
        while (true) {
            if (cin.peek() == '\n') {
                cin.ignore();
                break;
            }
            if (cin >> num) {
                store[i].push_back(num);
            } else {
                cin.clear(); // Clear error state if non-integer input is encountered
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore all input buffers due to error
                break;
            }
        }
    }

    int pageSize;
    cout << "Enter the size of the page: ";
    cin >> pageSize;

    // Validate that the page size is positive
    if (pageSize <= 0) {
        cout << "Page size should be positive." << endl;
        return 0;
    }

    unordered_map<int, vector<int>> filteredCache;
    vector<pair<int, int>> queries;
    int pageNumber, k;

    cout << "Enter the page number and k" << endl;
    while (cin >> pageNumber >> k) {

        // Validate that the page number and k are positive integers
        if (pageNumber <= 0 || k <= 0) {
            cout << "Page number and k should be positive integers." << endl;
            continue;
        }

        queries.push_back({pageNumber, k});
        
        // If the results for k are not already cached, filter and cache them
        if (filteredCache.find(k) == filteredCache.end()) {
            vector<int> filteredObjects;
            
            // Filter objects divisible by k from all array stores
            for (const auto& arr : store) {
                for (int obj : arr) {
                    if (obj % k == 0) {
                        filteredObjects.push_back(obj);
                    }
                }
            }
            
            filteredCache[k] = filteredObjects;
        }

        vector<int> filteredObjects = filteredCache[k];
        int totalPages = getTotalNumberOfPages(filteredObjects, pageSize);
        vector<int> objects = getObjects(filteredObjects, pageNumber, pageSize);
        
        // Output the query results
        cout << "Query " << queries.size() << ": [";
        if (objects.empty()) {
            if (pageNumber > totalPages) {
                cout << "No results (page number exceeds total pages)";
            } else {
                cout << "No results";
                totalPages = 0; // Set totalPages to 0 if there are no results
            }
            cout<<endl;
        } else {
            for (int i = 0; i < objects.size(); i++) {
                cout << (i > 0 ? ", " : "") << objects[i];
            }
        }
        
        cout << "], p = " << totalPages << endl;
        cout << endl;
    }

    return 0;
}
