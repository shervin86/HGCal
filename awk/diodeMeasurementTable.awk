function order(diodeList,fluenceList, newList){
    for(diode in fluenceList){
	fList[fluenceList[diode]]=fluenceList[diode]
    }
    n = asort(fList, orderedFluenceList)
#    print n
    j=1;
    for(i=1; i<=n; i++){
	fluenceValue=orderedFluenceList[i];
	for(diode in diodeList){
#	    print fluenceValue, fluenceList[diode]
	    if(fluenceList[diode]==fluenceValue){
		newList[j]=diode;
		#print j, diode
		j++;
#		break
	    }
	}
    }
   return j-1
}

function printTable(excludeString, orderedDiodeList){

    print "#============================================================"
    printf("#%20s\t%6s\t%7s\t%7s\t%3s\t%3s\t%3s\t%3s\n", "diode", "fluence", "TCT -20C", "TCT -30C", "IV -20C", "IV -30C", "CV -20C", "CV -30C")
    
    for(i=1; i <= length(orderedDiodeList); i++){
	diode=orderedDiodeList[i]
	if(match(diode, excludeString)) continue
	thickness=substr(diode,3,3);
	if(thickness!=oldthickness){
	    oldthickness=thickness;
	    print "#------------------------------", thickness, "mu m", "--------------------------------------------------"
	}

	if(temp20[diode]=="") temp20[diode]="-"
	if(temp30[diode]=="") temp30[diode]="-"
	if(IV20[diode]=="") IV20[diode]="-"
	if(IV30[diode]=="") IV30[diode]="-"
	if(CV20[diode]=="") CV20[diode]="-"
	if(CV30[diode]=="") CV30[diode]="-"

	printf("%21s\t%6s\t%8s\t%8s\t%6s\t%6s\t%6s\t%6s\n", diode, fluence[diode], temp20[diode], temp30[diode], IV20[diode], IV30[diode], CV20[diode], CV30[diode])
    }
}

BEGIN{

nDiodes=1
}

(NF!=0){
    diode=$2
    #print $0
    if($3=="-") fluence[diode]="0"
    else fluence[diode]=$3

    temp=$4
    IV=$8
    CV=$9
    TCT=$5
    if(temp=="-30"){
	if(TCT!="-")temp30[diode]++ #="Y"
	if(IV!="-") IV30[diode]++ #="Y"
	if(CV!="-") CV30[diode]++ #="Y"
    } else if(temp=="-20"){
	if(TCT!="-") temp20[diode]++ #="Y"
	if(IV!="-") IV20[diode]++ #="Y"
	if(CV!="-") CV20[diode]++ #="Y"
    }

    diodeList[diode]=diode
    
    nDiodes++
}


END{

    # make the ordered array of indexes
    n=order(diodeList, fluence, orderedDiodeList)
    #print n,length(orderedDiodeList)
    

    printTable("P_", orderedDiodeList);
    printTable("N_", orderedDiodeList);

}