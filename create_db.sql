-- Создание базы данных
CREATE DATABASE my_travels;

-- Подключение к базе данных (выполняется отдельно)
-- \c my_travels;

-- 1. Таблица стран
CREATE TABLE countries (
    country_id SERIAL PRIMARY KEY,
    country_name VARCHAR(100) NOT NULL UNIQUE,
    iso_code CHAR(3) NOT NULL UNIQUE,
    continent VARCHAR(50) NOT NULL CHECK (continent IN ('Азия', 'Европа', 'Африка', 'Северная Америка', 'Южная Америка', 'Австралия', 'Антарктида')),
    visa_required BOOLEAN NOT NULL DEFAULT FALSE,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- 2. Таблица городов
CREATE TABLE cities (
    city_id SERIAL PRIMARY KEY,
    city_name VARCHAR(100) NOT NULL,
    country_id INT NOT NULL,
    population INT CHECK (population >= 0),
    time_zone VARCHAR(50),
    has_airport BOOLEAN DEFAULT FALSE,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT fk_cities_countries FOREIGN KEY (country_id) 
        REFERENCES countries(country_id) ON DELETE CASCADE,
    CONSTRAINT unique_city_country UNIQUE (city_name, country_id)
);

-- 3. Таблица поездок
CREATE TABLE trips (
    trip_id SERIAL PRIMARY KEY,
    trip_name VARCHAR(200) NOT NULL,
    start_date DATE NOT NULL,
    end_date DATE NOT NULL,
    total_budget DECIMAL(12,2) CHECK (total_budget >= 0),
    actual_spent DECIMAL(12,2) DEFAULT 0,
    trip_status VARCHAR(20) DEFAULT 'planned' CHECK (trip_status IN ('planned', 'in_progress', 'completed', 'cancelled')),
    notes TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT chk_dates CHECK (end_date >= start_date)
);

-- 4. Таблица посещаемых мест в поездке (связь многие-ко-многим)
CREATE TABLE trip_destinations (
    trip_destination_id SERIAL PRIMARY KEY,
    trip_id INT NOT NULL,
    city_id INT NOT NULL,
    visit_order INT NOT NULL CHECK (visit_order > 0),
    arrival_date DATE NOT NULL,
    departure_date DATE NOT NULL,
    accommodation VARCHAR(200),
    transport_type VARCHAR(50) CHECK (transport_type IN ('plane', 'train', 'bus', 'car', 'ship', 'other')),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT fk_tripdest_trips FOREIGN KEY (trip_id) 
        REFERENCES trips(trip_id) ON DELETE CASCADE,
    CONSTRAINT fk_tripdest_cities FOREIGN KEY (city_id) 
        REFERENCES cities(city_id) ON DELETE CASCADE,
    CONSTRAINT chk_dest_dates CHECK (departure_date >= arrival_date),
    CONSTRAINT unique_trip_city_order UNIQUE (trip_id, visit_order)
);

-- 5. Таблица расходов
CREATE TABLE expenses (
    expense_id SERIAL PRIMARY KEY,
    trip_destination_id INT NOT NULL,
    expense_date DATE NOT NULL,
    amount DECIMAL(10,2) NOT NULL CHECK (amount > 0),
    currency VARCHAR(3) NOT NULL DEFAULT 'USD',
    category VARCHAR(50) NOT NULL CHECK (category IN ('transport', 'accommodation', 'food', 'entertainment', 'shopping', 'sightseeing', 'other')),
    description VARCHAR(300) NOT NULL,
    payment_method VARCHAR(50) CHECK (payment_method IN ('cash', 'card', 'mobile', 'other')),
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT fk_expenses_tripdest FOREIGN KEY (trip_destination_id) 
        REFERENCES trip_destinations(trip_destination_id) ON DELETE CASCADE
);

-- 6. Таблица фотографий
CREATE TABLE photos (
    photo_id SERIAL PRIMARY KEY,
    trip_destination_id INT NOT NULL,
    photo_name VARCHAR(200) NOT NULL,
    file_path VARCHAR(500) NOT NULL,
    taken_date DATE NOT NULL,
    description TEXT,
    rating INT CHECK (rating >= 1 AND rating <= 5),
    tags JSONB,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT fk_photos_tripdest FOREIGN KEY (trip_destination_id) 
        REFERENCES trip_destinations(trip_destination_id) ON DELETE CASCADE
);

-- 7. Таблица сопутешественников
CREATE TABLE travel_companions (
    companion_id SERIAL PRIMARY KEY,
    first_name VARCHAR(100) NOT NULL,
    last_name VARCHAR(100) NOT NULL,
    email VARCHAR(150) UNIQUE,
    phone VARCHAR(20),
    contact_emergency VARCHAR(200),
    notes TEXT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT chk_email_phone CHECK (email IS NOT NULL OR phone IS NOT NULL OR contact_emergency IS NOT NULL)
);

-- 8. Таблица связи сопутешественников и поездок (many-to-many)
CREATE TABLE trip_companions (
    trip_id INT NOT NULL,
    companion_id INT NOT NULL,
    role VARCHAR(50) DEFAULT 'companion' CHECK (role IN ('organizer', 'companion', 'guide')),
    joined_date DATE NOT NULL,
    left_date DATE,
    share_expenses BOOLEAN DEFAULT TRUE,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
	PRIMARY KEY (trip_id, companion_id),
    CONSTRAINT fk_tripcomp_trips FOREIGN KEY (trip_id) 
        REFERENCES trips(trip_id) ON DELETE CASCADE,
    CONSTRAINT fk_tripcomp_companions FOREIGN KEY (companion_id) 
        REFERENCES travel_companions(companion_id) ON DELETE CASCADE,
    CONSTRAINT chk_join_leave_dates CHECK (left_date IS NULL OR left_date >= joined_date),
    CONSTRAINT unique_trip_companion UNIQUE (trip_id, companion_id)
);

-- ТРИГГЕР: Автоматическое вычисление общей суммы расходов в поездке
CREATE OR REPLACE FUNCTION update_trip_total_spent()
RETURNS TRIGGER AS $$
DECLARE
    total DECIMAL(12,2);
    trip_id_val INT;
BEGIN
    -- Определяем trip_id для изменяемой записи расходов
    IF TG_OP = 'INSERT' OR TG_OP = 'UPDATE' THEN
        SELECT td.trip_id INTO trip_id_val
        FROM trip_destinations td
        WHERE td.trip_destination_id = NEW.trip_destination_id;
    ELSE
        SELECT td.trip_id INTO trip_id_val
        FROM trip_destinations td
        WHERE td.trip_destination_id = OLD.trip_destination_id;
    END IF;
    
    -- Суммируем все расходы для данной поездки
    SELECT COALESCE(SUM(e.amount), 0) INTO total
    FROM expenses e
    JOIN trip_destinations td ON e.trip_destination_id = td.trip_destination_id
    WHERE td.trip_id = trip_id_val;
    
    -- Обновляем поле actual_spent в таблице trips
    UPDATE trips 
    SET actual_spent = total 
    WHERE trip_id = trip_id_val;
    
    RETURN COALESCE(NEW, OLD);
END;
$$ LANGUAGE plpgsql;

-- Создание триггеров для INSERT, UPDATE и DELETE
CREATE TRIGGER trigger_update_trip_total_spent
    AFTER INSERT OR UPDATE OR DELETE ON expenses
    FOR EACH ROW
    EXECUTE FUNCTION update_trip_total_spent();

-- Создание индексов для улучшения производительности
CREATE INDEX idx_trip_destinations_trip_id ON trip_destinations(trip_id);
CREATE INDEX idx_trip_destinations_city_id ON trip_destinations(city_id);
CREATE INDEX idx_expenses_trip_dest_id ON expenses(trip_destination_id);
CREATE INDEX idx_expenses_date ON expenses(expense_date);
CREATE INDEX idx_photos_trip_dest_id ON photos(trip_destination_id);
CREATE INDEX idx_cities_country_id ON cities(country_id);

-- Индексы для новых таблиц
CREATE INDEX idx_trip_companions_trip_id ON trip_companions(trip_id);
CREATE INDEX idx_trip_companions_companion_id ON trip_companions(companion_id);
CREATE INDEX idx_companions_email ON travel_companions(email);
CREATE INDEX idx_companions_name ON travel_companions(first_name, last_name);

-- Дополнительные индексы для PostgreSQL
CREATE INDEX idx_trips_dates ON trips(start_date, end_date);
CREATE INDEX idx_trip_destinations_dates ON trip_destinations(arrival_date, departure_date);
CREATE INDEX idx_expenses_category ON expenses(category);
CREATE INDEX idx_trip_companions_dates ON trip_companions(joined_date, left_date);