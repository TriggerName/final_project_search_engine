#include "inverted_index.h"
#include <sstream>
#include <algorithm>
#include <unordered_map>

void InvertedIndex::UpdateDocumentBase(const std::vector<std::string>& input_docs) {
    docs = input_docs; // Обновляем базу документов
    freq_dictionary.clear(); // Очищаем словарь

    for (size_t doc_id = 0; doc_id < docs.size(); ++doc_id) {
        std::unordered_map<std::string, size_t> word_count; // Локальный счётчик слов

        // Разбиваем текст на слова
        std::istringstream stream(docs[doc_id]);
        std::string word;
        while (stream >> word) {
            ++word_count[word];
        }

        // Обновляем freq_dictionary
        for (const auto& [word, count] : word_count) {
            freq_dictionary[word].push_back({doc_id, count});
        }
    }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) const {
    if (freq_dictionary.find(word) != freq_dictionary.end()) {
        return freq_dictionary.at(word);
    }
    return {};
}