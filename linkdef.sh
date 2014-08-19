cat > LinkDef <<EOF
#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
EOF

for class in  `grep '^class' include/*.h | cut -d ':' -f 2 | sed 's|class ||;s|[{:]||'`; 
do
    echo "#pragma link C++ class ${class}+;" >> LinkDef
done

cat >> LinkDef <<EOF
#pragma link C++ function  langaufun;
#pragma link C++ global gROOT;

#endif
EOF
