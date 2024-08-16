#!/bin/bash

# 获取当前时间，格式为 YYYY-MM-DD_HH-MM-SS
timestamp=$(date +"%H-%M-%S")

# 创建日志文件，文件名为当前时间
logfile="${timestamp}.log"

# 记录事件到日志文件
echo "[$(date +"%Y-%m-%d %H:%M:%S")]：事件" > "$logfile"
