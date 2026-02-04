#!/bin/bash
# =============================================================================
# MPF 开发环境设置脚本 (Linux/macOS)
# =============================================================================
#
# 用法: source scripts/setup-env.sh [选项]
#
# 选项:
#   --qt-dir PATH      Qt 安装路径
#   --sdk-path PATH    MPF SDK 安装路径
#
# 此脚本设置以下环境变量:
#   - QT_DIR: Qt 安装路径
#   - MPF_SDK: MPF SDK 安装路径
#   - CMAKE_PREFIX_PATH: CMake 搜索路径
#   - LD_LIBRARY_PATH (Linux) / DYLD_LIBRARY_PATH (macOS): 动态库搜索路径
#   - PATH: 添加 Qt 工具路径
#
# =============================================================================

# 检测操作系统
if [[ "$OSTYPE" == "darwin"* ]]; then
    OS_TYPE="macOS"
    DEFAULT_QT_DIR="$HOME/Qt/6.8.3/macos"
    LIB_PATH_VAR="DYLD_LIBRARY_PATH"
else
    OS_TYPE="Linux"
    DEFAULT_QT_DIR="$HOME/Qt/6.8.3/gcc_64"
    LIB_PATH_VAR="LD_LIBRARY_PATH"
fi

# 默认值
QT_PATH="${QT_DIR:-$DEFAULT_QT_DIR}"
SDK_PATH="${MPF_SDK:-$HOME/mpf-sdk}"

# 解析参数
while [[ $# -gt 0 ]]; do
    case $1 in
        --qt-dir)
            QT_PATH="$2"
            shift 2
            ;;
        --sdk-path)
            SDK_PATH="$2"
            shift 2
            ;;
        *)
            shift
            ;;
    esac
done

# 验证路径
if [[ ! -d "$QT_PATH" ]]; then
    echo "[WARN] Qt 路径不存在: $QT_PATH"
    echo "       请设置正确的 QT_DIR 或使用 --qt-dir 参数"
fi

# 设置环境变量
export QT_DIR="$QT_PATH"
export MPF_SDK="$SDK_PATH"
export CMAKE_PREFIX_PATH="$QT_PATH;$SDK_PATH"

# 添加库路径
if [[ "$OS_TYPE" == "Linux" ]]; then
    export LD_LIBRARY_PATH="$QT_PATH/lib:$SDK_PATH/lib:${LD_LIBRARY_PATH:-}"
else
    export DYLD_LIBRARY_PATH="$QT_PATH/lib:$SDK_PATH/lib:${DYLD_LIBRARY_PATH:-}"
fi

# 添加 Qt 工具到 PATH
export PATH="$QT_PATH/bin:$PATH"

# 打印配置
echo "=========================================="
echo "MPF 开发环境已配置 ($OS_TYPE)"
echo "=========================================="
echo "QT_DIR:            $QT_DIR"
echo "MPF_SDK:           $MPF_SDK"
echo "CMAKE_PREFIX_PATH: $CMAKE_PREFIX_PATH"
echo "$LIB_PATH_VAR:     ${!LIB_PATH_VAR}"
echo ""
echo "现在可以使用以下命令:"
echo "  cmake --preset linux-debug   # 配置插件 (Linux)"
echo "  cmake --build build          # 构建"
echo ""
