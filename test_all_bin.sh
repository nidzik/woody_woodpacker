#!/bin/bash

#for r in
func()
{
	for bina in `ls /bin | tail -n 50 `;
	do
		file /bin/$bina | grep "ELF 64-bit LSB" | awk '{ print $1}' |  rev | cut -c 2- | rev  ;
	done
}
#do echo $r ;
#done
for i in `func`;
do
	./woody_woodpacker $i > packer.out;
	echo -e "\n---------- output woody  -------------:";
	./woody
	echo -e "\n--------------- output $i --------------";
	$i
	echo -ne "\n\n\n"
	sleep 1;

			 echo $i ;
done
