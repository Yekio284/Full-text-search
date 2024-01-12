#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cctype>
#include <utility>
#include <parser.hpp>

Parser::Parser() {
    //std::cout << "Parser started the work." << std::endl;
}

void Parser::parse_xml(const char *xmldir, std::vector<const pugi::char_t *> &sw, int &ngrmin, int &ngrmax) {
    pugi::xml_document document;
    pugi::xml_parse_result result = document.load_file(xmldir);

    if (!result) { // Если не удалось загрузить конфиг, то выводим сообщение об ошибке
        std::cout << "Can't download the xml file." << std::endl;
        return;
    }

    for (pugi::xml_node tool: document.child("fts")) { // Минимальная и максимальная длина ngram
        ngrmin = tool.attribute("ngram_min_length").as_int();
        ngrmax = tool.attribute("ngram_max_length").as_int();
    }

    for (pugi::xml_node tool: document.child("fts").child("parser").child("stop_words")) { // Возьмем стоп-слова в vector
        sw.push_back(tool.child_value());
    }
}

void Parser::delete_stop_words(std::ifstream &textfile, const std::vector<const pugi::char_t *> &sw, std::vector<std::string> &str_vec) {
    std::string words_from_file;

    for (textfile >> words_from_file; !textfile.eof(); textfile >> words_from_file) {
        std::transform(words_from_file.begin(), words_from_file.end(), words_from_file.begin(), tolower);
        auto it = std::find(sw.begin(), sw.end(), words_from_file);

        if (it == sw.end()) {
            str_vec.push_back(words_from_file);
        }
    }                                                 
}

bool Parser::check_length(const int &ngrmin, const std::string &word) const {
    return (word.length() >= ngrmin); // проверка длины слова
}

void Parser::delete_punct_dotolower(std::vector<std::string> &str_vec) { // Удаляем знаки пунктуации и понижаем в регистре. Находим позиции "удаляемых символов", затем в erase передаем эти позиции
    for (auto &str : str_vec) {
        auto it = std::remove_if(str.begin(), str.end(), [](char const &c) { return (std::ispunct(c)); }); 
        str.erase(it, str.end());
        std::transform(str.begin(), str.end(), str.begin(), tolower);
    }
}

void Parser::make_ngrams(const std::vector<std::string> &words, std::vector<std::pair<std::string, int>> &ngrams, const int &ngrmin, const int &ngrmax) { // генерируем ngram'ы. Проходимся по вектору, проверяем, подходит ли слово по длине, 
                                                                                                             // затем генерируем pair<string, int> как ngrmin символов и индекс, затем ngrmin+1 символов и индекс и так до word.length или ngrmax, если word.length == ngrmax.
    int k = 0;

    for (const auto it : words) {
        if (Parser::check_length(ngrmin, it)) {
            for (int i = ngrmin; i <= (ngrmax > it.length() ? it.length() : ngrmax); i++) {
                std::string tmp;
                for (int j = 0; j < i; j++) {
                    tmp = tmp + it.at(j);
                }
                ngrams.push_back(make_pair(tmp, k));
            }
            k = k + 1;
        }
    }
}

int& Parser::get_ngram_length_min() {
    return ngram_length_min;
}

int& Parser::get_ngram_length_max() {
    return ngram_length_max;
}

std::vector<const pugi::char_t *>& Parser::get_stop_words_vector() {
    return stop_words;
}

std::vector<std::string>& Parser::get_string_vector() {
    return string_vector;
}

std::vector<std::pair<std::string, int>>& Parser::get_n_grams_vector() {
    return n_grams;
}

Parser::~Parser() {
    //std::cout << "Parser finished his work." << std::endl;
}