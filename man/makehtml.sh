#!/bin/sh
cd man1
for i in *.1 ; do
  page=`basename $i .1`
  man -M .. $page | ../../../man2html3.0.1/man2html -topm 0 -botm 0 -cgiurl \$title.html > ../html1/$page.html
done
