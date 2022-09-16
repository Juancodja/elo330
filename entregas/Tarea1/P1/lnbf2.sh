#/bin/sh

n=${1:-1000}
directory=${2:-`pwd`}

cd $directory
tmpfilename="/tmp/f$$"
ls -lS > "$tmpfilename"


i=0
until ! read -r -a linewords
do
    if [ $i -gt 0 ]
        then echo -e "${linewords[4]} \t$directory/${linewords[8]}\t${linewords[@]:5:3}"    
    fi

    if [ $i -ge $n ] 
        then break
    fi
    let i++
done < "$tmpfilename"

rm "$tmpfilename"