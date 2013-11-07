1.#!/usr/bin/env python  
2.# -*- coding: utf-8 -*-   
import numpy as np
import pylab as pl

# Use numpy to load the data contained in the file

# read "data.txt"
fp = open ("../data/data.txt", "r")
firstLine = fp.readline ()
index = firstLine.find (" ")
name = firstLine[0:index]
delay = firstLine[index+1:]

result = []
for line in fp:
	end = line.find ("\n")
	if line > 0:
		line = line[0:end-2];
	result.extend(map(str,line.split(' ')))

t = pl.arange(0, int(delay) * len(result), int(delay))
pl.plot(t, result, linewidth = 1.0)
pl.ylabel("speed(KB/s)")
pl.xlabel("time(s)")
pl.title("process: "+name+"\n")
pl.savefig("data.png")
