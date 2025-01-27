#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H

#include <string>
#include <vector>
#include <map>

struct Entry {
    size_t doc_id; // Идентификатор документа
    size_t count;  // Количество вхождений слова в документ

    // Оператор сравнения для тестов
    bool operator==(const Entry& other) const {
        return doc_id == other.doc_id && count == other.count;
    }
};

class InvertedIndex {
public:
    InvertedIndex() = default;

    // Обновить или заполнить базу документов
    void UpdateDocumentBase(const std::vector<std::string>& input_docs);

    // Получить частотный список для слова
    std::vector<Entry> GetWordCount(const std::string& word) const;

private:
    std::vector<std::string> docs;                          // Список документов
    std::map<std::string, std::vector<Entry>> freq_dictionary; // Частотный словарь
};

#endif // INVERTED_INDEX_H