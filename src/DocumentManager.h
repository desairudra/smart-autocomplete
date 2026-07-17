#include "Document.h"
#include "Trie.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#pragma once

namespace fs = std::filesystem;
class DocumentManager {
private:
  vector<Document> documents;

  int docid = 1;

public:
  int uploadDocument(string filename, Trie *root) {
    fs::path source = filename;
    fs::path file = source.filename();
    if (fs::exists(source)) {

      if (fs::is_regular_file(source)) {
        cout << "The file exists! and uploading of the file on the server in "
                "the progress.....\n";

      } else {
        cout << "The file doesn't exist\n";
        return -1;
      }

    } else {
      cout << "The file does not exist.\n";
      return -1;
    }
    fs::path destination = fs::path("../content") / file;

    fs::copy_file(source, destination, fs::copy_options::overwrite_existing);
    ifstream in(filename);

    if (!in.is_open()) {
      cout << "Failed to open file\n";
      return -1;
    }

    string word;
    Document uploaddocuments(this->docid, destination.string());
    while (in >> word) {
      string result = normalize(word);
      if (result.empty())
        continue;

      root->insert(result, uploaddocuments.id);

      uploaddocuments.totalWords++;
    }
    documents.push_back(uploaddocuments);
    saveDocuments();
    docid++;
    return 0;
  }
  string normalize(string word) {
    string result;

    for (char c : word) {
      if (isalpha(c))
        result += tolower(c);
    }

    return result;
  }

  Document &getDocument(int id) { return documents[id - 1]; }

  int totalDocuments() { return documents.size(); }
  void saveDocuments() {
    ofstream out("../data/documents.txt");

    for (auto &doc : documents) {
      out << doc.id << " " << doc.filename << " " << doc.totalWords << "\n";
    }
  }
  void loadDocuments(Trie *root) {
    ifstream in("../data/documents.txt");

    if (!in.is_open())
      return;

    documents.clear();

    int id, totalWords;
    string path;

    while (in >> id >> path >> totalWords) {
      Document doc(id, path);
      doc.totalWords = totalWords;

      documents.push_back(doc);

      docid = max(docid, id + 1);

      ifstream file(path);

      string word;

      while (file >> word) {
        word = normalize(word);

        if (word.empty())
          continue;

        root->insert(word, id);
      }
    }
  }
  double averageDocumentLength() {
    if (documents.empty())
      return 0;

    long long total = 0;

    for (auto &doc : documents)
      total += doc.totalWords;

    return (double)total / documents.size();
  }
};