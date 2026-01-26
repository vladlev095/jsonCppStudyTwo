#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <stdio.h>
#include <cstdlib>
#include <random>
#include <unordered_map>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

class Stopwatch
{
private:
    std::chrono::steady_clock::time_point start;
public:
    Stopwatch() : start(std::chrono::steady_clock::now()) {}
    ~Stopwatch() {
        auto end = std::chrono::steady_clock::now();
        auto secs = std::chrono::duration_cast<std::chrono::seconds>(end - start);
        std::cout << "elapsed: " << secs.count() << " seconds\n";
    }
};

class Timer
{
public:
    void setTimer(int seconds) {
        endTime = Clock::now();
        isRunning = true;
    }
    double elapsedMillSec() {
        if(!isRunning) {
            return 0.0;
        }
        auto endTime = Clock::now();
    }
    void stop() {
        isRunning = false;
    }
private:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;
    TimePoint endTime;
    bool isRunning = false;
};

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
    std::string message;
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

class Test {
public:
    const int x = 2;
    int y = 3;
    void print() {
        std::cout << "x = " << x << " y = " << y << "\n";
    }
};

void parseFile(std::string filename, ordered_json& j) { //ref to a class obj?
    std::ifstream inputFile(filename);
    if(!inputFile) {
        std::cout << "file not opened\n";
        return;
    }
    if(inputFile.peek() != std::ifstream::traits_type::eof()) {
        j = ordered_json::parse(inputFile);
    }
}

void writeToFile(std::string filename, ordered_json& j) {
    std::cout << "here\n";
    std::ofstream outputFile(filename);
    outputFile << j.dump(4);
}

class Shopper {
public:
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
        std::cout << j.dump(4) << "\n";
    }
    void changeItemCount() {
        std::cout << "let's specify the amount.\n";
        getItemName();
        for(auto &it : j) {
            if(it["item"] == itemName) {
                std::cout << "how many pieces would you like to buy?\nenter a number: ";
                std::cin >> count; //what if a char or smth else
                it["count"] = count;
                std::cin.clear(); // useless?
                std::cin.ignore();
                return;
            }
        }
        addNotFound();
    }
    void removeItem() {
        std::cout << "let's remove an item.\n";
        getItemName();
        for(auto it = j.begin(); it != j.end(); ++it) {
            if(it->operator[]("item") == itemName) {
                j.erase(it);
                return;
            }
        }
        std::cout << "item not found.\n";
    }
    void editItem() {
        std::cout << "let's edit an item.\n";
        getItemName();
        for(auto &it : j) {
            if(it["item"] == itemName) {
                std::cout << "enter new name: ";
                std::getline(std::cin, itemName);
                it["item"] = itemName;
                std::cout << "enter the amount: ";
                std::cin >> count;
                it["count"] = count;
                std::cin.clear();
                std::cin.ignore();
                return;
            }
        }
        addNotFound();
    }
    void print() {
        parseFile(filename, j);
        std::cout << j.dump(0) << "\n";
    }
    void run() {
        parseFile(filename, j);
        while(true) {
            std::cout << "choose an action(add/change/remove/edit): ";
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
            writeToFile(filename, j);
        }
    }
    
private:
    const std::string filename = "shopping.json";
    ordered_json j;
    std::string choice;
    std::string itemName;
    int count;
    char yesOrNo;
    enum C {ADD, CHANGE, REMOVE, EDIT};
    std::unordered_map<std::string, C> const choices = 
    { {"add", C::ADD}, {"change", C::CHANGE}, {"remove", C::REMOVE}, {"edit", C::EDIT} };

    void getItemName() {
        std::cout << "enter item name: ";
        std::getline(std::cin, itemName);
    }
    void addNotFound() {
        std::cout << "item not found.\nwould you like to add? (y/n): ";
        std::cin.get(yesOrNo);
        if(yesOrNo == 'y') {
            j.push_back({{"item", itemName}, {"count", 1}});
        }
        std::cin.clear();
        std::cin.ignore();
    }
};

//Каждые N секунд программа опрашивает пользователя (или генерирует случайно) 
//температуру и сохраняет её в JSON-массив, учитывая дату и время замера.
//[ {"temperature": 41.2, "date": 12/30/2025, "time": 12:40}, ... ]
void temperatureSensor() {
    ordered_json j;
    srand (static_cast <unsigned> (time(0)));
    float temperature;
    int i = 0;
    while(i != 1000) {
        temperature = 40 + static_cast <float> (rand()) / ( static_cast <float> (RAND_MAX/(42-40)));
        printf("%.1f\n", temperature);
        ++i;
    }
}

void setStopwatch() {

}

int main() {
    // messagesDates();
    // wordCounter();

    // Stopwatch Stopwatch;
    // Shopper shopper;
    // shopper.run();

    // temperatureSensor();

    ordered_json j = ordered_json::array();
    float a = 41.2;
    float b = 42.0;
    std::string date = "12/30/2025";
    std::string date2 = "12/30/2025";
    std::string time = "12:40";
    std::string time2 = "12:45";
    j[0] = {{"temperature", a}, {"date", date}, {"time", time}};
    j[1] = {{"temperature", b}, {"date", date2}, {"time", time2}};
    std::cout << j.dump(4) << "\n";
}