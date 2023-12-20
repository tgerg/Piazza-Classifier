#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <set>
#include <cmath>
#include "csvstream.hpp"
using namespace std;

class Classifier {
  public:
    Classifier(int argc, char **argv) {
      csvstream train(argv[1]);
      csvstream test(argv[2]);
      map<string, string> row;

      while (train >> row) {
        set<string> u;
        string label;
        string content;
        for (auto &col:row) {
          const string &column_name = col.first;
          const string &datum = col.second;
          if (column_name == "content"){
            content = datum;
            u = unique_words(datum);
            string w;
            istringstream is(datum);  
            while (is >> w) {
              ++unique[w];
            }
          }else if (column_name == "tag") {
            ++num_labels[datum];
            label  = datum;
          }
        }
        for (auto it = u.begin(); it != u.end(); ++it) {
          ++num_contains[*it];
          ++num_C_and_w[{label, *it}];
        }
        total_unique = unique.size();
        if (argc == 4) {
          cout << "  label = " << label << ", content = " << content << endl;
        }
        ++total_posts;
      }

      cout << "trained on " << total_posts << " examples" << endl;

      if (argc == 4) { 
        cout << "vocabulary size = " << total_unique << endl << endl << "classes:" << endl;
        for (auto i : num_labels) {
          cout << "  " << i.first << ", " << i.second << " examples, log-prior = " << log_prior(i.first) << endl;
        }
        cout << "classifier parameters:" << endl;
        for (auto i : num_C_and_w) {
          cout << "  " << (i.first).first << ":" << (i.first).second << ", count = " << i.second << ", log-likelihood = " << log_likelihood((i.first).second, (i.first).first) << endl;
        }    
      }
      
      //Prediction
      string l;
      string c;
      cout << endl << "test data:" << endl;
      map<string, string> trow;
      int correct = 0;
      set<string> un;
      int count = 0;
      string content;

      while (test >> trow) {
        for (auto &col:trow) {
          const string &column_name = col.first;
          const string &datum = col.second;
          if (column_name == "tag") {
            l = datum;
          }else if (column_name == "content") {
            content = datum;
            un = unique_words(datum);
          }
        }
        int cou = 0;
        for (auto i : num_labels) {
          double log_probability = log_prior(i.first);
          for (auto it = un.begin(); it != un.end(); ++it) {
              log_probability += log_likelihood(*it, i.first);
          }
          if (cou == 0) {
            max_log = log_probability;
            best_label = i.first;
          }else if (log_probability > max_log) {
            max_log = log_probability;
            best_label = i.first;
          }else if (log_probability == max_log) {
            if (i.first < best_label) {
              best_label = i.first;
            }
          }
          ++cou;
        }
        
        cout << "  correct = " << l << ", predicted = " << best_label << ", log-probability score = " << max_log << endl << "  content = " << content << endl << endl;
        if (l == best_label) {
          ++correct;
        }
            ++count;
      }
        cout << "performance: " << correct << " / " << count << " posts predicted correctly" << endl;
    }

    double log_prior(string l) {
      double posts = num_labels[l];
      return log(posts/total_posts);
    }

    double log_likelihood(string w, string l) {
      double posts = num_C_and_w[{l,w}];
      double labels = num_labels[l];
      if (posts == 0 && unique[w] > 0) {
        double n = num_contains[w];
        return log(n/total_posts);
      }else if (posts == 0 && unique[w] == 0){
        return log(1.0/total_posts);
      }
      return log(posts/labels);
    }

    set<string> unique_words(const string &str) {
      istringstream source(str);
      set<string> words;
      string word;
      while (source >> word) {
        words.insert(word);
      }
      return words;
    }
    
    private:
      int total_posts = 0;
      double total_unique = 0;
      vector<set<string>> v;
      map<string, int> unique;
      map<string, int> num_contains;
      map<string, int> num_labels;
      map<pair<string, string>, int> num_C_and_w;  //{{label:w}, num}
      double max_log;
      string best_label;
      map<string, int> w_unique;
};

int main(int argc, char **argv) {
  cout.precision(3);
  string s;
  if (argc == 4) {
    s = argv[3];
  }
  if ((argc != 3 && argc != 4) || (argc == 4 && s != "--debug")) {
    cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
    return 1;
  }
  ifstream ftrain(argv[1]);
  ifstream ftest(argv[2]);
  if (!ftrain.is_open()) {
    cout << "Error opening file: " << argv[1] << endl;
    return 1;
  }
  if (!ftest.is_open()) {
    cout << "Error opening file: " << argv[2] << endl;
    return 1;
  }

  if (argc == 4) {
    cout << "training data:" << endl;
  }
  Classifier c(argc, argv);
}