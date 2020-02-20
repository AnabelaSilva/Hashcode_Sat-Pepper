#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <unordered_set>

using namespace std;

vector<int> books;
vector<int> libscore;

void output() {
    ofstream myfile;
    myfile.open ("result.txt");
    myfile << "Writing this to a file.\n";
    myfile.close();
}

// Compares two intervals according to staring times.
bool compareBooks(int i1, int i2)
{
    return books[i1] > books[i2];
}

bool compareLib(int i1, int i2)
{
    return libscore[i1] > libscore[i2];
}

int main(int argc, char* argv[]) {
    string line;
    string filename = argv[1];
    ifstream myfile (filename + ".txt");
    vector<string> inputVec;
    if (myfile.is_open())
    {
    while ( getline (myfile,line) )
    {
        inputVec.push_back(line);
    }
    myfile.close();
    }
    istringstream iss(inputVec[0]);
    std::vector<std::string> results(istream_iterator<string>{iss},
                                 istream_iterator<string>());
    string daysStr = results.at(2);
    int days = stoi(daysStr);
    istringstream iss2(inputVec[1]);
    std::vector<std::string> results2(istream_iterator<string>{iss2},
                                 istream_iterator<string>());

    for (int i=0; i < results2.size(); i++) {
        books.push_back(stoi(results2.at(i)));
    }


    vector<int> librarySignupTime;
    vector<int> libraryBooksperDay;

    vector<vector<int>> booksInLibrary;
    vector<unordered_set<int>> booksInLibSet;

    for (int i = 2; i < inputVec.size(); i+=2) {
        istringstream iss3(inputVec[i]);
        std::vector<std::string> results3(istream_iterator<string>{iss3},
                                 istream_iterator<string>());
        librarySignupTime.push_back(stoi(results3.at(1)));
        libraryBooksperDay.push_back(stoi(results3.at(2)));

        istringstream iss4(inputVec[i+1]);
        std::vector<std::string> results4(istream_iterator<string>{iss4},
                                 istream_iterator<string>());
        vector<int> aux;
        unordered_set<int> booksSet;
        for (int j=0; j < results4.size(); j++) {
            aux.push_back(stoi(results4.at(j)));
            booksSet.insert(stoi(results4.at(j)));
        }
        sort(aux.begin(), aux.end(), compareBooks);

        booksInLibrary.push_back(aux);
        booksInLibSet.push_back(booksSet);
    }
    vector<int> libBookScores;
    vector<int> sol_lib;
    for (int i = 0; i < librarySignupTime.size(); i++) {
        int value = 0;
        for (int j = 0; j < booksInLibrary.at(i).size(); j++)
        {
            value += books.at(booksInLibrary.at(i).at(j));
        }
        // value = value / booksInLibrary.at(i).size();
        // value = value * (days-librarySignupTime.size());
        libscore.push_back(value);
        // sol_lib.push_back(i);
    }

    // sort(sol_lib.begin(), sol_lib.end(), compareLib);

    vector<vector<int>> outputBooksInLibraries(booksInLibrary.size(), vector<int>());
    vector<int> currentBookInLibraries(libraryBooksperDay.size(), 0);
    unordered_set<int> librariesNotSignedUp;
    for (int l = 0; l < libraryBooksperDay.size(); l++) {
        librariesNotSignedUp.insert(l);
    }

    int currentDay = 0;
    while (currentDay < days && librariesNotSignedUp.size() > 0) {
        int daysRemaining = days - currentDay;
        int max_lib_score = 0;
        int best_lib_index = -1;
        for (int l = 0; l < libraryBooksperDay.size(); l++) {
            if (librariesNotSignedUp.count(l)) {
                int val = libscore.at(l) * (daysRemaining - librarySignupTime.at(l));
                if (val > max_lib_score) {
                    max_lib_score = val;
                    best_lib_index = l;
                }
            }
        }

        if (best_lib_index == -1) {
            cout << "Interrupted simulation, possibly not optimal\n";
            break;
        }

        // Scan books from signed up libraries, while that one signs up
        int daysInSignup = min(librarySignupTime.at(best_lib_index), daysRemaining);
        for (int l = 0; l < sol_lib.size(); l++) {
            int booksDuringSignup = libraryBooksperDay.at(l) * daysInSignup;
            int booksRemaining = booksInLibrary.at(l).size() - outputBooksInLibraries.at(l).size();
            while (booksDuringSignup-- && booksRemaining > 0) {
                int currBook = booksInLibrary.at(l).at(currentBookInLibraries.at(l));
                if (booksInLibSet.at(l).count(currBook)) { // if exists
                    outputBooksInLibraries.at(l).push_back(currBook);
                    currentBookInLibraries.at(l)++;
                    booksRemaining--;
                    // Decrement potential scores of other libraries
                    for (int lib = 0; lib < booksInLibrary.size(); lib++) {
                        if (booksInLibSet.at(lib).count(currBook)) { // if exists
                            libscore.at(lib) -= books.at(currBook);
                        }
                        booksInLibSet.at(lib).erase(currBook);
                    }
                } else {
                    booksDuringSignup++;
                }
            }
        }

        // Sign up best lib
        sol_lib.push_back(best_lib_index);
        librariesNotSignedUp.erase(best_lib_index);

        currentDay += daysInSignup;
    }

    ofstream output_file;
    output_file.open ("out_" + filename + ".txt");
    output_file << sol_lib.size() << "\n";
    for(int i = 0; i < sol_lib.size(); i++){
        if (outputBooksInLibraries.at(sol_lib.at(i)).size() == 0) {
            output_file << sol_lib.at(i) <<" "<< booksInLibrary.at(sol_lib.at(i)).size()<< "\n";
            for(int j=0; j< booksInLibrary.at(sol_lib.at(i)).size(); j++){
                output_file << booksInLibrary.at(sol_lib.at(i)).at(j) << " ";
            }
        } else {
            output_file << sol_lib.at(i) <<" "<< outputBooksInLibraries.at(sol_lib.at(i)).size()<< "\n";
            for(int j=0; j< outputBooksInLibraries.at(sol_lib.at(i)).size(); j++){
                output_file << outputBooksInLibraries.at(sol_lib.at(i)).at(j) << " ";
            }
        }
        
        output_file << "\n";
    }
    output_file.close();

    cout << "GG EZ WIN\n";

}
