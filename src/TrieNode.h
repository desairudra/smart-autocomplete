#include "suggestion.h"
#include <map>
#include <set>
#include <unordered_map>
class TrieNode {
private:
  map<char, TrieNode *> child;
  bool is_End;
  int frequency;
public:
  unordered_map<int, int> postingList;

public:
  set<Suggestion> SuggestionList;

public:
  TrieNode() {
    is_End = false;
    frequency = 0;
  }
  TrieNode *get_TrieNode(char c) {
    auto it = child.find(c);
    if (it == child.end()) {
      return nullptr;
    }
    return it->second;
  }
  void set_child(TrieNode *child, char setchildindex) {
    this->child[setchildindex] = child;
  }
  void setFrequency(int frequency) { this->frequency = frequency; }
  bool hasChild(char check) {
    auto it = child.find(check);
    if (it == child.end()) {
      return false;
    }
    return true;
  }
  void SetIsEndTrue() { this->is_End = true; }
  bool isEnd() { return this->is_End; }
  const map<char, TrieNode *> &get_children() { return child; }
  int get_frequency() { return this->frequency; }
  void increase_frequency() { frequency++; }
};