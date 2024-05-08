#include "ConverterJSON.h" // Подключаем заголовочный файл для класса ConverterJSON, который отвечает за преобразование данных в формат JSON.
#include "InvertedIndex.h" // Подключаем заголовочный файл для класса InvertedIndex, который отвечает за обратный индекс.
#include "SearchServer.h" // Подключаем заголовочный файл для класса SearchServer, который отвечает за поиск по индексу.

int main() {
    ConverterJSON converterJson("config.json","requests.json"); // Создаем объект класса ConverterJSON, который будет использоваться для чтения конфигурационного файла "config.json" и файла запросов "requests.json".
    InvertedIndex invertedIndex; // Создаем объект класса InvertedIndex, который будет использоваться для создания обратного индекса.
    invertedIndex.UpdateDocumentBase(converterJson); // Обновляем базу документов, используя объект converterJson для чтения данных.
    SearchServer searchServer(invertedIndex); // Создаем объект класса SearchServer, который будет использовать объект invertedIndex для поиска.
    searchServer.search(converterJson); // Используем объект searchServer для поиска по базе документов, используя объект converterJson для чтения запросов.
    return 0;
}