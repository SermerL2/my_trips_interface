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

struct Country {
    int id;
    std::string name;
    std::string iso_code;
    std::string continent;
    bool visa_required;
    std::string created_at;
};

struct City {
    int id;
    std::string name;
    int country_id;
    int population;
    std::string time_zone;
    bool has_airport;
    std::string created_at;
};

struct Photo {
    int id;
    int trip_destination_id;
    std::string name;
    std::string file_path;
    std::string taken_date;
    std::string description;
    int rating;
    std::string tags;
    std::string created_at;
};

struct TripCompanion {
    int trip_id;
    int companion_id;
    std::string role;
    std::string joined_date;
    std::string left_date;
    bool share_expenses;
    std::string created_at;
};

struct TripDestination {
    int id;
    int trip_id;
    int city_id;
    int visit_order;
    std::string arrival_date;
    std::string departure_date;
    std::string accommodation;
    std::string transport_type;
    std::string created_at;
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

    // CRUD для стран
    bool createCountry(const Country& country);
    std::vector<Country> readCountries();
    Country readCountry(int id);
    bool updateCountry(const Country& country);
    bool deleteCountry(int id);

    std::vector<Country> getCountriesByContinent(const std::string& continent);
    std::vector<Country> getCountriesRequiringVisa();
    std::vector<Country> getCountriesNotRequiringVisa();

    // CRUD для городов
    bool createCity(const City& city);
    std::vector<City> readCities();
    City readCity(int id);
    bool updateCity(const City& city);
    bool deleteCity(int id);

    // Специальные методы для городов
    std::vector<City> getCitiesByCountry(int country_id);
    std::vector<City> getCitiesWithAirport();
    int getCitiesCount();

    // CRUD для фотографий
    bool createPhoto(const Photo& photo);
    std::vector<Photo> readPhotos();
    Photo readPhoto(int id);
    bool updatePhoto(const Photo& photo);
    bool deletePhoto(int id);
    
    // Специальные методы для фотографий
    std::vector<Photo> getPhotosByDestination(int trip_destination_id);
    std::vector<Photo> getPhotosByRating(int min_rating);
    int getPhotosCount();

    // CRUD для участников поездок
    bool createTripCompanion(const TripCompanion& tc);
    std::vector<TripCompanion> readTripCompanions();
    TripCompanion readTripCompanion(int trip_id, int companion_id);
    bool updateTripCompanion(const TripCompanion& tc);
    bool deleteTripCompanion(int trip_id, int companion_id);
    bool deleteTripCompanionByTrip(int trip_id);
    bool deleteTripCompanionByCompanion(int companion_id);
    
    // Специальные методы
    std::vector<TripCompanion> getCompanionsByTrip(int trip_id);
    std::vector<TripCompanion> getTripsByCompanion(int companion_id);
    int getTripCompanionsCount();

    // CRUD для мест посещения
    bool createTripDestination(const TripDestination& td);
    std::vector<TripDestination> readTripDestinations();
    TripDestination readTripDestination(int id);
    bool updateTripDestination(const TripDestination& td);
    bool deleteTripDestination(int id);
    
    // Специальные методы
    std::vector<TripDestination> getDestinationsByTrip(int trip_id);
    std::vector<TripDestination> getDestinationsByCity(int city_id);
    std::vector<TripDestination> getDestinationsByTransport(const std::string& transport_type);
    int getTripDestinationsCount();
    
    // Специальные операции
    std::vector<Trip> getTripsByStatus(const std::string& status);
    std::vector<Expense> getExpensesByCategory(const std::string& category);
    double getTotalSpent();
    int getTripsCount();
    int getExpensesCount();
    int getCompanionsCount();
    int getCountriesCount();

    PGresult* executeRawQuery(const std::string& query);
    bool executeRawUpdate(const std::string& query);
    
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