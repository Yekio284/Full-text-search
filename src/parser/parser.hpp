#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <utility>
#include <../../external/pugixml/src/pugixml.hpp>

class Parser {
private:
    int ngram_length_min, ngram_length_max;
    std::vector<const pugi::char_t *> stop_words;
    std::vector<std::string> string_vector;
    std::vector<std::pair<std::string, int>> n_grams;

public:
    Parser();
    
    void parse_xml(const char *xmldir, std::vector<const pugi::char_t *> &sw, int &ngrmin, int &ngrmax); // парсим config.xml
    void delete_stop_words(std::ifstream &textfile, const std::vector<const pugi::char_t *> &sw, std::vector<std::string> &str_vec); // отбрасываем стоп слова и записываем их в str_vec
    bool check_length(const int &ngrmin, const std::string &word) const; // функция, которая проверяет, больше ли ngrmin длина слова 
    void delete_punct_dotolower(std::vector<std::string> &str_vec); // удаляем знаки пунктуации и переводим слова в нижний регистр
    void make_ngrams(const std::vector<std::string> &words, std::vector<std::pair<std::string, int>> &ngrams, const int &ngrmin, const int &ngrmax); // генерируем ngram'ы
    int& get_ngram_length_min();
    int& get_ngram_length_max();
    std::vector<const pugi::char_t *>& get_stop_words_vector();
    std::vector<std::string>& get_string_vector();
    std::vector<std::pair<std::string, int>>& get_n_grams_vector();

    ~Parser();
};