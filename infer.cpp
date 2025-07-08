#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <ctime>

using namespace std;

unordered_map<string, uint32_t> vocab;
unordered_map<uint64_t, vector<uint32_t>> hasilToOutput;

// === Fungsi: Load Model dari File ===
void loadModel(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Gagal membuka file model.\n";
        exit(1);
    }

    string line;
    bool readingVocab = false;
    bool readingMatch = false;

    while (getline(file, line)) {
        if (line == "Vocabs:") {
            readingVocab = true;
            readingMatch = false;
            continue;
        }
        if (line == "Matchs:") {
            readingVocab = false;
            readingMatch = true;
            continue;
        }

        if (readingVocab) {
            size_t pos = line.find(": ");
            if (pos != string::npos) {
                string word = line.substr(0, pos);
                uint32_t id = stoi(line.substr(pos + 2));
                vocab[word] = id;
            }
        } else if (readingMatch) {
            if (line.back() == ':') {
                uint64_t key = stoull(line.substr(0, line.size() - 1));
                getline(file, line); // [
                vector<uint32_t> targets;
                while (getline(file, line) && line != "]") {
                    if (!line.empty()) {
                        targets.push_back(stoi(line));
                    }
                }
                hasilToOutput[key] = targets;
            }
        }
    }

    file.close();
}

// === Fungsi: Prediksi Kata Berikutnya ===
string inferNextWord(const vector<string>& contextWords) {
    uint64_t total = 0;
    for (size_t i = 0; i < contextWords.size(); ++i) {
    const string& word = contextWords[i];
    if (vocab.count(word)) {
        total += vocab[word] * (i + 1); // Bobot posisi
    } else {
        return "<unknown word: " + word + ">";
    }
}

if (hasilToOutput.count(total) == 0) {
    // Cari key terdekat
    uint64_t closestKey = 0;
    uint64_t minDiff = UINT64_MAX;

    for (const auto& [key, _] : hasilToOutput) {
        uint64_t diff = (key > total) ? key - total : total - key;
        if (diff < minDiff) {
            minDiff = diff;
            closestKey = key;
        }
    }

    if (minDiff == UINT64_MAX) return "<no prediction>";
    total = closestKey;
}
    const auto& candidates = hasilToOutput[total];
    // uint32_t predictedID = candidates[rand() % candidates.size()]
    unordered_map<uint32_t, int> freq;
for (auto id : candidates) {
    freq[id]++;
}

uint32_t predictedID = max_element(freq.begin(), freq.end(),
    [](const pair<uint32_t, int>& a, const pair<uint32_t, int>& b) {
        return a.second < b.second;
    })->first;

    // Balikkan ID ke kata
    for (const auto& [word, id] : vocab) {
        if (id == predictedID) return word;
    }

    return "<not found>";
}

// === Main ===
int main() {
    srand(time(0));
    loadModel("model.txt");

    cout << "Masukkan kalimat sebagai konteks:\n";
    vector<string> context;
    string word;
    string words;
    getline(cin, words);
    stringstream ss(words);
    while(ss >> word) {
        context.push_back(word);
    }
    auto newContext = context;

    string prediction;

    int i = 0;

    while(prediction != "[AKHIR]" && i < 50){
    prediction = inferNextWord(newContext);
    newContext.push_back(prediction);
    i++;
    }
    cout << "Prediksi kata berikutnya:";
    for(auto m : newContext) {
	    cout << " " << m;
	    if(m.find("<unknown word:") != string::npos) break;
    }
    cout << endl;

    return 0;
}
