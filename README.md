# Full-text search system
Full-text search system project on C++  
Build automation: CMake  
Unit tests: Google Test

## User Guide  
### After cloning repository, make next commands:  
cmake -S . -B build/release  
cd build/release  
make  
cd bin  
./test_searcher *(For run GTest. After that command __you should__ delete directory Full-text-search/src/indexer/index)*  
./app index --csv <path .csv file> --index <path to index directory> --config <path to config.xml> *(This command creates index)*  
./app search --index <path to index directory> --config <path to config.xml> --query <your query> *(Search command)*