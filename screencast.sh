#!/bin/bash
ffmpeg -f x11grab -video_size 1366x768 -i :0.0 -c:v ffvhuff test.mkv
