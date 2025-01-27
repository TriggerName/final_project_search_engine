#include "search_server.h"
#include <sstream>
#include <algorithm>
#include <map>
#include <set>
#include <cmath>

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input) {
    std::vector<std::vector<RelativeIndex>> results;

    for (const auto& query : queries_input) {
        std::istringstream stream(query);
        std::string word;
        std::set<size_t> relevant_docs;
        std::map<size_t, size_t> doc_relevance;

        // Разбиваем запрос на уникальные слова
        std::set<std::string> unique_words;
        while (stream >> word) {
            unique_words.insert(word);
        }

        // Считаем абсолютную релевантность
        for (const auto& unique_word : unique_words) {
            auto entries = index.GetWordCount(unique_word);
            for (const auto& entry : entries) {
                relevant_docs.insert(entry.doc_id);
                doc_relevance[entry.doc_id] += entry.count;
            }
        }

        if (relevant_docs.empty()) {
            results.emplace_back(); // Запрос не дал результатов
            continue;
        }

        // Находим максимальную абсолютную релевантность
        size_t max_relevance = 0;
        for (const auto& [doc_id, relevance] : doc_relevance) {
            max_relevance = std::max(max_relevance, relevance);
        }

        // Рассчитываем относительную релевантность
        std::vector<RelativeIndex> relative_results;
        for (const auto& [doc_id, relevance] : doc_relevance) {
            relative_results.push_back({doc_id, static_cast<float>(relevance) / max_relevance});
        }

        // Сортируем результаты по убыванию релевантности
        std::sort(relative_results.begin(), relative_results.end(), [](const RelativeIndex& a, const RelativeIndex& b) {
            return a.rank > b.rank || (a.rank == b.rank && a.doc_id < b.doc_id);
        });

        // Ограничиваем результаты топ-5
        if (relative_results.size() > 5) {
            relative_results.resize(5);
        }

        results.push_back(relative_results);
    }

    return results;
}