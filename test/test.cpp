#include <gtest/gtest.h>
#include <indexer.hpp>
#include <searcher.hpp>

TEST(Test_searcher, first_test) {
    const char *xmldir= "../../../config.xml";

    IndexBuilder index_builder;

    index_builder.add_document(100, "Hello world", xmldir);
    index_builder.add_document(101, "Bye world", xmldir);
    index_builder.add_document(102, "Hello Earth", xmldir);

    TextIndexWriter text_writer;

    text_writer.write("../../../src/indexer/index", index_builder.get_index());

    TextIndexAccessor index_accessor("../../../src/indexer/index");
    
    std::vector<Result> correct_result;
    Result res1;
    res1.document_id = 100;
    res1.score = 2.43279;

    Result res2;
    res2.document_id = 101;
    res2.score = 1.2164;

    Result res3;
    res3.document_id = 102;
    res3.score = 1.2164;

    correct_result.push_back(res1);
    correct_result.push_back(res2);
    correct_result.push_back(res3);

    std::vector<Result> result = search("hello world", xmldir, index_accessor);

    bool fine = false;

    int k = 0;
    for (auto &cor_res : correct_result) {
        if (cor_res.document_id == result.at(k).document_id)
            fine = true;
        else
            fine = false;
        k++;
    }

    ASSERT_TRUE(fine);
}

TEST(Test_searcher, second_test) {
    const char *xmldir= "../../../config.xml";

    IndexBuilder index_builder;

    index_builder.add_document(100, "Hello world", xmldir);
    index_builder.add_document(101, "Bye world", xmldir);
    index_builder.add_document(102, "Hello Earth", xmldir);

    TextIndexWriter text_writer;

    text_writer.write("../../../src/indexer/index", index_builder.get_index());

    TextIndexAccessor index_accessor("../../../src/indexer/index");
    
    std::vector<Result> correct_result;
    Result res1;
    res1.document_id = 101;
    res1.score = 1.09861;

    Result res2;
    res2.document_id = 102;
    res2.score = 3.29584;

    correct_result.push_back(res1);
    correct_result.push_back(res2);

    std::vector<Result> result = search("bye earth", xmldir, index_accessor);

    bool fine = false;

    int k = 0;
    for (auto &cor_res : correct_result) {
        if (cor_res.document_id == result.at(k).document_id)
            fine = true;
        else
            fine = false;
        k++;
    }

    ASSERT_TRUE(fine);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}