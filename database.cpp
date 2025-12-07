#include "database.h"
#include <iostream>
#include <sstream>
#include <iomanip>

Database& Database::getInstance() {
    static Database instance;
    return instance;
}

Database::Database() : connection_(nullptr) {}

Database::~Database() {
    disconnect();
}

bool Database::connect(const std::string& host, 
                      const std::string& port,
                      const std::string& dbname,
                      const std::string& user,
                      const std::string& password) {
    
    std::string conn_str = "host=" + host + 
                          " port=" + port + 
                          " dbname=" + dbname + 
                          " user=" + user + 
                          " password=" + password;
    
    connection_ = PQconnectdb(conn_str.c_str());
    
    if (PQstatus(connection_) != CONNECTION_OK) {
        last_error_ = PQerrorMessage(connection_);
        PQfinish(connection_);
        connection_ = nullptr;
        return false;
    }
    
    return true;
}

bool Database::isConnected() const {
    return connection_ && PQstatus(connection_) == CONNECTION_OK;
}

void Database::disconnect() {
    if (connection_) {
        PQfinish(connection_);
        connection_ = nullptr;
    }
}

PGresult* Database::executeQuery(const std::string& query) {
    if (!isConnected()) {
        last_error_ = "Not connected to database";
        return nullptr;
    }
    
    PGresult* result = PQexec(connection_, query.c_str());
    
    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        last_error_ = PQerrorMessage(connection_);
        PQclear(result);
        return nullptr;
    }
    
    return result;
}

PGresult* Database::executeQuery(const std::string& query, const std::vector<std::string>& params) {
    if (!isConnected()) {
        last_error_ = "Not connected to database";
        return nullptr;
    }
    
    const char* param_values[params.size()];
    for (size_t i = 0; i < params.size(); ++i) {
        param_values[i] = params[i].c_str();
    }
    
    PGresult* result = PQexecParams(connection_, query.c_str(),
                                   params.size(), nullptr,
                                   param_values, nullptr, nullptr, 0);
    
    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        last_error_ = PQerrorMessage(connection_);
        PQclear(result);
        return nullptr;
    }
    
    return result;
}

bool Database::executeUpdate(const std::string& query) {
    if (!isConnected()) {
        last_error_ = "Not connected to database";
        return false;
    }
    
    PGresult* result = PQexec(connection_, query.c_str());
    
    if (PQresultStatus(result) != PGRES_COMMAND_OK) {
        last_error_ = PQerrorMessage(connection_);
        PQclear(result);
        return false;
    }
    
    PQclear(result);
    return true;
}

/*bool Database::executeUpdate(const std::string& query, const std::vector<std::string>& params) {
    if (!isConnected()) {
        last_error_ = "Not connected to database";
        return false;
    }
    
    const char* param_values[params.size()];
    for (size_t i = 0; i < params.size(); ++i) {
        param_values[i] = params[i].c_str();
    }
    
    PGresult* result = PQexecParams(connection_, query.c_str(),
                                   params.size(), nullptr,
                                   param_values, nullptr, nullptr, 0);
    
    if (PQresultStatus(result) != PGRES_COMMAND_OK) {
        last_error_ = PQerrorMessage(connection_);
        PQclear(result);
        return false;
    }
    
    PQclear(result);
    return true;
}*/

bool Database::executeUpdate(const std::string& query, const std::vector<std::string>& params) {
    if (!isConnected()) {
        last_error_ = "Not connected to database";
        return false;
    }
    
    const char* param_values[params.size()];
    for (size_t i = 0; i < params.size(); ++i) {
        param_values[i] = params[i].c_str();
    }
    
    PGresult* result = PQexecParams(connection_, query.c_str(),
                                   params.size(), nullptr,
                                   param_values, nullptr, nullptr, 0);
    
    if (PQresultStatus(result) != PGRES_COMMAND_OK && 
        PQresultStatus(result) != PGRES_TUPLES_OK) {
        last_error_ = PQerrorMessage(connection_);
        PQclear(result);
        return false;
    }
    
    PQclear(result);
    return true;
}

// CRUD операции для поездок
/*
bool Database::createTrip(const Trip& trip) {
    std::string query = "INSERT INTO trips (trip_name, start_date, end_date, "
                       "total_budget, trip_status, notes) "
                       "VALUES ($1, $2, $3, $4, $5, $6)";
    
    std::vector<std::string> params = {
        trip.name,
        trip.start_date,
        trip.end_date,
        std::to_string(trip.total_budget),
        trip.status,
        trip.notes
    };
    
    return executeUpdate(query, params);
}
*/
bool Database::createTrip(const Trip& trip) {
    std::string query = "INSERT INTO trips (trip_name, start_date, end_date, "
                       "total_budget, trip_status, notes) "
                       "VALUES ($1, $2, $3, $4, $5, $6) RETURNING trip_id";
    
    std::vector<std::string> params = {
        trip.name,
        trip.start_date,
        trip.end_date,
        std::to_string(trip.total_budget),
        trip.status,
        trip.notes
    };
    
    PGresult* result = executeQuery(query, params);
    if (!result) return false;
    
    PQclear(result);
    return true;
}

std::vector<Trip> Database::readTrips() {
    std::vector<Trip> trips;
    std::string query = "SELECT trip_id, trip_name, start_date, end_date, "
                       "total_budget, actual_spent, trip_status, notes "
                       "FROM trips ORDER BY start_date DESC";
    
    PGresult* result = executeQuery(query);
    if (!result) return trips;
    
    int rows = PQntuples(result);
    for (int i = 0; i < rows; ++i) {
        Trip trip;
        trip.id = std::stoi(PQgetvalue(result, i, 0));
        trip.name = PQgetvalue(result, i, 1);
        trip.start_date = PQgetvalue(result, i, 2);
        trip.end_date = PQgetvalue(result, i, 3);
        trip.total_budget = std::stod(PQgetvalue(result, i, 4));
        trip.actual_spent = std::stod(PQgetvalue(result, i, 5));
        trip.status = PQgetvalue(result, i, 6);
        trip.notes = PQgetvalue(result, i, 7);
        trips.push_back(trip);
    }
    
    PQclear(result);
    return trips;
}

Trip Database::readTrip(int id) {
    Trip trip;
    trip.id = -1;
    
    std::string query = "SELECT trip_id, trip_name, start_date, end_date, "
                       "total_budget, actual_spent, trip_status, notes "
                       "FROM trips WHERE trip_id = $1";
    
    std::vector<std::string> params = {std::to_string(id)};
    PGresult* result = executeQuery(query, params);
    if (!result) return trip;
    
    if (PQntuples(result) > 0) {
        trip.id = std::stoi(PQgetvalue(result, 0, 0));
        trip.name = PQgetvalue(result, 0, 1);
        trip.start_date = PQgetvalue(result, 0, 2);
        trip.end_date = PQgetvalue(result, 0, 3);
        trip.total_budget = std::stod(PQgetvalue(result, 0, 4));
        trip.actual_spent = std::stod(PQgetvalue(result, 0, 5));
        trip.status = PQgetvalue(result, 0, 6);
        trip.notes = PQgetvalue(result, 0, 7);
    }
    
    PQclear(result);
    return trip;
}

bool Database::updateTrip(const Trip& trip) {
    std::string query = "UPDATE trips SET "
                       "trip_name = $1, "
                       "start_date = $2, "
                       "end_date = $3, "
                       "total_budget = $4, "
                       "trip_status = $5, "
                       "notes = $6 "
                       "WHERE trip_id = $7";
    
    std::vector<std::string> params = {
        trip.name,
        trip.start_date,
        trip.end_date,
        std::to_string(trip.total_budget),
        trip.status,
        trip.notes,
        std::to_string(trip.id)
    };
    
    return executeUpdate(query, params);
}

bool Database::deleteTrip(int id) {
    std::string query = "DELETE FROM trips WHERE trip_id = $1";
    std::vector<std::string> params = {std::to_string(id)};
    return executeUpdate(query, params);
}

// CRUD операции для расходов
/*bool Database::createExpense(const Expense& expense) {
    std::string query = "INSERT INTO expenses (trip_destination_id, expense_date, "
                       "amount, currency, category, description) "
                       "VALUES ($1, $2, $3, $4, $5, $6)";
    
    std::vector<std::string> params = {
        std::to_string(expense.trip_destination_id),
        expense.expense_date,
        std::to_string(expense.amount),
        expense.currency,
        expense.category,
        expense.description
    };
    
    return executeUpdate(query, params);
}*/

bool Database::createExpense(const Expense& expense) {
    std::string query = "INSERT INTO expenses (trip_destination_id, expense_date, "
                       "amount, currency, category, description) "
                       "VALUES ($1, $2, $3, $4, $5, $6) RETURNING expense_id";
    
    std::vector<std::string> params = {
        std::to_string(expense.trip_destination_id),
        expense.expense_date,
        std::to_string(expense.amount),
        expense.currency,
        expense.category,
        expense.description
    };
    
    PGresult* result = executeQuery(query, params);
    if (!result) return false;
    
    PQclear(result);
    return true;
}

std::vector<Expense> Database::readExpenses() {
    std::vector<Expense> expenses;
    std::string query = "SELECT expense_id, trip_destination_id, expense_date, "
                       "amount, currency, category, description "
                       "FROM expenses ORDER BY expense_date DESC";
    
    PGresult* result = executeQuery(query);
    if (!result) return expenses;
    
    int rows = PQntuples(result);
    for (int i = 0; i < rows; ++i) {
        Expense expense;
        expense.id = std::stoi(PQgetvalue(result, i, 0));
        expense.trip_destination_id = std::stoi(PQgetvalue(result, i, 1));
        expense.expense_date = PQgetvalue(result, i, 2);
        expense.amount = std::stod(PQgetvalue(result, i, 3));
        expense.currency = PQgetvalue(result, i, 4);
        expense.category = PQgetvalue(result, i, 5);
        expense.description = PQgetvalue(result, i, 6);
        expenses.push_back(expense);
    }
    
    PQclear(result);
    return expenses;
}

Expense Database::readExpense(int id) {
    Expense expense;
    expense.id = -1;
    
    std::string query = "SELECT expense_id, trip_destination_id, expense_date, "
                       "amount, currency, category, description "
                       "FROM expenses WHERE expense_id = $1";
    
    std::vector<std::string> params = {std::to_string(id)};
    PGresult* result = executeQuery(query, params);
    if (!result) return expense;
    
    if (PQntuples(result) > 0) {
        expense.id = std::stoi(PQgetvalue(result, 0, 0));
        expense.trip_destination_id = std::stoi(PQgetvalue(result, 0, 1));
        expense.expense_date = PQgetvalue(result, 0, 2);
        expense.amount = std::stod(PQgetvalue(result, 0, 3));
        expense.currency = PQgetvalue(result, 0, 4);
        expense.category = PQgetvalue(result, 0, 5);
        expense.description = PQgetvalue(result, 0, 6);
    }
    
    PQclear(result);
    return expense;
}

bool Database::updateExpense(const Expense& expense) {
    std::string query = "UPDATE expenses SET "
                       "trip_destination_id = $1, "
                       "expense_date = $2, "
                       "amount = $3, "
                       "currency = $4, "
                       "category = $5, "
                       "description = $6 "
                       "WHERE expense_id = $7";
    
    std::vector<std::string> params = {
        std::to_string(expense.trip_destination_id),
        expense.expense_date,
        std::to_string(expense.amount),
        expense.currency,
        expense.category,
        expense.description,
        std::to_string(expense.id)
    };
    
    return executeUpdate(query, params);
}

bool Database::deleteExpense(int id) {
    std::string query = "DELETE FROM expenses WHERE expense_id = $1";
    std::vector<std::string> params = {std::to_string(id)};
    return executeUpdate(query, params);
}

// CRUD операции для сопутешественников
/*bool Database::createCompanion(const TravelCompanion& companion) {
    std::string query = "INSERT INTO travel_companions (first_name, last_name, "
                       "email, phone) "
                       "VALUES ($1, $2, $3, $4)";
    
    std::vector<std::string> params = {
        companion.first_name,
        companion.last_name,
        companion.email,
        companion.phone,
        //companion.relationship
    };
    
    return executeUpdate(query, params);
}*/

bool Database::createCompanion(const TravelCompanion& companion) {
    std::string query = "INSERT INTO travel_companions (first_name, last_name, "
                       "email, phone) "
                       "VALUES ($1, $2, $3, $4) RETURNING companion_id";
    
    std::vector<std::string> params = {
        companion.first_name,
        companion.last_name,
        companion.email,
        companion.phone
        // Убрали companion.relationship
    };
    
    PGresult* result = executeQuery(query, params);
    if (!result) return false;
    
    PQclear(result);
    return true;
}

// В методе readCompanions
std::vector<TravelCompanion> Database::readCompanions() {
    std::vector<TravelCompanion> companions;
    std::string query = "SELECT companion_id, first_name, last_name, "
                       "email, phone "
                       "FROM travel_companions ORDER BY last_name, first_name";
    
    PGresult* result = executeQuery(query);
    if (!result) return companions;
    
    int rows = PQntuples(result);
    for (int i = 0; i < rows; ++i) {
        TravelCompanion companion;
        companion.id = std::stoi(PQgetvalue(result, i, 0));
        companion.first_name = PQgetvalue(result, i, 1);
        companion.last_name = PQgetvalue(result, i, 2);
        companion.email = PQgetvalue(result, i, 3);
        companion.phone = PQgetvalue(result, i, 4);
        // Не читаем relationship
        companions.push_back(companion);
    }
    
    PQclear(result);
    return companions;
}

TravelCompanion Database::readCompanion(int id) {
    TravelCompanion companion;
    companion.id = -1;
    
    std::string query = "SELECT companion_id, first_name, last_name, "
                       "email, phone "
                       "FROM travel_companions WHERE companion_id = $1";
    
    std::vector<std::string> params = {std::to_string(id)};
    PGresult* result = executeQuery(query, params);
    if (!result) return companion;
    
    if (PQntuples(result) > 0) {
        companion.id = std::stoi(PQgetvalue(result, 0, 0));
        companion.first_name = PQgetvalue(result, 0, 1);
        companion.last_name = PQgetvalue(result, 0, 2);
        companion.email = PQgetvalue(result, 0, 3);
        companion.phone = PQgetvalue(result, 0, 4);
        // Не читаем relationship
    }
    
    PQclear(result);
    return companion;
}

bool Database::updateCompanion(const TravelCompanion& companion) {
    std::string query = "UPDATE travel_companions SET "
                       "first_name = $1, "
                       "last_name = $2, "
                       "email = $3, "
                       "phone = $4 "
                       "WHERE companion_id = $5";  // Исправлен индекс параметра
    
    std::vector<std::string> params = {
        companion.first_name,
        companion.last_name,
        companion.email,
        companion.phone,
        std::to_string(companion.id)
    };
    
    return executeUpdate(query, params);
}

bool Database::deleteCompanion(int id) {
    std::string query = "DELETE FROM travel_companions WHERE companion_id = $1";
    std::vector<std::string> params = {std::to_string(id)};
    return executeUpdate(query, params);
}

// Специальные операции
std::vector<Trip> Database::getTripsByStatus(const std::string& status) {
    std::vector<Trip> trips;
    std::string query = "SELECT trip_id, trip_name, start_date, end_date, "
                       "total_budget, actual_spent, trip_status, notes "
                       "FROM trips WHERE trip_status = $1 ORDER BY start_date DESC";
    
    std::vector<std::string> params = {status};
    PGresult* result = executeQuery(query, params);
    if (!result) return trips;
    
    int rows = PQntuples(result);
    for (int i = 0; i < rows; ++i) {
        Trip trip;
        trip.id = std::stoi(PQgetvalue(result, i, 0));
        trip.name = PQgetvalue(result, i, 1);
        trip.start_date = PQgetvalue(result, i, 2);
        trip.end_date = PQgetvalue(result, i, 3);
        trip.total_budget = std::stod(PQgetvalue(result, i, 4));
        trip.actual_spent = std::stod(PQgetvalue(result, i, 5));
        trip.status = PQgetvalue(result, i, 6);
        trip.notes = PQgetvalue(result, i, 7);
        trips.push_back(trip);
    }
    
    PQclear(result);
    return trips;
}

std::vector<Expense> Database::getExpensesByCategory(const std::string& category) {
    std::vector<Expense> expenses;
    std::string query = "SELECT expense_id, trip_destination_id, expense_date, "
                       "amount, currency, category, description "
                       "FROM expenses WHERE category = $1 ORDER BY expense_date DESC";
    
    std::vector<std::string> params = {category};
    PGresult* result = executeQuery(query, params);
    if (!result) return expenses;
    
    int rows = PQntuples(result);
    for (int i = 0; i < rows; ++i) {
        Expense expense;
        expense.id = std::stoi(PQgetvalue(result, i, 0));
        expense.trip_destination_id = std::stoi(PQgetvalue(result, i, 1));
        expense.expense_date = PQgetvalue(result, i, 2);
        expense.amount = std::stod(PQgetvalue(result, i, 3));
        expense.currency = PQgetvalue(result, i, 4);
        expense.category = PQgetvalue(result, i, 5);
        expense.description = PQgetvalue(result, i, 6);
        expenses.push_back(expense);
    }
    
    PQclear(result);
    return expenses;
}

double Database::getTotalSpent() {
    std::string query = "SELECT SUM(actual_spent) FROM trips WHERE trip_status = 'completed'";
    PGresult* result = executeQuery(query);
    if (!result || PQntuples(result) == 0) return 0.0;
    
    const char* value = PQgetvalue(result, 0, 0);
    double total = value ? std::stod(value) : 0.0;
    
    PQclear(result);
    return total;
}

int Database::getTripsCount() {
    std::string query = "SELECT COUNT(*) FROM trips";
    PGresult* result = executeQuery(query);
    if (!result || PQntuples(result) == 0) return 0;
    
    const char* value = PQgetvalue(result, 0, 0);
    int count = value ? std::stoi(value) : 0;
    
    PQclear(result);
    return count;
}

int Database::getExpensesCount() {
    std::string query = "SELECT COUNT(*) FROM expenses";
    PGresult* result = executeQuery(query);
    if (!result || PQntuples(result) == 0) return 0;
    
    const char* value = PQgetvalue(result, 0, 0);
    int count = value ? std::stoi(value) : 0;
    
    PQclear(result);
    return count;
}

int Database::getCompanionsCount() {
    std::string query = "SELECT COUNT(*) FROM travel_companions";
    PGresult* result = executeQuery(query);
    if (!result || PQntuples(result) == 0) return 0;
    
    const char* value = PQgetvalue(result, 0, 0);
    int count = value ? std::stoi(value) : 0;
    
    PQclear(result);
    return count;
}

QString Database::getLastError() const {
    return last_error_;
}