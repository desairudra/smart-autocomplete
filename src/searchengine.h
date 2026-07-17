#pragma once

#include "DocumentManager.h"
#include <algorithm>
#include <cmath>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
class SearchEngine {
private:
  Trie trie;
  DocumentManager documentManager;

public:
  void load() {
    trie.loadDictionary("../data/dictonary.txt");
    trie.loadDictionary("../data/user_dictonary.txt");
    trie.loadfrequency("../data/frequency.txt");
    documentManager.loadDocuments(&trie);
    trie.call_dfs();
  }
  void save() {
    trie.saveFrequency("../data/frequency.txt");

    documentManager.saveDocuments();
  }

  vector<string> tokenizeQuery(string query) {
    stringstream in(query);
    vector<string> words;
    while (in >> query) {
      string result = correct_word(query);
      if (result.empty())
        continue;
      words.push_back(result);
    }
    return words;
  }
  string correct_word(string word) {
    string result;

    for (char c : word) {
      if (isalpha(c))
        result += tolower(c);
    }
    if (contains(result))
      return result;
    set<pair<pair<long long, long long>, string>> suggestionlist =
        this->genrate_suggestion(result);
    if (suggestionlist.empty())
      return result;

    result = suggestionlist.begin()->second;

    return result;
  }
  unordered_map<int, double> computeScore(vector<string> wordslist) {
    unordered_map<int, double> score;

    int N = documentManager.totalDocuments();

    if (N == 0)
      return score;

    double avgDL = documentManager.averageDocumentLength();
    if (avgDL == 0)
      return score;

    const double k1 = 1.5;
    const double b = 0.75;
    unordered_set<string> uniqueWords(wordslist.begin(), wordslist.end());
    for (auto &word : uniqueWords) {
      TrieNode *node = trie.findPrefixNode(word);

      if (node == nullptr)
        continue;

      int df = node->postingList.size();

      if (df == 0)
        continue;

      double idf = log(1.0 + (N - df + 0.5) / (df + 0.5));

      for (auto &[docId, tf] : node->postingList) {
        Document &doc = documentManager.getDocument(docId);

        double dl = doc.totalWords;

        double numerator = tf * (k1 + 1);

        double denominator = tf + k1 * (1 - b + b * (dl / avgDL));

        score[docId] += idf * (numerator / denominator);
      }
    }

    return score;
  }

  vector<pair<double, int>> search(const string &query) {
    vector<string> wordslist = tokenizeQuery(query);
    unordered_map<int, double> result = computeScore(wordslist);
    vector<pair<double, int>> Topresult;
    get_result(result, Topresult);
    printResults(Topresult, wordslist);

    return Topresult;
  }
  void get_result(unordered_map<int, double> &result,
                  vector<pair<double, int>> &Topresult) {
    for (auto [id, score] : result) {
      Topresult.push_back({score, id});
    }
    sort(Topresult.rbegin(), Topresult.rend());
    if (Topresult.size() > 5)
      Topresult.resize(5);
  }
  bool startsWith(const string &prefix) { return trie.startsWith(prefix); }

  bool contains(const string &word) { return trie.search(word); }

  void insert(const string &word) {
    trie.insert(word);
    trie.addtodictonary(word, "../data/user_dictonary.txt");
  }

  void recordSelection(const string &word) { trie.record_selection(word); }

  vector<Suggestion> autocomplete(const string &prefix) {
    return trie.autocomplete(prefix);
  }

  set<pair<pair<long long, long long>, string>>
  genrate_suggestion(const string &word) {
    return trie.genrate_suggestion(word);
  }

  set<Suggestion> wild_card_suggestion(const string &pattern) {
    return trie.wild_card_suggestion(pattern);
  }

  void uploadDocument(const string &filepath) {
    documentManager.uploadDocument(filepath, &trie);
  }

  int totalDocuments() { return documentManager.totalDocuments(); }

  Document &getDocument(int id) { return documentManager.getDocument(id); }
  bool checks(const string &d) { return trie.search(d); }
  string getSnippet(int documentId, const vector<string> &queryWords) {
    Document &doc = getDocument(documentId);

    ifstream in(doc.filename);

    string line;

    while (getline(in, line)) {
      string lower = line;

      for (char &c : lower)
        c = tolower(c);

      for (auto &word : queryWords) {
        if (lower.find(word) != string::npos)
          return line;
      }
    }

    return "No preview available.";
  }
  void printResults(const vector<pair<double, int>> &result,
                    const vector<string> &queryWords) {
    if (result.empty()) {
      cout << "No matching documents found.\n";
      return;
    }

    cout << "\n====================================\n";
    cout << "         Search Results\n";
    cout << "====================================\n\n";

    int rank = 1;

    for (auto &[score, id] : result) {
      Document &doc = getDocument(id);

      cout << rank++ << ". " << doc.filename << "\n";

      cout << "Score : " << score << "\n";

      cout << "Snippet :\n";

      cout << getSnippet(id, queryWords) << "\n";

      cout << "\n------------------------------------\n\n";
    }
  }
  double averageDocumentLength() {
    return documentManager.averageDocumentLength();
  }
};