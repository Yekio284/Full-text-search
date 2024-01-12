#pragma once

#include <unordered_map>
#include <filesystem>
#include <vector>
#include <string>

class Index {
private:
    std::unordered_map<unsigned int, std::string> docs;
    std::unordered_map<std::string, std::unordered_map<unsigned int, std::vector<size_t>>> entries;

public:
    Index();
    
    std::unordered_map<unsigned int, std::string>& get_docs();
    std::unordered_map<std::string, std::unordered_map<unsigned int, std::vector<size_t>>>& get_entries();

    ~Index();
};

class IndexBuilder {
private:
    Index index;

public:
    IndexBuilder();

    bool have_document_id(const unsigned int document_id);
    void add_document(const unsigned int &document_id, const std::string &text, const char *xmldir);
    Index& get_index();

    ~IndexBuilder();
};

class TextIndexWriter {
private:
    std::filesystem::path path_to_index;

public:
    TextIndexWriter();

    void write(const std::string &path, Index &index);
    std::filesystem::path get_path_to_index();

    ~TextIndexWriter();
};