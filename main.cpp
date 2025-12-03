#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <stdio.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

void messagesDates() {
    json j;
    j["messages"] = json::array();
    j["dates"] = json::array();
    time_t timestamp;
    std::tm* dateTimeStruct;
    char dateDelimiter = '-';
    char timeDelimiter = ':';
    std::string dateTime;
    std::stringstream dateTimeFormat;
    std::string message; //empty?
    std::ofstream messagesFile;
    
    while(true) {
        std::cout << "Enter your message: \n";
        std::getline(std::cin, message);
        if(message == "0") { //sigint?
            break;
        }
        time(&timestamp);
        dateTimeStruct = localtime(&timestamp);
        dateTimeFormat << dateTimeStruct->tm_year - 100 << dateDelimiter << dateTimeStruct->tm_mon + 1 << dateDelimiter <<
        dateTimeStruct->tm_mday << " " << dateTimeStruct->tm_hour << timeDelimiter << dateTimeStruct->tm_min <<
        timeDelimiter << dateTimeStruct->tm_sec;
        dateTime = dateTimeFormat.str();
        j["messages"].push_back(message);
        message = "";
        j["dates"].push_back(dateTime);
        dateTime = "";
        dateTimeFormat.str("");
        dateTimeFormat.clear();
        messagesFile.open("messages.json", std::ofstream::out | std::ofstream::trunc);
        messagesFile << j.dump(4) << "\n"; 
        messagesFile.close();
    }
}

void wordCounter() {
    std::ifstream inputFile("wordCounter.json");
    if(!inputFile) { // is empty
        std::cout << "file not opened\n";
        return;
    }
    ordered_json j = ordered_json::parse(inputFile);
    std::ofstream outFile;
    std::string message;
    while(true) {
        std::cout << "Enter your message: \n";
        std::getline(std::cin, message);
        if(message == "0") {
            break;
        }
        if(!j.contains(message)) {
            j[message] = 0;
        }
        j[message] = j[message].get<int>() + 1;
        outFile.open("wordCounter.json", std::ofstream::out | std::ofstream::trunc);
        outFile << j.dump(4) << "\n";
        outFile.close();
    }
}

void shopper() {
    ordered_json j;
    FILE* inputFile = fopen("shopping.json", "r");
    if (inputFile == NULL) { //no need right?
        std::cerr << "Error opening file!\n";
        delete inputFile;
        return;
    }
    j = ordered_json::parse(inputFile);
    std::cout << j.dump(4) << "\n";
}

int main() {
    // messagesDates();
    wordCounter();
    // shopper();
}