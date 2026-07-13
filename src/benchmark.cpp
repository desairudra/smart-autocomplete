#include "Trie.h"

#include <chrono>
#include <iostream>
#include <vector>

using namespace std;
using namespace std::chrono;

int main() {
  Trie trie;

  volatile bool found = false;
  volatile bool prefix = false;
  volatile size_t cnt = 0;

  vector<string> words = {"apple", "banana", "computer", "algorithm",
                          "tree",  "search", "data",     "hello",
                          "world", "orange"};

  cout << "============================================\n";
  cout << "         Trie Performance Benchmark\n";
  cout << "============================================\n\n";

  // ==========================
  // Dictionary Loading
  // ==========================

  auto start = high_resolution_clock::now();

  trie.loadDictionary("../data/dictonary.txt");
  trie.loadDictionary("../data/user_dictonary.txt");
  trie.loadfrequency("../data/frequency.txt");
  trie.call_dfs();

  auto end = high_resolution_clock::now();

  auto loadTime = duration_cast<milliseconds>(end - start);

  cout << "Dictionary Loading Time : " << loadTime.count() << " ms\n\n";

  // ==========================
  // Warm-up
  // ==========================

  for (int i = 0; i < 1000; i++) {
    found = trie.search(words[i % words.size()]);
    prefix = trie.startsWith("app");
    cnt += trie.autocomplete("app").size();
    cnt += trie.genrate_suggestion("aplpe").size();
    cnt += trie.wild_card_suggestion("app*").size();
  }

  // ==========================
  // Insert Benchmark
  // ==========================

  const int INSERT_ITER = 10000;

  Trie insertTrie;

  start = high_resolution_clock::now();

  for (int i = 0; i < INSERT_ITER; i++) {
    insertTrie.insert("word" + to_string(i));
  }

  end = high_resolution_clock::now();

  auto insertTime = duration_cast<nanoseconds>(end - start);

  // ==========================
  // Search Benchmark
  // ==========================

  const int SEARCH_ITER = 100000;

  start = high_resolution_clock::now();

  for (int i = 0; i < SEARCH_ITER; i++) {
    found = trie.search(words[i % words.size()]);
  }

  end = high_resolution_clock::now();

  auto searchTime = duration_cast<nanoseconds>(end - start);

  // ==========================
  // Prefix Benchmark
  // ==========================

  start = high_resolution_clock::now();

  for (int i = 0; i < SEARCH_ITER; i++) {
    prefix = trie.startsWith("app");
  }

  end = high_resolution_clock::now();

  auto prefixTime = duration_cast<nanoseconds>(end - start);

  // ==========================
  // Autocomplete Benchmark
  // ==========================

  const int AUTO_ITER = 10000;

  start = high_resolution_clock::now();

  for (int i = 0; i < AUTO_ITER; i++) {
    cnt += trie.autocomplete("app").size();
  }

  end = high_resolution_clock::now();

  auto autoTime = duration_cast<nanoseconds>(end - start);

  // ==========================
  // Spell Correction Benchmark
  // ==========================

  const int SPELL_ITER = 1000;

  start = high_resolution_clock::now();

  for (int i = 0; i < SPELL_ITER; i++) {
    cnt += trie.genrate_suggestion("aplpe").size();
  }

  end = high_resolution_clock::now();

  auto spellTime = duration_cast<nanoseconds>(end - start);

  // ==========================
  // Wildcard Benchmark
  // ==========================

  const int WILD_ITER = 1000;

  start = high_resolution_clock::now();

  for (int i = 0; i < WILD_ITER; i++) {
    cnt += trie.wild_card_suggestion("app*").size();
  }

  end = high_resolution_clock::now();

  auto wildcardTime = duration_cast<nanoseconds>(end - start);

  // ==========================
  // Print Results
  // ==========================

  cout << "============================================\n";
  cout << "              Benchmark Summary\n";
  cout << "============================================\n";

  cout << "Dictionary Loading      : " << loadTime.count() << " ms\n";

  cout << "Insert                  : "
       << (double)insertTime.count() / INSERT_ITER << " ns\n";

  cout << "Search                  : "
       << (double)searchTime.count() / SEARCH_ITER << " ns\n";

  cout << "Prefix Search           : "
       << (double)prefixTime.count() / SEARCH_ITER << " ns\n";

  cout << "Autocomplete            : " << (double)autoTime.count() / AUTO_ITER
       << " ns\n";

  cout << "Spell Correction        : " << (double)spellTime.count() / SPELL_ITER
       << " ns\n";

  cout << "Wildcard Search         : "
       << (double)wildcardTime.count() / WILD_ITER << " ns\n";

  cout << "============================================\n";

  // Prevent compiler from removing benchmarked code
  cout << "\nIgnore: " << found << prefix << cnt << endl;

  return 0;
}