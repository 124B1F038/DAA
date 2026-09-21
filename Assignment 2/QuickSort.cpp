#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>

using namespace std;

struct Movie {
    int id;
    string title;
    float imdbRating;
    int releaseYear;
    long watchTimePopularity; 
};

enum SortParameter {
    IMDB_RATING,
    RELEASE_YEAR,
    WATCH_TIME_POPULARITY
};

bool compareMovies(const Movie& a, const Movie& b, SortParameter param, bool descending = true) {
    bool result = false;
    switch (param) {
        case IMDB_RATING:
            result = (a.imdbRating == b.imdbRating) ? (a.id < b.id) : (a.imdbRating < b.imdbRating);
            break;
        case RELEASE_YEAR:
            result = (a.releaseYear == b.releaseYear) ? (a.id < b.id) : (a.releaseYear < b.releaseYear);
            break;
        case WATCH_TIME_POPULARITY:
            result = (a.watchTimePopularity == b.watchTimePopularity) ? (a.id < b.id) : (a.watchTimePopularity < b.watchTimePopularity);
            break;
    }
    return descending ? !result : result;
}

void insertionSort(vector<Movie>& movies, int low, int high, SortParameter param, bool descending) {
    for (int i = low + 1; i <= high; ++i) {
        Movie key = movies[i];
        int j = i - 1;
        while (j >= low && compareMovies(key, movies[j], param, descending)) {
            movies[j + 1] = movies[j];
            j--;
        }
        movies[j + 1] = key;
    }
}

int medianOfThree(vector<Movie>& movies, int low, int high, SortParameter param, bool descending) {
    int mid = low + (high - low) / 2;
    if (compareMovies(movies[mid], movies[low], param, descending))
        swap(movies[low], movies[mid]);
    if (compareMovies(movies[high], movies[low], param, descending))
        swap(movies[low], movies[high]);
    if (compareMovies(movies[high], movies[mid], param, descending))
        swap(movies[mid], movies[high]);
    return mid;
}

int partition(vector<Movie>& movies, int low, int high, SortParameter param, bool descending) {
    int pivotIdx = medianOfThree(movies, low, high, param, descending);
    swap(movies[pivotIdx], movies[high]);
    
    Movie pivot = movies[high];
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (compareMovies(movies[j], pivot, param, descending)) {
            i++;
            swap(movies[i], movies[j]);
        }
    }
    swap(movies[i + 1], movies[high]);
    return i + 1;
}

void quickSort(vector<Movie>& movies, int low, int high, SortParameter param, bool descending) {
    while (low < high) {
        if (high - low < 16) {
            insertionSort(movies, low, high, param, descending);
            break;
        }

        int pivotIndex = partition(movies, low, high, param, descending);

        if (pivotIndex - low < high - pivotIndex) {
            quickSort(movies, low, pivotIndex - 1, param, descending);
            low = pivotIndex + 1;
        } else {
            quickSort(movies, pivotIndex + 1, high, param, descending);
            high = pivotIndex - 1;
        }
    }
}

vector<Movie> loadMoviesFromCSV(const string& filename) {
    vector<Movie> movies;
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Error: Could not open CSV file " << filename << endl;
        return movies;
    }

    string line;
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string item;
        Movie m;

        try {
            getline(ss, item, ','); m.id = stoi(item);
            getline(ss, m.title, ',');
            getline(ss, item, ','); m.imdbRating = stof(item);
            getline(ss, item, ','); m.releaseYear = stoi(item);
            getline(ss, item, ','); m.watchTimePopularity = stol(item);

            movies.push_back(m);
        } catch (...) {
            continue;
        }
    }

    file.close();
    return movies;
}

void printTopMovies(const vector<Movie>& movies, int topN = 10) {
    cout << "\n----------------------------------------------------------------------------------\n";
    cout << "ID\t| Rating\t| Year\t| Watch Time (mins)\t| Title\n";
    cout << "----------------------------------------------------------------------------------\n";
    int limit = min((int)movies.size(), topN);
    for (int i = 0; i < limit; ++i) {
        cout << movies[i].id << "\t| "
             << movies[i].imdbRating << "\t\t| "
             << movies[i].releaseYear << "\t| "
             << movies[i].watchTimePopularity << "\t\t| "
             << movies[i].title << "\n";
    }
    cout << "----------------------------------------------------------------------------------\n";
}

int main() {
    string csvPath = "movies.csv";

    vector<Movie> movies = loadMoviesFromCSV(csvPath);

    if (movies.empty()) {
        cout << "No dataset found. Generating mock dataset for demonstration...\n";
        movies = {
            {101, "Inception", 8.8f, 2010, 150000000},
            {102, "The Dark Knight", 9.0f, 2008, 220000000},
            {103, "Interstellar", 8.6f, 2014, 180000000},
            {104, "Parasite", 8.5f, 2019, 95000000},
            {105, "Pulp Fiction", 8.9f, 1994, 130000000},
            {106, "Avengers: Endgame", 8.4f, 2019, 250000000}
        };
    }

    cout << "Loaded " << movies.size() << " movies successfully.\n";

    auto start = chrono::high_resolution_clock::now();
    quickSort(movies, 0, movies.size() - 1, IMDB_RATING, true);
    auto end = chrono::high_resolution_clock::now();
    
    chrono::duration<double, milli> duration = end - start;
    cout << "\n[Top Movies by IMDB Rating] (Sorted in " << duration.count() << " ms)";
    printTopMovies(movies, 5);

    quickSort(movies, 0, movies.size() - 1, RELEASE_YEAR, true);
    cout << "\n[Top Movies by Release Year]";
    printTopMovies(movies, 5);
  
    quickSort(movies, 0, movies.size() - 1, WATCH_TIME_POPULARITY, true);
    cout << "\n[Top Movies by Watch Time Popularity]";
    printTopMovies(movies, 5);

    return 0;
}
