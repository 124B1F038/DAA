#include <iostream>
#include <algorithm>
using namespace std;

struct Item {
    int weight;
    int value;
    double ratio;
};

// Compare items based on value/weight ratio
bool compare(Item a, Item b) {
    return a.ratio > b.ratio;
}

int main() {
    int n;

    cout << "Enter number of items: ";
    cin >> n;

    Item items[n];

    cout << "\nEnter Weight and Value of each item:\n";

    for (int i = 0; i < n; i++) {
        cin >> items[i].weight >> items[i].value;
        items[i].ratio = (double)items[i].value / items[i].weight;
    }

    int capacity;
    cout << "\nEnter maximum capacity of knapsack: ";
    cin >> capacity;

    // Sort according to value/weight ratio
    sort(items, items + n, compare);

    double maxValue = 0.0;

    cout << "\nSelected Items:\n";

    for (int i = 0; i < n; i++) {

        if (capacity >= items[i].weight) {
            // Take complete item
            capacity -= items[i].weight;
            maxValue += items[i].value;

            cout << "Take 100% of item (Weight = "
                 << items[i].weight << ")\n";
        }
        else if (capacity > 0) {
            // Take fraction of item
            double fraction = (double)capacity / items[i].weight;

            maxValue += items[i].value * fraction;

            cout << "Take " << fraction * 100
                 << "% of item (Weight = "
                 << items[i].weight << ")\n";

            capacity = 0;
            break;
        }
    }

    cout << "\nMaximum Utility Value = "
         << maxValue << endl;

    return 0;
}

