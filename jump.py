#!/usr/bin/env python2

import matplotlib.pyplot as plt
import numpy as np

def jump(t):
  t = t - 30
  v = (-(t * t / 2.0) + 450) / 450
  v = v if v > -0.8 else -0.8
  return v

t = range(120)
s = []

for v in t:
  s.append(jump(v))

plt.plot(t,s)  # on utilise la fonction sinus de Numpy
plt.ylabel('fonction sinus')
plt.xlabel("l'axe des abcisses")
plt.show()
