#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class BitcoinExchange {
private:
    std::map<std::string, float> _database;
    void displayHeader() const;

public:
    BitcoinExchange();
    BitcoinExchange(const BitcoinExchange& other);
    ~BitcoinExchange();
    BitcoinExchange& operator=(const BitcoinExchange& other);

    void loadDatabase(const std::string& filename);
    float getExchangeRate(const std::string& date) const;
    void processInput(const std::string& filename);
    
private:
    bool isValidDate(const std::string& date) const;
    bool isValidValue(const float value) const;
    std::string findClosestDate(const std::string& date) const;
};

#endif