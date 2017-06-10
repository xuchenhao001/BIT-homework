#!/bin/bash
# wget http://data.csail.mit.edu/soundnet/urls_public.txt
# http://www.flickr.com/videos/28042570@N08/4637976820/play/orig/5b222e6773

pause_at="http://www.flickr.com/videos/28042570@N08/4637976820/play/orig/5b222e6773"
thread_num=10

PID=()
pause=false
while read line
do
{
	downloaded=false
	if [[ ! $pause ]]; then
		if [[ $line = $pause_at ]]; then
			pause=true
			echo "continue at $line !"
		else
			echo "pass $line"
		fi
	else
		for ((index=0; index<$thread_num; )); do
		{
			if [[ ! "${PID[index]}" ]] || ! kill -0 ${PID[index]} 2>/dev/null; then
				wget $line &
				PID[index]=$!
				downloaded=true
			fi
			if [[ $downloaded = false && $index = `expr $index - 1` ]]; then
				index=0
			else
				index=`expr $index + 1`
			fi
		}
		done
	fi
}
done < urls_public.txt

