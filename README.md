---
## Project Structure
The project follows a modular design separating the indexing, document management, query processing, and benchmarking components.
The below is the Project Structure
```
MiniSearchEngine
│
├── data/
│   ├── benchmark_report.png
│   ├── dictionary.txt
│   ├── frequency.txt
│   ├── user_dictionary.txt
│   └── documents.txt
│
├── src/
│   ├── benchmark.cpp
│   ├── main.cpp
│   ├── Trie.cpp
│   ├── Trie.h
│   ├── TrieNode.h
│   ├── SearchEngine.h
│   ├── Document.h
│   ├── DocumentManager.h
│   └── Suggestion.h
│
└── CMakeLists.txt
```
---
## Algorithms Used
| Feature | Algorithm / Data Structure |
|----------|---------------------------|
| Dictionary | Trie |
| Prefix Search | Trie |
| Autocomplete | Trie + Top-K Ranking |
| Spell Correction | Levenshtein Distance |
| Document Index | Inverted Index |
| Ranking | TF-IDF |
| Wildcard Search | DFS on Trie |
| Query Processing | Tokenization + Normalization |
---
## Complexity
| Operation | Complexity |
|-----------|------------|
| Insert | O(L) |
| Search | O(L) |
| Prefix Search | O(L) |
| Autocomplete | O(L + K) |
| Spell Correction | Trie DFS + Dynamic Programming |
| Wildcard Search | Depends on wildcard expansion |
| Document Upload | O(T × L) |
| TF-IDF Search | O(total posting list size of query terms) |
where
- **L** = Length of a word
- **T** = Number of words in the uploaded document
---
## Building
Clone the repository
```bash
git clone https://github.com/desairudra/smart-autocomplete.git
cd smart-autocomplete
```
Build
```bash
mkdir build
cd build
cmake ..
make
```
Run
```bash
./MiniSearchEngine
```
---
## Example
```
1. Upload Document
Enter path:
notes.txt
Document uploaded successfully.
2. Search
Query:
operatng schedulling
Searching for:
operating scheduling
====================================
1.
OperatingSystem.txt
Score : 6.42
"...CPU scheduling is responsible for selecting the next process..."
------------------------------------
2.
LinuxNotes.txt
Score : 5.81
"...The Linux scheduler uses Completely Fair Scheduling..."
```
---
## Benchmark
A standalone benchmark suite is included to evaluate the performance of the major search engine operations.
### Benchmark Summary
| Operation | Average Time |
|-----------|-------------:|
| Dictionary Loading | 104 ms |
| Insert | 0.2468 μs |
| Search | < 1 μs |
| Prefix Search | < 1 μs |
| Autocomplete | 0.0062 μs |
| Spell Correction | 1717.15 μs |
| Wildcard Search | 0.045 μs |
### Benchmark Report
![Benchmark Report](data/benchmark_report.png)
The benchmark measures:
- Dictionary loading performance
- Word insertion
- Exact search
- Prefix search
- Frequency-aware autocomplete
- Levenshtein-distance spell correction
- Wildcard search
Run the benchmark
```bash
./benchmark
```
---
## Future Improvements
- BM25 ranking
- Phrase search
- Boolean queries (AND / OR / NOT)
- Parallel indexing
- Incremental indexing
- REST API
- Web interface
---
## Technologies Used
- C++17
- STL
- Trie
- Dynamic Programming
- TF-IDF
- Inverted Index
- File System Library
- Object-Oriented Design
---
## Author
**Rudra Desai**
GitHub: https://github.com/desairudra
