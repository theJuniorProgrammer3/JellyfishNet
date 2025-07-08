from nltk.translate.bleu_score import sentence_bleu, SmoothingFunction

def load_file(filename):
    with open(filename, 'r', encoding='utf-8') as f:
        return [line.strip().split() for line in f if line.strip()]

ref_lines = load_file('reference-bleu.txt')
out_lines = load_file('output-bleu.txt')

assert len(ref_lines) == len(out_lines), "Jumlah baris harus sama!"

smoothie = SmoothingFunction().method4
scores = []

for ref, out in zip(ref_lines, out_lines):
    score = sentence_bleu([ref], out, smoothing_function=smoothie)
    scores.append(score)

avg_score = sum(scores) / len(scores)
print(f"Skor BLEU rata-rata: {avg_score:.4f}")
