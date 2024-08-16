#!/usr/bin/bash

case $1 in
				100|9[0-9]) echo "优秀" ;;
				[7-8][0-9]) echo "良好";;
				6[0-9]) echo "中等";;
				*) echo "差";;
esac
