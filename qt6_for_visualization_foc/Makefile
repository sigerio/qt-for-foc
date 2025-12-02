# FOC可视化工具构建脚本
BUILD_DIR = build
TARGET = foc_visualization

.PHONY: all clean rebuild run

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(BUILD_DIR)/Makefile
	@echo "编译项目..."
	@cd $(BUILD_DIR) && make -j$$(nproc)

$(BUILD_DIR)/Makefile: CMakeLists.txt
	@echo "配置CMake..."
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake ..

clean:
	@echo "清理构建目录..."
	@rm -rf $(BUILD_DIR)

rebuild: clean all

run: $(BUILD_DIR)/$(TARGET)
	@echo "运行程序..."
	@./$(BUILD_DIR)/$(TARGET)
