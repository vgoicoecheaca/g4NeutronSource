#!/usr/bin/perl

$nevents    = 10000000;
$path       = $ENV{PWD};
my @ts      = (0.5,1,1.5,2,2.5,3,3.5,4);
my @ts_st   = ("0p5","1","1p5","2","2p5","3","3p5","4");
my @zs      = (-3.25,-3,-2.75,-2.5,-2.25,-2,-1.75,-1.5);
$mat        = "Water";
$dir_name   = "source_${mat}_pellet";
$simpath    = "${path}/${dir_name}";
$simpath_mac = "${path}/${dir_name}/mac";
$simpath_job = "${path}/${dir_name}/job";
$simpath_root = "${path}/${dir_name}/root";


if (! (-e $simpath)){
    system("mkdir $simpath");
    system("mkdir $simpath_mac");
    system("mkdir $simpath_job");
    system("mkdir $simpath_root");

}

for ($i=0;$i<8;++$i){
    $st       = $ts_st[$i];
    $name     = "${dir_name}${st}mm"; 
    $macfile  = "${name}.mac";
    $jobfile  = "${name}.sh";
    $rootfile = "${name}.root"; 
    $half_z   = $ts[$i] / 2;
    $center   = $zs[$i];
    $pelletH  = $ts[$i];

#Fill the macro file
open FOUT,">$macfile";
    print FOUT <<"EOF";
/run/verbose 0
/event/verbose 0
/tracking/verbose 0 
/run/initialize

/ns/gun/particle ion
/ns/gun/ion 95 241
/process/had/rdm/thresholdForVeryLongDecayTime 434 year
/ns/detector/pelletH $pelletH mm
/ns/detector/targetMat $mat

/ns/gun/type Volume
/ns/gun/shape Cylinder
/ns/gun/center 0. 0. $center mm
/ns/gun/radius 3 mm
/ns/gun/halfz  $half_z mm
/ns/gun/confine pellet

/run/beamOn $nevents
EOF
#end of macro ifle

# fill the job file    
open FOUT,">$jobfile";
    print FOUT <<"EOF"; 
#!/bin/bash

./neutronsource $macfile  
mv outfile.root $rootfile   

EOF
#end of job file

#run the job
system("chmod u+x $jobfile");
system("./$jobfile");
system("mv $macfile $simpath_mac");
system("mv $jobfile $simpath_job");
system("mv $rootfile $simpath_root");
}