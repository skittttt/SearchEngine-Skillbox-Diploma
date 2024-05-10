#include <fstream>
#include <iostream>
#include <thread>
#include <mutex>
#include "InvertedIndex.h"

// Заведем набор мьютексов для безопасности доступа к внешним данным
std::mutex entry_access;
std::mutex vEntry_access;
std::mutex freq_dictionary_access;
std::mutex countDocs_access;

// Разделяет строку на слова - разделитель "пробел" и считает повторы слов, если они есть
// Результат в map<слово, количество повторов>
std::map<std::string, int> InvertedIndex::wordsSplit(std::string str) {
    std::map<std::string, int> words;
    if(str.back() != ' ') // Если последний символ строки не пробел, добавляем пробел в конец
        str +=" ";
    int start = 0;
    int end = str.find(" ");
    while (end != -1) {
        std::string word = str.substr(start, end - start); // Извлекаем слово из строки
        if (words.find(word) == words.end()) { // Если слова еще нет в map, добавляем его с количеством 1
            words[word] = 1;
        } else { // Если слово уже есть в map, увеличиваем его количество на 1
            words[word] = ++words[word];
        }
        start = end + 1; // Перемещаем начальный индекс на следующий символ после пробела
        end = str.find(" ", start);
    }
    return words;
}


InvertedIndex::InvertedIndex() {}

// Метод, который выполняется в отдельном потоке для обработки документа
void InvertedIndex::threadExec(int iDoc) {
    Entry entry;
    // для подсчета повторов слов в документе
    std::map<std::string, int> wordsCount;
    // по заданию надо заполнить структуру Entry - формируем map <слово ->  {doc_id, count}>;
    std::map<std::string, Entry> entryCount;
    // делим документ на слова, считаем их повторения и кладем в wordsCount
    wordsCount = wordsSplit(docs[iDoc]);
    // кладем номер документа как первый элемент в структуру Entry
    entry_access.lock(); // Блокируем мьютекс для безопасного изменения структуры Entry
    entry.doc_id = iDoc; // Устанавливаем номер документа
    entry_access.unlock(); // Разблокируем мьютекс
    // и надо еще иметь инфу сколько слов вообще в документе - для знаменателя в Rel relevance.
    int tmpCount=0;
    // бежим по wordsCount
    for (auto it = wordsCount.begin(); it != wordsCount.end(); ++it){
        entry_access.lock();
        entry.count = it->second; // Устанавливаем количество повторов слова
        entry_access.unlock();
        entryCount[it->first] = entry; // Добавляем слово и структуру Entry в map entryCount
        tmpCount += it->second; // Увеличиваем счетчик слов в документе
    }
    countDocs_access.lock();
    countDocs.push_back(tmpCount); // Добавляем количество слов в документе в вектор countDocs
    countDocs_access.unlock();
    // таким образом тут у нас по документу iDoc сформирован map<std::string, Entry> entryCount
    // теперь надо ДОБАВИТЬ в частотный словарь ( private переменная класса)
    // map<string, vector<Entry>> freq_dictionary посчитанный для iDoc entryCount

    for (auto it1 = entryCount.begin(); it1 != entryCount.end(); ++it1) {
        std::vector<Entry> vEntry;
        vEntry_access.lock();
        vEntry.push_back(it1->second); // Добавляем структуру Entry в вектор vEntry
        vEntry_access.unlock();
        freq_dictionary_access.lock();
        freq_dictionary[it1->first].push_back(it1->second); // Добавляем слово и структуру Entry в частотный словарь
        freq_dictionary_access.unlock();
    }
}

// Метод для обновления частотного словаря
void InvertedIndex::Update_docs() {
    Entry entry;
    std::vector<Entry> vEntry;
    std::vector<std::thread> threads; // Создаем вектор потоков
    for(int iDoc = 0; iDoc < docs.size(); ++iDoc) {
        threads.push_back(std::thread (&InvertedIndex::threadExec, this, iDoc)); // Создаем поток для обработки документа
    }
    // тут по всем документам пробежали и freq_dictionary заполнили
    std::cout << "synchronizing all threads...\n";
    for (auto& th : threads) th.join(); // Ожидаем завершения всех потоков
}

// Метод для обновления базы документов
void InvertedIndex::UpdateDocumentBase(ConverterJSON converterJson)
{
    // считываем документы в которых искать из структуры conf
    std::vector<std::string> vDocs = converterJson.GetTextDocuments();
    for(int i = 0; i < vDocs.size(); ++i){
        std::ifstream  file;
        file.open(vDocs[i], std::ios::in);
        std::string str, strNew;
        while(getline(file, strNew)) {
            str += strNew + " ";
        }
        docs.push_back(str);
    }
    Update_docs(); // там считаем freq_dictionary
}

// Метод для получения количества слов в документах
std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word) {
    return freq_dictionary[word];
}