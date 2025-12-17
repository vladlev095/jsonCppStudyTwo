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
    void getItemName() { //private?
        std::cout << "enter item name:\n";
        std::getline(std::cin, itemName);
    }
    void writeToFile() { //private?
        outputFile.open(filename);
        outputFile << j.dump(4); // a bit different output
        outputFile.close();
    }
    void addItem() {
        std::cout << "let's add a new item.\n";
        getItemName();
        for(auto &it : j) {
            if(it["item"] == itemName) {
                it["count"] = it["count"].get<int>() + 1;
                std::cout << "item already exists. added one more. count = " << it["count"] << "\n";
                return;
            }
        }
        j.push_back({{"item", itemName}, {"count", 1}});
    }
    void changeItemCount() {
        std::cout << "let's specify the amount.\n";
        getItemName();
        for(auto &it : j) {
            if(it["item"] == itemName) {
                std::cout << "how many pieces would you like to buy?\n";
                std::cin >> count;
                it["count"] = count;
                std::cin.clear();
                std::cin.ignore();
                return;
            }
        }
        std::cout << "item not found. would you like to add?\n";
        std::cin.get(yesOrNo);
        if(yesOrNo == 'y') {
            j.push_back({{"item", itemName}, {"count", 1}});
        }
        std::cin.clear();
        std::cin.ignore();
    }
    void removeItem() {
        std::cout << "let's remove an item\n";
        getItemName();
        auto it = j.begin();
        for(int i = 0; i < j.size(); i++) {
            if(it->operator[]("item") == itemName) { //still don't get it
                j.erase(i);
                return;
            }
        }
        std::cout << "item not found\n";
    }
    void editItem() {
        std::cout << "let's edit an item\n";
        getItemName();
    }
    void run() {
        parseFile();
        while(true) {
            std::cout << "choose an action(add/change/remove/edit)\n";
            std::getline(std::cin, choice);
            auto it = choices.find(choice);
            if(it == choices.end()) {
                std::cout << "exit\n"; //any other choice (or a mistake) leads here
                return;
            }
            switch(it->second)
            {
                case(ADD):
                    addItem();
                    writeToFile();
                    break;
                case(CHANGE):
                    changeItemCount();
                    writeToFile();
                    break;
                case(REMOVE):
                    removeItem();
                    writeToFile();
                    break;
                case(EDIT):
                    editItem();
                    // writeToFile();
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
    std::ofstream outputFile;
    ordered_json j;
    std::string choice;
    std::string itemName;
    int count;
    char yesOrNo;
    enum C {ADD, CHANGE, REMOVE, EDIT};
    std::unordered_map<std::string, C> const choices = 
    { {"add", C::ADD}, {"change", C::CHANGE}, {"remove", C::REMOVE}, {"edit", C::EDIT} };
    
    void parseFile() { //constructor?
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

    Shopper shopper;
    shopper.run(); //how to make it static

    // std::ifstream inputF("shopping.json");
    // ordered_json j = ordered_json::parse(inputF);
    // j.push_back({{"item", "avocado"}, {"count", 1}});
    // auto foundEl = j.begin();
    // for(auto it = j.begin(); it != j.end(); it++) {
    //     if(it->operator[]("item") == "avocado") {
    //         std::cout << "item exists\n";
    //         foundEl = it;
    //     }
    // }
    // std::cout << foundEl->dump(4) << "\n";
}