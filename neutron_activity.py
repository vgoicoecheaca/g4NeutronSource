from audioop import mul
import matplotlib.pyplot as plt 
import numpy as np
import ROOT as rt
import scipy.optimize
from scipy.stats import binned_statistic_2d

plt.style.use('mystyle.mlstyle')

fnames = ["source_Water_pellet/root/source_Water_pellet0p5mm.root",
          "source_Water_pellet/root/source_Water_pellet1mm.root",
          "source_Water_pellet/root/source_Water_pellet1p5mm.root",
          "source_Water_pellet/root/source_Water_pellet2mm.root",
          "source_Water_pellet/root/source_Water_pellet2p5mm.root",
          "source_Water_pellet/root/source_Water_pellet3mm.root",
          "source_Water_pellet/root/source_Water_pellet3p5mm.root",
          "source_Water_pellet/root/source_Water_pellet4mm.root"]

ndecay       = 1e7
mus          = 0.03
buildups     = 1 

cut_neutrons = "type==\"neutron\""
def read_data(fname):
    f = rt.TFile(fname,"READ")
    tree    =  f.Get("nstree")
    edeps   = tree.GetEntries() 
    edeps_n = tree.GetEntries(cut_neutrons)

    return [edeps, edeps_n]

ts = np.arange(0.5,4.5,0.5) # in mm
min_t, max_t = ts[0]-1,ts[-1]+1
neutrons = np.array([read_data(fnames[i]) for i in range(len(fnames))])

def activity(size):
    "AmO2Be13 so 241 + 16*2 + 4*13 = 325g/mol so 0.74 of mol is Am241 "
    rho         = 0.00369 # g /m^3
    mass_pellet = (3.1416*(0.003**2))*(size/1000)*rho  
    mass_am241  = mass_pellet*0.74 #in g 
    avo_number  = 6.02*10**23 
    n_am241     = (avo_number/241)*mass_am241
    t_half      = 432.2*3.154e+7  

    return (0.693*n_am241)/t_half  # 1/s 

acts      = np.round(activity(ts),2)
lifetimes = np.round(ndecay/activity(ts),2)
print("Lifetime need now is ", np.round(1e8/activity(ts),2))


print(acts)
print("Simulated lifetimes in sec", lifetimes)

fig = plt.figure()
ax = fig.add_subplot(111)
ax_right = ax.twinx()

ax.plot(ts,neutrons[:,0]/lifetimes,"bx",label = "Neutrons")
ax.set_xlabel("Pellet Thickness [mm]", loc = 'center')
ax.set_ylabel("Neutron activity [1/s]",loc = 'center')
ax.tick_params(axis='y')
ax.grid(True)
ax.set_xlim(0, max_t)
ax.set_ylim(0)

def line(x,m,b):
    return m*x + b

pars,_ = scipy.optimize.curve_fit(line, ts,neutrons[:,0]/lifetimes)
ax.plot(np.linspace(min_t,max_t,100),line(np.linspace(min_t,max_t,100),pars[0],pars[1]), "r--",label="Linear Fit") 
ax.legend(loc='best')

ax_right.plot(ts,activity(ts), "wx",alpha = 0.0)
ax_right.tick_params(axis="y")
ax_right.set_ylabel("Source Activity (kBq)", loc = 'center')
plt.show()

