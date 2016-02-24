#!/bin/python
import numpy as np
import matplotlib.pyplot as pl
from sys import stdin

data = np.loadtxt(stdin, dtype=int)
pl.plot(data[:,0], data[:,1])
pl.margins(0.2)
pl.ylim(ymin=0)
pl.xkcd()
pl.savefig("plot.png")
pl.show()




