#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <searcher.hpp>
#include <indexer.hpp>
#include <parser.hpp>
#include <../../external/PicoSHA2/picosha2.h>

TextIndexAccessor::TextIndexAccessor(std::filesystem::path index_dir) {
    this->index_dir = index_dir;
    this->total_docs = 0;

    std::filesystem::path path_to_docs = index_dir / "docs";
    for (const auto &entry_docs : std::filesystem::directory_iterator(path_to_docs)) {
        if (entry_docs.is_regular_file())
            ++total_docs;
    }
}

TextIndexAccessor::~TextIndexAccessor() {}

Result::Result() {
    this->score = 0.0;
    this->document_id = 0;
}

Result::~Result() {}

size_t TextIndexAccessor::get_total_docs() {
    return total_docs;
}

std::filesystem::path TextIndexAccessor::get_index_dir() {
    return index_dir;
}

auto TextIndexAccessor::get_term_infos(std::vector<std::pair<std::string, std::string>> &term_and_entries) {
    std::vector<TermInfo> info;

    for (auto termsentries : term_and_entries) {
        std::string input = termsentries.second;
        std::stringstream ss(input);
        std::string term_;
        int doc_count_;
        std::vector<std::pair<int, std::vector<int>>> id_positions_;

        TermInfo info_to_input; // term / doc_count / id / positions

        ss >> term_ >> doc_count_;
        for (int _ = 0; _ < doc_count_; _++) {
            int doc_id, pos_count;
            std::vector<int> doc_positions;

            ss >> doc_id >> pos_count;
            for (int __ = 0; __ < pos_count; __++) {
                int pos;
                ss >> pos;
                doc_positions.push_back(pos);
            }
            id_positions_.push_back(make_pair(doc_id, doc_positions));
        }
        info_to_input.term = term_;
        info_to_input.doc_dount = doc_count_;
        info_to_input.id_positions = id_positions_;

        info.push_back(info_to_input);
    }

    for (auto &i : info) {
        std::sort(i.id_positions.begin(), i.id_positions.end());
    }

    return info;
}

std::string TextIndexAccessor::load_document(unsigned int document_id) {
    std::filesystem::path path_to_docs = TextIndexAccessor::get_index_dir() / "docs";
    std::stringstream doc_id;
    doc_id << document_id;
    std::string full_path = path_to_docs / doc_id.str();

    std::ifstream file(full_path);

    std::string content;
    if (file.is_open())
        getline(file, content);
    else 
        return "CANNOT OPEN THE FILE";
    
    file.close();

    return content;
}

std::vector<std::pair<std::string, int>> run_parser(const std::string &query, const char *xmldir) {
    Parser parser;
    parser.parse_xml(xmldir, parser.get_stop_words_vector(), parser.get_ngram_length_min(), parser.get_ngram_length_max());
    
    std::ofstream writewords;
    writewords.open("../../../src/main_searcher/tmp.txt");
    if (!(writewords)) {
        std::cout << "Can't open the file." << std::endl;
    }
    writewords << query << "\n";
    writewords.close();

    std::ifstream another_textvar;
    another_textvar.open("../../../src/main_searcher/tmp.txt");
    
    parser.delete_stop_words(another_textvar, parser.get_stop_words_vector(), parser.get_string_vector());
    another_textvar.close();
    std::remove("../../../src/main_searcher/tmp.txt");
    parser.delete_punct_dotolower(parser.get_string_vector());
    parser.make_ngrams(parser.get_string_vector(), parser.get_n_grams_vector(), parser.get_ngram_length_min(), parser.get_ngram_length_max());

    return parser.get_n_grams_vector();
}

void find_and_write_unique_id(std::vector<Result> &result, std::vector<TextIndexAccessor::TermInfo> &info) {
    std::vector<int> ids;

    for (auto i : info) {
        for (auto p : i.id_positions) {
            ids.push_back(p.first);
        }
    }

    std::sort(ids.begin(), ids.end());
    auto to_remove = std::unique(ids.begin(), ids.end());
    ids.erase(to_remove, ids.end());

    std::vector<Result> tmp(ids.size());

    int k = 0;
    for (auto &res : tmp) {
        res.document_id = ids.at(k);
        k++;
    }

    result = tmp;
}

std::vector<Result> search(const std::string &query, const char *xmldir, TextIndexAccessor &index_accessor) {
    std::vector<std::pair<std::string, int>> query_n_grams = run_parser(query, xmldir); // термы запроса

    std::vector<std::pair<std::string, std::string>> term_and_entr; // term запроса и содержимое обратного индекса
    for (const auto &term : query_n_grams) {
        std::string entries = index_accessor.get_index_dir() / "entries";
        std::string hash, line;
        picosha2::hash256_hex_string(term.first, hash);
        hash = hash.substr(0, 6);
        entries = entries + '/' + hash;
        std::ifstream out(entries);
        
        if (out.is_open()) {
            getline(out, line);
            term_and_entr.push_back(make_pair(term.first, line));
        }
    }

    auto info = index_accessor.get_term_infos(term_and_entr); // записываем в этот вектор term, doc_count, doc_id, positions, то есть информацию о терме
    std::vector<Result> result;

    find_and_write_unique_id(result, info);

    for (auto &res : result) {
        for (auto i : info) {
            for (auto i1 : i.id_positions) {
                if (i1.first == res.document_id) {
                    res.score = res.score + (i1.second.size() * log((double)index_accessor.get_total_docs() / (double)i.doc_dount));
                }
            }
        }
    }

    return result;
}