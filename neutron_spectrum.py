from dbm.ndbm import library
import uproot as ur
import matplotlib.pyplot as plt
import numpy as np
import awkward as aw

plt.style.use("mystyle.mlstyle")

fname = "data/neutrons.root"
fambe = "data/AmBe.txt"
f     = ur.open(fname)
enes  = f["nstree"]["etot"].array(library="ak")
cut_neutron  = "type==\"neutron\""
end_spectrum = 11
plt.hist(enes,range= (0,0.1) , bins=30)
plt.show()

scaleF = np.sum(enes)
hits, edges = np.histogram(enes/1000,bins = 30,range=(0,11))
hits = (hits - min(hits)) / (max(hits) - min(hits)) 
scaleF = np.sum(hits)
bins = np.diff(edges)/2 +  edges[:-1]

ambe_spectrum = np.loadtxt("data/AmBe.txt")
ambe_spectrum[:,1] = (ambe_spectrum[:,1] - min(ambe_spectrum[:,1])) / (max(ambe_spectrum[:,1]) - min(ambe_spectrum[:,1]))
plt.step(bins,hits,label = "Source")
#plt.plot(ambe_spectrum[:,0],ambe_spectrum[:,1],"r-", label="J.W.Marsh et al 1995 NIM A 366 (1995), 340-348 Fig.4")
plt.xlabel("Energy")
plt.ylabel("")
plt.grid(True)
plt.legend()
plt.show()

# get the coordinates of the image 
# transform the coordinates of the image to energy and height of spectrum (normalized)
# get the lines of each color into an array 



#def read_spectrum(file):
#
#    return spectrum
#
#ambe_spectrums   = 
#labels_spectrums = 
#spectrums        = np.array(read_spectrum([ambe_spectrums[i] for i in range(len(ambe_spectrums))]))
