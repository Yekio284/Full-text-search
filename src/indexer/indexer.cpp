#include <iostream>
#include <fstream>
#include <filesystem>
#include <../../external/PicoSHA2/picosha2.h>
#include <parser.hpp>
#include <indexer.hpp>

Index::Index() {}
Index::~Index() {}
IndexBuilder::IndexBuilder() {}
IndexBuilder::~IndexBuilder() {}
TextIndexWriter::TextIndexWriter() {}
TextIndexWriter::~TextIndexWriter() {}

std::unordered_map<unsigned int, std::string>& Index::get_docs() {
    return docs;
}

std::unordered_map<std::string, std::unordered_map<unsigned int, std::vector<size_t>>>& Index::get_entries() {
    return entries;
}

Index& IndexBuilder::get_index() {
    return index;
}

bool IndexBuilder::have_document_id(const unsigned int document_id) {
    return index.get_docs().find(document_id) != index.get_docs().end();
}

void IndexBuilder::add_document(const unsigned int &document_id, const std::string &text, const char *xmldir) {
    if (IndexBuilder::have_document_id(document_id)) {
        std::cout << "Indexer already has the document with id " << document_id << std::endl;
        return;
    }
    else
        index.get_docs()[document_id] = text;
    
    Parser parser;
    parser.parse_xml(xmldir, parser.get_stop_words_vector(), parser.get_ngram_length_min(), parser.get_ngram_length_max());
    
    std::ofstream writewords;
    writewords.open("../../../src/main_searcher/tmp.txt");
    if (!(writewords)) {
        std::cout << "Can't open the file." << std::endl;
        return;
    }
    writewords << index.get_docs()[document_id] << "\n";
    writewords.close();

    std::ifstream another_textvar;
    another_textvar.open("../../../src/main_searcher/tmp.txt");
    
    parser.delete_stop_words(another_textvar, parser.get_stop_words_vector(), parser.get_string_vector());
    another_textvar.close();
    std::remove("../../../src/main_searcher/tmp.txt");
    parser.delete_punct_dotolower(parser.get_string_vector());
    parser.make_ngrams(parser.get_string_vector(), parser.get_n_grams_vector(), parser.get_ngram_length_min(), parser.get_ngram_length_max());

    std::vector<std::pair<std::string, int>> n_grams = parser.get_n_grams_vector();

    for (const auto pair_term : n_grams) {
        index.get_entries()[pair_term.first][document_id].push_back(pair_term.second);
    }
}

void TextIndexWriter::write(const std::string &path, Index &index) {
    namespace fs = std::filesystem;
    path_to_index = path;
    fs::path path_to_create_docs = path_to_index / "docs";
    fs::path path_to_create_entries = path_to_index / "entries";
    fs::create_directories(path_to_create_docs);
    fs::create_directories(path_to_create_entries);

    for (auto doc : index.get_docs()) {
        std::ofstream make_docs_files(path_to_create_docs / std::to_string(doc.first));
        make_docs_files << doc.second;
    }

    for (auto entrs : index.get_entries()) {
        std::string hash;
        picosha2::hash256_hex_string(entrs.first, hash);
        std::ofstream make_entries_files(path_to_create_entries / hash.substr(0, 6));
        make_entries_files << entrs.first << " " << entrs.second.size() << " ";

        for (auto ent : entrs.second) {
            make_entries_files << ent.first << ' ' << ent.second.size() << ' ';
            for (auto count_positions : ent.second) {
                make_entries_files << count_positions << ' ';
            }
        }  
    }
}