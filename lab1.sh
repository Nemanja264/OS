#!/bin/bash

direktorijum=$1
find $direktorijum maxdepth 1 -type f -size +0c | wc -l  -- u direktorijumu koji se zadaje kao ulazni argument odredjuje broj datoteka čija je veličina veda od 0B. 

11.
#!/bin/bash

for i in {1..10}; do
    echo "$i"
    sleep 0.1  
done & 

for j in {11..20}; do
    echo "$j"
    sleep 0.1
done & 

8.
#!/bin/bash

filename=$1
line1=$2
line2=$2

if [ ! -f "$filename" ]; then
  echo "File not found"
  exit 1
fi

sed -n "${line1},${line2}p" "filename"

