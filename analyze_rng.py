import pandas as pd

df = pd.read_csv('random_sequences.txt', header=None, delimiter=" ")
df.columns = ['RNG method', 'Seq.']
print(df)