#!/bin/bash


error_flag=0
verbose_flag=0
no_execute_flag=0


if [ $# -lt 4 ];then
	error_flag=1;
fi
if [ $# -eq 5 ];then
	if [ "$5" = "-v" ];then
	#	echo 2
		verbose_flag=1
	fi
	if [ "$5" = "-noexecute" ];then
	#	echo 3
		no_execute_flag=1
	fi
fi

if [ $# -eq 6 ];then
	#echo 1
	verbose_flag=1
	no_execute_flag=1
fi

#echo $verbose_flag
#echo $no_execute_flag

if [ $error_flag -eq 1 ];then
	echo "usage:"
	echo "./organize.sh <submission_folder> <target_folder> <test_folder> <ans_folder> [-v] [-noexecute]"
	echo "-v: verbose"
	echo "-noexecute: not execute code files"
	exit
fi

submissions="$1"
targets="$2"
tests="$3"
answers="$4"

if [ -d "$targets" ];then
	rm -r "$targets"
fi
mkdir "$targets"

cd "$targets"


mkdir "C"
mkdir "Python"
mkdir "Java"
cd ..







# LETS ORGANIZE!!!!!
cd "$submissions"
for f in *
	do
	if [ "${f: -4}" = ".zip" ];then
		roll_no=${f: -11: -4}
		if [ $verbose_flag -eq 1 ];then
			echo "organizing files of $roll_no"
		fi
		mkdir -p -- "../$targets/$roll_no"

		#CHECKING FOR C FILE
		unzip -qq -j "$f" "*.c" -d "../$targets/$roll_no"
		#IF C FILE IS FOUND, CONTINUE THE LOOP AND MOVE THE DIRECTORY TO TARGET/C
		if [ -n "$(find ../$targets/$roll_no -mindepth 1 -print -quit)" ];then
			mv "../$targets/$roll_no" "../$targets/C/$roll_no"

			continue
		fi

		#SAME FOR JAVA AND PYTHON FILE

		unzip -qq -j "$f" "*.java" -d "../$targets/$roll_no"
 		if [ -n "$(find ../$targets/$roll_no -mindepth 1 -print -quit)" ];then
                        mv "../$targets/$roll_no" "../$targets/Java/$roll_no"

                        continue
                fi

		unzip -qq -j "$f" "*.py" -d "../$targets/$roll_no"
 		if [ -n "$(find ../$targets/$roll_no -mindepth 1 -print -quit)" ];then
                        mv "../$targets/$roll_no" "../$targets/Python/$roll_no"

                        continue
                fi


	fi

done
cd ..







#create csv file

cd "$targets"

echo "student_id,type,matched,not_matched" > result.csv



cd ..







#LETS RUN THE CODES..................

#AT first, lets compile and run the c codes............

cd "$targets/C"


for dir in */
do
	cd "$dir"

	#RENAME
	mv -i *c main.c
	if [ $no_execute_flag -eq 1 ];then
		cd ..
		continue
	fi
	if [ $verbose_flag -eq 1 ];then
		echo "executing files of ${dir:0: -1}"
	fi
	gcc main.c -o main.out
	totaltests=$(find "../../../$tests" -type f -name "*.txt" | wc -l)
	#echo $totaltests
	m=0
	for((i=1; i<=$totaltests ; i=`expr $i + 1`))
	do
		#echo $i

		./main.out < "../../../$tests/test$i.txt" > "out$i.txt"
                diff -s "out$i.txt" "../../../$answers/ans$i.txt" > /dev/null
                if [ $? -eq 0 ]; then
                        m=`expr $m + 1`
                fi
        done
        n=`expr $totaltests - $m`
        echo "${dir:0: -1},C,$m,$n" >> ../../result.csv


	cd ..
done
cd ..
cd ..









# then, for java


cd "$targets/Java"



for dir in */
do
        cd "$dir"

        #RENAME
	mv -i *.java main.java
        if [ $no_execute_flag -eq 1 ];then
		cd ..
                continue
        fi
        if [ $verbose_flag -eq 1 ];then
                echo "executing files of ${dir:0: -1}"
        fi
        javac main.java

        totaltests=$(find "../../../$tests" -type f -name "*.txt" | wc -l)
        #echo $totaltests
	m=0
        for((i=1; i<=$totaltests ; i=`expr $i + 1`))
        do
                #echo $i

                java Main < "../../../$tests/test$i.txt" > "out$i.txt"
		diff -s "out$i.txt" "../../../$answers/ans$i.txt" > /dev/null
		if [ $? -eq 0 ]; then
    			m=`expr $m + 1`
		fi
        done
	n=`expr $totaltests - $m`
	echo "${dir:0: -1},Java,$m,$n" >> ../../result.csv
        cd ..
done
cd ..
cd ..





# then, for java


cd "$targets/Python"



for dir in */
do
        cd "$dir"

        #RENAME
	mv -i *.py main.py
        if [ $no_execute_flag -eq 1 ];then
		cd ..
                continue
        fi
        if [ $verbose_flag -eq 1 ];then
                echo "executing files of ${dir:0: -1}"
        fi

        totaltests=$(find "../../../$tests" -type f -name "*.txt" | wc -l)
        #echo $totaltests
	m=0
        for((i=1; i<=$totaltests ; i=`expr $i + 1`))
        do
                #echo $i

                python3 main.py < "../../../$tests/test$i.txt" > "out$i.txt"
                diff -s "out$i.txt" "../../../$answers/ans$i.txt" > /dev/null
                if [ $? -eq 0 ]; then
                        m=`expr $m + 1`
                fi
        done
        n=`expr $totaltests - $m`
        echo "${dir:0: -1},Python,$m,$n" >> ../../result.csv
        cd ..
done
cd ..
cd ..


















