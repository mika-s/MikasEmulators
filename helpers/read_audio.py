#!/usr/bin/env python3

"""
Normalizes and prints out the content of a wav file,
so audio can be stored in header files.
"""

from scipy.io import wavfile as wav

rate, data = wav.read('file.wav')
data_as_float = data.astype(float)

x_max = max(data_as_float)
x_min = min(data_as_float)

delta = x_max - x_min
print('{', end='')
norm = []
volume = x_max
for p in data_as_float:
    normalized: float = ((p - x_max) / delta) - 0.5
    norm.append(normalized)
    print(str(normalized) + ', ', end='')
print('}')
