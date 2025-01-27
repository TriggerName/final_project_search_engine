#include <gtest/gtest.h>
#include "../src/converter_json.h"

// Тест: Проверка, что файл конфигурации загружается корректно
TEST(ConverterJSONTests, LoadConfig) {
    ConverterJSON converter;

    // Ожидается, что загрузка существующего файла пройдёт без исключений
    EXPECT_NO_THROW(converter.loadConfig("config.json"));
}

// Тест: Проверка обработки отсутствующего файла конфигурации
TEST(ConverterJSONTests, LoadConfigFileNotFound) {
    ConverterJSON converter;

    // Ожидается, что будет выброшено исключение std::runtime_error
    EXPECT_THROW(converter.loadConfig("nonexistent.json"), std::runtime_error);
}

// Тест: Проверка загрузки запросов
TEST(ConverterJSONTests, LoadRequests) {
    ConverterJSON converter;

    // Ожидается, что загрузка существующего файла запросов пройдёт без исключений
    EXPECT_NO_THROW(converter.processRequests("requests.json"));
}

// Тест: Проверка обработки отсутствующего файла запросов
TEST(ConverterJSONTests, LoadRequestsFileNotFound) {
    ConverterJSON converter;

    // Ожидается, что будет выброшено исключение std::runtime_error
    EXPECT_THROW(converter.processRequests("nonexistent_requests.json"), std::runtime_error);
}

// Тест: Проверка корректности обработки запросов
TEST(ConverterJSONTests, ProcessValidRequests) {
    ConverterJSON converter;

    // Загружаем конфигурацию и запросы
    EXPECT_NO_THROW(converter.loadConfig("config.json"));
    EXPECT_NO_THROW(converter.processRequests("requests.json"));

    // Здесь можно добавить проверки корректности обработки запросов
    // Например, проверить содержимое переменной requests
}