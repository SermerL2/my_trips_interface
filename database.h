#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <memory>
#include <libpq-fe.h>
#include <QString>

struct Trip {
    int id;
    std::string name;
    std::string start_date;
    std::string end_date;
    double total_budget;
    double actual_spent;
    std::string status;
    std::string notes;
};

struct Expense {
    int id;
    int trip_destination_id;
    std::string expense_date;
    double amount;
    std::string currency;
    std::string category;
    std::string description;
};

struct TravelCompanion {
    int id;
    std::string first_name;
    std::string last_name;
    std::string email;
    std::string phone;
    // Убрали std::string relationship;
};

class Database {
public:
    static Database& getInstance();
    
    bool connect(const std::string& host, 
                const std::string& port,
                const std::string& dbname,
                const std::string& user,
                const std::string& password);
    
    bool isConnected() const;
    void disconnect();
    
    // CRUD для поездок
    bool createTrip(const Trip& trip);
    std::vector<Trip> readTrips();
    Trip readTrip(int id);
    bool updateTrip(const Trip& trip);
    bool deleteTrip(int id);
    
    // CRUD для расходов
    bool createExpense(const Expense& expense);
    std::vector<Expense> readExpenses();
    Expense readExpense(int id);
    bool updateExpense(const Expense& expense);
    bool deleteExpense(int id);
    
    // CRUD для сопутешественников
    bool createCompanion(const TravelCompanion& companion);
    std::vector<TravelCompanion> readCompanions();
    TravelCompanion readCompanion(int id);
    bool updateCompanion(const TravelCompanion& companion);
    bool deleteCompanion(int id);
    
    // Специальные операции
    std::vector<Trip> getTripsByStatus(const std::string& status);
    std::vector<Expense> getExpensesByCategory(const std::string& category);
    double getTotalSpent();
    int getTripsCount();
    int getExpensesCount();
    int getCompanionsCount();
    
    QString getLastError() const;
    
private:
    Database();
    ~Database();
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;
    
    PGconn* connection_;
    QString last_error_;
    
    PGresult* executeQuery(const std::string& query);
    PGresult* executeQuery(const std::string& query, const std::vector<std::string>& params);
    bool executeUpdate(const std::string& query);
    bool executeUpdate(const std::string& query, const std::vector<std::string>& params);
};

#endif // DATABASE_H