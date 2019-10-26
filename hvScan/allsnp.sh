infile=$1
brfile=${infile::-4}

HVlongArray=(-100 -50 0 50 100)
HVshortArray=(-50 -25 0 25 50)

for i in {0..4}
do
    #Change long bars
    ./Build/mksnp ${HVlongArray[i]} 2 ${infile}
    #Define file for long Bars
    longfile=${brfile}_vset_${HVlongArray[i]}V_long.snp
    #Change short bars
    ./Build/mksnp ${HVshortArray[i]} 3 ${longfile}
    #Remove file for long Bars
    rm ${longfile}
done
