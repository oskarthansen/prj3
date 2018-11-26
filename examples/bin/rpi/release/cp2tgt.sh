#!/bin/bash
#Copies the file to the RPI
#Getting copied to folder "cp2tgt"
echo "The file "$1" is beeing transferred"
scp $1 root@10.9.8.2:/home/root
echo "File "$1" succeed"
echo "Transferred to folder: /home/root"
