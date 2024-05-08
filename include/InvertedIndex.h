#pragma once
#include "nlohmann/json.hpp"
#include "ConverterJSON.h"
#include <thread>

// Структура Entry, которая хранит информацию о документе и его количестве вхождений.
struct Entry {
    size_t doc_id, count; // Идентификатор документа и количество вхождений.

    // Перегрузка оператора == для сравнения двух объектов Entry.
    bool operator ==(const Entry& other) const {
        return (doc_id == other.doc_id && count == other.count);
    }
};

class InvertedIndex {
public:
    InvertedIndex();
    // Метод для обновления базы документов, используя объект ConverterJSON.
    void UpdateDocumentBase(ConverterJSON);
    // Метод для получения количества слов в документах.
    std::vector<Entry> GetWordCount(const std::string& word);

protected:
    // Метод для выполнения потока.
    void threadExec(int);
    // Метод для разделения строки на слова и подсчета их количества.
    std::map<std::string, int> wordsSplit(std::string);
    // Метод для обновления документов.
    void Update_docs();


private:
    // Вектор для хранения документов.
    std::vector<std::string> docs;
    std::vector<int> countDocs; // добавил этот вектор что бы в split сразу считать и хранить в нем общее число слов
    // в документе - нужно для подсчета Rel relevance в конце (как знаменатель)

    // Словарь для хранения частоты слов в документах.
    std::map<std::string, std::vector<Entry>> freq_dictionary;
};