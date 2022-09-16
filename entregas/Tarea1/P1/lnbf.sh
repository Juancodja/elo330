#! /bin/bash

#Funcion para guardar la informacion requerida en archivo temporal
#tamaño_en_bytes path_completo fecha_ultimo_acceso hora_ultimo_acceso,
#crea archivo temporal
save_info(){
    read field1 filename
    read field2 size trash2
    read trash3
    read trash4
    read field3 date_a time_a trash5
    filename=${filename/’}
    filename=${filename/‘}
    echo -e "$size \t$filename \t$date_a \t${time_a:0:8}" >> $1
}

#Funcion para imprimir a pantalla (o stdin) el numero de lineas (archivos)
#solicitados en el llamado al script
print_info(){
    i=0
    while [ $i -lt $1 ]
    do
        read line || break
        echo $line
        i=$((i+1))
    done
    rm $2
}


N=$1
TMP=/tmp/f$$

#Procesamiento del path, provisto o no
if [ $# -gt 1 ]
    then DIR=$2
    else DIR=$(pwd)
fi
if [ ${DIR: -1} != '/' ]
    then DIR="$DIR"/
fi

#Llamado a stat por cada archivo o directorio en el path
#Llama a la funcion save_info() por cada iteracion
for file in "$DIR"*
do
    FILE="$DIR"$(basename "$file")
    stat "$FILE" | save_info $TMP
    #La redundancia en estas ultimas lineas es para asegurar el
    #paso de informacion en el formato correcto a la funcion save_info,
    #ademas de asegurar la correcta interpretacion de los espacios
    #en los nombres de archivos y directorios
done

#Ordena y llama la funcion para imprimir,
#borra archivo temporal
sort -rg $TMP | print_info $N $TMP