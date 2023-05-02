#!/bin/bash
# 读取用户输入
echo "请输入要移动的文件的拓展名（不含点,例如：webm）："
read ext
#  建立目录
dirname=bak.$RANDOM
mkdir  $dirname
# 使用find命令查找当前目录下所有以指定拓展名结尾的文件，并使用xargs命令将它们移动到kkk目录下
find . -maxdepth 1 -type f -name "*.$ext" | xargs -I {} mv {} $dirname/
# 打印移动完成的信息
echo "已将所有.$ext文件移动到$dirname目录下。"
