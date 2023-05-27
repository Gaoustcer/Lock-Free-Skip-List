for((i=1;i<33;i++))
do
    (time ./build/test_$i) 2>> ./build/runtime.log
done 