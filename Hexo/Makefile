# 编译器与参数
CXX := g++
CXXFLAGS := -march=native -O2 -Wall -Wextra -Wpedantic -I_includes
MAKEFLAGS="-j $(nproc --all)"

# 源文件与目标文件
SRC := main.cpp
OUT := t

# 默认目标
all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

# 安装目标
install: $(OUT)
	install -Dm755 $(OUT) ~/.local/bin/$(OUT)

# 卸载目标
uninstall:
	rm -f ~/.local/bin/$(OUT)

# 清理构建产物
clean:
	rm -f $(OUT)
