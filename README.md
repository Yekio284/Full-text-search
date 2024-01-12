# Full-text search system
Full-text search system project on C++  
Build automation: [CMake](https://cmake.org/)  
Unit tests: [Google Test](https://github.com/google/googletest)

## User Guide  
#### After cloning repository, make next commands:  
* cmake -S . -B build/release  
* cd build/release  
* make  
* cd bin  
* ./test_searcher *(For run GTest. After that command __you should__ delete directory __Full-text-search/src/indexer/index__)*  
* ./app index --csv <path .csv file> --index <path to index directory> --config <path to config.xml> *(This command creates index)*  
* ./app search --index <path to index directory> --config <path to config.xml> --query <your query> *(Search command)*