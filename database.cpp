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

// database.cpp - добавить после других CRUD операций

// CRUD операции для стран
bool Database::createCountry(const Country& country) {
    std::string query = "INSERT INTO countries (country_name, iso_code, "
                       "continent, visa_required) "
                       "VALUES ($1, $2, $3, $4) RETURNING country_id";
    
    std::vector<std::string> params = {
        country.name,
        country.iso_code,
        country.continent,
        country.visa_required ? "true" : "false"
    };
    
    PGresult* result = executeQuery(query, params);
    if (!result) return false;
    
    PQclear(result);
    return true;
}

std::vector<Country> Database::readCountries() {
    std::vector<Country> countries;
    std::string query = "SELECT country_id, country_name, iso_code, "
                       "continent, visa_required, "
                       "TO_CHAR(created_at, 'YYYY-MM-DD HH24:MI') "
                       "FROM countries ORDER BY country_name";
    
    PGresult* result = executeQuery(query);
    if (!result) return countries;
    
    int rows = PQntuples(result);
    for (int i = 0; i < rows; ++i) {
        Country country;
        country.id = std::stoi(PQgetvalue(result, i, 0));
        country.name = PQgetvalue(result, i, 1);
        country.iso_code = PQgetvalue(result, i, 2);
        country.continent = PQgetvalue(result, i, 3);
        country.visa_required = std::string(PQgetvalue(result, i, 4)) == "t";
        country.created_at = PQgetvalue(result, i, 5);
        countries.push_back(country);
    }
    
    PQclear(result);
    return countries;
}

Country Database::readCountry(int id) {
    Country country;
    country.id = -1;
    
    std::string query = "SELECT country_id, country_name, iso_code, "
                       "continent, visa_required, "
                       "TO_CHAR(created_at, 'YYYY-MM-DD HH24:MI') "
                       "FROM countries WHERE country_id = $1";
    
    std::vector<std::string> params = {std::to_string(id)};
    PGresult* result = executeQuery(query, params);
    if (!result) return country;
    
    if (PQntuples(result) > 0) {
        country.id = std::stoi(PQgetvalue(result, 0, 0));
        country.name = PQgetvalue(result, 0, 1);
        country.iso_code = PQgetvalue(result, 0, 2);
        country.continent = PQgetvalue(result, 0, 3);
        country.visa_required = std::string(PQgetvalue(result, 0, 4)) == "t";
        country.created_at = PQgetvalue(result, 0, 5);
    }
    
    PQclear(result);
    return country;
}

bool Database::updateCountry(const Country& country) {
    std::string query = "UPDATE countries SET "
                       "country_name = $1, "
                       "iso_code = $2, "
                       "continent = $3, "
                       "visa_required = $4 "
                       "WHERE country_id = $5";
    
    std::vector<std::string> params = {
        country.name,
        country.iso_code,
        country.continent,
        country.visa_required ? "true" : "false",
        std::to_string(country.id)
    };
    
    return executeUpdate(query, params);
}

bool Database::deleteCountry(int id) {
    std::string query = "DELETE FROM countries WHERE country_id = $1";
    std::vector<std::string> params = {std::to_string(id)};
    return executeUpdate(query, params);
}

// Специальные методы для стран
std::vector<Country> Database::getCountriesByContinent(const std::string& continent) {
    std::vector<Country> countries;
    std::string query = "SELECT country_id, country_name, iso_code, "
                       "continent, visa_required, "
                       "TO_CHAR(created_at, 'YYYY-MM-DD HH24:MI') "
                       "FROM countries WHERE continent = $1 ORDER BY country_name";
    
    std::vector<std::string> params = {continent};
    PGresult* result = executeQuery(query, params);
    if (!result) return countries;
    
    int rows = PQntuples(result);
    for (int i = 0; i < rows; ++i) {
        Country country;
        country.id = std::stoi(PQgetvalue(result, i, 0));
        country.name = PQgetvalue(result, i, 1);
        country.iso_code = PQgetvalue(result, i, 2);
        country.continent = PQgetvalue(result, i, 3);
        country.visa_required = std::string(PQgetvalue(result, i, 4)) == "t";
        country.created_at = PQgetvalue(result, i, 5);
        countries.push_back(country);
    }
    
    PQclear(result);
    return countries;
}

std::vector<Country> Database::getCountriesRequiringVisa() {
    std::vector<Country> countries;
    std::string query = "SELECT country_id, country_name, iso_code, "
                       "continent, visa_required, "
                       "TO_CHAR(created_at, 'YYYY-MM-DD HH24:MI') "
                       "FROM countries WHERE visa_required = true ORDER BY country_name";
    
    PGresult* result = executeQuery(query);
    if (!result) return countries;
    
    int rows = PQntuples(result);
    for (int i = 0; i < rows; ++i) {
        Country country;
        country.id = std::stoi(PQgetvalue(result, i, 0));
        country.name = PQgetvalue(result, i, 1);
        country.iso_code = PQgetvalue(result, i, 2);
        country.continent = PQgetvalue(result, i, 3);
        country.visa_required = true;
        country.created_at = PQgetvalue(result, i, 5);
        countries.push_back(country);
    }
    
    PQclear(result);
    return countries;
}

std::vector<Country> Database::getCountriesNotRequiringVisa() {
    std::vector<Country> countries;
    std::string query = "SELECT country_id, country_name, iso_code, "
                       "continent, visa_required, "
                       "TO_CHAR(created_at, 'YYYY-MM-DD HH24:MI') "
                       "FROM countries WHERE visa_required = false ORDER BY country_name";
    
    PGresult* result = executeQuery(query);
    if (!result) return countries;
    
    int rows = PQntuples(result);
    for (int i = 0; i < rows; ++i) {
        Country country;
        country.id = std::stoi(PQgetvalue(result, i, 0));
        country.name = PQgetvalue(result, i, 1);
        country.iso_code = PQgetvalue(result, i, 2);
        country.continent = PQgetvalue(result, i, 3);
        country.visa_required = false;
        country.created_at = PQgetvalue(result, i, 5);
        countries.push_back(country);
    }
    
    PQclear(result);
    return countries;
}

int Database::getCountriesCount() {
    std::string query = "SELECT COUNT(*) FROM countries";
    PGresult* result = executeQuery(query);
    if (!result || PQntuples(result) == 0) return 0;
    
    const char* value = PQgetvalue(result, 0, 0);
    int count = value ? std::stoi(value) : 0;
    
    PQclear(result);
    return count;
}

// database.cpp - добавить после методов для стран

// CRUD операции для городов
bool Database::createCity(const City& city) {
    std::string query = "INSERT INTO cities (city_name, country_id, population, "
                       "time_zone, has_airport) "
                       "VALUES ($1, $2, $3, $4, $5) RETURNING city_id";
    
    std::vector<std::string> params = {
        city.name,
        std::to_string(city.country_id),
        std::to_string(city.population),
        city.time_zone,
        city.has_airport ? "true" : "false"
    };
    
    PGresult* result = executeQuery(query, params);
    if (!result) return false;
    
    PQclear(result);
    return true;
}

std::vector<City> Database::readCities() {
    std::vector<City> cities;
    std::string query = "SELECT c.city_id, c.city_name, c.country_id, "
                       "c.population, c.time_zone, c.has_airport, "
                       "TO_CHAR(c.created_at, 'YYYY-MM-DD HH24:MI'), "
                       "ct.country_name "
                       "FROM cities c "
                       "LEFT JOIN countries ct ON c.country_id = ct.country_id "
                       "ORDER BY ct.country_name, c.city_name";
    
    PGresult* result = executeQuery(query);
    if (!result) return cities;
    
    int rows = PQntuples(result);
    for (int i = 0; i < rows; ++i) {
        City city;
        city.id = std::stoi(PQgetvalue(result, i, 0));
        city.name = PQgetvalue(result, i, 1);
        city.country_id = std::stoi(PQgetvalue(result, i, 2));
        city.population = PQgetvalue(result, i, 3) ? std::stoi(PQgetvalue(result, i, 3)) : 0;
        city.time_zone = PQgetvalue(result, i, 4);
        city.has_airport = std::string(PQgetvalue(result, i, 5)) == "t";
        city.created_at = PQgetvalue(result, i, 6);
        cities.push_back(city);
    }
    
    PQclear(result);
    return cities;
}

City Database::readCity(int id) {
    City city;
    city.id = -1;
    
    std::string query = "SELECT city_id, city_name, country_id, population, "
                       "time_zone, has_airport, "
                       "TO_CHAR(created_at, 'YYYY-MM-DD HH24:MI') "
                       "FROM cities WHERE city_id = $1";
    
    std::vector<std::string> params = {std::to_string(id)};
    PGresult* result = executeQuery(query, params);
    if (!result) return city;
    
    if (PQntuples(result) > 0) {
        city.id = std::stoi(PQgetvalue(result, 0, 0));
        city.name = PQgetvalue(result, 0, 1);
        city.country_id = std::stoi(PQgetvalue(result, 0, 2));
        city.population = PQgetvalue(result, 0, 3) ? std::stoi(PQgetvalue(result, 0, 3)) : 0;
        city.time_zone = PQgetvalue(result, 0, 4);
        city.has_airport = std::string(PQgetvalue(result, 0, 5)) == "t";
        city.created_at = PQgetvalue(result, 0, 6);
    }
    
    PQclear(result);
    return city;
}

bool Database::updateCity(const City& city) {
    std::string query = "UPDATE cities SET "
                       "city_name = $1, "
                       "country_id = $2, "
                       "population = $3, "
                       "time_zone = $4, "
                       "has_airport = $5 "
                       "WHERE city_id = $6";
    
    std::vector<std::string> params = {
        city.name,
        std::to_string(city.country_id),
        std::to_string(city.population),
        city.time_zone,
        city.has_airport ? "true" : "false",
        std::to_string(city.id)
    };
    
    return executeUpdate(query, params);
}

bool Database::deleteCity(int id) {
    std::string query = "DELETE FROM cities WHERE city_id = $1";
    std::vector<std::string> params = {std::to_string(id)};
    return executeUpdate(query, params);
}

// Специальные методы для городов
std::vector<City> Database::getCitiesByCountry(int country_id) {
    std::vector<City> cities;
    std::string query = "SELECT city_id, city_name, country_id, population, "
                       "time_zone, has_airport, "
                       "TO_CHAR(created_at, 'YYYY-MM-DD HH24:MI') "
                       "FROM cities WHERE country_id = $1 ORDER BY city_name";
    
    std::vector<std::string> params = {std::to_string(country_id)};
    PGresult* result = executeQuery(query, params);
    if (!result) return cities;
    
    int rows = PQntuples(result);
    for (int i = 0; i < rows; ++i) {
        City city;
        city.id = std::stoi(PQgetvalue(result, i, 0));
        city.name = PQgetvalue(result, i, 1);
        city.country_id = std::stoi(PQgetvalue(result, i, 2));
        city.population = PQgetvalue(result, i, 3) ? std::stoi(PQgetvalue(result, i, 3)) : 0;
        city.time_zone = PQgetvalue(result, i, 4);
        city.has_airport = std::string(PQgetvalue(result, i, 5)) == "t";
        city.created_at = PQgetvalue(result, i, 6);
        cities.push_back(city);
    }
    
    PQclear(result);
    return cities;
}

std::vector<City> Database::getCitiesWithAirport() {
    std::vector<City> cities;
    std::string query = "SELECT c.city_id, c.city_name, c.country_id, "
                       "c.population, c.time_zone, c.has_airport, "
                       "TO_CHAR(c.created_at, 'YYYY-MM-DD HH24:MI'), "
                       "ct.country_name "
                       "FROM cities c "
                       "LEFT JOIN countries ct ON c.country_id = ct.country_id "
                       "WHERE c.has_airport = true "
                       "ORDER BY ct.country_name, c.city_name";
    
    PGresult* result = executeQuery(query);
    if (!result) return cities;
    
    int rows = PQntuples(result);
    for (int i = 0; i < rows; ++i) {
        City city;
        city.id = std::stoi(PQgetvalue(result, i, 0));
        city.name = PQgetvalue(result, i, 1);
        city.country_id = std::stoi(PQgetvalue(result, i, 2));
        city.population = PQgetvalue(result, i, 3) ? std::stoi(PQgetvalue(result, i, 3)) : 0;
        city.time_zone = PQgetvalue(result, i, 4);
        city.has_airport = true;
        city.created_at = PQgetvalue(result, i, 6);
        cities.push_back(city);
    }
    
    PQclear(result);
    return cities;
}

int Database::getCitiesCount() {
    std::string query = "SELECT COUNT(*) FROM cities";
    PGresult* result = executeQuery(query);
    if (!result || PQntuples(result) == 0) return 0;
    
    const char* value = PQgetvalue(result, 0, 0);
    int count = value ? std::stoi(value) : 0;
    
    PQclear(result);
    return count;
}

// CRUD операции для фотографий
bool Database::createPhoto(const Photo& photo) {
    std::string query = "INSERT INTO photos (trip_destination_id, photo_name, "
                       "file_path, taken_date, description, rating, tags) "
                       "VALUES ($1, $2, $3, $4, $5, $6, $7) RETURNING photo_id";
    
    std::vector<std::string> params = {
        std::to_string(photo.trip_destination_id),
        photo.name,
        photo.file_path,
        photo.taken_date,
        photo.description,
        std::to_string(photo.rating),
        photo.tags
    };
    
    PGresult* result = executeQuery(query, params);
    if (!result) return false;
    
    PQclear(result);
    return true;
}

std::vector<Photo> Database::readPhotos() {
    std::vector<Photo> photos;
    std::string query = "SELECT photo_id, trip_destination_id, photo_name, "
                       "file_path, taken_date, description, rating, tags, "
                       "TO_CHAR(created_at, 'YYYY-MM-DD HH24:MI') "
                       "FROM photos ORDER BY taken_date DESC";
    
    PGresult* result = executeQuery(query);
    if (!result) return photos;
    
    int rows = PQntuples(result);
    for (int i = 0; i < rows; ++i) {
        Photo photo;
        photo.id = std::stoi(PQgetvalue(result, i, 0));
        photo.trip_destination_id = std::stoi(PQgetvalue(result, i, 1));
        photo.name = PQgetvalue(result, i, 2);
        photo.file_path = PQgetvalue(result, i, 3);
        photo.taken_date = PQgetvalue(result, i, 4);
        photo.description = PQgetvalue(result, i, 5);
        photo.rating = PQgetvalue(result, i, 6) ? std::stoi(PQgetvalue(result, i, 6)) : 0;
        photo.tags = PQgetvalue(result, i, 7);
        photo.created_at = PQgetvalue(result, i, 8);
        photos.push_back(photo);
    }
    
    PQclear(result);
    return photos;
}

Photo Database::readPhoto(int id) {
    Photo photo;
    photo.id = -1;
    
    std::string query = "SELECT photo_id, trip_destination_id, photo_name, "
                       "file_path, taken_date, description, rating, tags, "
                       "TO_CHAR(created_at, 'YYYY-MM-DD HH24:MI') "
                       "FROM photos WHERE photo_id = $1";
    
    std::vector<std::string> params = {std::to_string(id)};
    PGresult* result = executeQuery(query, params);
    if (!result) return photo;
    
    if (PQntuples(result) > 0) {
        photo.id = std::stoi(PQgetvalue(result, 0, 0));
        photo.trip_destination_id = std::stoi(PQgetvalue(result, 0, 1));
        photo.name = PQgetvalue(result, 0, 2);
        photo.file_path = PQgetvalue(result, 0, 3);
        photo.taken_date = PQgetvalue(result, 0, 4);
        photo.description = PQgetvalue(result, 0, 5);
        photo.rating = PQgetvalue(result, 0, 6) ? std::stoi(PQgetvalue(result, 0, 6)) : 0;
        photo.tags = PQgetvalue(result, 0, 7);
        photo.created_at = PQgetvalue(result, 0, 8);
    }
    
    PQclear(result);
    return photo;
}

bool Database::updatePhoto(const Photo& photo) {
    std::string query = "UPDATE photos SET "
                       "trip_destination_id = $1, "
                       "photo_name = $2, "
                       "file_path = $3, "
                       "taken_date = $4, "
                       "description = $5, "
                       "rating = $6, "
                       "tags = $7 "
                       "WHERE photo_id = $8";
    
    std::vector<std::string> params = {
        std::to_string(photo.trip_destination_id),
        photo.name,
        photo.file_path,
        photo.taken_date,
        photo.description,
        std::to_string(photo.rating),
        photo.tags,
        std::to_string(photo.id)
    };
    
    return executeUpdate(query, params);
}

bool Database::deletePhoto(int id) {
    std::string query = "DELETE FROM photos WHERE photo_id = $1";
    std::vector<std::string> params = {std::to_string(id)};
    return executeUpdate(query, params);
}

// Специальные методы для фотографий
std::vector<Photo> Database::getPhotosByDestination(int trip_destination_id) {
    std::vector<Photo> photos;
    std::string query = "SELECT photo_id, trip_destination_id, photo_name, "
                       "file_path, taken_date, description, rating, tags, "
                       "TO_CHAR(created_at, 'YYYY-MM-DD HH24:MI') "
                       "FROM photos WHERE trip_destination_id = $1 "
                       "ORDER BY taken_date DESC";
    
    std::vector<std::string> params = {std::to_string(trip_destination_id)};
    PGresult* result = executeQuery(query, params);
    if (!result) return photos;
    
    int rows = PQntuples(result);
    for (int i = 0; i < rows; ++i) {
        Photo photo;
        photo.id = std::stoi(PQgetvalue(result, i, 0));
        photo.trip_destination_id = std::stoi(PQgetvalue(result, i, 1));
        photo.name = PQgetvalue(result, i, 2);
        photo.file_path = PQgetvalue(result, i, 3);
        photo.taken_date = PQgetvalue(result, i, 4);
        photo.description = PQgetvalue(result, i, 5);
        photo.rating = PQgetvalue(result, i, 6) ? std::stoi(PQgetvalue(result, i, 6)) : 0;
        photo.tags = PQgetvalue(result, i, 7);
        photo.created_at = PQgetvalue(result, i, 8);
        photos.push_back(photo);
    }
    
    PQclear(result);
    return photos;
}

std::vector<Photo> Database::getPhotosByRating(int min_rating) {
    std::vector<Photo> photos;
    std::string query = "SELECT photo_id, trip_destination_id, photo_name, "
                       "file_path, taken_date, description, rating, tags, "
                       "TO_CHAR(created_at, 'YYYY-MM-DD HH24:MI') "
                       "FROM photos WHERE rating >= $1 "
                       "ORDER BY rating DESC, taken_date DESC";
    
    std::vector<std::string> params = {std::to_string(min_rating)};
    PGresult* result = executeQuery(query, params);
    if (!result) return photos;
    
    int rows = PQntuples(result);
    for (int i = 0; i < rows; ++i) {
        Photo photo;
        photo.id = std::stoi(PQgetvalue(result, i, 0));
        photo.trip_destination_id = std::stoi(PQgetvalue(result, i, 1));
        photo.name = PQgetvalue(result, i, 2);
        photo.file_path = PQgetvalue(result, i, 3);
        photo.taken_date = PQgetvalue(result, i, 4);
        photo.description = PQgetvalue(result, i, 5);
        photo.rating = PQgetvalue(result, i, 6) ? std::stoi(PQgetvalue(result, i, 6)) : 0;
        photo.tags = PQgetvalue(result, i, 7);
        photo.created_at = PQgetvalue(result, i, 8);
        photos.push_back(photo);
    }
    
    PQclear(result);
    return photos;
}

int Database::getPhotosCount() {
    std::string query = "SELECT COUNT(*) FROM photos";
    PGresult* result = executeQuery(query);
    if (!result || PQntuples(result) == 0) return 0;
    
    const char* value = PQgetvalue(result, 0, 0);
    int count = value ? std::stoi(value) : 0;
    
    PQclear(result);
    return count;
}

// CRUD операции для участников поездок
bool Database::createTripCompanion(const TripCompanion& tc) {
    std::string query = "INSERT INTO trip_companions (trip_id, companion_id, "
                       "role, joined_date, left_date, share_expenses) "
                       "VALUES ($1, $2, $3, $4, $5, $6)";
    
    std::vector<std::string> params = {
        std::to_string(tc.trip_id),
        std::to_string(tc.companion_id),
        tc.role,
        tc.joined_date,
        tc.left_date.empty() ? "NULL" : tc.left_date,
        tc.share_expenses ? "true" : "false"
    };
    
    return executeUpdate(query, params);
}

std::vector<TripCompanion> Database::readTripCompanions() {
    std::vector<TripCompanion> tripCompanions;
    std::string query = "SELECT tc.trip_id, tc.companion_id, tc.role, "
                       "tc.joined_date, tc.left_date, tc.share_expenses, "
                       "TO_CHAR(tc.created_at, 'YYYY-MM-DD HH24:MI'), "
                       "t.trip_name, c.first_name, c.last_name "
                       "FROM trip_companions tc "
                       "LEFT JOIN trips t ON tc.trip_id = t.trip_id "
                       "LEFT JOIN travel_companions c ON tc.companion_id = c.companion_id "
                       "ORDER BY tc.joined_date DESC";
    
    PGresult* result = executeQuery(query);
    if (!result) return tripCompanions;
    
    int rows = PQntuples(result);
    for (int i = 0; i < rows; ++i) {
        TripCompanion tc;
        tc.trip_id = std::stoi(PQgetvalue(result, i, 0));
        tc.companion_id = std::stoi(PQgetvalue(result, i, 1));
        tc.role = PQgetvalue(result, i, 2);
        tc.joined_date = PQgetvalue(result, i, 3);
        
        const char* left_date = PQgetvalue(result, i, 4);
        tc.left_date = left_date ? std::string(left_date) : "";
        
        tc.share_expenses = std::string(PQgetvalue(result, i, 5)) == "t";
        tc.created_at = PQgetvalue(result, i, 6);
        tripCompanions.push_back(tc);
    }
    
    PQclear(result);
    return tripCompanions;
}

TripCompanion Database::readTripCompanion(int trip_id, int companion_id) {
    TripCompanion tc;
    tc.trip_id = -1;
    
    std::string query = "SELECT trip_id, companion_id, role, "
                       "joined_date, left_date, share_expenses, "
                       "TO_CHAR(created_at, 'YYYY-MM-DD HH24:MI') "
                       "FROM trip_companions WHERE trip_id = $1 AND companion_id = $2";
    
    std::vector<std::string> params = {
        std::to_string(trip_id),
        std::to_string(companion_id)
    };
    
    PGresult* result = executeQuery(query, params);
    if (!result) return tc;
    
    if (PQntuples(result) > 0) {
        tc.trip_id = std::stoi(PQgetvalue(result, 0, 0));
        tc.companion_id = std::stoi(PQgetvalue(result, 0, 1));
        tc.role = PQgetvalue(result, 0, 2);
        tc.joined_date = PQgetvalue(result, 0, 3);
        
        const char* left_date = PQgetvalue(result, 0, 4);
        tc.left_date = left_date ? std::string(left_date) : "";
        
        tc.share_expenses = std::string(PQgetvalue(result, 0, 5)) == "t";
        tc.created_at = PQgetvalue(result, 0, 6);
    }
    
    PQclear(result);
    return tc;
}

bool Database::updateTripCompanion(const TripCompanion& tc) {
    std::string query = "UPDATE trip_companions SET "
                       "role = $1, "
                       "joined_date = $2, "
                       "left_date = $3, "
                       "share_expenses = $4 "
                       "WHERE trip_id = $5 AND companion_id = $6";
    
    std::vector<std::string> params = {
        tc.role,
        tc.joined_date,
        tc.left_date.empty() ? "NULL" : tc.left_date,
        tc.share_expenses ? "true" : "false",
        std::to_string(tc.trip_id),
        std::to_string(tc.companion_id)
    };
    
    return executeUpdate(query, params);
}

bool Database::deleteTripCompanion(int trip_id, int companion_id) {
    std::string query = "DELETE FROM trip_companions WHERE trip_id = $1 AND companion_id = $2";
    std::vector<std::string> params = {
        std::to_string(trip_id),
        std::to_string(companion_id)
    };
    
    return executeUpdate(query, params);
}

bool Database::deleteTripCompanionByTrip(int trip_id) {
    std::string query = "DELETE FROM trip_companions WHERE trip_id = $1";
    std::vector<std::string> params = {std::to_string(trip_id)};
    return executeUpdate(query, params);
}

bool Database::deleteTripCompanionByCompanion(int companion_id) {
    std::string query = "DELETE FROM trip_companions WHERE companion_id = $1";
    std::vector<std::string> params = {std::to_string(companion_id)};
    return executeUpdate(query, params);
}

// Специальные методы
std::vector<TripCompanion> Database::getCompanionsByTrip(int trip_id) {
    std::vector<TripCompanion> tripCompanions;
    std::string query = "SELECT tc.trip_id, tc.companion_id, tc.role, "
                       "tc.joined_date, tc.left_date, tc.share_expenses, "
                       "TO_CHAR(tc.created_at, 'YYYY-MM-DD HH24:MI'), "
                       "c.first_name, c.last_name "
                       "FROM trip_companions tc "
                       "LEFT JOIN travel_companions c ON tc.companion_id = c.companion_id "
                       "WHERE tc.trip_id = $1 "
                       "ORDER BY tc.joined_date";
    
    std::vector<std::string> params = {std::to_string(trip_id)};
    PGresult* result = executeQuery(query, params);
    if (!result) return tripCompanions;
    
    int rows = PQntuples(result);
    for (int i = 0; i < rows; ++i) {
        TripCompanion tc;
        tc.trip_id = std::stoi(PQgetvalue(result, i, 0));
        tc.companion_id = std::stoi(PQgetvalue(result, i, 1));
        tc.role = PQgetvalue(result, i, 2);
        tc.joined_date = PQgetvalue(result, i, 3);
        
        const char* left_date = PQgetvalue(result, i, 4);
        tc.left_date = left_date ? std::string(left_date) : "";
        
        tc.share_expenses = std::string(PQgetvalue(result, i, 5)) == "t";
        tc.created_at = PQgetvalue(result, i, 6);
        tripCompanions.push_back(tc);
    }
    
    PQclear(result);
    return tripCompanions;
}

std::vector<TripCompanion> Database::getTripsByCompanion(int companion_id) {
    std::vector<TripCompanion> tripCompanions;
    std::string query = "SELECT tc.trip_id, tc.companion_id, tc.role, "
                       "tc.joined_date, tc.left_date, tc.share_expenses, "
                       "TO_CHAR(tc.created_at, 'YYYY-MM-DD HH24:MI'), "
                       "t.trip_name "
                       "FROM trip_companions tc "
                       "LEFT JOIN trips t ON tc.trip_id = t.trip_id "
                       "WHERE tc.companion_id = $1 "
                       "ORDER BY tc.joined_date DESC";
    
    std::vector<std::string> params = {std::to_string(companion_id)};
    PGresult* result = executeQuery(query, params);
    if (!result) return tripCompanions;
    
    int rows = PQntuples(result);
    for (int i = 0; i < rows; ++i) {
        TripCompanion tc;
        tc.trip_id = std::stoi(PQgetvalue(result, i, 0));
        tc.companion_id = std::stoi(PQgetvalue(result, i, 1));
        tc.role = PQgetvalue(result, i, 2);
        tc.joined_date = PQgetvalue(result, i, 3);
        
        const char* left_date = PQgetvalue(result, i, 4);
        tc.left_date = left_date ? std::string(left_date) : "";
        
        tc.share_expenses = std::string(PQgetvalue(result, i, 5)) == "t";
        tc.created_at = PQgetvalue(result, i, 6);
        tripCompanions.push_back(tc);
    }
    
    PQclear(result);
    return tripCompanions;
}

int Database::getTripCompanionsCount() {
    std::string query = "SELECT COUNT(*) FROM trip_companions";
    PGresult* result = executeQuery(query);
    if (!result || PQntuples(result) == 0) return 0;
    
    const char* value = PQgetvalue(result, 0, 0);
    int count = value ? std::stoi(value) : 0;
    
    PQclear(result);
    return count;
}

// CRUD операции для мест посещения
bool Database::createTripDestination(const TripDestination& td) {
    std::string query = "INSERT INTO trip_destinations (trip_id, city_id, "
                       "visit_order, arrival_date, departure_date, "
                       "accommodation, transport_type) "
                       "VALUES ($1, $2, $3, $4, $5, $6, $7) RETURNING trip_destination_id";
    
    std::vector<std::string> params = {
        std::to_string(td.trip_id),
        std::to_string(td.city_id),
        std::to_string(td.visit_order),
        td.arrival_date,
        td.departure_date,
        td.accommodation,
        td.transport_type
    };
    
    PGresult* result = executeQuery(query, params);
    if (!result) return false;
    
    PQclear(result);
    return true;
}

std::vector<TripDestination> Database::readTripDestinations() {
    std::vector<TripDestination> destinations;
    std::string query = "SELECT td.trip_destination_id, td.trip_id, td.city_id, "
                       "td.visit_order, td.arrival_date, td.departure_date, "
                       "td.accommodation, td.transport_type, "
                       "TO_CHAR(td.created_at, 'YYYY-MM-DD HH24:MI'), "
                       "t.trip_name, c.city_name, ct.country_name "
                       "FROM trip_destinations td "
                       "LEFT JOIN trips t ON td.trip_id = t.trip_id "
                       "LEFT JOIN cities c ON td.city_id = c.city_id "
                       "LEFT JOIN countries ct ON c.country_id = ct.country_id "
                       "ORDER BY td.trip_id, td.visit_order";
    
    PGresult* result = executeQuery(query);
    if (!result) return destinations;
    
    int rows = PQntuples(result);
    for (int i = 0; i < rows; ++i) {
        TripDestination td;
        td.id = std::stoi(PQgetvalue(result, i, 0));
        td.trip_id = std::stoi(PQgetvalue(result, i, 1));
        td.city_id = std::stoi(PQgetvalue(result, i, 2));
        td.visit_order = std::stoi(PQgetvalue(result, i, 3));
        td.arrival_date = PQgetvalue(result, i, 4);
        td.departure_date = PQgetvalue(result, i, 5);
        td.accommodation = PQgetvalue(result, i, 6);
        td.transport_type = PQgetvalue(result, i, 7);
        td.created_at = PQgetvalue(result, i, 8);
        destinations.push_back(td);
    }
    
    PQclear(result);
    return destinations;
}

TripDestination Database::readTripDestination(int id) {
    TripDestination td;
    td.id = -1;
    
    std::string query = "SELECT trip_destination_id, trip_id, city_id, "
                       "visit_order, arrival_date, departure_date, "
                       "accommodation, transport_type, "
                       "TO_CHAR(created_at, 'YYYY-MM-DD HH24:MI') "
                       "FROM trip_destinations WHERE trip_destination_id = $1";
    
    std::vector<std::string> params = {std::to_string(id)};
    PGresult* result = executeQuery(query, params);
    if (!result) return td;
    
    if (PQntuples(result) > 0) {
        td.id = std::stoi(PQgetvalue(result, 0, 0));
        td.trip_id = std::stoi(PQgetvalue(result, 0, 1));
        td.city_id = std::stoi(PQgetvalue(result, 0, 2));
        td.visit_order = std::stoi(PQgetvalue(result, 0, 3));
        td.arrival_date = PQgetvalue(result, 0, 4);
        td.departure_date = PQgetvalue(result, 0, 5);
        td.accommodation = PQgetvalue(result, 0, 6);
        td.transport_type = PQgetvalue(result, 0, 7);
        td.created_at = PQgetvalue(result, 0, 8);
    }
    
    PQclear(result);
    return td;
}

bool Database::updateTripDestination(const TripDestination& td) {
    std::string query = "UPDATE trip_destinations SET "
                       "trip_id = $1, "
                       "city_id = $2, "
                       "visit_order = $3, "
                       "arrival_date = $4, "
                       "departure_date = $5, "
                       "accommodation = $6, "
                       "transport_type = $7 "
                       "WHERE trip_destination_id = $8";
    
    std::vector<std::string> params = {
        std::to_string(td.trip_id),
        std::to_string(td.city_id),
        std::to_string(td.visit_order),
        td.arrival_date,
        td.departure_date,
        td.accommodation,
        td.transport_type,
        std::to_string(td.id)
    };
    
    return executeUpdate(query, params);
}

bool Database::deleteTripDestination(int id) {
    std::string query = "DELETE FROM trip_destinations WHERE trip_destination_id = $1";
    std::vector<std::string> params = {std::to_string(id)};
    return executeUpdate(query, params);
}

// Специальные методы
std::vector<TripDestination> Database::getDestinationsByTrip(int trip_id) {
    std::vector<TripDestination> destinations;
    std::string query = "SELECT td.trip_destination_id, td.trip_id, td.city_id, "
                       "td.visit_order, td.arrival_date, td.departure_date, "
                       "td.accommodation, td.transport_type, "
                       "TO_CHAR(td.created_at, 'YYYY-MM-DD HH24:MI'), "
                       "c.city_name "
                       "FROM trip_destinations td "
                       "LEFT JOIN cities c ON td.city_id = c.city_id "
                       "WHERE td.trip_id = $1 "
                       "ORDER BY td.visit_order";
    
    std::vector<std::string> params = {std::to_string(trip_id)};
    PGresult* result = executeQuery(query, params);
    if (!result) return destinations;
    
    int rows = PQntuples(result);
    for (int i = 0; i < rows; ++i) {
        TripDestination td;
        td.id = std::stoi(PQgetvalue(result, i, 0));
        td.trip_id = std::stoi(PQgetvalue(result, i, 1));
        td.city_id = std::stoi(PQgetvalue(result, i, 2));
        td.visit_order = std::stoi(PQgetvalue(result, i, 3));
        td.arrival_date = PQgetvalue(result, i, 4);
        td.departure_date = PQgetvalue(result, i, 5);
        td.accommodation = PQgetvalue(result, i, 6);
        td.transport_type = PQgetvalue(result, i, 7);
        td.created_at = PQgetvalue(result, i, 8);
        destinations.push_back(td);
    }
    
    PQclear(result);
    return destinations;
}

std::vector<TripDestination> Database::getDestinationsByCity(int city_id) {
    std::vector<TripDestination> destinations;
    std::string query = "SELECT td.trip_destination_id, td.trip_id, td.city_id, "
                       "td.visit_order, td.arrival_date, td.departure_date, "
                       "td.accommodation, td.transport_type, "
                       "TO_CHAR(td.created_at, 'YYYY-MM-DD HH24:MI'), "
                       "t.trip_name "
                       "FROM trip_destinations td "
                       "LEFT JOIN trips t ON td.trip_id = t.trip_id "
                       "WHERE td.city_id = $1 "
                       "ORDER BY td.arrival_date DESC";
    
    std::vector<std::string> params = {std::to_string(city_id)};
    PGresult* result = executeQuery(query, params);
    if (!result) return destinations;
    
    int rows = PQntuples(result);
    for (int i = 0; i < rows; ++i) {
        TripDestination td;
        td.id = std::stoi(PQgetvalue(result, i, 0));
        td.trip_id = std::stoi(PQgetvalue(result, i, 1));
        td.city_id = std::stoi(PQgetvalue(result, i, 2));
        td.visit_order = std::stoi(PQgetvalue(result, i, 3));
        td.arrival_date = PQgetvalue(result, i, 4);
        td.departure_date = PQgetvalue(result, i, 5);
        td.accommodation = PQgetvalue(result, i, 6);
        td.transport_type = PQgetvalue(result, i, 7);
        td.created_at = PQgetvalue(result, i, 8);
        destinations.push_back(td);
    }
    
    PQclear(result);
    return destinations;
}

std::vector<TripDestination> Database::getDestinationsByTransport(const std::string& transport_type) {
    std::vector<TripDestination> destinations;
    std::string query = "SELECT td.trip_destination_id, td.trip_id, td.city_id, "
                       "td.visit_order, td.arrival_date, td.departure_date, "
                       "td.accommodation, td.transport_type, "
                       "TO_CHAR(td.created_at, 'YYYY-MM-DD HH24:MI'), "
                       "t.trip_name, c.city_name "
                       "FROM trip_destinations td "
                       "LEFT JOIN trips t ON td.trip_id = t.trip_id "
                       "LEFT JOIN cities c ON td.city_id = c.city_id "
                       "WHERE td.transport_type = $1 "
                       "ORDER BY td.arrival_date DESC";
    
    std::vector<std::string> params = {transport_type};
    PGresult* result = executeQuery(query, params);
    if (!result) return destinations;
    
    int rows = PQntuples(result);
    for (int i = 0; i < rows; ++i) {
        TripDestination td;
        td.id = std::stoi(PQgetvalue(result, i, 0));
        td.trip_id = std::stoi(PQgetvalue(result, i, 1));
        td.city_id = std::stoi(PQgetvalue(result, i, 2));
        td.visit_order = std::stoi(PQgetvalue(result, i, 3));
        td.arrival_date = PQgetvalue(result, i, 4);
        td.departure_date = PQgetvalue(result, i, 5);
        td.accommodation = PQgetvalue(result, i, 6);
        td.transport_type = PQgetvalue(result, i, 7);
        td.created_at = PQgetvalue(result, i, 8);
        destinations.push_back(td);
    }
    
    PQclear(result);
    return destinations;
}

int Database::getTripDestinationsCount() {
    std::string query = "SELECT COUNT(*) FROM trip_destinations";
    PGresult* result = executeQuery(query);
    if (!result || PQntuples(result) == 0) return 0;
    
    const char* value = PQgetvalue(result, 0, 0);
    int count = value ? std::stoi(value) : 0;
    
    PQclear(result);
    return count;
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

PGresult* Database::executeRawQuery(const std::string& query) {
    if (!isConnected()) {
        last_error_ = "Not connected to database";
        return nullptr;
    }
    
    PGresult* result = PQexec(connection_, query.c_str());
    
    if (PQresultStatus(result) != PGRES_TUPLES_OK && 
        PQresultStatus(result) != PGRES_COMMAND_OK) {
        last_error_ = PQerrorMessage(connection_);
        return result; // Возвращаем результат даже при ошибке для анализа
    }
    
    return result;
}

bool Database::executeRawUpdate(const std::string& query) {
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