#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <iterator>

using namespace std;

void output() {
    ofstream myfile;
    myfile.open ("result.txt");
    myfile << "Writing this to a file.\n";
    myfile.close();
}

int main() {
    string line;
    ifstream myfile ("a_example.txt");
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
    vector<int> books;
    for (int i=0; i < results2.size(); i++) {
        books.push_back(stoi(results2.at(i)));
    }

    
    vector<int> librarySignupTime;
    vector<int> libraryBooksperDay;
    vector<vector<int>> booksInLibrary; 


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
        for (int j=0; j < results4.size(); j++) {
            
            aux.push_back(stoi(results4.at(j)));
        }
        booksInLibrary.push_back(aux);
    }

    




    for(int currentDay=0; currentDay < days; currentDay++) {

    }



    cout << "GG EZ WIN\n";

}