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
* ./app index --csv &lt;path to .csv file&gt; --index &lt;path to index directory&gt; --config &lt;path to config.xml&gt; *(This command creates index)*  
* ./app search --index &lt;path to index directory&gt; --config &lt;path to config.xml&gt; --query &lt;your query in __double quotes__&gt; *(Search command. Output format: &lt;Document id&gt; &lt;Document relevance assessment&gt; &lt;Title of book&gt;)*