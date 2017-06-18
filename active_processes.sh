for ((;;))
do
ps -e -o %cpu,%pid | sort -r -k 1 | head
sleep 0.5
clear
done
