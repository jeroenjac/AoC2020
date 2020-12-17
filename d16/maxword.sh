longest=0
for word in $(<input.txt)
do
    len=${#word}
    if (( len > longest ))
    then
        longest=$len
        longword=$word
    fi
done
longestline=0
for line in $(<input.txt)
do
    len=${#line}
    if (( len > longestline ))
    then
        longestline=$len
        longline=$line
    fi
done

printf 'The longest word is %s and its length is %d.\n' "$longword" "$longest"
printf 'Longest line is %s and l = %d :\n' "$longline" "$longestline"
awk '{print length, $0}' input.txt |sort -nr|head -1
