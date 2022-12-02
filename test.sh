#!/bin/bash
# ./build.zsh > build.log
# flag=0
# for ((var = 1 ; $var <= 13; var++))
# do
#     FILENAME='testcases/basic-testcases/test'$var
#     startTime=`date +"%s.%N"`
#     ./bin/pyinter < $FILENAME.in > test.out
#     endTime=`date +"%s.%N"` 
#     DIFF=$(diff test.out $FILENAME.out)
#     if ["$DIFF" == ""]
#     then
#         echo "Answer correct on testdata basic-testcases/"$var"."
#     else
#         flag=1
#     fi
#     echo `awk -v x1="$(echo $endTime | cut -d '.' -f 1)" -v x2="$(echo $startTime | cut -d '.' -f 1)" -v y1="$[$(echo $endTime | cut -d '.' -f 2) / 1000]" -v y2="$[$(echo $startTime | cut -d '.' -f 2) /1000]" 'BEGIN{printf "RunTime:%.6f s",(x1-x2)+(y1-y2)/1000000}'`
# done
# if [ $flag == 1 ]
# then
#     echo "Failed test on basic-testcases"
# else
#     echo "Succeed on basic-testcases"
# fi
# flag=0
# for ((var = 1 ; $var <= 13; var++))
# do
#     FILENAME='testcases/bigint-testcases/BigIntegerTest'$var
#     startTime=`date +"%s.%N"`
#     ./bin/pyinter < $FILENAME.in > test.out
#     endTime=`date +"%s.%N"` 
#     DIFF=$(diff test.out $FILENAME.out)
#     if ["$DIFF" == ""]
#     then
#         echo "Answer correct on testdata bigint-testcases/"$var"."
#     else
#         flag=1
#     fi
#     echo `awk -v x1="$(echo $endTime | cut -d '.' -f 1)" -v x2="$(echo $startTime | cut -d '.' -f 1)" -v y1="$[$(echo $endTime | cut -d '.' -f 2) / 1000]" -v y2="$[$(echo $startTime | cut -d '.' -f 2) /1000]" 'BEGIN{printf "RunTime:%.6f s",(x1-x2)+(y1-y2)/1000000}'`
# done
# if [ $flag == 1 ]
# then
#     echo "Failed test on bigint-testcases"
# else
#     echo "Succeed on bigint-testcases"
# fi
flag=0
for ((var = 1 ; $var <= 53; var++))
do
    FILENAME='testcases/testData/test'$var
    startTime=`date +"%s.%N"`
    ./bin/pyinter < $FILENAME.in > test.out
    endTime=`date +"%s.%N"` 
    DIFF=$(diff -b test.out $FILENAME.out)
    if ["$DIFF" == ""]
    then
        echo "Answer correct on testdata custom testdata/"$var"."
    else
        flag=1
    fi
    echo `awk -v x1="$(echo $endTime | cut -d '.' -f 1)" -v x2="$(echo $startTime | cut -d '.' -f 1)" -v y1="$[$(echo $endTime | cut -d '.' -f 2) / 1000]" -v y2="$[$(echo $startTime | cut -d '.' -f 2) /1000]" 'BEGIN{printf "RunTime:%.6f s",(x1-x2)+(y1-y2)/1000000}'`
done
if [ $flag == 1 ]
then
    echo "Failed test on custom testdata"
else
    echo "Succeed on custom testdata"
fi