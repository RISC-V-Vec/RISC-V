import pandas as pd
import functools as ft
import matplotlib.pyplot as plt 

categories = ["scalar", "avx2", "avx512"]

instruction_counts = [15053891747413, 4866719040862, 3407582745411]

speedup_ratios = [1, 15053891747413/4866719040862, 15053891747413/3407582745411]

print(speedup_ratios)

plt.bar(categories, speedup_ratios)

plt.show()

# Read space-delimited file using pd.read_csv()
df_sca = pd.read_csv('TSVC/output-sca.txt',  delim_whitespace=True).drop("Checksum", axis=1)
df_avx2 = pd.read_csv('TSVC/output-avx2.txt',  delim_whitespace=True).drop("Checksum", axis=1)
df_avx512 = pd.read_csv('TSVC/output-avx512.txt',  delim_whitespace=True).drop("Checksum", axis=1)

frames = [df_sca, df_avx2, df_avx512]

df_final = ft.reduce(lambda left, right: pd.merge(left, right, on='Loop'), frames)

df_final.columns = ['Loop', 'Time(Sec)_scalar', 'Time(Sec)_avx2', 
                    'Time(Sec)_avx512']

labels = df_final["Loop"].tolist()

print(labels)
