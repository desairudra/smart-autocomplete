#include <string>
using namespace std;
class Document {
public:
  int id;
  string filename;
  int totalWords;

  Document(int id, string filename) {
    this->id = id;
    this->filename = filename;
    totalWords = 0;
  }
};