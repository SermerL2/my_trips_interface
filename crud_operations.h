#ifndef CRUD_OPERATIONS_H
#define CRUD_OPERATIONS_H

#include "database.h"
#include <iostream>
#include <string>

class CrudOperations {
public:
    CrudOperations();
    
    void showMenu();
    void handleChoice(int choice);
    
private:
    Database& db;
    
    void connectToDatabase();
    void showTrips();
    void showExpenses();
    void showCompanions();
    
    void addTrip();
    void addExpense();
    void addCompanion();
    
    void editTrip();
    void editExpense();
    void editCompanion();
    
    void deleteTrip();
    void deleteExpense();
    void deleteCompanion();
    
    void showStatistics();
    void showTripsByStatus();
    void showExpensesByCategory();
    
    // Вспомогательные методы
    void printTrip(const Trip& trip);
    void printExpense(const Expense& expense);
    void printCompanion(const TravelCompanion& companion);
    
    void printSeparator();
    void printHeader(const std::string& title);
    
    // Ввод данных
    std::string getStringInput(const std::string& prompt);
    double getDoubleInput(const std::string& prompt);
    int getIntInput(const std::string& prompt);
    std::string getDateInput(const std::string& prompt);
};

#endif // CRUD_OPERATIONS_H