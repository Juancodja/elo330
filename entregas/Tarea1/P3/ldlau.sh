#/bin/sh

tmpfile="/tmp/f$$"
last -w > $tmpfile # Store the result of last in a temp file


declare -A tldcount # Declare a hash dict like object

until ! read -r -a line 
    do
        IFS="." read -a tld <<< ${line[2]} # Strip the ip line and get the TLD
        if [ ${#tld[@]} -gt 1 ]
        then
            if [[ ! tldcount[${tld[-1]}] =~ [0-9] ]]
            then    
                if [ ! -v tldcount[${tld[-1]}] ]
                then
                        let tldcount[${tld[-1]}]++
                fi
            fi
        fi
    done < $tmpfile

rm $tmpfile  #Clear the temp file

for key in "${!tldcount[@]}"; do # Save the count in the temp file and sort the result
    echo "${tldcount[$key]} $key " >> $tmpfile
done
sort -r $tmpfile

rm $tmpfile

