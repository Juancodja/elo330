#/bin/sh

variable=${1:-`pwd`}
echo $variable



for element in ${array[@]}
do echo $element
done


echo 
echo

until ! read -r -a array2
do for element in ${array[@]}
    do echo $element
    done
    echo "---line finished"
done < last.txt