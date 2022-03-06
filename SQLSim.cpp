//  Created by Aditya Dhir on 11/1/21.
//
 
#include <iostream>
#include <vector>
#include <deque>
#include <getopt.h>
#include <string>
#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <math.h>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <fstream>
#include "SQLSim.hpp"
//#include "xcode_redirect.hpp"
 
 
using namespace std;
 
vector<Master> MasterLog;
 
class Logman{
public:
    void get_options(int argc, char** argv);
    void readingLog(char* input);
    void readingCommand();
    uint64_t timestampConversion(const string &ts);
    void funcA(int &num);
    void funcT(const string &ts1, const string &ts2);
    void funcM(const string &ts1);
    void funcC(string &cat);
    void funcP();
    void funcG();
    void funcR();
    void funcD(const int &num);
    void funcL();
    void funcB(const int &num);
    void funcE(const int &num);
    void funcS();
    void funcK(string &in);
//    bool compareIndices(size_t i1, size_t i2);
private:
    string ts, category, message;
    char cmd;
    
    
    char mostRecentChar = '\0';

    vector<size_t> entryIDtoIndex;
    unordered_map<string, vector<size_t>> categoryToIndex;
    
    //for k
    unordered_map<string, vector<size_t>> wordsToindex;
    

    vector<size_t> mostRecent;
    vector<size_t> excerptList;
};
 
// Other comparators useful for future implementations.

//struct justCompareTimeStamps{
//    bool operator()(Master m1, Master m2){
//        return m1.timestamp < m2.timestamp;
//    }
//};
    
//    struct compareMasterToTimeStamp{
//        bool operator()(Master m1, uint64_t ts){
//            return m1.timestamp < ts;
//        }
//    };

//bool compareIndicesDescend(const size_t &i1, const size_t &i2){
//    const Master &ref1 = MasterLog[i1];
//    const Master &ref2 = MasterLog[i2];
//    if(ref1.timestamp == ref2.timestamp){
//        string temp1 = ref1.category;
//        string temp2 = ref2.category;
//        transform(temp1.begin(), temp1.end(), temp1.begin(), ::tolower);
//        transform(temp2.begin(), temp2.end(), temp2.begin(), ::tolower);
//        if(temp1 == temp2){
//            return ref1.entryID < ref2.entryID;
//        }
//        return temp1 < temp2;
//    }
//    return ref1.timestamp < ref2.timestamp;
//}
 
 
bool compareTimeStamps(const Master &m1, const Master &m2){
    if(m1.timestamp == m2.timestamp){
        string temp1 = m1.category;
        string temp2 = m2.category;
        transform(temp1.begin(), temp1.end(), temp1.begin(), ::tolower);
        transform(temp2.begin(), temp2.end(), temp2.begin(), ::tolower);
        if(temp1 == temp2){
            return m1.entryID < m2.entryID;
        }
        return temp1 < temp2;
    }
    return m1.timestamp < m2.timestamp;
}
 

bool compareIndices(size_t i1, size_t i2){
    if(MasterLog[i1].timestamp == MasterLog[i2].timestamp){
        string temp1 = MasterLog[i1].category;
        string temp2 = MasterLog[i2].category;
        transform(temp1.begin(), temp1.end(), temp1.begin(), ::tolower);
        transform(temp2.begin(), temp2.end(), temp2.begin(), ::tolower);
        if(temp1 == temp2){
            return MasterLog[i1].entryID < MasterLog[i2].entryID;
        }
        return temp1 < temp2;
    }
    return MasterLog[i1].timestamp < MasterLog[i2].timestamp;
}
 
 
 
 
 
int main(int argc, char** argv) {
    ios_base::sync_with_stdio(false);
//    xcode_redirect(argc, argv);
//    string my_variable;
//    string my_result;
//    cin >> my_variable;   // Reads from file instead of keyboard when redirected
//    cout << my_result;   // Prints to file instead of screen when redirected
 
    Logman log;
//    if(argc < 2) {
//        cerr << "not enough input" << endl;
//    }
    
    log.get_options(argc, argv);
    log.readingLog(argv[1]);
    // STOP UNDOING
    log.readingCommand();
    
    return 0;
    
}
 
 
 
void Logman::get_options(int argc, char** argv) {
    int option_index = 0, option = 0;
    opterr = false;
    struct option longOpts[] = {{ "help", no_argument, nullptr, 'h'}};
    while ((option = getopt_long(argc, argv, "h",
            longOpts, &option_index)) != -1) {
        switch (option) {
                //        { "LOGFILE", required_argument, nullptr, 'l'},
//            case 'l':
//                filename = optarg;
//                break;
            case 'h':
                cout << "here to help!";
                exit(0);
        }
    }
}
 
 
 
 
uint64_t Logman::timestampConversion(const string &ts){
    uint64_t result = 0;
    result += static_cast<uint64_t>(ts[0] - '0') * 1000000000ull;
    result += static_cast<uint64_t>(ts[1] - '0') * 100000000ull;
    result += static_cast<uint64_t>(ts[3] - '0') * 10000000ull;
    result += static_cast<uint64_t>(ts[4] - '0') * 1000000ull;
    result += static_cast<uint64_t>(ts[6] - '0') * 100000ull;
    result += static_cast<uint64_t>(ts[7] - '0') * 10000ull;
    result += static_cast<uint64_t>(ts[9] - '0') * 1000ull;
    result += static_cast<uint64_t>(ts[10] - '0') * 100ull;
    result += static_cast<uint64_t>(ts[12] - '0') * 10ull;
    result += static_cast<uint64_t>(ts[13] - '0') * 1ull;
    return result;
}
 

// Reads in input
void Logman::readingLog(char* input){
    ifstream input_stream;
    input_stream.open(input);
 
    // Timestamp is mm:dd:hh:mm:ss
    size_t counter = 0;
    // USE GETLINE
    while (getline(input_stream,ts,'|')){
       
        getline(input_stream,category,'|');
        getline(input_stream,message);
        
        
        Master entry;
        entry.timestamp = timestampConversion(ts);
        
        entry.ts = ts;
        entry.category = category;
        entry.message = message;
        entry.entryID = counter;
        MasterLog.push_back(entry);
      
        counter++;
        
    }
    input_stream.close();
    
    entryIDtoIndex.resize(MasterLog.size());
    sort(MasterLog.begin(), MasterLog.end(), compareTimeStamps);
    for(size_t i = 0; i < MasterLog.size(); i++){
 
//        entryidToIndex[MasterLog[i].entryID] = i;
        entryIDtoIndex[MasterLog[i].entryID] = i;
        string cat = MasterLog[i].category;
        transform(cat.begin(), cat.end(), cat.begin(), ::tolower);
        size_t k = 0;
        string categoryWord;
        while(k < cat.size()) {
            if(isalnum(cat[k])){
                categoryWord += cat[k];
            }
             if(((k + 1) == cat.size() || !isalnum(cat[k])) && !categoryWord.empty()){
//                 transform(in.begin(), in.end(), in.begin(), ::tolower);
                wordsToindex[categoryWord].push_back(i);
                 categoryWord = "";
             }
            k++;
        }
 
        categoryToIndex[cat].push_back(i);
        
        
        string keyword = "";
        size_t j = 0;
        string in = MasterLog[i].message;
        transform(in.begin(), in.end(), in.begin(), ::tolower);
        
        while(j < in.size()) {
            if(isalnum(in[j])){
                keyword += in[j];
            }
             if(((j + 1) == in.size()) || !isalnum(in[j])){
//                 transform(in.begin(), in.end(), in.begin(), ::tolower);
 
                 auto point = wordsToindex.find(keyword);
                 if((point != wordsToindex.end() &&
                    i != point->second[point->second.size()-1])){
                    point->second.push_back(i);
                 }
                 else if(point == wordsToindex.end()){
//                     wordsToindex.insert(keyword, {i});
                     wordsToindex[keyword].push_back(i);
                 }
                keyword = "";
            }
            j++;
        }
 
    }
    cout << counter << " entries read" << endl;
}
 
void Logman::readingCommand(){
    

   
    // USE DO WHILE LOOP for all commands
    do {
        cout << "% ";
        cin >> cmd;
//        cout << cmd;
        //process command
        switch(cmd){
            case '#': {
                string gone;
                getline(cin, gone);
                break;
            }
            case 'a': {
                int num;
                cin >> num;
                funcA(num);
                break;
            }
            case 't': {
                cin.ignore();
                string ts1;
                string ts2;
                getline(cin, ts1, '|');
                getline(cin, ts2);
                // .length()?
                if(ts1.size() != 14 || ts2.size() != 14) {
                    cerr << "Messed up timestamps length" << endl;
                }
                else{
                    funcT(ts1, ts2);
                }
                break;
            }
            case 'm': {
                cin.ignore();
                string ts;
                getline(cin, ts);
 
                if(ts.length() != 14) {
                    cerr << "Messed up timestamps length" << endl;
                }
                else {
                    funcM(ts);
                }
                break;
            }
            case 'c': {
                cin.ignore();
                string cat;
                getline(cin, cat);
              
                funcC(cat);
                break;
            }
            case 'p': {
                funcP();
                break;
            }
            case 'r': {
                funcR();
                break;
            }
            case 's': {
                funcS();
                break;
            }
            case 'd': {
                int number;
                cin >> number;
                funcD(number);
                break;
            }
            case 'g': {
                funcG();
                break;
            }
            case 'e':{
                int numberE;
                cin >> numberE;
                funcE(numberE);
                break;
            }
            case 'b':{
                int numberB;
                cin >> numberB;
                funcB(numberB);
                break;
            }
            case 'l':{
                funcL();
                break;
            }
            case 'k':{
                cin.ignore();
                string input;
                getline(cin, input);
                funcK(input);
                break;
            }
            case 'q': {
                return;
            }
                
            default: {
                cerr << "invalid command" << endl;
                string NO;
                getline(cin, NO);
            }
        }
    }
    while (cmd != 'q');
}
 

// Appends given number of entry from masterlog into excerptList
void Logman::funcA(int &num){
//    size_t numIn = static_cast<size_t>(num);
 
    size_t numIn = size_t(num);
    if(num < 0 || numIn > MasterLog.size()-1){
        cerr << "out of bounds index for append" << endl;
    }
 
    else{
        excerptList.push_back(entryIDtoIndex[numIn]);
        cout << "log entry " << numIn << " appended" << endl;
    }
}
 

// Stores all entries within the two entry timestamps
void Logman::funcT(const string &ts1, const string &ts2){
//    if(ts1.size() != 14 || ts2.size() != 14){
//        cerr << "invalid timestamp length" << endl;
//        return;
//    }
    mostRecent.clear();
    uint64_t lower = timestampConversion(ts1);
    uint64_t upper = timestampConversion(ts2);
    
    Master dummyUpper;
    Master dummyLower;
    
    dummyLower.timestamp = lower;
    dummyUpper.timestamp = upper;
    
    dummyLower.category = static_cast<char>(31);
    dummyUpper.category = static_cast<char>(127);
    
    auto low = lower_bound(MasterLog.begin(), MasterLog.end(), dummyLower, compareTimeStamps);
    auto up = upper_bound(low, MasterLog.end(), dummyUpper, compareTimeStamps);
    
    mostRecent.push_back(static_cast<size_t>(low - MasterLog.begin()));
    mostRecent.push_back(static_cast<size_t>(up - MasterLog.begin()));
 
    mostRecentChar = 't';
    cout << "Timestamps search: " << (up - low)
    << " entries found" << endl;
    
}

 
// Stores all entries with matching timestamps
void Logman::funcM(const string &ts1){
    uint64_t actualTime = timestampConversion(ts1);
    
    Master dummyLower;
 
    
    mostRecent.clear();
    
    
    dummyLower.timestamp = actualTime;
    dummyLower.category = static_cast<char>(31);
    
    auto low = lower_bound(MasterLog.begin(), MasterLog.end(), dummyLower, compareTimeStamps);
    
    size_t x = static_cast<size_t>(low - MasterLog.begin());
    while(MasterLog[x].timestamp == actualTime) {
        

        mostRecent.push_back(x);
        x++;
        
    }

    
    
    mostRecentChar = 'm';
 
    cout << "Timestamp search: " << mostRecent.size()
    << " entries found" << endl;
}

 
// Searches based on matching category in entries and stores matching entries
void Logman::funcC(string &cat){
//    foundCategory = categoryToIndex.find(cat);
    
    mostRecentChar = 'c';
    mostRecent.clear();
    
    transform(cat.begin(), cat.end(), cat.begin(), ::tolower);
    
    auto point = categoryToIndex.find(cat);
    if(point != categoryToIndex.end()){
        for(size_t i = 0; i < point->second.size(); i++){
            mostRecent.push_back(point->second[i]);
        }
        cout << "Category search: " << point->second.size() << " entries found" << endl;
    }
    else {
        cout << "Category search: 0 entries found" << endl;
    }
//    if(categoryToIndex[cat].size() > 0){
//    }
    //categoryToIndex.find(cat)->second.size()
    
}
 
// Prints out all entries in the masterlog
void Logman::funcP(){

    for(size_t i = 0; i < excerptList.size(); i++){
        cout << i << '|' << MasterLog[excerptList[i]].entryID << '|'
        << MasterLog[excerptList[i]].ts << '|' <<
        MasterLog[excerptList[i]].category << '|' <<
        MasterLog[excerptList[i]].message << endl;
    }

}
 
// Prints out the most recent function calls' entries
void Logman::funcG(){
    
    if(mostRecent.size() == 0){
        return;
    }
    else if(mostRecentChar == 't'){
        for(size_t i = mostRecent[0]; i < mostRecent[1]; i++){
            cout << MasterLog[i].entryID << '|'
            << MasterLog[i].ts << '|' <<
            MasterLog[i].category << '|' <<
            MasterLog[i].message << endl;
        }
    }
    else if(mostRecentChar == 'm'){
       
        for(size_t i = 0; i < mostRecent.size(); i++){
            cout << MasterLog[mostRecent[i]].entryID << '|' <<
            MasterLog[mostRecent[i]].ts << '|' << MasterLog[mostRecent[i]].category << '|'
            << MasterLog[mostRecent[i]].message << endl;
        }

    }
    else if(mostRecentChar == 'c'){
        for(size_t i = 0; i < mostRecent.size(); i++){
            cout << MasterLog[mostRecent[i]].entryID << '|' <<
            MasterLog[mostRecent[i]].ts << '|' << MasterLog[mostRecent[i]].category << '|'
            << MasterLog[mostRecent[i]].message << endl;
        }
    }
    else if(mostRecentChar == 'k'){
        for(size_t i = 0; i < mostRecent.size(); i++){
            cout << MasterLog[mostRecent[i]].entryID << '|' <<
            MasterLog[mostRecent[i]].ts << '|' << MasterLog[mostRecent[i]].category << '|'
            << MasterLog[mostRecent[i]].message << endl;
        }
    }
    
}


 // Adds the most recent function utilized to the excerpt list
void Logman::funcR(){
    if(mostRecentChar == '\0'){
//        cout << 0 << " log entries appended" << endl;
        return;
    }
    
    if(mostRecentChar == 'm'){
//        for(size_t i = mostRecent[0]; i <= mostRecent[mostRecent.size()]; i++)
        for(size_t i = 0; i < mostRecent.size(); i++){
            excerptList.push_back(mostRecent[i]);
        }
    }
    else if(mostRecentChar == 't'){
        size_t old_size = excerptList.size();
        for(size_t i = mostRecent[0]; i < mostRecent[1]; i++){
            excerptList.push_back(i);
        }
        cout << excerptList.size() - old_size << " log entries appended" << endl;
        return;
    }
    else if(mostRecentChar == 'c'){
        for(size_t i = 0; i < mostRecent.size(); i++){
            excerptList.push_back(mostRecent[i]);
        }
    }
    else if(mostRecentChar == 'k'){
        for(size_t i = 0; i < mostRecent.size(); i++){
            excerptList.push_back(mostRecent[i]);
            
        }
    }
//    sort(mostRecent.begin(), mostRecent.end(), compareIndices);
    
    cout << mostRecent.size() << " log entries appended" << endl;
}

// Deletes the number of the entry that is input
void Logman::funcD(const int &num){
    size_t numIn = size_t(num);
//    cout << excerptList.size() << endl;
    if(num < 0 || numIn > excerptList.size()-1){
        cerr << "Invalid index for D " << numIn << endl;
        return;
    }
    if(excerptList.empty()){
        return;
    }
    excerptList.erase(excerptList.begin() + num);
    cout << "Deleted excerpt list entry " << num << endl;
}

 // Shifts entry within database to the beginning
void Logman::funcB(const int &num){
    size_t numIn = size_t(num);
    if(num < 0 || numIn > excerptList.size()-1){
        cerr << "Invalid index for B" << endl;
        return;
    }
    if(excerptList.empty()){
        return;
    }
    excerptList.insert(excerptList.begin(), excerptList[numIn]);
    
    excerptList.erase(excerptList.begin() + num + 1);
    cout << "Moved excerpt list entry " << num << endl;
}

 // Shifts entry within database to the end
void Logman::funcE(const int &num){
    size_t numIn = size_t(num);
    if(num < 0 || numIn > excerptList.size()-1){
        cerr << "Invalid index for E" << endl;
        return;
    }
    if(excerptList.empty()){
        return;
    }
    excerptList.push_back(excerptList[numIn]);
    excerptList.erase(excerptList.begin() + num);
    cout << "Moved excerpt list entry " << num << endl;
}


 // Prints out contents in the database and clears it
void Logman::funcL(){
    if(excerptList.size() == 0){
        cout << "excerpt list cleared" << endl;
        cout << "(previously empty)" << endl;
    }
    else {
        cout << "excerpt list cleared" << endl;
        cout << "previous contents:" << endl;
        cout << 0 << '|' << MasterLog[excerptList.front()].entryID << '|'
        << MasterLog[excerptList.front()].ts << '|' <<
        MasterLog[excerptList.front()].category << '|' <<
        MasterLog[excerptList.front()].message << endl;
        cout <<"..." << endl;
        cout << excerptList.size()-1 << '|' << MasterLog[excerptList.back()].entryID << '|'
        << MasterLog[excerptList.back()].ts << '|' <<
        MasterLog[excerptList.back()].category << '|' <<
        MasterLog[excerptList.back()].message << endl;
        
        excerptList.clear();
    }
}

 // Sorting implementation
void Logman::funcS(){
    if(excerptList.empty()){
        cout << "excerpt list sorted" << endl;
        cout << "(previously empty)" << endl;
    }
    else {
        cout << "excerpt list sorted" << endl;
        cout << "previous ordering:" << endl;
        cout << 0 << '|' << MasterLog[excerptList.front()].entryID << '|'
        << MasterLog[excerptList.front()].ts << '|' <<
        MasterLog[excerptList.front()].category << '|' <<
        MasterLog[excerptList.front()].message << endl;
        cout <<"..." << endl;
        cout << excerptList.size()-1 << '|' << MasterLog[excerptList.back()].entryID << '|'
        << MasterLog[excerptList.back()].ts << '|' <<
        MasterLog[excerptList.back()].category << '|' <<
        MasterLog[excerptList.back()].message << endl;
        
//        sort(excerptList.begin(), excerptList.end(), compareIndices);
        sort(excerptList.begin(), excerptList.end());
        cout << "new ordering:" << endl;
        cout << 0 << '|' << MasterLog[excerptList.front()].entryID << '|'
        << MasterLog[excerptList.front()].ts << '|' <<
        MasterLog[excerptList.front()].category << '|' <<
        MasterLog[excerptList.front()].message << endl;
        cout <<"..." << endl;
        cout << excerptList.size()-1 << '|' << MasterLog[excerptList.back()].entryID << '|'
        << MasterLog[excerptList.back()].ts << '|' <<
        MasterLog[excerptList.back()].category << '|' <<
        MasterLog[excerptList.back()].message << endl;
    }
}


 // Keyword search implementation
void Logman::funcK(string &in){
    vector<string> keywords;
    string word = "";
    size_t i = 0;
    mostRecent.clear();
    mostRecentChar = 'k';
    
    // set current input to lowercase to avoid bypass capitalization differeces
    transform(in.begin(), in.end(), in.begin(), ::tolower);
    
 // Store matching logs with keywords
    while(i < in.size()) {
        if(isalnum(in[i])){
            word += in[i];
        }
         if((i + 1 == in.size() || !isalnum(in[i])) && !word.empty()){
 
            keywords.push_back(word);
            word = "";
        }
        i++;
    }

    if(keywords.size() == 1){
        auto it = wordsToindex.find(keywords[0]);
        if(it != wordsToindex.end()){
//            mostRecent.push_back(it->second);
            for(size_t i = 0; i < it->second.size(); i++){
                mostRecent.push_back(it->second[i]);
            }
        }
    }
    else {
        
        size_t a = 1;
        for(size_t i = 0; i < keywords.size(); i++){
            if(wordsToindex.find(keywords[i]) == wordsToindex.end()){
                cout << "Keyword search: 0 entries found" << endl;
                return;
            }
        }
        vector<size_t> total = wordsToindex.find(keywords[0])->second;
        while(a < keywords.size()){
            
            vector<size_t> temp;
            // use set intersection to see if keyword is seen earlier
            set_intersection(total.begin(), total.end(),
                    wordsToindex.find(keywords[a])->second.begin(),
                    wordsToindex.find(keywords[a])->second.end(), back_inserter(temp), compareIndices);
 
            
//            temp.resize(size_t(it - temp.begin()));
            swap(temp,total);
            a++;
        }
        for(size_t i = 0; i < total.size(); i++){
            mostRecent.push_back(total[i]);
        }
    }
    // return number of matching entries to keyword
    cout << "Keyword search: " << mostRecent.size() << " entries found" << endl;
}
