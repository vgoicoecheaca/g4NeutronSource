import uproot as ur
import matplotlib.pyplot as plt
import numpy as np
import awkward as ak

plt.style.use("mystyle.mlstyle")

fname = "data/source_1e8evts.root"
f     = ur.open(fname)
enes  = f["nstree"]["etot"].array(library= "np")
time  = f["nstree"]["time"].array(library= "ak")
cut_neutron  = "type==\"neutron\""
cut_gamma    = "type==\"gamma\""

#time is stored in awkward array so move only first deposit time 
times_0 = np.array([time[i,0]/1e9 for i in range(len(time))])
plt.hist(times_0,bins=100,histtype = 'step')
plt.ylabel("Events")
plt.xlabel("Time [ns]")
plt.yscale("log")
plt.grid(True)
plt.show()