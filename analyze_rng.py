import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('random_sequences.txt', header=None, delimiter=" ")
df.columns = ['RNG method', 'Seq.'] # dataframe isn't setup in the nicest way. 
                                    # May reformat for more practice, but works fine for purpose

rs1 = df[df["RNG method"] == "rs1_0"]
rs2 = df[df["RNG method"] == "rs2_0"]
# rs3 = df[df["RNG method"] == "rs3_0"]

# plot histogram of each method
rs1.plot.hist(bins = 10)
rs2.plot.hist(bins = 10)
# rs3.plot.hist(bins = 10)

# plot line plot of each method
rs1.plot.line()
rs2.plot.line()
# rs3.plot.line()

# plot scatter plot of each method
x1 = rs1.iloc[:-1, -1].to_numpy()
x2 = rs2.iloc[:-1, -1].to_numpy()
# x3 = rs3.iloc[:-1, -1].to_numpy()
y1 = rs1.iloc[1:, -1].to_numpy()
y2 = rs2.iloc[1:, -1].to_numpy()
# y3 = rs3.iloc[1:, -1].to_numpy()

_, ax1 = plt.subplots()
ax1.scatter(x1, y1)
_, ax2 = plt.subplots()
ax2.scatter(x2, y2)
# _, ax3 = plt.subplots()
# ax3.scatter(x3, y3)


plt.show()


# @TODO randomness in our appplication does not need to be good between sequences. Can take out one test and only test ONE trial