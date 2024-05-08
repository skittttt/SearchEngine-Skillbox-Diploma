#pragma once
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include "nlohmann/json.hpp"
#include "InvertedIndex.h"
#include "ConverterJSON.h"

// Структура RelativeIndex, которая хранит информацию об относительном ранге документа.
struct RelativeIndex {
    size_t doc_id; // Идентификатор документа.
    float rank; // Ранг документа.

    // Перегрузка оператора == для сравнения двух объектов RelativeIndex.
    bool operator==(const RelativeIndex &other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};
// Структура comp, которая используется для сравнения пар в map.
struct comp
{
    template<typename T>
    bool operator()(const T &l, const T &r) const
    {
        if (l.second != r.second) {
            return l.second < r.second;
        }
        return l.first < r.first;
    }
};

class SearchServer {
public:
    SearchServer(InvertedIndex&);
    std::vector<std::vector<RelativeIndex>> search(ConverterJSON);

protected:
    std::map<std::string, int> wordsSplit(std::string);
    void calcResult();

private:
    InvertedIndex _index;
    // Вектор запросов {"milk water banana milk water water", "sugar"};
    // будет вот так: vector < {(banana,1)(milk 2)(water 3)}, {sugar, 1}>
    // т е индекс вектора это новый запрос в котором слово->сколько раз
    std::vector<std::map <std::string, int>> _requests;
    std::vector<std::vector<RelativeIndex>> _result;
};