#!/bin/sh
cd man1
for i in *.1 ; do
  page=`basename $i .1`
  man -M .. $page | ../../../man2html3.0.1/man2html -pgsize 1000 -cgiurl \$title.html > ../html1/$page.html
done
