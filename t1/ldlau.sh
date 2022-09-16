#/bin/sh

tmpfile="/tmp/f$$"
last -w -n 100 > $tmpfile

#mision 1, seperar tld


declare -A tldcount

until ! read -r -a line
    do
        IFS="." read -a tld <<< ${line[2]}
        if [ ${#tld} -gt 0 ]
            then
            if [ ! -v tldcount[${tld[-1]}] ]
                then
                    let tldcount[${tld[-1]}]++
            fi
        fi
    done < $tmpfile

for key in "${!tldcount[@]}"; do
    echo "$key ${tldcount[$key]}"
done


rm $tmpfile