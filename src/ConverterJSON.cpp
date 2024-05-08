#include <fstream> // Подключаем библиотеку для работы с файлами
#include <iostream> // Подключаем библиотеку для ввода-вывода
#include "ConverterJSON.h" // Подключаем заголовочный файл для класса ConverterJSON

// Определяем класс исключения для ситуации, когда файл конфигурации отсутствует
class FileConfigDoesNotExist: public std::exception{
public:
    const char* what() const noexcept override{ // Переопределяем метод what для исключения
        return "Сonfig file is missing";
    }
};

// Определяем класс исключения для ситуации, когда в файле конфигурации отсутствует поле "Config"
class configFieldIsMissing:public  std::exception{
public:
    const char* what() const noexcept override{ //Переопределяем метод what для исключения
        return "Field \"Config\" is missing.";
    }
};
// Конструктор класса ConverterJSON, который принимает два пути к файлам: config для файла конфигурации и request для файла запросов
ConverterJSON::ConverterJSON(std::string config, std::string request) {
    // считываем requests.json и config.json в структуру conf
    try {
        readConfigFile(config); // Пытаемся прочитать файл конфигурации
    }
    catch (FileConfigDoesNotExist &x){
        std::cerr << "Wrong configuration: " << x.what() << std::endl; // Выводим сообщение об ошибке, если файл конфигурации отсутствует
    }

    std::ifstream readFrom(request); // Открываем файл запросов для чтения
    std::vector<std::string> req;
    nlohmann::json j;
    readFrom >> j; // Считываем JSON из файла запросов
    for (auto const& val : j["requests"]){
        conf.requests.push_back(val); // Добавляем запросы в конфигурацию
    }
}

// Метод, возвращающий лимит ответов
int ConverterJSON::GetResponsesLimit() {
    return conf.max_responses; // Возвращаем максимальное количество ответов из конфигурации
}
// Метод, возвращающий запросы
std::vector<std::string> ConverterJSON::GetRequests() {
    return conf.requests; // Возвращаем список запросов из конфигурации
}
// Метод, возвращающий текстовые документы
std::vector<std::string> ConverterJSON::GetTextDocuments() {
    return conf.files; // Возвращаем список файлов из конфигурации
}
// Метод для чтения файла конфигурации
void ConverterJSON::readConfigFile(std::string filePath) {
    std::ifstream readFrom(filePath.c_str()); // Открываем файл конфигурации для чтения
    if(!readFrom.is_open()) {
        throw FileConfigDoesNotExist(); // Если файл не открылся, выбрасываем исключение
    }
    nlohmann::json j;
    readFrom >> j; // Считываем JSON из файла конфигурации
    if (!j.contains("config")) {
        throw configFieldIsMissing(); // Если в JSON отсутствует поле "config", выбрасываем исключение
    }
    conf.name = j["config"]["name"]; // Получаем имя из конфигурации
    std::cout << "Search Engine " << conf.name << " is running" << std::endl; // Выводим сообщение о запуске движка
    conf.version = j["config"]["version"]; // Получаем версию из конфигурации
    std::cout << "Search Engine version is " << conf.version << std::endl; // Выводим сообщение о версии движка
    conf.max_responses = j["config"]["max_responses"]; // Получаем максимальное количество ответов из конфигурации

    if (j.find("files") != j.end()) {
        for (auto const &val: j["files"]) {
            conf.files.push_back(val); // Добавляем файлы в конфигурацию
            int i = 0;
        }
    }
    readFrom.close(); // Закрываем файл конфигурации
    for(int i=0; i < conf.files.size(); ++i){
        // по условию надо указать что файла нет но программу не прерывать
        std::ifstream readFrom(conf.files[i]); // Пытаемся открыть файл
        if(!readFrom.is_open()) {
            std::cout << "Data file " << conf.files[i] << " is missing" << std::endl; // Выводим сообщение, если файл не открылся
        }
    }
}
