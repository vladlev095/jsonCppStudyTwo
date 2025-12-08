#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <stdio.h>
#include <unordered_map>

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
        if(message == "0") {
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
    ordered_json j;
    std::ifstream inputFile("wordCounter.json");
    std::ofstream outFile;
    std::string message;

    if(!inputFile) {
        std::cout << "file not opened\n";
        return;
    }
    if(inputFile.peek() != std::ifstream::traits_type::eof()) {
        j = ordered_json::parse(inputFile); //parses if not empty
        std::cout << j.dump(4) << "\n";
    }

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

// Пользователь может добавлять товар, указывать количество товара, удалять или изменять запись.
class Shopper {
    public:
    void addItem() {
        std::cout << "addItem\n";
    }
    void changeItemCount() {
        std::cout << "changeItemCount\n";
    }
    void removeItem() {
        std::cout << "removeItem\n";
    }
    void editItem() {
        std::cout << "editItem\n";
    }
    void run() {
        parseFile();
        while(true) {
            std::cout << "choose an action(add/change/remove/edit)\n";
            std::getline(std::cin, choice); // how to check for possible buffer mistakes
            auto it = choices.find(choice);
            if(it == choices.end()) {
                std::cout << "exit\n"; //any other choice (or a mistake) leads here
                return;
            }
            switch(it->second)
            {
                case(ADD):
                    addItem();
                    break;
                case(CHANGE):
                    changeItemCount();
                    break;
                case(REMOVE):
                    removeItem();
                    break;
                case(EDIT):
                    editItem();
                    break;
                default:
                    std::cout << "default case\n"; //probably unreachable
                    return;
            }
        }
    }
    
    private:
    const std::string filename = "shopping.json"; // looks ok...
    std::ifstream inputFile; // cannot init here
    ordered_json j;
    std::string choice;
    enum C {ADD, CHANGE, REMOVE, EDIT};
    std::unordered_map<std::string, C> const choices = 
    { {"add", C::ADD}, {"change", C::CHANGE}, {"remove", C::REMOVE}, {"edit", C::EDIT} };
    
    void parseFile() {
        inputFile.open(filename);
        if(!inputFile) {
            std::cout << "file not opened\n";
            return;
        }
        if(inputFile.peek() != std::ifstream::traits_type::eof()) {
            j = ordered_json::parse(inputFile);
        }
    }
};

int main() {
    // messagesDates();
    // wordCounter();
    // Shopper shopper;
    // shopper.run(); //need static
    std::ifstream inputF("shopping.json");
    ordered_json j = ordered_json::parse(inputF);
    j[2]["item"] = "avocado";
    j[2]["count"] = 1;
    for(auto it : j) {
        if(it["item"] == "avocado") {
            std::cout << "item exists\n";
        }
    }
}