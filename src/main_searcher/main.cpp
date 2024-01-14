#include <CLI/App.hpp>
#include <CLI/Formatter.hpp>
#include <CLI/Config.hpp>
#include <rapidcsv.h>
#include <iostream>
#include <vector>
#include <indexer.hpp>
#include <searcher.hpp>

void index_command(const std::string &path_to_csv, const std::string &path, const char *xmldir) {
    rapidcsv::Document document(path_to_csv);

    std::vector<int> id = document.GetColumn<int>("bookID");
    std::vector<std::string> text = document.GetColumn<std::string>("title");

    IndexBuilder index_builder;
    for (int i = 0; i < id.size(); i++) {
        index_builder.add_document(id[i], text[i], xmldir);
    }

    TextIndexWriter text_writer;
    text_writer.write(path, index_builder.get_index());
}

void search_command(const std::string &query, const char *xmldir, std::filesystem::path &index_dir) {
    TextIndexAccessor index_accessor(index_dir);
    std::vector<Result> result = search(query, xmldir, index_accessor);

    std::sort(result.begin(), result.end(), [](const Result &a, const Result &b){
        return a.score > b.score;
    });

    for (auto res : result) {
        std::cout << res.document_id << ' ' << res.score << ' ' << index_accessor.load_document(res.document_id) << std::endl;
    }
}

int main(int argc, char **argv) {
    CLI::App app;
    CLI::App *index = app.add_subcommand("index", "index_command");

    std::string path_to_csv;
    std::string path_to_index;
    char *xmldir_tmp = (char *)malloc(sizeof(char) * 100);
    
    index->add_option("--csv", path_to_csv, "path_to_csv_file");
    index->add_option("--index", path_to_index, "path_to_index_directory");
    index->add_option("--config", xmldir_tmp, "path_to_config.xml");
    
    const char *xmldir_index = xmldir_tmp;

    index->callback([&path_to_csv, &path_to_index, &xmldir_index](){ index_command(path_to_csv, path_to_index, xmldir_index); });

    CLI::App *search_ = app.add_subcommand("search", "search_command");
    
    std::string query;
    std::filesystem::path index_dir;

    search_->add_option("--index", index_dir, "path_to_index");
    search_->add_option("--config", xmldir_tmp, "path_to_config.xml");
    search_->add_option("--query", query, "query");

    const char *xmldir_searcher = xmldir_tmp;

    if (argc == 6) {
        std::cout << "Enter query: ";
        std::getline(std::cin, query);
    }

    search_->callback([&query, &xmldir_searcher, &index_dir](){ search_command(query, xmldir_searcher, index_dir); });

    CLI11_PARSE(app, argc, argv);
}