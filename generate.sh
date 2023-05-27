for((i=1;i<33;i++))
do
    make SEARCHCOUNT=$i
    mv ./build/test ./build/test_$i
done