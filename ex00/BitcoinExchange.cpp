#include "BitcoinExchange.hpp"
#include <cstdlib>
#include <iomanip>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

BitcoinExchange::BitcoinExchange() {
   this->displayHeader();
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) {
   *this = other;
}

BitcoinExchange::~BitcoinExchange() {}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) {
   if (this != &other) {
       _database = other._database;
   }
   return *this;
}

void BitcoinExchange::displayHeader() const {
   std::cout << std::setfill('-') << std::setw(65) << "-" << std::endl;
   std::cout << std::setfill(' ') << std::fixed;
   std::cout << "| " << std::setw(10) << "Date";
   std::cout << " | " << std::setw(10) << "Value";
   std::cout << " | " << std::setw(14) << "Exchange Rate";
   std::cout << " | " << std::setw(14) << "Result" << " |" << std::endl;
   std::cout << std::setfill('-') << std::setw(65) << "-" << std::endl;
   std::cout << std::setfill(' ');
}

void BitcoinExchange::loadDatabase(const std::string& filename) {
   std::ifstream file(filename.c_str());//ouverture du fichier
   if (!file.is_open()) {
       throw std::runtime_error("could not open database file.");
   }

   std::string line;
   std::getline(file, line);  // Skip header

   while (std::getline(file, line)) {
       size_t separator = line.find(",");
       if (separator != std::string::npos) {
           std::string date = line.substr(0, separator);//recuperation de la date
           std::string value = line.substr(separator + 1);//recuperation de la valeur
           
           while (!date.empty() && (date[0] == ' ' || date[0] == '\t'))//suppression des espaces
               date.erase(0, 1);
           while (!date.empty() && (date[date.length()-1] == ' ' || date[date.length()-1] == '\t'))
               date.erase(date.length()-1, 1);

           while (!value.empty() && (value[0] == ' ' || value[0] == '\t'))
               value.erase(0, 1);
           while (!value.empty() && (value[value.length()-1] == ' ' || value[value.length()-1] == '\t'))
               value.erase(value.length()-1, 1);

           float rate = static_cast<float>(std::atof(value.c_str()));
           if (isValidDate(date)) {
               _database[date] = rate;
           }
       }
   }
}

float BitcoinExchange::getExchangeRate(const std::string& date) const {
   std::map<std::string, float>::const_iterator it = _database.find(date);
   if (it != _database.end()) {
       return it->second;
   }
   std::string closestDate = findClosestDate(date);
   return _database.find(closestDate)->second;
}

void BitcoinExchange::processInput(const std::string& filename) {
   std::ifstream file(filename.c_str());
   if (!file.is_open()) {
       throw std::runtime_error("Error: could not open file.");
   }

   std::string line;
   std::getline(file, line);  // Get first line to check format
   bool isCsvFormat = (line.find(",") != std::string::npos);

   float previousRate = 0;

   while (std::getline(file, line)) {
       std::string date, valueStr;
       size_t separator;
       
       if (isCsvFormat) {
           separator = line.find(",");
           if (separator != std::string::npos) {
               date = line.substr(0, separator);
               valueStr = line.substr(separator + 1);
           }
       } else {
           separator = line.find(" | ");
           if (separator != std::string::npos) {
               date = line.substr(0, separator);
               valueStr = line.substr(separator + 3);
           }
       }

       if (separator == std::string::npos) {
           std::cerr << "Error: bad input => " << line << std::endl;
           continue;
       }

       // Trim whitespace
       while (!date.empty() && (date[0] == ' ' || date[0] == '\t'))
           date.erase(0, 1);
       while (!date.empty() && (date[date.length()-1] == ' ' || date[date.length()-1] == '\t'))
           date.erase(date.length()-1, 1);

       while (!valueStr.empty() && (valueStr[0] == ' ' || valueStr[0] == '\t'))
           valueStr.erase(0, 1);
       while (!valueStr.empty() && (valueStr[valueStr.length()-1] == ' ' || valueStr[valueStr.length()-1] == '\t'))
           valueStr.erase(valueStr.length()-1, 1);

       if (!isValidDate(date)) {
           std::cerr << "Error: bad input => " << date << std::endl;
           continue;
       }

       char* endPtr;
       float value = static_cast<float>(std::strtod(valueStr.c_str(), &endPtr));
       if (*endPtr != '\0' || value <= 0) {
           std::cerr << "Error: not a positive number." << std::endl;
           continue;
       }
       
       if (!isCsvFormat && value > 1000) {
           std::cerr << "Error: too large a number." << std::endl;
           continue;
       }

       try {
           float rate = getExchangeRate(date);
           float result = value * rate;
           
           std::cout << "| " << std::setw(10) << date << " | " 
                    << std::setw(10) << std::fixed << std::setprecision(2) << value << " | ";
           
           if (previousRate != 0) {
               if (rate > previousRate)
                   std::cout << GREEN;
               else if (rate < previousRate)
                   std::cout << RED;
           }
           std::cout << std::setw(14) << rate << RESET;
           std::cout << " | " << std::setw(14) << result << " |" << std::endl;
           
           previousRate = rate;
       }
       catch (const std::exception& e) {
           std::cerr << "Error: " << e.what() << std::endl;
       }
   }
   std::cout << std::setfill('-') << std::setw(65) << "-" << std::endl;
}

bool BitcoinExchange::isValidDate(const std::string& date) const {
   if (date.length() != 10) return false;
   if (date[4] != '-' || date[7] != '-') return false;

   int year = std::atoi(date.substr(0, 4).c_str());
   int month = std::atoi(date.substr(5, 2).c_str());
   int day = std::atoi(date.substr(8, 2).c_str());

   if (month < 1 || month > 12) return false;
   if (day < 1 || day > 31) return false;
   if (year < 2009) return false;

   const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
   if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0))//annee bissextile
       return day <= 29;
   return day <= daysInMonth[month - 1];
}

bool BitcoinExchange::isValidValue(const float value) const {
   return value > 0 && value <= 1000;
}

std::string BitcoinExchange::findClosestDate(const std::string& date) const {
   std::map<std::string, float>::const_iterator it = _database.upper_bound(date);
   if (it == _database.begin()) {
       throw std::runtime_error("no valid date found in database");
   }
   --it;
   return it->first;
}