cd test-performance

i=1
while [ $i -le 16384 ]
do
echo $i":"
php execute-dql.php $i

i=`expr $i "*" 2`
done
