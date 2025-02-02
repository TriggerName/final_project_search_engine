#include "inverted_index.h"
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <cctype>

// Функция для нормализации слова: переводим в lowercase, удаляем пунктуацию
static std::string NormalizeWord(const std::string& rawWord) {
    std::string result;
    result.reserve(rawWord.size());
    for (char c : rawWord) {
        result.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
    }
    // Удаляем любые знаки пунктуации
    result.erase(
        std::remove_if(result.begin(), result.end(), [](unsigned char ch){
            return std::ispunct(ch);
        }),
        result.end()
    );
    return result;
}

void InvertedIndex::UpdateDocumentBase(const std::vector<std::string>& input_docs) {
    docs = input_docs; 
    freq_dictionary.clear(); 

    for (size_t doc_id = 0; doc_id < docs.size(); ++doc_id) {
        std::unordered_map<std::string, size_t> word_count;
        std::istringstream stream(docs[doc_id]);
        std::string rawWord;

        while (stream >> rawWord) {
            // Нормализуем
            std::string word = NormalizeWord(rawWord);
            if (!word.empty()) {
                ++word_count[word];
            }
        }

        for (const auto& [word, count] : word_count) {
            freq_dictionary[word].push_back({doc_id, count});
        }
    }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) const {
    auto it = freq_dictionary.find(word);
    if (it != freq_dictionary.end()) {
        return it->second;
    }
    return {};
}