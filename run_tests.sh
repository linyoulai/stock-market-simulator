#!/bin/bash

# --- 配置 ---
# 可执行文件名
EXECUTABLE="./market"
# 测试文件所在目录
TEST_DIR="./"
# 编译命令
MAKE_COMMAND="make -R -r" # 这是什么意思

# --- 颜色定义 (可选，为了好看) ---
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# --- 脚本开始 ---
echo "--- 1. Building Project ---"
$MAKE_COMMAND
if [ $? -ne 0 ]; then
    echo -e "${RED}Build failed. Aborting tests.${NC}"
    exit 1
fi

if [ ! -f "$EXECUTABLE" ]; then
    echo -e "${RED}Executable '$EXECUTABLE' not found after build. Aborting tests.${NC}"
    exit 1
fi
echo "Build successful."
echo

echo "--- 2. Running Tests ---"
PASS_COUNT=0
FAIL_COUNT=0

# 寻找所有 test-*.txt 文件
for test_input in ${TEST_DIR}/test-*.txt; do
    # 从输入文件名推导出其他相关文件名
    test_name=$(basename "$test_input" .txt)
    
    expected_stdout="${TEST_DIR}/expected-${test_name}.txt"
    expected_stderr="${TEST_DIR}/expected-${test_name}.stderr"
    
    actual_stdout="${TEST_DIR}/actual-${test_name}.txt" # 这给我输出到哪了?
    actual_stderr="${TEST_DIR}/actual-${test_name}.stderr" # 这给我输出到哪了?

    # 检查预期文件是否存在，如果不存在则跳过此测试
    if [ ! -f "$expected_stdout" ] || [ ! -f "$expected_stderr" ]; then
        echo "WARNING: Skipping test '$test_name' because expected output files are missing."
        continue
    fi

    # 从文件名中提取命令行选项 (例如 test-1-v.txt -> v)
    option_char=$(echo "$test_name" | cut -d'-' -f3)
    cli_option=""
    case "$option_char" in
        v) cli_option="--verbose" ;;
        m) cli_option="--median" ;;
        i) cli_option="--trader_info" ;;
        t) cli_option="--time_travelers" ;;
        # 如果有组合选项，可以继续添加
        vmit) cli_option="-vmit" ;;
    esac

    # 执行程序，并将 stdout 和 stderr 分别重定向到临时文件
    $EXECUTABLE $cli_option < "$test_input" > "$actual_stdout" 2> "$actual_stderr"

    # 使用 diff 命令比较实际输出和预期输出
    # -q 选项让 diff 在有差异时不输出具体内容，只通过退出码告诉我们结果
    diff -q "$expected_stdout" "$actual_stdout" > /dev/null 2>&1
    stdout_match=$?

    diff -q "$expected_stderr" "$actual_stderr" > /dev/null 2>&1
    stderr_match=$?

    # 判断测试是否通过
    if [ $stdout_match -eq 0 ] && [ $stderr_match -eq 0 ]; then
        echo -e "Test: ${test_name} ... ${GREEN}PASS${NC}"
        ((PASS_COUNT++))
    else
        echo -e "Test: ${test_name} ... ${RED}FAIL${NC}"
        ((FAIL_COUNT++))
        # 如果失败，显示详细的差异
        echo "--- STDOUT Diff ---"
        diff -u "$expected_stdout" "$actual_stdout"
        echo "--- STDERR Diff ---"
        diff -u "$expected_stderr" "$actual_stderr"
        echo "-------------------"
    fi

    # 清理临时文件
    rm "$actual_stdout" "$actual_stderr"
done

echo
echo "--- 3. Test Summary ---"
echo -e "${GREEN}Passed: ${PASS_COUNT}${NC}"
echo -e "${RED}Failed: ${FAIL_COUNT}${NC}"
echo

# 如果有失败的测试，脚本以非零状态退出（用于CI/CD系统）
if [ $FAIL_COUNT -ne 0 ]; then
    exit 1
fi