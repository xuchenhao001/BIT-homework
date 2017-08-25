#!/bin/bash

# wget http://data.csail.mit.edu/soundnet/urls_public.txt

pause="false"
while read line
do
	if test "$pause" = "false" 
		then
		if test "$line" = "http://www.flickr.com/videos/27606531@N02/2898394758/play/orig/241c1c0d25" 
			then
			pause="true"
			echo "continue at $line !"
		else
			echo "pass $line"
		fi
	else
		wget $line
		# echo "download $line"
	fi
done < urls_public.txt

# http://www.flickr.com/videos/27606531@N02/2898394758/play/orig/241c1c0d25