#/bin/sh

variable=${1:-`pwd`}
echo $variable



for element in ${array[@]}
do echo $element
done

text=`<sometext.txt`
echo $text

echo 
echo

until ! read -r -a array
do for element in ${array[@]}
    do echo $element
    done
    echo "---line finished"
done < sometext.txt 
echo ${array[@]}

