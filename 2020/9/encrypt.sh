#!/bin/bash

chr() {
    printf \\$(printf '%03o' $1)
}

function hex() {
    printf '%02X\n' $1
}

function encrypt() {
    key=$1
    msg=$2
    crpt_msg=""
    for ((i=0; i<${#msg}; i++)); do
        c=${msg:$i:1}
        asc_chr=$(echo -ne "$c" | od -An -tuC)
        key_pos=$((${#key} - 1 - ${i}))
        key_char=${key:$key_pos:1}
        crpt_chr=$(( $asc_chr ^ ${key_char} ))
        hx_crpt_chr=$(hex $crpt_chr)
        crpt_msg=${crpt_msg}${hx_crpt_chr}
    done
    echo $crpt_msg
}



#chr "65"
#hex "99999"

key=$1
msg=$2
iteration=$3

#key=40614178165780923111223
#msg="514;248;980;347;145;332"
#iteration=-1

#msg="000;000;000;000;000;000"

#echo "for ((i=0; i<${#msg}; i++)); do"
for ((i=0; i<${#msg}; i++)); do
    #echo "c=${msg:$i:1}"
    c=${msg:$i:1}
    #echo "asciic=$(echo -ne $c | od -An -tuC)"
    asciic=$(echo -ne "$c" | od -An -tuC)
    #echo "keypos=$((${#key} - 1 - ${i}))"
    key_pos=$((${#key} - 1 - ${i}))
    #echo "key_char=${key:$key_pos:1}"
    key_char=${key:$key_pos:1}
    crpt_chr=$(( $asciic ^ ${key_char} ))
    #echo "hx_crpt_chr=$(hex $crpt_chr)"
    hx_crpt_chr=$(hex $crpt_chr)
    #echo "crpt_msg=${crpt_msg}${hx_crpt_chr}" 
    crpt_msg=${crpt_msg}${hx_crpt_chr}
    [ $i == $iteration ] && echo "$crpt_msg" && echo "$crpt_msg" > output
done

[ $iteration == -1 ] && echo $crpt_msg

#echo $(encrypt $key $msg)
