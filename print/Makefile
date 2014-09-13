.PHONY: clean all
.SECONDARY:
OPT ?= 3
SECMARGIN ?= 128
all: everything.pdf

cifrario.tex:	
	ln -s ../instruction_resistance_map.tex cifrario.tex

cifrario.s: cifrario.c $(shell which clang)
	./clang -O0 $< -emit-llvm -S -g -o cifrario.s

cifrario.opt.$(OPT).s: cifrario.s $(shell which opt)
ifeq ($(OPT),0)
	cp $< $@
else
	./opt -sroa -O2 -TaggedData $< -S -o $@
endif

cifrario.dot: cifrario.opt.$(OPT).s $(shell which opt)
	mkdir out.dir || true
	nice -n19 ./opt -dfgprint -righe-cifrario=righe_cifrario $< -analyze -nocryptofa-mask-everything=$(FULLMASK) -nocryptofa-security-margin=$(SECMARGIN) |grep -v '^Printing '>$@
	rm -r $(basename $@).dir ||true
	mv out.dir $(basename $@).dir
	sed -i 's/, !.*\\n}/\\n}/' cifrario.dot
	sed -i 's/, align [0-9]\+//' cifrario.dot
cifrario.svg: cifrario.dot
	dot -Tsvg $< -o $@

cifrario.graphml: cifrario.dot
	cat $< | sed 's/cifrario\([0-9]\{2\}\)/cifrario  \1/g'|graph-easy - --graphml >$@

cifrario.yed.graphml: cifrario.graphml yedize.xslt
	saxon8  $< yedize.xslt >$@

everything.pdf:	prot_dpa.pdf backward.pdf forward.pdf nonlinbit.pdf
	ln -s ../everything.tex . ||true
	pdflatex everything.tex

prot_dpa.pdf: cifrario.dot cifrario.tex
	pdflatex -jobname=prot_dpa '\newcommand{\filename}{prot_dpa.dat}\newcommand{\currentalgo}{$(shell basename $$PWD)}\input{cifrario}'

backward.pdf: cifrario.dot cifrario.tex
	pdflatex -jobname=backward '\newcommand{\filename}{backward.dat}\newcommand{\currentalgo}{$(shell basename $$PWD)}\input{cifrario}'

forward.pdf: cifrario.dot cifrario.tex
	pdflatex -jobname=forward '\newcommand{\filename}{forward.dat}\newcommand{\currentalgo}{$(shell basename $$PWD)}\input{cifrario}'
	
nonlinbit.pdf: cifrario.dot cifrario.tex
	pdflatex -jobname=nonlin '\newcommand{\filename}{nonlinbit.dat}\newcommand{\currentalgo}{$(shell basename $$PWD)}\input{cifrario}'

clean:
	rm -r *.dir *.s *.bc *.dot *.asm *.svg *.pdf *.aux *.dat *.log *.tex
