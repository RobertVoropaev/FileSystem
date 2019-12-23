#!/bin/bash
dirname="$1"
tarname="$2"

mkdir $HOME/$dirname

while [ "$3" ]
do
find ~ -name "*.$3" -exec cp {} $HOME/$dirname \;
shift
done

tar -cf $HOME/$tarname.tar $HOME/$dirname

apt-get install openssl
openssl rand -base64 32 > key.bin
openssl enc -aes-256-cfb -salt -in $HOME/$tarname.tar -out $HOME/$tarname.tar.des3 -pass file:./key.bin

echo "done"
