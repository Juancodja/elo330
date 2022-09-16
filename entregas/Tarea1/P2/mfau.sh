#! /bin/bash

#Funcion para guardar solo los usuarios en un archivo temporal
save_occurences(){
    read blank
    while read user rest
    do 
        if [[ "$user" == 'wtmp' ]]
            then continue
        fi
        echo $user
    done
}

#Adquiere logins | ordena (por nombre) | guarda usuario |
#elimina repetidos y entrefa numero de ocurrencias |
#ordena de mayor a menor ocurrencias
last -w | sort -b | save_occurences | uniq -c | sort -rg
