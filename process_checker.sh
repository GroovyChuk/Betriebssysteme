for ((;;))
do
first=$(ps -e -o %cpu,%pid | sort -r -k 1 | head -n 2 | tail -n 1 > prozess.txt)
sleep 5
second=$(ps -e -o %cpu,%pid | sort -r -k 1 | head -n 2 | tail -n 1 )
prozent=$(grep " 0.5 " prozess.txt)
echo $prozent
if [ "$first" == "$second" ]  && [ -z "$prozent" ];
then 
	echo $first
	echo $second
fi
done
