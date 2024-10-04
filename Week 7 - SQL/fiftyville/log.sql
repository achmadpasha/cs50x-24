-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Theft took place on July 28, 2023 and that it took place on Humphrey Street.


-- Find the crime scene reports description
-- +--------------------------------------------------------------------------------------+
-- |                                      descripton                                      |
-- +--------------------------------------------------------------------------------------+
-- | 1. Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery.       |
-- | 2. Interviews were conducted today with three witnesses who were present at the time |
-- |    each of their interview transcripts mentions the bakery.                          |
-- | 3. Littering took place at 16:36. No known witnesses.                                |
-- +--------------------------------------------------------------------------------------+
SELECT description
FROM crime_scene_reports
WHERE street = 'Humphrey Street'
AND day = 28
AND month = 7
AND year = 2023;


-- Find the transcripts of the three witnesses who mentioned the bakery
-- +----------+---------------------------------------------------------------------------------------------------------------------+
-- |   name   |                                                    transcript                                                       |
-- +----------+---------------------------------------------------------------------------------------------------------------------+
-- | Ruth     | Ruth saw the thief get into a car in the bakery parking lot and drive away.                                         |
-- | Eugene   | At Earlier morning, Eugene was walking by the ATM on Leggett Street and saw the thief there withdrawing some money. |
-- | Raymond  | As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call,    |
-- |          | Raymond heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.         |
-- +----------+---------------------------------------------------------------------------------------------------------------------+
SELECT name, transcript
FROM interviews
WHERE transcript LIKE '%bakery%'
AND day = 28
AND month = 7
AND year = 2023;


-- Find cars that leave the bakery between 10:15am and 10:25am.
-- +------+--------+----------+---------------+
-- | hour | minute | activity | license_plate |
-- +------+--------+----------+---------------+
-- | 10   | 16     | exit     | 5P2BI95       |
-- | 10   | 18     | exit     | 94KL13X       |
-- | 10   | 18     | exit     | 6P58WS2       |
-- | 10   | 19     | exit     | 4328GD8       |
-- | 10   | 20     | exit     | G412CB7       |
-- | 10   | 21     | exit     | L93JTIZ       |
-- | 10   | 23     | exit     | 322W7JE       |
-- | 10   | 23     | exit     | 0NTHK55       |
-- +------+--------+----------+---------------+
SELECT hour, minute, activity, license_plate
FROM bakery_security_logs
WHERE day = 28
AND month = 7
AND year = 2023
AND hour = 10
AND minute >= 15
AND minute <= 25;


-- Find call history with less than a minute duration
-- +----------------+----------------+----------+
-- |     caller     |    receiver    | duration |
-- +----------------+----------------+----------+
-- | (130) 555-0289 | (996) 555-8899 | 51       |
-- | (499) 555-9472 | (892) 555-8872 | 36       |
-- | (367) 555-5533 | (375) 555-8161 | 45       |
-- | (499) 555-9472 | (717) 555-1342 | 50       |
-- | (770) 555-1861 | (725) 555-3243 | 49       |
-- +----------------+----------------+----------+
SELECT caller, receiver, duration
FROM phone_calls
WHERE duration < 60
AND day = 28
AND month = 7
AND year = 2023
AND caller IN
    (
        SELECT phone_number
        FROM people
        JOIN bakery_security_logs
        ON people.license_plate = bakery_security_logs.license_plate
        WHERE day = 28
        AND month = 7
        AND year = 2023
        AND hour = 10
        AND minute >= 15
        AND minute <= 25
    );


-- Find the airport of Fiftyville with its destinations and the earliest flight IDs.
-- +----+-----------------------------+---------------+-----------+-----------+----------------+------+-------+-----+------+--------+
-- | id |          full_name          |     city      | flight_id | origin_id | destination_id | year | month | day | hour | minute |
-- +----+-----------------------------+---------------+-----------+-----------+----------------+------+-------+-----+------+--------+
-- | 8  | Fiftyville Regional Airport |  Fiftyville   | 36        | 8         | 4              | 2023 | 7     | 29  | 8    | 20     |
-- +----+-----------------------------+---------------+-----------+-----------+----------------+------+-------+-----+------+--------+
SELECT a.id, a.full_name, a.city, f.id, f.origin_airport_id, f.destination_airport_id, f.year, f.month,f.day, f.hour, f.minute
FROM airports AS a
JOIN flights AS f
ON a.id = f.origin_airport_id
WHERE day = 29
AND month = 7
AND year = 2023
ORDER BY hour ASC
LIMIT 1;

-- Find the city where the thief escaped
-- +----+-----------------------------+---------------+
-- | id |          full_name          |     city      |
-- +----+-----------------------------+---------------+
-- | 4  |      LaGuardia Airport      | New York City |
-- +----+-----------------------------+---------------+
SELECT id, full_name, city
FROM airports
WHERE id = 4;


-- Find passengers based on their call history and activity at the bakery
-- +--------+--------+-----------+-----------------+------+
-- |   id   |  name  | flight_id | passport_number | seat |
-- +--------+--------+-----------+-----------------+------+
-- | 398010 | Sofia  | 36        | 1695452385      | 3B   |
-- | 560886 | Kelsey | 36        | 8294398571      | 6C   |
-- | 686048 | Bruce  | 36        | 5773159633      | 4A   |
-- +--------+--------+-----------+-----------------+------+
SELECT id, name, pass.flight_id, pass.passport_number, seat
FROM people
JOIN passengers AS pass ON people.passport_number = pass.passport_number
WHERE people.passport_number IN
    (
        SELECT passport_number
        FROM passengers
        WHERE flight_id = 36
        AND passport_number IN
            (
                SELECT passport_number
                FROM people
                WHERE phone_number IN
                    (
                        SELECT caller
                        FROM phone_calls
                        WHERE duration < 60
                        AND day = 28
                        AND month = 7
                        AND year = 2023
                        AND caller IN
                            (
                                SELECT phone_number
                                FROM people
                                JOIN bakery_security_logs
                                ON people.license_plate = bakery_security_logs.license_plate
                                WHERE day = 28
                                AND month = 7
                                AND year = 2023
                                AND hour = 10
                                AND minute >= 15
                                AND minute <= 25
                            )
                    )
            )
    );


-- Find out the withdrawal history of an ATM located on Leggett Street.
-- +----------------+------------------+--------+
-- | account_number | transaction_type | amount |
-- +----------------+------------------+--------+
-- | 28500762       | withdraw         | 48     |
-- | 28296815       | withdraw         | 20     |
-- | 76054385       | withdraw         | 60     |
-- | 49610011       | withdraw         | 50     |
-- | 16153065       | withdraw         | 80     |
-- | 25506511       | withdraw         | 20     |
-- | 81061156       | withdraw         | 30     |
-- | 26013199       | withdraw         | 35     |
-- +----------------+------------------+--------+
SELECT account_number, transaction_type, amount
FROM atm_transactions
WHERE atm_location = 'Leggett Street'
AND day = 28
AND month = 7
AND year = 2023
AND transaction_type = 'withdraw';


-- Find the thief based on the results of the investigation conducted
-- +----------------+-------+-----------+----------------+---------------+
-- |  phone_number  | name  | person_id | account_number | creation_year |
-- +----------------+-------+-----------+----------------+---------------+
-- | (367) 555-5533 | Bruce | 686048    | 49610011       | 2010          |
-- +----------------+-------+-----------+----------------+---------------+
SELECT people.phone_number, people.name, ba.person_id, ba.account_number, ba.creation_year
FROM bank_accounts AS ba
JOIN people ON ba.person_id = people.id
WHERE ba.person_id IN
    (
        SELECT id
        FROM people
        JOIN passengers AS pass ON people.passport_number = pass.passport_number
        WHERE people.passport_number IN
            (
                SELECT passport_number
                FROM passengers
                WHERE flight_id = 36
                AND passport_number IN
                    (
                        SELECT passport_number
                        FROM people
                        WHERE phone_number IN
                            (
                                SELECT caller
                                FROM phone_calls
                                WHERE duration < 60
                                AND day = 28
                                AND month = 7
                                AND year = 2023
                                AND caller IN
                                    (
                                        SELECT phone_number
                                        FROM people
                                        JOIN bakery_security_logs
                                        ON people.license_plate = bakery_security_logs.license_plate
                                        WHERE day = 28
                                        AND month = 7
                                        AND year = 2023
                                        AND hour = 10
                                        AND minute >= 15
                                        AND minute <= 25
                                    )
                            )
                    )
            )
    )
AND ba.account_number IN
    (
        SELECT account_number
        FROM atm_transactions
        WHERE atm_location = 'Leggett Street'
        AND day = 28
        AND month = 7
        AND year = 2023
        AND transaction_type = 'withdraw'
    );

-- Find the accomplice called by Bruce
-- +----------------+----------------+----------+
-- |     caller     |    receiver    | duration |
-- +----------------+----------------+----------+
-- | (367) 555-5533 | (375) 555-8161 | 45       |
-- +----------------+----------------+----------+
SELECT caller, receiver, duration
FROM phone_calls
WHERE duration < 60
AND day = 28
AND month = 7
AND year = 2023
AND caller IN
    (
        SELECT phone_number
        FROM people
        JOIN bakery_security_logs
        ON people.license_plate = bakery_security_logs.license_plate
        WHERE day = 28
        AND month = 7
        AND year = 2023
        AND hour = 10
        AND minute >= 15
        AND minute <= 25
        AND name = 'Bruce'
    );

-- Find the receiver name
-- +----------------+-------+
-- |  phone_number  | name  |
-- +----------------+-------+
-- | (375) 555-8161 | Robin |
-- +----------------+-------+
SELECT phone_number, name
FROM people
WHERE phone_number = '(375) 555-8161';
