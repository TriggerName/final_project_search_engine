#include <iostream>
#include <string>
#include <filesystem> // C++17 для std::filesystem
#include <vector>

#include "src/converter_json.h"
#include "src/inverted_index.h"
#include "src/search_server.h"

/**
 * @brief Получает путь к нужному файлу (config.json / requests.json) в текущей директории.
 *        Если файла нет, выбрасывает исключение.
 */
std::filesystem::path getFilePath(const std::string& fileName) {
    std::filesystem::path current = std::filesystem::current_path();
    std::filesystem::path filePath = current / fileName;

    if (!std::filesystem::exists(filePath)) {
        throw std::runtime_error("Файл не найден: " + filePath.string());
    }
    return filePath;
}

int main() {
    try {
        // 1) Определяем пути к config.json и requests.json (в одной папке с исполняемым файлом)
        std::filesystem::path configPath   = getFilePath("config.json");
        std::filesystem::path requestsPath = getFilePath("requests.json");

        // 2) Создаём объект ConverterJSON и загружаем config
        ConverterJSON converter;
        converter.loadConfig(configPath.string());

        // 3) Предполагаем, что в ConverterJSON есть метод getTextDocuments(),
        //    который читает поле "files" из config.json и возвращает список строк (содержимого файлов).
        //    Если у вас такого метода пока нет — нужно его реализовать аналогично:
        //
        //      std::vector<std::string> ConverterJSON::getTextDocuments() {
        //          // 1) Считать из config["files"] пути
        //          // 2) Для каждого пути открыть файл, прочитать текст, вернуть в виде вектора строк
        //      }
        //
        //    Ниже — пример использования:
        std::vector<std::string> docs; 
        // Допустим, у вас есть метод converter.getTextDocuments() — раскомментируйте, когда сделаете:
        // docs = converter.getTextDocuments();

        // (Если пока нет — можно вручную задать docs, чтобы протестировать)
        docs.push_back("milk sugar salt");
        docs.push_back("milk a milk b milk c milk d");

        // 4) Создаём InvertedIndex и загружаем документы
        InvertedIndex index;
        index.UpdateDocumentBase(docs);

        // 5) Создаём SearchServer, передаём ему ссылку на index и max_responses
        int maxResp = converter.getMaxResponses();
        SearchServer server(index, maxResp);

        // 6) Загружаем запросы из requests.json
        converter.processRequests(requestsPath.string());

        // Предположим, что у вас есть метод converter.getRequests(), который возвращает std::vector<std::string>.
        // Если такого нет, нужно реализовать — логика аналогична getTextDocuments, но читается поле "requests".
        std::vector<std::string> queries = {
            // Заглушка (для теста). Потом замените на converter.getRequests()!
            "milk sugar",
            "salt",
            "bread"
        };

        // 7) Запускаем поиск
        // Метод server.search возвращает std::vector<std::vector<RelativeIndex>> (по текущему коду).
        auto searchResults = server.search(queries);

        // 8) Преобразуем searchResults в формат std::vector<std::vector<std::pair<int,float>>>.
        //    Потому что putAnswers, по примеру, принимает именно такой тип.
        //    Если вы хотите напрямую передавать RelativeIndex, то поменяйте сигнатуру putAnswers.
        std::vector<std::vector<std::pair<int, float>>> finalResults;
        finalResults.reserve(searchResults.size());

        for (auto &singleQueryResult : searchResults) {
            std::vector<std::pair<int, float>> pairs;
            pairs.reserve(singleQueryResult.size());
            for (auto &relIndex : singleQueryResult) {
                pairs.push_back({static_cast<int>(relIndex.doc_id), relIndex.rank});
            }
            finalResults.push_back(std::move(pairs));
        }

        // 9) Записываем результаты в answers.json
        converter.putAnswers(finalResults);

        std::cout << "Все запросы успешно обработаны, результаты записаны в answers.json!" << std::endl;
    }
    catch (const std::exception& e) {
        // Обработка ошибок
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
    return 0;
}