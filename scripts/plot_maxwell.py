import numpy as np
import matplotlib.pyplot as plt

V_MAX = 10000.0  # максимальная скорость, подставь свою

with open("output/Histogramm.csv") as f:
    lines = f.read().strip().splitlines()

# вторая строка: счётчики, пустой элемент после последней запятой отбрасываем
counts = np.array([float(s) for s in lines[1].split(",") if s.strip()])

n_bins = len(counts)
bin_width = V_MAX / n_bins
v = (np.arange(n_bins) + 0.5) * bin_width  # центры бинов

plt.figure(figsize=(8, 5))
plt.plot(v, counts, "o-", markersize=4)
plt.xlabel("Скорость v")
plt.ylabel("Число частиц")
plt.title("Распределение частиц по скоростям")
plt.grid(True)
plt.show()