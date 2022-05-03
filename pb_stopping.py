from re import I
from tkinter import N, W
import matplotlib.pyplot as plt
import numpy as np
import ROOT as rt
import scipy.optimize

rt.gROOT
plt.style.use("mystyle.mlstyle")

fnames4p4 = ["source_Ge_4p4MeV_pb/root_dir/outfile.root",
"source_Ge_4p4MeV_pb/root_dir/source_Ge_4p4MeV_pb2mm.root",
"source_Ge_4p4MeV_pb/root_dir/source_Ge_4p4MeV_pb3mm.root",
"source_Ge_4p4MeV_pb/root_dir/source_Ge_4p4MeV_pb4mm.root",
"source_Ge_4p4MeV_pb/root_dir/source_Ge_4p4MeV_pb5mm.root",
"source_Ge_4p4MeV_pb/root_dir/source_Ge_4p4MeV_pb6mm.root",
"source_Ge_4p4MeV_pb/root_dir/source_Ge_4p4MeV_pb7mm.root",
"source_Ge_4p4MeV_pb/root_dir/source_Ge_4p4MeV_pb8mm.root",
"source_Ge_4p4MeV_pb/root_dir/source_Ge_4p4MeV_pb9mm.root",
"source_Ge_4p4MeV_pb/root_dir/source_Ge_4p4MeV_pb10mm.root",
"source_Ge_4p4MeV_pb/root_dir/source_Ge_4p4MeV_pb11mm.root"]

fnames60 = ["source_Ge_60keVGammas_pb/root_dir/outfile.root",
"source_Ge_60keVGammas_pb/root_dir/source_Ge_60keVGammas_pb2mm.root",
"source_Ge_60keVGammas_pb/root_dir/source_Ge_60keVGammas_pb3mm.root",
"source_Ge_60keVGammas_pb/root_dir/source_Ge_60keVGammas_pb4mm.root",
"source_Ge_60keVGammas_pb/root_dir/source_Ge_60keVGammas_pb5mm.root",
"source_Ge_60keVGammas_pb/root_dir/source_Ge_60keVGammas_pb6mm.root",
"source_Ge_60keVGammas_pb/root_dir/source_Ge_60keVGammas_pb7mm.root",
"source_Ge_60keVGammas_pb/root_dir/source_Ge_60keVGammas_pb8mm.root",
"source_Ge_60keVGammas_pb/root_dir/source_Ge_60keVGammas_pb9mm.root",
"source_Ge_60keVGammas_pb/root_dir/source_Ge_60keVGammas_pb10mm.root",
"source_Ge_60keVGammas_pb/root_dir/source_Ge_60keVGammas_pb11mm.root"]


fnamesLinear = ["source_Ge_4p4MeV_linear_pb/root_dir/outfile.root",
"source_Ge_4p4MeV_linear_pb/root_dir/source_Ge_4p4MeV_linear_pb2mm.root",
"source_Ge_4p4MeV_linear_pb/root_dir/source_Ge_4p4MeV_linear_pb3mm.root",
"source_Ge_4p4MeV_linear_pb/root_dir/source_Ge_4p4MeV_linear_pb4mm.root",
"source_Ge_4p4MeV_linear_pb/root_dir/source_Ge_4p4MeV_linear_pb5mm.root",
"source_Ge_4p4MeV_linear_pb/root_dir/source_Ge_4p4MeV_linear_pb6mm.root",
"source_Ge_4p4MeV_linear_pb/root_dir/source_Ge_4p4MeV_linear_pb7mm.root",
"source_Ge_4p4MeV_linear_pb/root_dir/source_Ge_4p4MeV_linear_pb8mm.root",
"source_Ge_4p4MeV_linear_pb/root_dir/source_Ge_4p4MeV_linear_pb9mm.root",
"source_Ge_4p4MeV_linear_pb/root_dir/source_Ge_4p4MeV_linear_pb10mm.root",
"source_Ge_4p4MeV_linear_pb/root_dir/source_Ge_4p4MeV_linear_pb11mm.root"]


f = rt.TFile("source_Water_Neutrons_pb/root_dir/outfile.root","READ")
tree      =  f.Get("nstree")
n_0  = tree.GetEntries("type==\"neutron\"") 
print(n_0)
def exp(x, t):
    return np.exp(-t * x)

def read_data(fname):
    f = rt.TFile(fname,"READ")
    tree      =  f.Get("nstree")
    edeps  = tree.GetEntries()
    
    return edeps

ts = range(2,12)
min_t, max_t = ts[0]-1,ts[-1]+1

gammas60  = np.array([read_data(fnames60[i]) for i in range(len(fnames60))])
gammas4p4 = np.array([read_data(fnamesLinear[i]) for i in range(len(fnamesLinear))])

I_over_I0_60 = gammas60[1:]/gammas60[0]
I_over_I0_4p4 = gammas4p4[1:]/gammas4p4[0]

pars4p4 , cv4p4 = scipy.optimize.curve_fit(exp, ts, I_over_I0_4p4)

mu4p4          = pars4p4[0]
rho            = 11.29
print("Linear Attenuation Coefficient @4.4 MeV " ,round(mu4p4,2),"1/cm" )
print("Mean Free path at 4.4 MeV", round(1/mu4p4,2), "cm")
print("Mass Attenuation Coefficient @4.4 MeV " ,round(mu4p4/rho,4),"cm^2/g" )

#plt.plot(ts,I_over_I0_60,"b^",label="60 keV")4
plt.plot(np.linspace(min_t,max_t,100),exp(np.linspace(min_t,max_t,100),pars4p4[0]), "r--",label=r"$e^{-\mu\cdot t}$") 
plt.plot(ts,I_over_I0_4p4,"bx")
plt.xlabel("Pb thickness [mm]")
plt.ylabel(r"$I\over{I_0}$",size = 24)
plt.legend(loc='best')
plt.xlim(min_t,max_t)
plt.grid(True)
plt.show()

gammas4p4 = np.array([read_data(fnames4p4[i]) for i in range(len(fnames4p4))])

##Buildup factor plot as function of thickness for both energies
def buildup(ts, mu, intensity):
    return intensity*np.exp(ts*mu) 

#try firt calculating the buildup factor and fitting to mu

buildups4p4 = buildup(ts,mu4p4,I_over_I0_4p4)
pars4p4 , cv4p4 = scipy.optimize.curve_fit(exp, ts, I_over_I0_4p4)

plt.plot(ts,buildups4p4, "bx",label = "4.4 MeV")
plt.xlabel("Pb thickness [mm]")
plt.ylabel("Buildup Factor")
plt.legend(loc='best')
plt.xlim(min_t,max_t)
plt.grid(True)
plt.show()

