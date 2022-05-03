#!/usr/bin/perl

#for reference, inner holder is now set to 14mm D and 9 mm H
$nevents    = 1000000;
$path       = $ENV{PWD};
my @ds      = (16,17,18,19,20,21,22,23,24,25);
my @hs      = (13,15,17,19,21,23,25,27,29,31);
my @st      = ("2","3","4","5","6","7","8","9","10","11");
$pelletH    = 4;
$half_z     = 2;
$center     = -1.5;
$mat        = "Ge";
$dir_name   = "source_${mat}_4p4MeV_pb";
$simpath    = "${path}/${dir_name}";
$simpath_mac = "${path}/${dir_name}/mac";
$simpath_job = "${path}/${dir_name}/job";
$simpath_root = "${path}/${dir_name}/root_dir";

if (! (-e $simpath)){
    system("mkdir $simpath");
    system("mkdir $simpath_mac");
    system("mkdir $simpath_job");
    system("mkdir $simpath_root");
}

for ($i=0;$i<10;++$i){
    $s        = $st[$i];
    $name     = "${dir_name}${s}mm"; 
    $macfile  = "${name}.mac";
    $jobfile  = "${name}.sh";
    $rootfile = "${name}.root"; 
    $caseD    = $ds[$i];
    $caseH    = $hs[$i];

#Fill the macro file
open FOUT,">$macfile";
    print FOUT <<"EOF";
/run/verbose 0
/event/verbose 0
/tracking/verbose 0 
/run/initialize

#/ns/gun/particle ion
#/ns/gun/ion 95 241
#/process/had/rdm/thresholdForVeryLongDecayTime 434 year

/ns/gun/particle gamma
/ns/gun/energy 4.4 MeV

/ns/detector/pelletH $pelletH mm
/ns/detector/targetMat $mat
/ns/detector/caseH $caseH mm
/ns/detector/caseD $caseD mm

#Plane
#/ns/gun/type Plane
#/ns/gun/shape Disk
#/ns/gun/center 0. 0. $center mm
#/ns/gun/radius 3 mm

/ns/gun/type Volume
/ns/gun/shape Cylinder
/ns/gun/center 0. 0. $center mm
/ns/gun/radius 3 mm
/ns/gun/halfz  $half_z mm
/ns/gun/confine Pellet

/run/beamOn $nevents
EOF
#end of macro ifle

# fill the job file    
open FOUT,">$jobfile";
    print FOUT <<"EOF"; 
#!/bin/bash

./neutronsource $macfile  
mv outfile.root $simpath_root/$rootfile   

EOF
#end of job file

#run the job
system("chmod u+x $jobfile");
system("./$jobfile");
system("mv $macfile $simpath_mac");
system("mv $jobfile $simpath_job");
}