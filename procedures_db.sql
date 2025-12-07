CREATE OR REPLACE PROCEDURE add_new_trip(
    p_trip_name VARCHAR(200),
    p_start_date DATE,
    p_end_date DATE,
    p_total_budget DECIMAL(12,2),
    p_trip_status VARCHAR(20) DEFAULT 'planned',
    p_notes TEXT DEFAULT NULL
)
-- 1. Добавление новой поездки
LANGUAGE plpgsql
AS $$
BEGIN
    -- Проверка дат
    IF p_end_date < p_start_date THEN
        RAISE EXCEPTION 'Дата окончания не может быть раньше даты начала';
    END IF;
    
    -- Проверка бюджета
    IF p_total_budget < 0 THEN
        RAISE EXCEPTION 'Бюджет не может быть отрицательным';
    END IF;
    
    -- Проверка статуса
    IF p_trip_status NOT IN ('planned', 'in_progress', 'completed', 'cancelled') THEN
        RAISE EXCEPTION 'Недопустимый статус поездки';
    END IF;
    
    -- Вставка данных
    INSERT INTO trips (trip_name, start_date, end_date, total_budget, trip_status, notes)
    VALUES (p_trip_name, p_start_date, p_end_date, p_total_budget, p_trip_status, p_notes);
    
    RAISE NOTICE 'Поездка "%" успешно добавлена!', p_trip_name;
END;
$$;

-- 2. Добавление участника в поездку
CREATE OR REPLACE PROCEDURE add_companion_to_trip(
    p_trip_id INT,
    p_companion_id INT,
    p_role VARCHAR(50) DEFAULT 'companion',
    p_joined_date DATE DEFAULT NULL,
    p_share_expenses BOOLEAN DEFAULT TRUE
)
LANGUAGE plpgsql
AS $$
DECLARE
    v_trip_start_date DATE;
    v_trip_end_date DATE;
BEGIN
    -- Получаем даты поездки
    SELECT start_date, end_date INTO v_trip_start_date, v_trip_end_date
    FROM trips WHERE trip_id = p_trip_id;
    
    IF NOT FOUND THEN
        RAISE EXCEPTION 'Поездка с ID % не найдена', p_trip_id;
    END IF;
    
    -- Проверяем существование сопутешественника
    IF NOT EXISTS (SELECT 1 FROM travel_companions WHERE companion_id = p_companion_id) THEN
        RAISE EXCEPTION 'Сопутешественник с ID % не найден', p_companion_id;
    END IF;
    
    -- Проверяем, не добавлен ли уже этот участник
    IF EXISTS (SELECT 1 FROM trip_companions 
               WHERE trip_id = p_trip_id AND companion_id = p_companion_id) THEN
        RAISE EXCEPTION 'Этот участник уже добавлен в поездку';
    END IF;
    
    -- Устанавливаем дату присоединения (по умолчанию - начало поездки)
    IF p_joined_date IS NULL THEN
        p_joined_date := v_trip_start_date;
    END IF;
    
    -- Проверяем корректность даты присоединения
    IF p_joined_date < v_trip_start_date OR p_joined_date > v_trip_end_date THEN
        RAISE EXCEPTION 'Дата присоединения должна быть в пределах дат поездки';
    END IF;
    
    -- Проверяем роль
    IF p_role NOT IN ('organizer', 'companion', 'guide') THEN
        RAISE EXCEPTION 'Недопустимая роль участника';
    END IF;
    
    -- Добавляем участника
    INSERT INTO trip_companions (trip_id, companion_id, role, joined_date, share_expenses)
    VALUES (p_trip_id, p_companion_id, p_role, p_joined_date, p_share_expenses);
    
    RAISE NOTICE 'Участник успешно добавлен в поездку!';
END;
$$;

-- 3. Добавление расходов
CREATE OR REPLACE PROCEDURE add_expense(
    p_trip_destination_id INT,
    p_amount DECIMAL(10,2),
    p_category VARCHAR(50),
    p_description VARCHAR(300),
    p_expense_date DATE DEFAULT NULL,
    p_currency VARCHAR(3) DEFAULT 'RUB',
    p_payment_method VARCHAR(50) DEFAULT 'card'
)
LANGUAGE plpgsql
AS $$
DECLARE
    v_trip_id INT;
    v_arrival_date DATE;
    v_departure_date DATE;
BEGIN
    -- Получаем информацию о месте посещения
    SELECT td.trip_id, td.arrival_date, td.departure_date 
    INTO v_trip_id, v_arrival_date, v_departure_date
    FROM trip_destinations td
    WHERE td.trip_destination_id = p_trip_destination_id;
    
    IF NOT FOUND THEN
        RAISE EXCEPTION 'Место посещения с ID % не найдено', p_trip_destination_id;
    END IF;
    
    -- Проверяем сумму
    IF p_amount <= 0 THEN
        RAISE EXCEPTION 'Сумма расхода должна быть положительной';
    END IF;
    
    -- Проверяем категорию
    IF p_category NOT IN ('transport', 'accommodation', 'food', 'entertainment', 'shopping', 'sightseeing', 'other') THEN
        RAISE EXCEPTION 'Недопустимая категория расхода';
    END IF;
    
    -- Устанавливаем дату расхода (по умолчанию - сегодня)
    IF p_expense_date IS NULL THEN
        p_expense_date := CURRENT_DATE;
    END IF;
    
    -- Проверяем, что дата расхода в пределах поездки
    IF p_expense_date < v_arrival_date OR p_expense_date > v_departure_date THEN
        RAISE NOTICE 'Внимание: Дата расхода выходит за пределы пребывания в этом месте';
    END IF;
    
    -- Проверяем метод оплаты
    IF p_payment_method NOT IN ('cash', 'card', 'mobile', 'other') THEN
        RAISE EXCEPTION 'Недопустимый метод оплаты';
    END IF;
    
    -- Добавляем расход
    INSERT INTO expenses (
        trip_destination_id, expense_date, amount, currency, 
        category, description, payment_method
    ) VALUES (
        p_trip_destination_id, p_expense_date, p_amount, p_currency,
        p_category, p_description, p_payment_method
    );
    
    RAISE NOTICE 'Расход "%" на сумму % успешно добавлен!', p_description, p_amount;
END;
$$;

-- 4. Обновление статуса поездки
CREATE OR REPLACE PROCEDURE update_trip_status(
    p_trip_id INT,
    p_new_status VARCHAR(20)
)
LANGUAGE plpgsql
AS $$
DECLARE
    v_old_status VARCHAR(20);
    v_trip_name VARCHAR(200);
BEGIN
    -- Получаем текущий статус и название поездки
    SELECT trip_status, trip_name INTO v_old_status, v_trip_name
    FROM trips WHERE trip_id = p_trip_id;
    
    IF NOT FOUND THEN
        RAISE EXCEPTION 'Поездка с ID % не найдена', p_trip_id;
    END IF;
    
    -- Проверяем новый статус
    IF p_new_status NOT IN ('planned', 'in_progress', 'completed', 'cancelled') THEN
        RAISE EXCEPTION 'Недопустимый статус поездки';
    END IF;
    
    -- Проверяем логику изменения статусов
    IF v_old_status = 'completed' AND p_new_status != 'completed' THEN
        RAISE EXCEPTION 'Нельзя изменить статус завершенной поездки';
    END IF;
    
    IF v_old_status = 'cancelled' AND p_new_status != 'cancelled' THEN
        RAISE EXCEPTION 'Нельзя изменить статус отмененной поездки';
    END IF;
    
    -- Обновляем статус
    UPDATE trips 
    SET trip_status = p_new_status
    WHERE trip_id = p_trip_id;
    
    -- Если поездка завершена, устанавливаем актуальную дату окончания
    IF p_new_status = 'completed' THEN
        UPDATE trips 
        SET end_date = LEAST(end_date, CURRENT_DATE)
        WHERE trip_id = p_trip_id;
    END IF;
    
    RAISE NOTICE 'Статус поездки "%" изменен с "%" на "%"', 
        v_trip_name, v_old_status, p_new_status;
END;
$$;

-- 5. Добавление фотографии
CREATE OR REPLACE PROCEDURE add_photo(
    p_trip_destination_id INT,
    p_photo_name VARCHAR(200),
    p_file_path VARCHAR(500),
    p_taken_date DATE DEFAULT NULL,
    p_description TEXT DEFAULT NULL,
    p_rating INT DEFAULT NULL,
    p_tags JSONB DEFAULT NULL
)
LANGUAGE plpgsql
AS $$
DECLARE
    v_arrival_date DATE;
    v_departure_date DATE;
BEGIN
    -- Проверяем существование места посещения
    SELECT arrival_date, departure_date INTO v_arrival_date, v_departure_date
    FROM trip_destinations 
    WHERE trip_destination_id = p_trip_destination_id;
    
    IF NOT FOUND THEN
        RAISE EXCEPTION 'Место посещения с ID % не найдено', p_trip_destination_id;
    END IF;
    
    -- Устанавливаем дату съемки (по умолчанию - сегодня)
    IF p_taken_date IS NULL THEN
        p_taken_date := CURRENT_DATE;
    END IF;
    
    -- Проверяем, что дата съемки в пределах пребывания
    IF p_taken_date < v_arrival_date OR p_taken_date > v_departure_date THEN
        RAISE NOTICE 'Внимание: Дата съемки выходит за пределы пребывания в этом месте';
    END IF;
    
    -- Проверяем рейтинг
    IF p_rating IS NOT NULL AND (p_rating < 1 OR p_rating > 5) THEN
        RAISE EXCEPTION 'Рейтинг должен быть от 1 до 5';
    END IF;
    
    -- Добавляем фотографию
    INSERT INTO photos (
        trip_destination_id, photo_name, file_path, taken_date, 
        description, rating, tags
    ) VALUES (
        p_trip_destination_id, p_photo_name, p_file_path, p_taken_date,
        p_description, p_rating, p_tags
    );
    
    RAISE NOTICE 'Фотография "%" успешно добавлена!', p_photo_name;
END;
$$;