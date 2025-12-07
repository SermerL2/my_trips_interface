-- Проверка работы триггера - посмотрим автоматически рассчитанные суммы
SELECT 
    t.trip_id,
    t.trip_name,
    t.total_budget,
    t.actual_spent,
    t.actual_spent * 100 / t.total_budget as percent_spent,
    t.trip_status
FROM trips t
ORDER BY t.trip_id;


-- 1. Расходы по категориям для каждой поездки
SELECT 
    t.trip_name,
    e.category,
    SUM(e.amount) as total_category,
    COUNT(*) as expense_count
FROM trips t
JOIN trip_destinations td ON t.trip_id = td.trip_id
JOIN expenses e ON td.trip_destination_id = e.trip_destination_id
GROUP BY t.trip_name, e.category
ORDER BY t.trip_name, total_category DESC;

-- 2. Количество сопутешественников по поездкам
SELECT 
    t.trip_name,
    COUNT(tc.companion_id) as companion_count,
    STRING_AGG(c.first_name || ' ' || c.last_name, ', ') as companions
FROM trips t
JOIN trip_companions tc ON t.trip_id = tc.trip_id
JOIN travel_companions c ON tc.companion_id = c.companion_id
GROUP BY t.trip_name
ORDER BY companion_count DESC;

-- 3. Фотографии с рейтингом
SELECT 
    t.trip_name,
    c.city_name,
    p.photo_name,
    p.rating,
    p.taken_date
FROM photos p
JOIN trip_destinations td ON p.trip_destination_id = td.trip_destination_id
JOIN trips t ON td.trip_id = t.trip_id
JOIN cities c ON td.city_id = c.city_id
WHERE p.rating >= 4
ORDER BY p.rating DESC, p.taken_date;

-- 4. Все поздки в хронологическом порядке
SELECT 
    trip_name,
    start_date,
    end_date,
    trip_status,
    actual_spent
FROM trips 
ORDER BY start_date;

-- 5. Самые дорогие поездки (топ-5)
SELECT 
    trip_name,
    total_budget,
    actual_spent,
    actual_spent * 100 / total_budget as budget_usage_percent
FROM trips 
WHERE actual_spent > 0
ORDER BY actual_spent DESC 
LIMIT 5;

-- 6. Поездки по странам (группировка)
SELECT 
    c.country_name,
    COUNT(DISTINCT t.trip_id) as trip_count,
    SUM(t.actual_spent) as total_spent
FROM trips t
JOIN trip_destinations td ON t.trip_id = td.trip_id
JOIN cities ct ON td.city_id = ct.city_id
JOIN countries c ON ct.country_id = c.country_id
GROUP BY c.country_name
ORDER BY trip_count DESC;

-- 7. Расходы по категориям для конкретной поездки
SELECT 
    e.category,
    SUM(e.amount) as total_amount,
    COUNT(*) as expense_count,
    AVG(e.amount) as avg_expense
FROM expenses e
JOIN trip_destinations td ON e.trip_destination_id = td.trip_destination_id
WHERE td.trip_id = 1  -- ID поездки в Чехию
GROUP BY e.category
ORDER BY total_amount DESC;

-- 8. Количество участников в каждой поездке
SELECT 
    t.trip_name,
    COUNT(tc.companion_id) as companion_count,
    STRING_AGG(c.first_name || ' ' || c.last_name, ', ') as companions
FROM trips t
LEFT JOIN trip_companions tc ON t.trip_id = tc.trip_id
LEFT JOIN travel_companions c ON tc.companion_id = c.companion_id
GROUP BY t.trip_id, t.trip_name
ORDER BY companion_count DESC;

-- 9. Города, которые посещались больше всего раз)
SELECT 
    ct.city_name,
    c.country_name,
    COUNT(DISTINCT td.trip_id) as visit_count,
    STRING_AGG(t.trip_name, ', ') as trip_names
FROM cities ct
JOIN countries c ON ct.country_id = c.country_id
JOIN trip_destinations td ON ct.city_id = td.city_id
JOIN trips t ON td.trip_id = t.trip_id
GROUP BY ct.city_name, c.country_name
ORDER BY visit_count DESC;

-- 10. Путешествия по сезонам
SELECT 
    CASE 
        WHEN EXTRACT(MONTH FROM start_date) IN (12, 1, 2) THEN 'Зима'
        WHEN EXTRACT(MONTH FROM start_date) IN (3, 4, 5) THEN 'Весна'
        WHEN EXTRACT(MONTH FROM start_date) IN (6, 7, 8) THEN 'Лето'
        WHEN EXTRACT(MONTH FROM start_date) IN (9, 10, 11) THEN 'Осень'
    END as season,
    COUNT(*) as trips_count,
    SUM(actual_spent) as total_spent,
    AVG(actual_spent) as avg_spent
FROM trips
WHERE trip_status = 'completed'
GROUP BY season
ORDER BY trips_count DESC;

-- 11. Длительность пребывания в каждом городе
SELECT 
    ct.city_name,
    c.country_name,
    COUNT(*) as times_visited,
    SUM(td.departure_date - td.arrival_date) as total_days,
    AVG(td.departure_date - td.arrival_date) as avg_days_per_visit
FROM trip_destinations td
JOIN cities ct ON td.city_id = ct.city_id
JOIN countries c ON ct.country_id = c.country_id
GROUP BY ct.city_name, c.country_name
ORDER BY total_days DESC;

-- 12. Поездки с самым большим количеством городов в маршруте
SELECT 
    t.trip_name,
    COUNT(DISTINCT td.city_id) as cities_count,
    STRING_AGG(ct.city_name, ' → ') as route
FROM trips t
JOIN trip_destinations td ON t.trip_id = td.trip_id
JOIN cities ct ON td.city_id = ct.city_id
GROUP BY t.trip_id, t.trip_name
ORDER BY cities_count DESC;

-- 13. Самые популярные типы транспорта
SELECT 
    td.transport_type,
    COUNT(*) as usage_count,
    STRING_AGG(DISTINCT ct.city_name, ', ') as destinations
FROM trip_destinations td
JOIN cities ct ON td.city_id = ct.city_id
WHERE td.transport_type IS NOT NULL
GROUP BY td.transport_type
ORDER BY usage_count DESC;

-- 14. Стоимость на человека
SELECT 
    t.trip_name,
    COUNT(tc.companion_id) as participants_count,
    t.total_budget,
    t.actual_spent,
    ROUND(t.actual_spent / NULLIF(COUNT(tc.companion_id), 0), 2) as cost_per_person
FROM trips t
LEFT JOIN trip_companions tc ON t.trip_id = tc.trip_id
WHERE t.actual_spent > 0
GROUP BY t.trip_id, t.trip_name, t.total_budget, t.actual_spent
ORDER BY cost_per_person DESC;

-- 15. Поиск по тегам фотографий
SELECT 
    p.photo_name,
    p.description,
    p.rating,
    p.taken_date,
    ct.city_name,
    c.country_name,
    t.trip_name
FROM photos p
JOIN trip_destinations td ON p.trip_destination_id = td.trip_destination_id
JOIN trips t ON td.trip_id = t.trip_id
JOIN cities ct ON td.city_id = ct.city_id
JOIN countries c ON ct.country_id = c.country_id
WHERE p.tags::text LIKE '%море%'  -- поиск по тегу "море"
ORDER BY p.rating DESC;

-- 16. Статистика по странам (нужна ли виза)
SELECT 
    c.country_name,
    c.visa_required,
    COUNT(DISTINCT t.trip_id) as trips_count,
    SUM(t.actual_spent) as total_spent,
    STRING_AGG(DISTINCT t.trip_name, ', ') as trip_list
FROM countries c
JOIN cities ct ON c.country_id = ct.country_id
JOIN trip_destinations td ON ct.city_id = td.city_id
JOIN trips t ON td.trip_id = t.trip_id
GROUP BY c.country_name, c.visa_required
ORDER BY c.visa_required, trips_count DESC;

-- 17. Поездки с разбивкой по континентам
SELECT 
    c.continent,
    COUNT(DISTINCT t.trip_id) as trips_count,
    COUNT(DISTINCT td.city_id) as cities_count,
    SUM(t.actual_spent) as total_spent,
    STRING_AGG(DISTINCT c2.country_name, ', ') as countries
FROM trips t
JOIN trip_destinations td ON t.trip_id = td.trip_id
JOIN cities ct ON td.city_id = ct.city_id
JOIN countries c ON ct.country_id = c.country_id
JOIN countries c2 ON ct.country_id = c2.country_id
GROUP BY c.continent
ORDER BY trips_count DESC;

-- 18. Создание представления для статистики поездок по годам
CREATE OR REPLACE VIEW trips_year_statistics AS
SELECT 
    EXTRACT(YEAR FROM start_date) as travel_year,
    COUNT(*) as trips_count,
    SUM(actual_spent) as total_spent,
    ROUND(AVG(actual_spent), 2) as avg_spent_per_trip,
    MIN(start_date) as first_trip_date,
    MAX(end_date) as last_trip_date,
    STRING_AGG(trip_name, '; ' ORDER BY start_date) as trips_list,
    -- Дополнительная статистика
    COUNT(DISTINCT EXTRACT(MONTH FROM start_date)) as months_with_travel,
    SUM(end_date - start_date) as total_travel_days,
    ROUND(AVG(end_date - start_date), 1) as avg_trip_duration
FROM trips
WHERE trip_status = 'completed'
GROUP BY EXTRACT(YEAR FROM start_date)
ORDER BY travel_year DESC;

-- Использование представления
SELECT * FROM trips_year_statistics;

-- 19. Фотографии по поездкам
CREATE OR REPLACE VIEW simple_trip_photos AS
SELECT 
    t.trip_name,
    ct.city_name || ', ' || c.country_name as location,
    p.photo_name,
    p.taken_date,
    p.rating,
    p.description
FROM photos p
JOIN trip_destinations td ON p.trip_destination_id = td.trip_destination_id
JOIN trips t ON td.trip_id = t.trip_id
JOIN cities ct ON td.city_id = ct.city_id
JOIN countries c ON ct.country_id = c.country_id
ORDER BY p.taken_date DESC;

SELECT * FROM simple_trip_photos;

-- 20. Список рассходов
CREATE OR REPLACE VIEW simple_expenses_list AS
SELECT 
    e.expense_id,
    t.trip_name,
    e.expense_date,
    e.amount,
    e.currency,
    e.category,
    e.description,
    e.payment_method,
    c.city_name
FROM expenses e
JOIN trip_destinations td ON e.trip_destination_id = td.trip_destination_id
JOIN trips t ON td.trip_id = t.trip_id
JOIN cities c ON td.city_id = c.city_id
ORDER BY e.expense_date DESC;

SELECT * FROM simple_expenses_list;