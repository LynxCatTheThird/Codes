#!/bin/bash
# 读取用户输入的传入格式和传出格式
echo "请输入传入格式（例如：webm）："
read input_format
echo "请输入传出格式（例如：mp4）："
read output_format
# 遍历当前目录下的所有符合传入格式的文件
for file in *.$input_format; do
  # 获取文件名（不含扩展名）
  filename=${file%.*}
  # 调用ffmpeg将文件转换为传出格式，保持视频和音频编码不变
  ffmpeg -i "$file" "$filename.$output_format"
done
