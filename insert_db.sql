TRUNCATE TABLE countries CASCADE;
TRUNCATE TABLE cities CASCADE;
TRUNCATE TABLE expenses CASCADE;
TRUNCATE TABLE photos CASCADE;
TRUNCATE TABLE travel_companions CASCADE;
TRUNCATE TABLE trip_companions CASCADE;
TRUNCATE TABLE trip_destinations CASCADE;
TRUNCATE TABLE trips CASCADE;

-- 1. Таблица стран
INSERT INTO countries VALUES
(1, 'Российская Федерация', 'RUS', 'Европа', false),
(2, 'Чехия', 'CZE', 'Европа', true),
(3, 'Турция', 'TUR', 'Европа', false),
(4, 'Египет', 'EGY', 'Африка', false),
(5, 'Тайланд', 'THA', 'Азия', false),
(6, 'Китай', 'CHN', 'Азия', false),
(7, 'Мальдивы', 'MDV', 'Азия', false),
(8, 'Кыргызстан', 'KGZ', 'Азия', false),
(9, 'Гонконг', 'HKG', 'Азия', true),
(10, 'Беларусь', 'BLR', 'Европа', false);

-- 2. Таблица городов
INSERT INTO cities VALUES
(1, 'Москва', 1, 13300000, 'UTC+3', true),
(2, 'Сочи', 1, 600000, 'UTC+3', true),
(3, 'Новосибирск', 1, 1600000, 'UTC+7', true),
(4, 'Рязань', 1, 500000, 'UTC+3', false),
(5, 'Прага', 2, 1400000, 'UTC+1', true),
(6, 'Карловы Вары', 2, 50000, 'UTC+1', true),
(7, 'Анталия', 3, 1500000, 'UTC+3', true),
(8, 'Хургада', 4, 200000, 'UTC+2', true),
(9, 'Шарм-эш-Шейх', 4, 70000, 'UTC+2', true),
(10, 'Бангкок', 5, 10500000, 'UTC+7', true),
(11, 'Пхукет', 5, 500000, 'UTC+7', true),
(12, 'Чжухай', 6, 2500000, 'UTC+8', true),
(13, 'Мале', 7, 200000, 'UTC+5', true),
(14, 'Бишкек', 8, 1300000, 'UTC+6', true),
(15, 'Гонконг', 9, 7500000, 'UTC+8', true),
(16, 'Минск', 10, 2000000, 'UTC+3', true);

-- 3. Поездки (trips) с обновленными датами
INSERT INTO trips (trip_id, trip_name, start_date, end_date, total_budget, trip_status, notes) VALUES
(1, 'Отпуск в Чехии', '2017-07-15', '2017-07-30', 150000.00, 'completed', 'Поездка по историческим местам Чехии'),
(2, 'Зимний отдых в Египте', '2014-02-10', '2014-02-24', 120000.00, 'completed', 'Пляжный отдых в Хургаде'),
(3, 'Экзотический Тайланд', '2020-03-10', '2020-03-22', 200000.00, 'completed', 'Тур по Бангкоку и Пхукету'),
(4, 'Быстрая поездка в Беларусь', '2005-11-05', '2005-11-09', 50000.00, 'completed', 'Визит к родственникам в Минск'),
(5, 'Лечение в Карловых Варах', '2019-10-01', '2019-10-14', 180000.00, 'completed', 'Санаторно-курортное лечение'),
(6, 'Мальдивская мечта', '2022-07-15', '2022-07-22', 250000.00, 'completed', 'Роскошный отдых на Мальдивах'),
(7, 'Учебная поездка в Китай', '2026-01-03', '2026-01-16', 80000.00, 'planned', 'Учеба в BIT в Чжухае'),
(8, 'Семейный отдых в Турции', '2023-08-03', '2023-08-12', 170000.00, 'completed', 'Отдых с семьей в Анталии'),
(9, 'Родственные визиты по России', '2023-12-31', '2024-01-07', 40000.00, 'completed', 'Посещение родственников в разных городах'),
(10, 'Посещение Гонконга', '2015-11-14', '2015-11-17', 150000.00, 'completed', 'Знакомство с Гонконгом');

-- 4. Места посещения в поездках (trip_destinations) с обновленными датами
INSERT INTO trip_destinations (trip_destination_id, trip_id, city_id, visit_order, arrival_date, departure_date, accommodation, transport_type) VALUES
-- Поездка в Чехию (ID 1) - 2017
(1, 1, 5, 1, '2017-07-15', '2017-07-25', 'Отель Hilton Prague', 'plane'),
(2, 1, 6, 2, '2017-07-25', '2017-07-30', 'Санаторий Carlsbad Plaza', 'train'),

-- Египет (ID 2) - 2014
(3, 2, 8, 1, '2014-02-10', '2014-02-24', 'Steigenberger Alcazar Resort', 'plane'),

-- Тайланд (ID 3) - 2020
(4, 3, 10, 1, '2020-03-10', '2020-03-11', 'Bangkok Marriott Hotel', 'plane'),
(5, 3, 11, 2, '2020-03-11', '2020-03-22', 'Phuket Marriott Resort', 'plane'),

-- Беларусь (ID 4) - 2005
(6, 4, 16, 1, '2005-11-05', '2005-11-09', 'Квартира родственников', 'train'),

-- Карловы Вары (ID 5) - 2019
(7, 5, 6, 1, '2019-10-01', '2019-10-14', 'Санаторий Thermal', 'plane'),

-- Мальдивы (ID 6) - 2022
(8, 6, 13, 1, '2022-07-15', '2022-07-22', 'Huvafen Fushi Overwater Villa', 'plane'),

-- Китай (ID 7) - 2026
(9, 7, 12, 1, '2026-01-03', '2026-01-16', 'Marriott Hotel Zhuhai', 'plane'),

-- Турция (ID 8) - 2023
(10, 8, 7, 1, '2023-08-03', '2023-08-12', 'Rixos Premium Belek', 'plane'),

-- Россия (ID 9) - 2023-2024
(11, 9, 1, 1, '2023-12-31', '2024-01-03', 'Квартира в Москве', 'train'),
(12, 9, 3, 2, '2024-01-03', '2024-01-06', 'Гостиница Новосибирск', 'plane'),
(13, 9, 4, 3, '2024-01-06', '2024-01-07', 'Дом родственников', 'train'),

-- Гонконг (ID 10) - 2015
(14, 10, 15, 1, '2015-11-14', '2015-11-17', 'The Peninsula Hong Kong', 'plane');

-- 5. Расходы (expenses) с обновленными датами
INSERT INTO expenses (expense_id, trip_destination_id, expense_date, amount, currency, category, description, payment_method) VALUES
-- Расходы для поездки в Чехию (trip_id = 1) - 2017
(1, 1, '2017-07-15', 50000.00, 'RUB', 'transport', 'Авиабилеты Москва-Прага', 'card'),
(2, 1, '2017-07-16', 30000.00, 'RUB', 'accommodation', 'Отель Прага - первый взнос', 'card'),
(3, 1, '2017-07-17', 15000.00, 'RUB', 'food', 'Ужин в ресторане', 'cash'),
(4, 1, '2017-07-18', 5000.00, 'RUB', 'sightseeing', 'Экскурсия по Пражскому граду', 'card'),
(5, 2, '2017-07-25', 20000.00, 'RUB', 'transport', 'Поезд Прага-Карловы Вары', 'card'),
(6, 2, '2017-07-26', 25000.00, 'RUB', 'accommodation', 'Санаторий Карловы Вары', 'card'),

-- Расходы для Египта (trip_id = 2) - 2014
(7, 3, '2014-02-10', 60000.00, 'RUB', 'transport', 'Авиабилеты Москва-Хургада', 'card'),
(8, 3, '2014-02-11', 40000.00, 'RUB', 'accommodation', 'Отель Хургада', 'card'),
(9, 3, '2014-02-12', 10000.00, 'RUB', 'entertainment', 'Экскурсия на коралловый риф', 'cash'),

-- Расходы для Тайланда (trip_id = 3) - 2020
(10, 4, '2020-02-20', 80000.00, 'RUB', 'transport', 'Предоплата за авиабилеты', 'card'),

-- Расходы для Беларуси (trip_id = 4) - 2005
(11, 6, '2005-11-05', 15000.00, 'RUB', 'transport', 'ЖД билеты Москва-Минск', 'card'),
(12, 6, '2005-11-06', 5000.00, 'RUB', 'food', 'Продукты для семьи', 'cash'),

-- Расходы для Карловых Вар (trip_id = 5) - 2019
(13, 7, '2019-09-20', 90000.00, 'RUB', 'accommodation', 'Предоплата санатория', 'card'),
(14, 7, '2019-10-01', 30000.00, 'RUB', 'transport', 'Авиабилеты', 'card'),

-- Расходы для Китая (trip_id = 7) - 2026
(15, 9, '2025-12-20', 50000.00, 'RUB', 'transport', 'Предоплата авиабилетов Москва-Чжухай', 'card'),
(16, 9, '2026-01-03', 20000.00, 'RUB', 'accommodation', 'Отель в Чжухае', 'card'),

-- Расходы для России (trip_id = 9) - 2023-2024
(17, 11, '2023-12-31', 10000.00, 'RUB', 'transport', 'Поезд Москва-Новосибирск', 'card'),
(18, 12, '2024-01-04', 8000.00, 'RUB', 'transport', 'Авиабилеты Новосибирск-Рязань', 'card'),

-- Расходы для Гонконга (trip_id = 10) - 2015
(19, 14, '2015-11-01', 70000.00, 'RUB', 'transport', 'Авиабилеты Москва-Гонконг', 'card'),
(20, 14, '2015-11-14', 30000.00, 'RUB', 'shopping', 'Электроника в Duty Free', 'card');

-- 6. Фотографии (photos) с обновленными датами
INSERT INTO photos (photo_id, trip_destination_id, photo_name, file_path, taken_date, description, rating, tags) VALUES
(1, 1, 'prague_castle.jpg', '/photos/czech/2017/prague_castle.jpg', '2017-07-17', 'Пражский Град вечером', 5, '["Прага", "замок", "вечер", "архитектура"]'),
(2, 1, 'charles_bridge.jpg', '/photos/czech/2017/charles_bridge.jpg', '2017-07-18', 'Карлов мост на рассвете', 4, '["Прага", "мост", "рассвет", "река"]'),
(3, 2, 'karlovy_vary_spring.jpg', '/photos/czech/2017/karlovy_vary_spring.jpg', '2017-07-26', 'Гейзер в Карловых Варах', 5, '["Карловы Вары", "гейзер", "лечение", "курорт"]'),
(4, 3, 'hurghada_beach.jpg', '/photos/egypt/2014/hurghada_beach.jpg', '2014-02-12', 'Пляж в Хургаде', 4, '["Египет", "пляж", "море", "отдых"]'),
(5, 3, 'coral_reef.jpg', '/photos/egypt/2014/coral_reef.jpg', '2014-02-13', 'Коралловый риф', 5, '["Египет", "кораллы", "дайвинг", "море"]'),
(6, 6, 'minsk_church.jpg', '/photos/belarus/2005/minsk_church.jpg', '2005-11-07', 'Церковь в Минске', 3, '["Беларусь", "Минск", "церковь", "архитектура"]'),
(7, 7, 'thermal_spa.jpg', '/photos/czech/2019/thermal_spa.jpg', '2019-10-05', 'Термальный бассейн', 4, '["Карловы Вары", "спа", "лечение", "термальные воды"]'),
(8, 9, 'zhuhai_business.jpg', '/photos/china/2026/zhuhai_business.jpg', '2026-01-12', 'Бизнес-центр в Чжухае', 3, '["Китай", "Чжухай", "бизнес", "небоскребы"]'),
(9, 11, 'moscow_kremlin.jpg', '/photos/russia/2023/moscow_kremlin.jpg', '2023-12-31', 'Московский Кремль', 5, '["Россия", "Москва", "Кремль", "история"]'),
(10, 12, 'novosibirsk_opera.jpg', '/photos/russia/2024/novosibirsk_opera.jpg', '2024-01-04', 'Оперный театр в Новосибирске', 4, '["Россия", "Новосибирск", "театр", "архитектура"]');

-- 7. Сопутешественники (travel_companions)
INSERT INTO travel_companions (companion_id, first_name, last_name, email, phone, contact_emergency, notes) VALUES
(1, 'Sergei', 'Mitrofanov', 'sermerl2@outlook.com', '8-926-780-65-73', '8-926-542-88-88', 'myself'),
(2, 'Tatyana', 'Mitrofanova', 'tan_mit@fake.com', '8-926-780-88-88', '8-926-542-88-88', 'sibling'),
(3, 'Pavel', 'Mitrofanov', 'pav_mit@fake.com', '8-926-542-88-88', '8-926-542-88-88', 'father'),
(4, 'Natalia', 'Sukhoverhova', 'net_sukh@fake.com', '8-926-542-99-99', '8-926-542-88-88', 'mother'),
(5, 'Galina', 'Mitrofanova', 'gal_mit@fake.com', '8-926-542-52-52', '8-926-542-88-88', 'grandmother'),
(6, 'Vladimir', 'Mitrofanov', 'vladim_mit@fake.com', '8-926-542-47-47', '8-926-542-88-88', 'grandfather'),
(7, 'Danila', 'Mitrofanov', 'dan_mit@fake.com', '8-926-780-88-14', '8-926-542-14-14', 'cousin'),
(8, 'Denis', 'Mitrofanov', 'den_mit@fake.com', '8-926-542-14-14', '8-926-542-14-14', 'uncle'),
(9, 'Student', 'Mietov', 'stud_miet@fake.com', '8-926-000-11-22', '8-926-000-11-11', 'random student'),
(10, 'Vasilisa', 'Mitrofanova', NULL, NULL, '8-926-542-14-14', 'cat');

-- 8. Связь поездок и сопутешественников (trip_companions) с обновленными датами
INSERT INTO trip_companions (trip_id, companion_id, role, joined_date, left_date, share_expenses) VALUES
-- Поездка в Чехию (trip_id = 1) - 2017
(1, 1, 'organizer', '2017-07-15', '2017-07-30', true),
(1, 2, 'companion', '2017-07-15', '2017-07-30', true),
(1, 3, 'companion', '2017-07-15', '2017-07-30', true),

-- Египет (trip_id = 2) - 2014
(2, 1, 'organizer', '2014-02-10', '2014-02-24', true),
(2, 4, 'companion', '2014-02-10', '2014-02-24', true),

-- Тайланд (trip_id = 3) - 2020
(3, 1, 'organizer', '2020-03-10', '2020-03-22', true),
(3, 2, 'companion', '2020-03-10', '2020-03-22', true),
(3, 7, 'companion', '2020-03-10', '2020-03-22', true),

-- Беларусь (trip_id = 4) - 2005
(4, 1, 'organizer', '2005-11-05', '2005-11-09', false),
(4, 5, 'companion', '2005-11-05', '2005-11-09', false),
(4, 6, 'companion', '2005-11-05', '2005-11-09', false),

-- Карловы Вары (trip_id = 5) - 2019
(5, 1, 'organizer', '2019-10-01', '2019-10-14', true),
(5, 3, 'companion', '2019-10-01', '2019-10-14', true),

-- Мальдивы (trip_id = 6) - 2022
(6, 1, 'organizer', '2022-07-15', '2022-07-22', true),
(6, 2, 'companion', '2022-07-15', '2022-07-22', true),

-- Китай (trip_id = 7) - 2026
(7, 1, 'organizer', '2026-01-03', '2026-01-16', true),
(7, 9, 'companion', '2026-01-03', '2026-01-16', false),

-- Турция (trip_id = 8) - 2023
(8, 1, 'organizer', '2023-08-03', '2023-08-12', true),
(8, 2, 'companion', '2023-08-03', '2023-08-12', true),
(8, 3, 'companion', '2023-08-03', '2023-08-12', true),
(8, 4, 'companion', '2023-08-03', '2023-08-12', true),

-- Россия (trip_id = 9) - 2023-2024
(9, 1, 'organizer', '2023-12-31', '2024-01-07', true),
(9, 8, 'companion', '2023-12-31', '2024-01-07', true),

-- Гонконг (trip_id = 10) - 2015
(10, 1, 'organizer', '2015-11-14', '2015-11-17', true);

SELECT * FROM countries;