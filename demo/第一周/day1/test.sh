#!/usr/bin/bash

:<<EOF t1
sum=0

for ((i=0;i<=100;i+=2))
do sum=$[sum + i]
done

echo $sum
EOF

sum=0
for i in $(seq 1 100)
do if [ $[i%2] -eq 0 ]
then sum=$[sum+i]
fi
done
echo $sum > 1.txt
						
