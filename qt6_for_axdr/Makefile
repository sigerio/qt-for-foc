# Qt6 For AXDR 构建脚本

BUILD_DIR = build
TARGET = qt6_for_axdr

.PHONY: all build run clean rebuild

# 默认目标：构建
all: build

# 构建项目
build:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake .. && make -j$$(nproc)

# 编译并运行
run: build
	@./$(BUILD_DIR)/$(TARGET)

# 清理构建文件
clean:
	@rm -rf $(BUILD_DIR)

# 重新构建
rebuild: clean build
