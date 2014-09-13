#!/bin/bash
TMPD=$(mktemp -d)
ALGORITHMS="3des2 3des3 aes128 aes192 aes256 camellia cast5 des desx gost serpent"
AWKPROG='BEGIN{ks=0;ciph=0; less128=0; less80=0;} {FS=";"; if($20 == 1 && $12 == 0 ){ks +=1;} if($20==1 && $12 == 1){ciph+=1; if($11 < 128){less128+=1;} if($11 < 80){less80+=1;}     } }; END { print "ks " ks "\nciph " ciph "\nless128gt80 " less128-less80 "\nless80 " less80 "\nciphgt128 " ciph-less128;}' 
declare -A STYLES
declare -A TITLES
STYLES[ks]='color=gray';
STYLES[ciphgt128]='color=green';
STYLES[less128gt80]='color=orange';
STYLES[less80]='color=red';
TITLES[ks]="Key scheduler";
TITLES[ciphgt128]="Cipher, at least 128 bits of protection";
TITLES[less128gt80]="Cipher, protection between 80 and 128";
TITLES[less80]="Cipher, less than 80 bits ";

cat <<EOF >$TMPD/chart.tex
\documentclass{article}
\usepackage{pgfplots}
\usepackage[margin=1mm,paperwidth=15cm,paperheight=13cm]{geometry}
\pgfplotsset{compat=newest}


\usepgfplotslibrary{units}
\usepgfplotslibrary{units}

\begin{document}

\begin{tikzpicture}
\begin{axis}[width=14cm,height=12cm,
    ybar stacked,
    ymin=0,
    ylabel={\#instructions},
    symbolic x coords={$(echo $ALGORITHMS|sed 's/ /,/g')},
    xtick=data,
    x tick label style={rotate=45,anchor=east},
    ]
EOF
for i in ks ciphgt128 less128gt80 less80; do
	echo '\addplot+[ybar,'${STYLES[$i]}'] plot coordinates {' >>$TMPD/chart.tex
	for a in $ALGORITHMS;do
		echo '('$a','$(cat $a/cifrario.dir/*.csv| awk "$AWKPROG" |grep $i |cut -d\  -f2)')' >>$TMPD/chart.tex
	done
	echo '}; \addlegendentry{'${TITLES[$i]}'};'>>$TMPD/chart.tex
done
cat <<EOF >>$TMPD/chart.tex
\end{axis}
\end{tikzpicture}
\end{document}
EOF
pdflatex $TMPD/chart.tex

