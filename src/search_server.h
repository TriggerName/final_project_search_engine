#ifndef SEARCH_SERVER_H
#define SEARCH_SERVER_H

#include <string>
#include <vector>
#include "inverted_index.h"

// Структура для хранения относительной релевантности
struct RelativeIndex {
    size_t doc_id;  // Идентификатор документа
    float rank;     // Относительная релевантность

    bool operator==(const RelativeIndex& other) const {
        return doc_id == other.doc_id && rank == other.rank;
    }
};

class SearchServer {
public:
    // Добавлен аргумент по умолчанию = 5 для maxResponses
    SearchServer(InvertedIndex& idx, int maxResponses = 5)
        : index(idx), maxResponses_(maxResponses) {}

    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);

private:
    InvertedIndex& index;
    int maxResponses_;  // Храним лимит ответов
};

#endif // SEARCH_SERVER_H