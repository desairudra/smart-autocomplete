#include "searchengine.h"
#include <limits>

void printMenu() {
  cout << "=============================\n"
       << "     Mini Search Engine\n"
       << "=============================\n"
       << "1. Search Word\n"
       << "2. Check Prefix\n"
       << "3. Autocomplete\n"
       << "4. Insert New Word\n"
       << "5. Spell Correction\n"
       << "6. Wildcard Search\n"
       << "7. Upload Document\n"
       << "8. Exit\n"
       << "Choice > ";
}

int main() {
  SearchEngine engine;

  engine.load();

  while (true) {
    printMenu();

    int choice;
    cin >> choice;

    if (choice == 1) {
      string word;

      cout << "Enter word : ";
      cin >> word;

      vector<pair<double, int>> result = engine.search(word);

    }

    else if (choice == 2) {
      string prefix;

      cout << "Enter prefix : ";
      cin >> prefix;

      if (engine.startsWith(prefix))
        cout << "Prefix exists.\n";
      else
        cout << "No words with this prefix.\n";
    }

    else if (choice == 3) {
      string prefix;

      cout << "Enter prefix : ";
      cin >> prefix;

      auto result = engine.autocomplete(prefix);

      if (result.empty()) {
        cout << "No suggestions found.\n";
      } else {
        cout << "Top Suggestions\n";

        for (const auto &s : result)
          cout << s.word << '\n';
      }
    }

    else if (choice == 4) {
      string word;

      cout << "Enter word : ";
      cin >> word;

      if (engine.checks(word)) {
        cout << "Word already exists.\n";
      } else {
        engine.insert(word);
        cout << "Inserted successfully.\n";
      }
    }

    else if (choice == 5) {
      string word;

      cout << "Enter incorrect word : ";
      cin >> word;

      auto suggestions = engine.genrate_suggestion(word);

      if (suggestions.empty()) {
        cout << "No suggestions found.\n";
      } else {
        int i = 1;

        for (const auto &s : suggestions) {
          cout << i++ << ". " << s.second << '\n';
        }
      }
    }

    else if (choice == 6) {
      string pattern;

      cout << "Enter wildcard pattern : ";
      cin >> pattern;

      auto suggestions = engine.wild_card_suggestion(pattern);

      if (suggestions.empty()) {
        cout << "No suggestions found.\n";
      } else {
        int i = 1;

        for (const auto &s : suggestions) {
          cout << i++ << ". " << s.word << '\n';
        }
      }
    }

    else if (choice == 7) {
      string path;

      cout << "Enter path of .txt file : ";
      cin >> path;

      engine.uploadDocument(path);
    }

    else if (choice == 8) {
      engine.save();
      break;
    }

    else {
      cout << "Invalid choice.\n";
    }

    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();

    system("clear");
  }

  return 0;
}