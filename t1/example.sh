#/bin/sh

VAR="ALPHANUMER"

if [[ $VAR =~ [0-9] ]]
then echo hola
else echo chao
fi


echo [0-9]