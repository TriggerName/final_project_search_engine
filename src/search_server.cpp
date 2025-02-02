#include "search_server.h"
#include <sstream>
#include <algorithm>
#include <map>
#include <set>
#include <cmath>
#include <cctype>

// Аналогичная функция для нормализации слов в запросах.
// Можно вынести в общий .h, чтобы не дублировать.
// Или использовать ту же, что в InvertedIndex, но придётся сделать её доступной.
static std::string NormalizeWord(const std::string& rawWord) {
    std::string result;
    result.reserve(rawWord.size());
    for (char c : rawWord) {
        result.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
    }
    result.erase(
        std::remove_if(result.begin(), result.end(), [](unsigned char ch){
            return std::ispunct(ch);
        }),
        result.end()
    );
    return result;
}

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input) {
    std::vector<std::vector<RelativeIndex>> results;
    results.reserve(queries_input.size());

    for (const auto& query : queries_input) {
        // Разбиваем запрос на слова
        std::istringstream stream(query);
        std::string rawWord;
        std::set<std::string> unique_words;

        while (stream >> rawWord) {
            std::string norm = NormalizeWord(rawWord);
            if (!norm.empty()) {
                unique_words.insert(norm);
            }
        }

        // Считаем абсолютную релевантность
        std::map<size_t, size_t> doc_relevance;
        for (const auto& w : unique_words) {
            auto entries = index.GetWordCount(w);
            for (const auto& entry : entries) {
                doc_relevance[entry.doc_id] += entry.count;
            }
        }

        if (doc_relevance.empty()) {
            // Нет документов (ни одно слово не найдено)
            results.emplace_back();
            continue;
        }

        // Находим максимальную абсолютную релевантность
        size_t max_relevance = 0;
        for (auto& [doc_id, rel] : doc_relevance) {
            if (rel > max_relevance) {
                max_relevance = rel;
            }
        }

        // Рассчитываем относительную релевантность
        std::vector<RelativeIndex> relative_results;
        relative_results.reserve(doc_relevance.size());

        for (const auto& [doc_id, rel] : doc_relevance) {
            float rank = static_cast<float>(rel) / static_cast<float>(max_relevance); 
            relative_results.push_back({ doc_id, rank });
        }

        // Сортируем по убыванию rank, при равенстве — doc_id по возрастанию
        std::sort(relative_results.begin(), relative_results.end(),
                  [](const RelativeIndex& a, const RelativeIndex& b){
                      return (a.rank > b.rank) ||
                             (a.rank == b.rank && a.doc_id < b.doc_id);
                  });

        // Обрезаем до maxResponses_
        if (relative_results.size() > static_cast<size_t>(maxResponses_)) {
            relative_results.resize(maxResponses_);
        }

        results.push_back(relative_results);
    }

    return results;
}