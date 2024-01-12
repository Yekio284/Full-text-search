#pragma once

#include <vector>
#include <string>
#include <filesystem>

class TextIndexAccessor {
private:
    size_t total_docs;
    std::filesystem::path index_dir;

public:
    TextIndexAccessor(std::filesystem::path index_dir);
    
    struct TermInfo {
        std::string term;
        int doc_dount;
        std::vector<std::pair<int, std::vector<int>>> id_positions; // id / positions
    };
    
    std::string load_document(unsigned int document_id);
    size_t get_total_docs();
    auto get_term_infos(std::vector<std::pair<std::string, std::string>> &term_and_entries);
    std::filesystem::path get_index_dir();

    ~TextIndexAccessor();
};

class Result {
public:
    Result();

    double score;
    unsigned int document_id;

    ~Result();
};

std::vector<std::pair<std::string, int>> run_parser(const std::string &query, const char *xmldir);
void find_and_write_unique_id(std::vector<Result> &result, std::vector<TextIndexAccessor::TermInfo> &info);
std::vector<Result> search(const std::string &query, const char *xmldir, TextIndexAccessor &index_accessor); 