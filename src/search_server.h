#ifndef SEARCH_SERVER_H
#define SEARCH_SERVER_H

#include <string>
#include <vector>
#include "inverted_index.h"

// Структура для хранения относительной релевантности
struct RelativeIndex {
    size_t doc_id;  // Идентификатор документа
    float rank;     // Относительная релевантность

    // Оператор сравнения для тестов
    bool operator==(const RelativeIndex& other) const {
        return doc_id == other.doc_id && rank == other.rank;
    }
};

class SearchServer {
public:
    explicit SearchServer(InvertedIndex& idx) : index(idx) {}

    // Метод обработки поисковых запросов
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);

private:
    InvertedIndex& index;  // Ссылка на инвертированный индекс
};

#endif // SEARCH_SERVER_H