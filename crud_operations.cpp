#include "crud_operations.h"
#include <iomanip>
#include <sstream>
#include <limits>

CrudOperations::CrudOperations() : db(Database::getInstance()) {}

void CrudOperations::showMenu() {
    while (true) {
        printSeparator();
        std::cout << "=== ПУТЕВОЙ ДНЕВНИК - CRUD ОПЕРАЦИИ ===\n";
        std::cout << "1. Подключиться к базе данных\n";
        std::cout << "2. Показать все поездки\n";
        std::cout << "3. Показать все расходы\n";
        std::cout << "4. Показать всех сопутешественников\n";
        std::cout << "5. Добавить поездку\n";
        std::cout << "6. Добавить расход\n";
        std::cout << "7. Добавить сопутешественника\n";
        std::cout << "8. Редактировать поездку\n";
        std::cout << "9. Редактировать расход\n";
        std::cout << "10. Редактировать сопутешественника\n";
        std::cout << "11. Удалить поездку\n";
        std::cout << "12. Удалить расход\n";
        std::cout << "13. Удалить сопутешественника\n";
        std::cout << "14. Статистика\n";
        std::cout << "15. Поездки по статусу\n";
        std::cout << "16. Расходы по категории\n";
        std::cout << "0. Выход\n";
        printSeparator();
        
        std::cout << "Выберите действие: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        if (choice == 0) break;
        handleChoice(choice);
    }
}

void CrudOperations::handleChoice(int choice) {
    switch (choice) {
        case 1: connectToDatabase(); break;
        case 2: showTrips(); break;
        case 3: showExpenses(); break;
        case 4: showCompanions(); break;
        case 5: addTrip(); break;
        case 6: addExpense(); break;
        case 7: addCompanion(); break;
        case 8: editTrip(); break;
        case 9: editExpense(); break;
        case 10: editCompanion(); break;
        case 11: deleteTrip(); break;
        case 12: deleteExpense(); break;
        case 13: deleteCompanion(); break;
        case 14: showStatistics(); break;
        case 15: showTripsByStatus(); break;
        case 16: showExpensesByCategory(); break;
        default:
            std::cout << "Неверный выбор!\n";
    }
}

void CrudOperations::connectToDatabase() {
    std::cout << "\n=== ПОДКЛЮЧЕНИЕ К БАЗЕ ДАННЫХ ===\n";
    
    std::string host = getStringInput("Хост [localhost]: ");
    if (host.empty()) host = "localhost";
    
    std::string port = getStringInput("Порт [5432]: ");
    if (port.empty()) port = "5432";
    
    std::string dbname = getStringInput("Имя базы данных [my_travels]: ");
    if (dbname.empty()) dbname = "my_travels";
    
    std::string user = getStringInput("Пользователь [postgres]: ");
    if (user.empty()) user = "postgres";
    
    std::string password = getStringInput("Пароль: ");
    
    if (db.connect(host, port, dbname, user, password)) {
        std::cout << "Успешно подключено к базе данных!\n";
    } else {
        std::cout << "Ошибка подключения: " << db.getLastError() << "\n";
    }
}

void CrudOperations::showTrips() {
    if (!db.isConnected()) {
        std::cout << "Сначала подключитесь к базе данных!\n";
        return;
    }
    
    printHeader("СПИСОК ПОЕЗДОК");
    std::vector<Trip> trips = db.readTrips();
    
    if (trips.empty()) {
        std::cout << "Поездок не найдено.\n";
        return;
    }
    
    for (const auto& trip : trips) {
        printTrip(trip);
    }
    std::cout << "Всего поездок: " << trips.size() << "\n";
}

void CrudOperations::showExpenses() {
    if (!db.isConnected()) {
        std::cout << "Сначала подключитесь к базе данных!\n";
        return;
    }
    
    printHeader("СПИСОК РАСХОДОВ");
    std::vector<Expense> expenses = db.readExpenses();
    
    if (expenses.empty()) {
        std::cout << "Расходов не найдено.\n";
        return;
    }
    
    for (const auto& expense : expenses) {
        printExpense(expense);
    }
    std::cout << "Всего расходов: " << expenses.size() << "\n";
}

void CrudOperations::showCompanions() {
    if (!db.isConnected()) {
        std::cout << "Сначала подключитесь к базе данных!\n";
        return;
    }
    
    printHeader("СПИСОК СОПУТЕШЕСТВЕННИКОВ");
    std::vector<TravelCompanion> companions = db.readCompanions();
    
    if (companions.empty()) {
        std::cout << "Сопутешественников не найдено.\n";
        return;
    }
    
    for (const auto& companion : companions) {
        printCompanion(companion);
    }
    std::cout << "Всего сопутешественников: " << companions.size() << "\n";
}

void CrudOperations::addTrip() {
    if (!db.isConnected()) {
        std::cout << "Сначала подключитесь к базе данных!\n";
        return;
    }
    
    printHeader("ДОБАВЛЕНИЕ НОВОЙ ПОЕЗДКИ");
    
    Trip trip;
    trip.name = getStringInput("Название поездки: ");
    trip.start_date = getDateInput("Дата начала (ГГГГ-ММ-ДД): ");
    trip.end_date = getDateInput("Дата окончания (ГГГГ-ММ-ДД): ");
    trip.total_budget = getDoubleInput("Бюджет (руб.): ");
    trip.status = getStringInput("Статус (planned/in_progress/completed/cancelled): ");
    trip.notes = getStringInput("Заметки (опционально): ");
    
    if (db.createTrip(trip)) {
        std::cout << "Поездка успешно добавлена!\n";
    } else {
        std::cout << "Ошибка добавления поездки: " << db.getLastError() << "\n";
    }
}

void CrudOperations::addExpense() {
    if (!db.isConnected()) {
        std::cout << "Сначала подключитесь к базе данных!\n";
        return;
    }
    
    printHeader("ДОБАВЛЕНИЕ НОВОГО РАСХОДА");
    
    Expense expense;
    expense.trip_destination_id = getIntInput("ID места посещения: ");
    expense.expense_date = getDateInput("Дата расхода (ГГГГ-ММ-ДД): ");
    expense.amount = getDoubleInput("Сумма: ");
    expense.currency = getStringInput("Валюта [RUB]: ");
    if (expense.currency.empty()) expense.currency = "RUB";
    
    std::cout << "Категории: transport, accommodation, food, entertainment, shopping, sightseeing, other\n";
    expense.category = getStringInput("Категория: ");
    expense.description = getStringInput("Описание: ");
    
    if (db.createExpense(expense)) {
        std::cout << "Расход успешно добавлен!\n";
    } else {
        std::cout << "Ошибка добавления расхода: " << db.getLastError() << "\n";
    }
}

void CrudOperations::addCompanion() {
    if (!db.isConnected()) {
        std::cout << "Сначала подключитесь к базе данных!\n";
        return;
    }
    
    printHeader("ДОБАВЛЕНИЕ НОВОГО СОПУТЕШЕСТВЕННИКА");
    
    TravelCompanion companion;
    companion.first_name = getStringInput("Имя: ");
    companion.last_name = getStringInput("Фамилия: ");
    companion.email = getStringInput("Email (опционально): ");
    companion.phone = getStringInput("Телефон (опционально): ");
    
    std::cout << "Тип отношений: friend, family, colleague, partner, other\n";
    companion.relationship = getStringInput("Тип отношений: ");
    
    if (db.createCompanion(companion)) {
        std::cout << "Сопутешественник успешно добавлен!\n";
    } else {
        std::cout << "Ошибка добавления сопутешественника: " << db.getLastError() << "\n";
    }
}

void CrudOperations::editTrip() {
    if (!db.isConnected()) {
        std::cout << "Сначала подключитесь к базе данных!\n";
        return;
    }
    
    printHeader("РЕДАКТИРОВАНИЕ ПОЕЗДКИ");
    
    int id = getIntInput("Введите ID поездки для редактирования: ");
    Trip trip = db.readTrip(id);
    
    if (trip.id == -1) {
        std::cout << "Поездка с ID " << id << " не найдена.\n";
        return;
    }
    
    printTrip(trip);
    std::cout << "\nВведите новые данные (оставьте пустым для сохранения старого значения):\n";
    
    std::string input;
    
    input = getStringInput("Название [" + trip.name + "]: ");
    if (!input.empty()) trip.name = input;
    
    input = getStringInput("Дата начала [" + trip.start_date + "]: ");
    if (!input.empty()) trip.start_date = input;
    
    input = getStringInput("Дата окончания [" + trip.end_date + "]: ");
    if (!input.empty()) trip.end_date = input;
    
    input = getStringInput("Бюджет [" + std::to_string(trip.total_budget) + "]: ");
    if (!input.empty()) trip.total_budget = std::stod(input);
    
    input = getStringInput("Статус [" + trip.status + "]: ");
    if (!input.empty()) trip.status = input;
    
    input = getStringInput("Заметки [" + trip.notes + "]: ");
    if (!input.empty()) trip.notes = input;
    
    if (db.updateTrip(trip)) {
        std::cout << "Поездка успешно обновлена!\n";
    } else {
        std::cout << "Ошибка обновления поездки: " << db.getLastError() << "\n";
    }
}

void CrudOperations::editExpense() {
    if (!db.isConnected()) {
        std::cout << "Сначала подключитесь к базе данных!\n";
        return;
    }
    
    printHeader("РЕДАКТИРОВАНИЕ РАСХОДА");
    
    int id = getIntInput("Введите ID расхода для редактирования: ");
    Expense expense = db.readExpense(id);
    
    if (expense.id == -1) {
        std::cout << "Расход с ID " << id << " не найдена.\n";
        return;
    }
    
    printExpense(expense);
    std::cout << "\nВведите новые данные (оставьте пустым для сохранения старого значения):\n";
    
    std::string input;
    
    input = getStringInput("ID места посещения [" + std::to_string(expense.trip_destination_id) + "]: ");
    if (!input.empty()) expense.trip_destination_id = std::stoi(input);
    
    input = getStringInput("Дата расхода [" + expense.expense_date + "]: ");
    if (!input.empty()) expense.expense_date = input;
    
    input = getStringInput("Сумма [" + std::to_string(expense.amount) + "]: ");
    if (!input.empty()) expense.amount = std::stod(input);
    
    input = getStringInput("Валюта [" + expense.currency + "]: ");
    if (!input.empty()) expense.currency = input;
    
    input = getStringInput("Категория [" + expense.category + "]: ");
    if (!input.empty()) expense.category = input;
    
    input = getStringInput("Описание [" + expense.description + "]: ");
    if (!input.empty()) expense.description = input;
    
    if (db.updateExpense(expense)) {
        std::cout << "Расход успешно обновлен!\n";
    } else {
        std::cout << "Ошибка обновления расхода: " << db.getLastError() << "\n";
    }
}

void CrudOperations::editCompanion() {
    if (!db.isConnected()) {
        std::cout << "Сначала подключитесь к базе данных!\n";
        return;
    }
    
    printHeader("РЕДАКТИРОВАНИЕ СОПУТЕШЕСТВЕННИКА");
    
    int id = getIntInput("Введите ID сопутешественника для редактирования: ");
    TravelCompanion companion = db.readCompanion(id);
    
    if (companion.id == -1) {
        std::cout << "Сопутешественник с ID " << id << " не найден.\n";
        return;
    }
    
    printCompanion(companion);
    std::cout << "\nВведите новые данные (оставьте пустым для сохранения старого значения):\n";
    
    std::string input;
    
    input = getStringInput("Имя [" + companion.first_name + "]: ");
    if (!input.empty()) companion.first_name = input;
    
    input = getStringInput("Фамилия [" + companion.last_name + "]: ");
    if (!input.empty()) companion.last_name = input;
    
    input = getStringInput("Email [" + companion.email + "]: ");
    if (!input.empty()) companion.email = input;
    
    input = getStringInput("Телефон [" + companion.phone + "]: ");
    if (!input.empty()) companion.phone = input;
    
    input = getStringInput("Тип отношений [" + companion.relationship + "]: ");
    if (!input.empty()) companion.relationship = input;
    
    if (db.updateCompanion(companion)) {
        std::cout << "Сопутешественник успешно обновлен!\n";
    } else {
        std::cout << "Ошибка обновления сопутешественника: " << db.getLastError() << "\n";
    }
}

void CrudOperations::deleteTrip() {
    if (!db.isConnected()) {
        std::cout << "Сначала подключитесь к базе данных!\n";
        return;
    }
    
    printHeader("УДАЛЕНИЕ ПОЕЗДКИ");
    
    int id = getIntInput("Введите ID поездки для удаления: ");
    
    std::cout << "Вы уверены, что хотите удалить поездку с ID " << id << "? (y/n): ";
    char confirm;
    std::cin >> confirm;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (confirm == 'y' || confirm == 'Y') {
        if (db.deleteTrip(id)) {
            std::cout << "Поездка успешно удалена!\n";
        } else {
            std::cout << "Ошибка удаления поездки: " << db.getLastError() << "\n";
        }
    } else {
        std::cout << "Удаление отменено.\n";
    }
}

void CrudOperations::deleteExpense() {
    if (!db.isConnected()) {
        std::cout << "Сначала подключитесь к базе данных!\n";
        return;
    }
    
    printHeader("УДАЛЕНИЕ РАСХОДА");
    
    int id = getIntInput("Введите ID расхода для удаления: ");
    
    std::cout << "Вы уверены, что хотите удалить расход с ID " << id << "? (y/n): ";
    char confirm;
    std::cin >> confirm;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (confirm == 'y' || confirm == 'Y') {
        if (db.deleteExpense(id)) {
            std::cout << "Расход успешно удален!\n";
        } else {
            std::cout << "Ошибка удаления расхода: " << db.getLastError() << "\n";
        }
    } else {
        std::cout << "Удаление отменено.\n";
    }
}

void CrudOperations::deleteCompanion() {
    if (!db.isConnected()) {
        std::cout << "Сначала подключитесь к базе данных!\n";
        return;
    }
    
    printHeader("УДАЛЕНИЕ СОПУТЕШЕСТВЕННИКА");
    
    int id = getIntInput("Введите ID сопутешественника для удаления: ");
    
    std::cout << "Вы уверены, что хотите удалить сопутешественника с ID " << id << "? (y/n): ";
    char confirm;
    std::cin >> confirm;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (confirm == 'y' || confirm == 'Y') {
        if (db.deleteCompanion(id)) {
            std::cout << "Сопутешественник успешно удален!\n";
        } else {
            std::cout << "Ошибка удаления сопутешественника: " << db.getLastError() << "\n";
        }
    } else {
        std::cout << "Удаление отменено.\n";
    }
}

void CrudOperations::showStatistics() {
    if (!db.isConnected()) {
        std::cout << "Сначала подключитесь к базе данных!\n";
        return;
    }
    
    printHeader("СТАТИСТИКА");
    
    double totalSpent = db.getTotalSpent();
    std::cout << "Общая сумма расходов: " << std::fixed << std::setprecision(2) 
              << totalSpent << " руб.\n";
    
    std::vector<Trip> trips = db.readTrips();
    int completed = 0, planned = 0, inProgress = 0, cancelled = 0;
    
    for (const auto& trip : trips) {
        if (trip.status == "completed") completed++;
        else if (trip.status == "planned") planned++;
        else if (trip.status == "in_progress") inProgress++;
        else if (trip.status == "cancelled") cancelled++;
    }
    
    std::cout << "\nСтатистика по поездкам:\n";
    std::cout << "  Завершено: " << completed << "\n";
    std::cout << "  Запланировано: " << planned << "\n";
    std::cout << "  В процессе: " << inProgress << "\n";
    std::cout << "  Отменено: " << cancelled << "\n";
    std::cout << "  Всего: " << trips.size() << "\n";
}

void CrudOperations::showTripsByStatus() {
    if (!db.isConnected()) {
        std::cout << "Сначала подключитесь к базе данных!\n";
        return;
    }
    
    printHeader("ПОЕЗДКИ ПО СТАТУСУ");
    
    std::string status = getStringInput("Введите статус (planned/in_progress/completed/cancelled): ");
    std::vector<Trip> trips = db.getTripsByStatus(status);
    
    if (trips.empty()) {
        std::cout << "Поездок со статусом '" << status << "' не найдено.\n";
        return;
    }
    
    for (const auto& trip : trips) {
        printTrip(trip);
    }
    std::cout << "Найдено поездок: " << trips.size() << "\n";
}

void CrudOperations::showExpensesByCategory() {
    if (!db.isConnected()) {
        std::cout << "Сначала подключитесь к базе данных!\n";
        return;
    }
    
    printHeader("РАСХОДЫ ПО КАТЕГОРИИ");
    
    std::string category = getStringInput("Введите категорию: ");
    std::vector<Expense> expenses = db.getExpensesByCategory(category);
    
    if (expenses.empty()) {
        std::cout << "Расходов в категории '" << category << "' не найдено.\n";
        return;
    }
    
    double total = 0;
    for (const auto& expense : expenses) {
        printExpense(expense);
        total += expense.amount;
    }
    std::cout << "Найдено расходов: " << expenses.size() << "\n";
    std::cout << "Общая сумма: " << std::fixed << std::setprecision(2) << total << "\n";
}

// Вспомогательные методы
void CrudOperations::printTrip(const Trip& trip) {
    printSeparator();
    std::cout << "ID: " << trip.id << "\n";
    std::cout << "Название: " << trip.name << "\n";
    std::cout << "Даты: " << trip.start_date << " - " << trip.end_date << "\n";
    std::cout << "Бюджет: " << std::fixed << std::setprecision(2) 
              << trip.total_budget << " руб. (факт: " 
              << trip.actual_spent << " руб.)\n";
    std::cout << "Статус: " << trip.status << "\n";
    if (!trip.notes.empty()) {
        std::cout << "Заметки: " << trip.notes << "\n";
    }
}

void CrudOperations::printExpense(const Expense& expense) {
    printSeparator();
    std::cout << "ID: " << expense.id << "\n";
    std::cout << "Место посещения ID: " << expense.trip_destination_id << "\n";
    std::cout << "Дата: " << expense.expense_date << "\n";
    std::cout << "Сумма: " << std::fixed << std::setprecision(2) 
              << expense.amount << " " << expense.currency << "\n";
    std::cout << "Категория: " << expense.category << "\n";
    std::cout << "Описание: " << expense.description << "\n";
}

void CrudOperations::printCompanion(const TravelCompanion& companion) {
    printSeparator();
    std::cout << "ID: " << companion.id << "\n";
    std::cout << "Имя: " << companion.first_name << " " << companion.last_name << "\n";
    if (!companion.email.empty()) {
        std::cout << "Email: " << companion.email << "\n";
    }
    if (!companion.phone.empty()) {
        std::cout << "Телефон: " << companion.phone << "\n";
    }
    std::cout << "Отношения: " << companion.relationship << "\n";
}

void CrudOperations::printSeparator() {
    std::cout << "----------------------------------------\n";
}

void CrudOperations::printHeader(const std::string& title) {
    std::cout << "\n";
    printSeparator();
    std::cout << "  " << title << "\n";
    printSeparator();
}

// Методы ввода
std::string CrudOperations::getStringInput(const std::string& prompt) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

double CrudOperations::getDoubleInput(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);
        
        if (input.empty()) return 0.0;
        
        try {
            return std::stod(input);
        } catch (...) {
            std::cout << "Неверный формат числа. Попробуйте снова.\n";
        }
    }
}

int CrudOperations::getIntInput(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);
        
        if (input.empty()) return 0;
        
        try {
            return std::stoi(input);
        } catch (...) {
            std::cout << "Неверный формат числа. Попробуйте снова.\n";
        }
    }
}

std::string CrudOperations::getDateInput(const std::string& prompt) {
    while (true) {
        std::string date = getStringInput(prompt);
        
        if (date.empty()) return "";
        
        // Простая проверка формата ГГГГ-ММ-ДД
        if (date.length() == 10 && date[4] == '-' && date[7] == '-') {
            return date;
        }
        
        std::cout << "Неверный формат даты. Используйте ГГГГ-ММ-ДД\n";
    }
}