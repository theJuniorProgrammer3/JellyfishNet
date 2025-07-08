#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

// === Global Variables ===
unordered_map<string, uint32_t> vocab;
unordered_map<uint64_t, vector<uint32_t>> hasilToOutput;

// === Fungsi: Bangun Vocab dari Dataset ===
void vocabing() {
    ifstream file("dataset.txt");
    if (!file) {
        cerr << "file tidak ada" << endl;
        return;
    }

    string line, word;
    stringstream ss;

    while (getline(file, line)) {
        ss.str(line);
        ss.clear();  // reset flags

        while (ss >> word) {
            if (vocab.find(word) == vocab.end()) {
                vocab[word] = vocab.size() + 1;
            }
        }

        ss.str("");  // reset isi stream
    }

    file.close();
    cout << "Nge-vocab udah selesai! Jumlah kata: " << vocab.size() << endl;
}

// === Fungsi: Training Model dengan Context Window ===
void train() {
    ifstream file("dataset.txt");
    if (!file) {
        cerr << "file tidak ada" << endl;
        return;
    }

    const int window = 30;
    string line, word;

    while (getline(file, line)) {
        stringstream ss(line);
        vector<uint32_t> tokens;

        // Tokenisasi baris ke ID
        while (ss >> word) {
            if (vocab.count(word)) {
                tokens.push_back(vocab[word]);
            }
        }


        // Sliding window
const int maxWindow = 30;

for (size_t i = 0; i < tokens.size(); ++i) {
    for (int w = 1; w <= maxWindow && i + w < tokens.size(); ++w) {
        uint64_t total = 0;
        for (int j = 0; j < w; ++j) {
            total += (j + 1) * tokens[i + j];
        }
        uint32_t target = tokens[i + w];
        hasilToOutput[total].push_back(target);
    }
}
}
    file.close();
    cout << "Training selesai! Jumlah sampel: " << hasilToOutput.size() << endl;
}
// === Simpan Model ===
void saveModel() {
	ofstream file("model.txt");
	file << "Vocabs:\n";
	for(auto a : vocab) {
		file << a.first << ": " << a.second << "\n";
	}
	file << "Matchs:\n";
	for(auto b : hasilToOutput) {
		file << b.first <<":\n[\n";
		for(auto c : b.second) {
			file << c << "\n";
		}
		file << "]\n";
	}
	file.close();
}

// === Main ===
int main() {
    vocabing();
    train();
    saveModel();
    return 0;
}
