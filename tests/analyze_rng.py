import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sb
from matplotlib import rc

rc('text', usetex=True)

df = pd.read_csv('random_sequences.txt', header=None, delimiter=" ")
df.columns = ['RNG method', 'Seq.'] # dataframe isn't setup in the nicest way. 
                                    # May reformat for more practice, but works fine for purpose

rs1 = df[df["RNG method"] == "rs1_0"]
rs2 = df[df["RNG method"] == "rs2_0"].reset_index(drop=True)


##### plot line plot of each method #####
sb.set_theme(style="whitegrid")
fig1, (ax1, ax2) = plt.subplots(2, 1, sharex=True, figsize=(12,5))
fig1.suptitle('Random number sequence generated using a PRNG')
sb.lineplot(ax=ax1, data=rs1.iloc[:-5000, -1], palette=['.15'], linewidth=0.2)
#ax1.set_xticklabels(['0','1000','2000','3000','4000','5000'])
ax1.set_title('Random numbers generated with rand() function')
ax1.set_ylabel('Random number')#
#ax1.set_xlabel('Sequence index')
sb.lineplot(ax=ax2, data=rs2.iloc[:-5000, -1], palette=['.15'], linewidth=0.2)
#ax2.set_xticklabels(['0','1000','2000','3000','4000','5000'])
ax2.set_title('Random numbers generated with standard uniform real distribution')
ax2.set_ylabel('Random number')
ax2.set_xlabel('Sequence index')
fig1.tight_layout()

##### plot scatter plot of each method #####
x1 = rs1.iloc[:-1, -1].to_numpy()
x2 = rs2.iloc[:-1, -1].to_numpy()
y1 = rs1.iloc[1:, -1].to_numpy()
y2 = rs2.iloc[1:, -1].to_numpy()

sb.set_theme(style="ticks")
fig2 = sb.jointplot(x=x1, y=y1, kind="scatter", s=7, color=".15", marginal_kws=dict(bins=25, fill=True))
fig2.set_axis_labels(xlabel='r$_i$', ylabel='r$_{i+1}$')
fig3 = sb.jointplot(x=x2, y=y2, kind="scatter", s=7, color=".15", marginal_kws=dict(bins=25, fill=True)) 
fig3.set_axis_labels(xlabel='r$_i$', ylabel='r$_{i+1}$')

plt.show()