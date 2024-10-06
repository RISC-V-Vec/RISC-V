# sanity check the outputs, parse them into usable data points

import pandas as pd

# Read space-delimited file using pd.read_csv()
df-sca = pd.read_csv('TSVC/output-sca.txt',  delim_whitespace=True)
df-avx2 = pd.read_csv('TSVC/output-avx2.txt',  delim_whitespace=True)
df-avx512 = pd.read_csv('TSVC/output-avx512.txt',  delim_whitespace=True)

# Display the DataFrame
print(df)