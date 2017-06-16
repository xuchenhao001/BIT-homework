#!/bin/bash
# wget http://data.csail.mit.edu/soundnet/urls_public.txt

pause_at="http://www.flickr.com/videos/47762553@N00/9419455509/play/orig/8461e78d0d"
thread_num=20

PID=()
pause="false"
while read line
do
{
	downloaded="false"
	if test "$pause" = "false"
		then
		if test "$line" = "$pause_at"
			then
			pause="true"
			echo "continue at $line !"
		fi
	else
		index=0
		while (( $index<$thread_num ))
		do
			if test "$downloaded" = "false"
				then
				if [[ ! "${PID[index]}" ]] || ! kill -0 ${PID[index]} 2>/dev/null; then
					wget $line &
					PID[$index]=$!
					downloaded="true"
					echo -e "\n\n##############################\ndownloading...\n$line\nAt thread [$index]\n##############################\n\n"
				elif [[ $index -eq `expr $thread_num - 1` ]]; then
					index=0
				else
					let "index++"
				fi
			else
				let "index++"
			fi
			# set -x
			# echo $index
			# if [[ "${PID[index]}" ]] && kill -0 ${PID[index]} 2>/dev/null; then
			# 	echo "${PID[index]}"
			# fi
			set +x
		done
	fi
}
done < urls_public.txt

