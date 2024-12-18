1.
#!/bin/bash

passed=$1
if [ -f "${passed}" ]; then
    cat $passed;
elif [ -d "${passed}" ]; then
    cd $passed; ls;
else
    echo "Nije ni jedno ni drugo";
fi
-------------
2.
#!/bin/bash

file=$1

if [ ! -f "$file ]; then
    echo "$file not valid!"
    exit 1
fi

case "$file" in
    *.c)
        gcc "$file" -o "${file%.c}.out"  -- %.c removing sufix .c
        if [ $? -eq 0 ]; then
            echo "Compiled '$file' successfully."
        else
            echo "Failed to compile '$file'."
        fi
        ;;  # End of the *.c case block
    *.h)
        echo "Displaying contents of '$file':"
        cat "$file"
        ;;  # End of the *.h case block
    *)
        echo "'$file' is not a .c or .h file. Deleting it."
        rm "$file"
        ;;  # End of the default case block
esac

-------------
3.
#!/bin/bash

>"spoj.dat"

echo "Unosite nazive datoteke, za prekid unesite 'KRAJ'"

while true; do
    read -p "Naziv dokumenta: " input_file

    if [ "$input_file" = "KRAJ" ]; then
        break
    fi
    
    if [ -f $input_file ]; then
        cat "$input_file" >> "spoj.dat"
        echo "Concatenate done!"
    fi

    echo
    echo "Sadrzaj Filea nakon spajanja:"
    cat "spoj.dat"
done

--------
4.
#!/bin/bash

if [ $(( "$#" % 2 )) -ne 0 ]; then
    echo "Number of arguments is not even!"
    exit 1
fi

for ((i=1; i<="$#"; i+=2)) do
    source_file="${!i}"
    next=$((i+1))
    dest_file="${!next}"

    if [ -f "$source_file" ]; then
        cp "$source_file" "$dest_file"
        echo "$source_file copied to $dest_file"
    else
        "$source_file not valid!"
    fi
done

--------
5.
#!/bin/bash

direktorijum=$1
find $direktorijum maxdepth 1 -type f -size +0c | wc -l  -- u direktorijumu koji se zadaje kao ulazni argument odredjuje broj datoteka čija je veličina veda od 0B. 

----------
6.
find / -type f -name "*$1*" -size 0c 2>/dev/null -- 2>/dev/null da ne prikazuje datoteke gde je permission denied
-------------
7.
#1/bin/bash

dir=$1
newdir="$dir/KOPIJA"
if [ -d "$newdir" ]; then
    echo "Directory already exists!"
else                                            ------ sve ovo se moze zameniti sa mkdir -p "$newdir"
    mkdir "$newdir"
fi

for file in "$dir"/*.c; do
    if { -f "$file" ]; then
        cp "$file" "$newdir/$(basename "file").bak"
        fi
done

---------
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

----------
9.
#!/bin/bash

dir_path=$1

read -p "Unesite ekstenziju: " extension

for file in "$dir_path"/*"$extension"; do
    if [ -f "$file" ]; then
        echo "$file"
        head -n 2 "$file" >> "$dir_path/headers"
    fi
done

---------------
10.
#!/bin/bash

path=$1
string=$2

for file in $(grep -rl "$string" "$path"); do
    echo "Datoteka $file sadrzi string $string"
done

------------------
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

