alphaI = 1.23e-17
beta = 3.07e-18
kb = 8.617e-05
t0 = 60
#GPFUN_alpha0 = "alpha0(Ta)=-8.9e-17+4.6e-14/Ta"
#GPFUN_tI = "tI(Ta)=(1.2e13*exp(-1.11/(kb*Ta)))"
#GPFUN_alpha = "alpha(t,Ta)=alphaI * exp(-t*tI(Ta))+alpha0(Ta)-beta*log(t/t0)"
alpha0(Ta)=-8.9e-17+4.6e-14/Ta
tI(Ta)=(1.2e13*exp(-1.11/(kb*Ta)))
alpha(t,Ta)=alphaI * exp(-t*tI(Ta))+alpha0(Ta)-beta*log(t/t0)


TA=60+273

if(!exists("tB")){tB=3600*24}
TB=20
set logscale x
p [1:1e6] alpha(x,60+273)*1e17 t '\@60 C', alpha(tB,TB+273)*1e17 lw 2 t ''.(tB/(3600*24)).' days \@ '.TB." C"

