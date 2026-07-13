#include "TrieNode.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
class Trie {
public:
  Trie() { root = new TrieNode(); }
  ~Trie() { deleteTrie(root); }
  void deleteTrie(TrieNode *root) {
    if (root == nullptr) {
      return;
    }
    for (auto &child : root->get_children()) {
      deleteTrie(child.second);
    }

    delete root;
  }
  void saveFrequency(const string &filename) {
    vector<Suggestion> words;
    dfs(root, "", words);
    std::ofstream outFile(filename);
    if (!outFile) {
      cerr << "Error: Could not open the file for writing!" << endl;
      return;
    }
    for (const auto &s : words) {
      outFile << s.word << " " << s.frequency << endl;
    }
    outFile.close();
  }
  void addtodictonary(const string &word, const string &filename) {
    ofstream out(filename, ios::app);
    if (!out) {
      cerr << "failed to open file " << filename << endl;
      return;
    }
    out << word << endl;
  }
  void loadfrequency(const string &filename) {
    ifstream infile(filename);
    if (!infile.is_open()) {
      cerr << "Can't open file please check it's path" << endl;
      return;
    }
    string word;
    int frequency;
    while (infile >> word >> frequency) {

      TrieNode *node = findPrefixNode(word);
      if (node != nullptr) {
        node->setFrequency(frequency);
      }
    }
    infile.close();
  }
  void insert(const string &word) {
    TrieNode *node = root;
    for (int i = 0; i < word.size(); i++) {
      TrieNode *current;
      if (node->hasChild(word[i]) == false) {
        current = new TrieNode();
        node->set_child(current, word[i]);

      } else {
        current = node->get_TrieNode(word[i]);
      }

      node = current;
    }

    node->SetIsEndTrue();
  }
  void insert(const string &word, int documentId) {
    TrieNode *node = root;

    for (char c : word) {
      if (!node->hasChild(c))
        node->set_child(new TrieNode(), c);

      node = node->get_TrieNode(c);
    }

    node->SetIsEndTrue();

    node->postingList[documentId]++;
  }

  bool search(const string &word) {
    TrieNode *node = findPrefixNode(word);

    return node != nullptr && node->isEnd();
  }
  void record_selection(const string &word) {
    TrieNode *node = this->findPrefixNode(word);
    TrieNode *root1 = root;
    if (node != nullptr && node->isEnd()) {
      node->increase_frequency();

      for (int i = 0; i < word.size(); i++) {
        Suggestion s;
        s.word = word;
        s.frequency = node->get_frequency() - 1;
        root1->SuggestionList.erase(s);
        s.frequency++;
        root1->SuggestionList.insert(s);

        if (root1->SuggestionList.size() > 5)
          root1->SuggestionList.erase(*root1->SuggestionList.begin());
        root1 = root1->get_TrieNode(word[i]);
      }
      if (root1 != nullptr) {
        Suggestion s;
        s.word = word;
        s.frequency = node->get_frequency() - 1;
        root1->SuggestionList.erase(s);
        s.frequency++;
        root1->SuggestionList.insert(s);

        if (root1->SuggestionList.size() > 5)
          root1->SuggestionList.erase(*root1->SuggestionList.begin());
      }
    }
  }

  bool startsWith(const string &prefix) {
    return this->findPrefixNode(prefix) != nullptr;
  }
  void loadDictionary(const string &filename) {
    ifstream dictionary(filename);
    if (!dictionary.is_open()) {
      cerr << "Can't open file please check it's path" << endl;
      return;
    }
    string word;
    while (getline(dictionary, word)) {

      if (word.empty())
        continue;

      this->insert(word);
    }
    dictionary.close();
  }
  TrieNode *findPrefixNode(const string &prefix) {
    TrieNode *node = root;
    for (int i = 0; i < prefix.size(); i++) {
      TrieNode *current;
      if (node->hasChild(prefix[i]) == false) {

        return nullptr;

      } else {
        current = node->get_TrieNode(prefix[i]);
      }

      node = current;
    }
    return node;
  }

  vector<Suggestion> autocomplete(const string &prefix) {
    TrieNode *node = findPrefixNode(prefix);
    vector<Suggestion> result;
    if (node == nullptr)
      return result;

    set<Suggestion> suggestions = node->SuggestionList;
    while (suggestions.size() != 0) {
      result.push_back(*suggestions.begin());
      suggestions.erase(*suggestions.begin());
    }

    if (result.size() > 5)
      result.resize(5);
    return result;
  }
  void call_dfs() { root->SuggestionList = this->dfs(this->root, "", 5); }
  set<pair<pair<long long, long long>, string>>
  genrate_suggestion(string curr) {
    set<pair<pair<long long, long long>, string>> result;
    vector<long long> result1;
    get_suggestion(root, curr, result, result1, "");
    return result;
  }

  set<Suggestion> wild_card_suggestion(string s) {
    return get_suggestion_wild_card(root, s, 0, "");
  }

private:
  TrieNode *root;
  set<Suggestion> get_suggestion_wild_card(TrieNode *root, string a,
                                           long long i, string d) {
    set<Suggestion> list;
    if (root == nullptr)
      return list;
    else if (i == a.size() && root->isEnd()) {
      Suggestion word1;
      word1.word = d;
      word1.frequency = root->get_frequency();

      list.insert(word1);
      return list;
    } else if (a.size() == i) {
      return list;

    } else {
      Suggestion word1;
      if (a[i] != '?' && a[i] != '*') {
        return get_suggestion_wild_card(root->get_TrieNode(a[i]), a, i + 1,
                                        d + a[i]);
      } else if (a[i] == '?') {
        for (const auto &s : root->get_children()) {
          set<Suggestion> list1 =
              get_suggestion_wild_card(s.second, a, i + 1, d + s.first);
          while (list1.size() != 0) {
            list.insert(*list1.begin());

            list1.erase(list1.begin());
            if (list.size() > 5)
              list.erase(list.begin());
          }
        }
        return list;
      } else {
        for (const auto &s : root->get_children()) {

          set<Suggestion> list1 =
              get_suggestion_wild_card(s.second, a, i + 1, d + s.first);
          while (list1.size() != 0) {
            list.insert(*list1.begin());

            list1.erase(list1.begin());
            if (list.size() > 5)
              list.erase(list.begin());
          }

          list1 = get_suggestion_wild_card(s.second, a, i, d + s.first);
          while (list1.size() != 0) {
            list.insert(*list1.begin());

            list1.erase(list1.begin());
            if (list.size() > 5)
              list.erase(list.begin());
          }
        }
        set<Suggestion> list1 = get_suggestion_wild_card(root, a, i + 1, d);
        while (list1.size() != 0) {
          list.insert(*list1.begin());

          list1.erase(list1.begin());
          if (list.size() > 5)
            list.erase(list.begin());
        }
        return list;
      }
    }
  }
  void get_suggestion(TrieNode *node, const string &d,
                      set<pair<pair<long long, long long>, string>> &result,
                      vector<long long> &result1, string curr) {
    vector<long long> tmp_result;
    if (node == root) {
      for (int i = 0; i < d.size() + 1; i++)
        tmp_result.push_back(i);
    } else {

      for (int i = 0; i < d.size() + 1; i++) {
        if (i == 0) {
          tmp_result.push_back(result1[0] + 1);
        } else {
          long long ans =
              min(min(result1[i] + 1, tmp_result.back() + 1),
                  result1[i - 1] + (d[i - 1] != curr[curr.size() - 1]));
          tmp_result.push_back(ans);
        }
      }
      if (node->isEnd())
        result.insert({{tmp_result.back(), -node->get_frequency()}, curr});
      if (result.size() > 5) {
        result.erase(prev(result.end()));
      }
    }

    if (*min_element(tmp_result.begin(), tmp_result.end()) > 3)
      return;
    for (const auto &children : node->get_children()) {
      get_suggestion(children.second, d, result, tmp_result,
                     curr + children.first);
    }
  }

  set<Suggestion> dfs(TrieNode *node, string current, long long k) {
    set<Suggestion> pq;

    if (node->isEnd()) {
      Suggestion s;
      s.word = current;
      s.frequency = node->get_frequency();
      pq.insert(s);
      while (pq.size() > k) {
        pq.erase(pq.begin());
      }
    }

    for (const auto &children : node->get_children()) {
      set<Suggestion> pq1 = dfs(children.second, current + children.first, 5);
      for (const auto &s : pq1) {
        pq.insert(s);

        if (pq.size() > k) {
          pq.erase(pq.begin());
        }
      }
    }
    node->SuggestionList = pq;
    return pq;
  }
  void dfs(TrieNode *node, string current, vector<Suggestion> &answer) {

    if (node->isEnd()) {
      Suggestion s;
      s.word = current;
      s.frequency = node->get_frequency();
      answer.push_back(s);
    }

    for (const auto &children : node->get_children()) {
      dfs(children.second, current + children.first, answer);
    }
  }
};