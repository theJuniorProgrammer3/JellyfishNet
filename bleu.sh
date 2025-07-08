#!/bin/bash

INPUT_FILE="input-bleu.txt"
OUTPUT_FILE="output-bleu.txt"

# Kosongkan file output
> "$OUTPUT_FILE"

echo "Mulai infer otomatis..."

while IFS= read -r prompt; do
    echo "Prompt: $prompt"

    # Jalankan ./infer dengan input prompt
    result=$(echo "$prompt" | ./infer)

    # Ambil baris yang mengandung hasil prediksi
    prediction=$(echo "$result" | grep "Prediksi kata berikutnya:" | sed 's/Prediksi kata berikutnya: //')

    # Simpan ke output.txt
    echo "$prediction" >> "$OUTPUT_FILE"
done < "$INPUT_FILE"

echo "Selesai! Hasil disimpan di $OUTPUT_FILE"
