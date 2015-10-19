#!/bin/bash
ffmpeg -f x11grab -video_size 1366x768 -i :0.0 -c:v ffvhuff test.mkv
ffmpeg -i test.mkv -c:v libvpx -qmin 0 -qmax 50 -crf 5 -b:v 1M -c:a libvorbis  -cpu-used 4 -threads 8 output.webm
