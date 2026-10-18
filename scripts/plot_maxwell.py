import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
df = pd.read_csv(ROOT / "output" / "TimeStats.csv")
path = ROOT / "output" / "TimeStats.csv"

data = np.loadtxt(path, delimiter=",", skiprows=1)   

V = data[:, 0]   
P = data[:, 1]   

plt.scatter(V, P, s=10)
plt.xlabel("V")
plt.ylabel("P")
plt.grid(True)
plt.show()