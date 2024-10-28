#!/bin/bash

##################################################
############## 1 - ADJUST SETTINGS ###############
##################################################

	# manage the operations from outside
	cd ..

	# copy the makefie & run_tests executable to outside
	cp grader/Makefile ./
	cp grader/run_tests ./

	# remove result.txt
	if [ -f result.txt ]; then
		rm -f result.txt
	fi

	# remove previous outputs
	make clean

	# remove temp folder
	if [ -d temp ]; then
		rm -rf temp
	fi

	# remove submission files
	rm -rf submission/*

	# remove grades.txt
	if [ -f grades.txt ]; then
		rm -f grades.txt
	fi

##################################################
############## 2 - START EVALUATION ##############
##################################################

for i in $(ls all_submits); do

	# unzip student files
	cp all_submits/"$i"/* submission/
	unzip submission/*.zip -d submission/
	if [ -d submission/HW4 ]; then
		mv submission/HW4/* submission/
	fi
	if [ -d submission/hw4 ]; then
		mv submission/hw4/* submission/
	fi

	# extract the core parts of headers and embed into the original templates
	mkdir temp
	for j in 'Cell.h' 'Circular.h' 'MicroOrganism.h' 'Squadratic.h' 'Particle.h' 'Tissue.h' 'Triangular.h' 'CurvyWall.h' 'NucleoBase.h' 'Wall.h'; do
		if [ -f submission/$j ]; then
			# compute class definition string to be searched in the files
			part1="class "
			part2=$(echo "${j::-2}")	# remove ".h" extension
			part3=" "
			class_defn=$part1$part2$part3

			# find the start line and number of lines to be clipped in submitted header file
			start=$(cat -n submission/$j | grep "$class_defn" | awk '{print $1;}')
			end=$(cat -n submission/$j | grep "public:" | awk '{print $1;}')
			let start_index_in_submission=$start+1
			let number_of_clipped_lines_in_submission=$end-$start_index_in_submission

			# find the number of initial lines to be taken and resumption index in template header file
			number_of_initial_lines_in_template=$(cat -n grader/templates/$j | grep "$class_defn" | awk '{print $1;}')
			resumption_in_template=$(cat -n grader/templates/$j | grep "public:" | awk '{print $1;}')

			# combine the pieces clipped from submitted and template header files
			head -n $number_of_initial_lines_in_template grader/templates/$j > temp/$j
			tail --lines=+$start_index_in_submission submission/$j | head -n $number_of_clipped_lines_in_submission >> temp/$j
			tail --lines=+$resumption_in_template grader/templates/$j >> temp/$j
		fi
	done

	# gather the right format of the submitted files
	cp -f grader/templates/Exception.h submission/
	cp -f grader/templates/Main.h submission/
	cp -f temp/* submission/

	# run tests
	studentID=$(echo $i | cut -c2-)	# remove the initial 'e' character from the ID
	./run_tests $studentID

	# remove result.txt
	if [ -f result.txt ]; then
		rm -f result.txt
	fi

	# remove previous outputs
	make clean

	# remove submission files
	rm -rf submission/*

	# remove temp folder
	rm -rf temp

done

##################################################
################ 3 - CLEAR FOLDERS ###############
##################################################
	# remove result.txt
	if [ -f result.txt ]; then
		rm -f result.txt
	fi

	# remove previous outputs
	make clean

	# remove submission files
	rm -rf submission/*

	# remove makefie & run_tests executable
	rm -f Makefile
	rm -f run_tests
