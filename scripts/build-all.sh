#!/bin/bash
# =============================================================================
# MPF 全量构建脚本 (Linux/macOS)
# =============================================================================
#
# 用法:
#   ./build-all.sh [选项]
#
# 选项:
#   --qt-dir PATH      Qt 安装路径 (默认: $QT_DIR 或 ~/Qt/6.8.3/gcc_64)
#   --sdk-path PATH    MPF SDK 安装路径 (默认: $MPF_SDK 或 ~/mpf-sdk)
#   --build-type TYPE  构建类型: Debug|Release (默认: Release)
#   --clean            构建前清理
#   --skip-sdk         跳过 SDK 构建 (已安装时使用)
#   --skip-libs        跳过库构建
#   --skip-host        跳过 Host 构建
#   --skip-plugins     跳过插件构建
#   -j N               并行构建任务数 (默认: $(nproc))
#   -h, --help         显示帮助
#
# 环境变量:
#   QT_DIR             Qt 安装路径
#   MPF_SDK            MPF SDK 安装路径
#
# =============================================================================

set -e  # 出错时退出

# 颜色输出
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 默认值
QT_PATH="${QT_DIR:-$HOME/Qt/6.8.3/gcc_64}"
SDK_PATH="${MPF_SDK:-$HOME/mpf-sdk}"
BUILD_TYPE="Release"
CLEAN=false
SKIP_SDK=false
SKIP_LIBS=false
SKIP_HOST=false
SKIP_PLUGINS=false
JOBS=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

# 项目根目录
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

# 帮助信息
show_help() {
    head -35 "$0" | tail -30
    exit 0
}

# 日志函数
log_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

log_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

log_warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

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
        --build-type)
            BUILD_TYPE="$2"
            shift 2
            ;;
        --clean)
            CLEAN=true
            shift
            ;;
        --skip-sdk)
            SKIP_SDK=true
            shift
            ;;
        --skip-libs)
            SKIP_LIBS=true
            shift
            ;;
        --skip-host)
            SKIP_HOST=true
            shift
            ;;
        --skip-plugins)
            SKIP_PLUGINS=true
            shift
            ;;
        -j)
            JOBS="$2"
            shift 2
            ;;
        -h|--help)
            show_help
            ;;
        *)
            log_error "未知选项: $1"
            exit 1
            ;;
    esac
done

# 验证 Qt 路径
if [[ ! -d "$QT_PATH" ]]; then
    log_error "Qt 路径不存在: $QT_PATH"
    log_info "请设置 QT_DIR 环境变量或使用 --qt-dir 参数"
    exit 1
fi

# 检查 Ninja
if ! command -v ninja &> /dev/null; then
    log_error "未找到 Ninja，请先安装: sudo apt install ninja-build"
    exit 1
fi

# 打印配置
echo ""
log_info "=========================================="
log_info "MPF 构建配置"
log_info "=========================================="
log_info "Qt 路径:     $QT_PATH"
log_info "SDK 路径:    $SDK_PATH"
log_info "构建类型:    $BUILD_TYPE"
log_info "并行任务:    $JOBS"
log_info "项目根目录:  $PROJECT_ROOT"
echo ""

# 导出环境变量 (供后续构建使用)
export QT_DIR="$QT_PATH"
export MPF_SDK="$SDK_PATH"
export CMAKE_PREFIX_PATH="$QT_PATH;$SDK_PATH"

# 构建函数
build_component() {
    local name="$1"
    local dir="$2"
    local install="${3:-false}"
    
    log_info "=========================================="
    log_info "构建 $name"
    log_info "=========================================="
    
    cd "$PROJECT_ROOT/$dir"
    
    if [[ "$CLEAN" == true && -d "build" ]]; then
        log_info "清理 build 目录..."
        rm -rf build
    fi
    
    log_info "配置 CMake..."
    cmake -B build -G Ninja \
        -DCMAKE_PREFIX_PATH="$QT_PATH;$SDK_PATH" \
        -DCMAKE_INSTALL_PREFIX="$SDK_PATH" \
        -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    
    log_info "编译..."
    cmake --build build -j "$JOBS"
    
    if [[ "$install" == true ]]; then
        log_info "安装到 $SDK_PATH..."
        cmake --install build
    fi
    
    log_success "$name 构建完成!"
    echo ""
}

# 构建插件
build_plugin() {
    local name="$1"
    local dir="$2"
    
    log_info "=========================================="
    log_info "构建插件: $name"
    log_info "=========================================="
    
    cd "$PROJECT_ROOT/$dir"
    
    if [[ "$CLEAN" == true && -d "build" ]]; then
        log_info "清理 build 目录..."
        rm -rf build
    fi
    
    log_info "配置 CMake..."
    cmake -B build -G Ninja \
        -DCMAKE_PREFIX_PATH="$QT_PATH;$SDK_PATH" \
        -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    
    log_info "编译..."
    cmake --build build -j "$JOBS"
    
    log_success "插件 $name 构建完成!"
    echo ""
}

# 开始构建
START_TIME=$(date +%s)

# 1. Foundation SDK
if [[ "$SKIP_SDK" == false ]]; then
    build_component "Foundation SDK" "foundation-sdk" true
else
    log_warn "跳过 Foundation SDK"
fi

# 2. HTTP Client 库
if [[ "$SKIP_LIBS" == false ]]; then
    build_component "HTTP Client" "libs/http-client" true
    build_component "UI Components" "libs/ui-components" true
else
    log_warn "跳过库构建"
fi

# 3. Host 应用
if [[ "$SKIP_HOST" == false ]]; then
    build_component "Host Application" "host" false
else
    log_warn "跳过 Host 构建"
fi

# 4. 插件
if [[ "$SKIP_PLUGINS" == false ]]; then
    # 构建所有插件
    for plugin_dir in "$PROJECT_ROOT"/plugins/*/; do
        if [[ -f "${plugin_dir}CMakeLists.txt" ]]; then
            plugin_name=$(basename "$plugin_dir")
            build_plugin "$plugin_name" "plugins/$plugin_name"
        fi
    done
else
    log_warn "跳过插件构建"
fi

# 完成
END_TIME=$(date +%s)
DURATION=$((END_TIME - START_TIME))

echo ""
log_info "=========================================="
log_success "构建完成! 耗时: ${DURATION}s"
log_info "=========================================="
echo ""
log_info "SDK 安装位置: $SDK_PATH"
log_info "Host 可执行文件: $PROJECT_ROOT/host/build/bin/mpf-host"
echo ""
log_info "运行 Host:"
log_info "  export LD_LIBRARY_PATH=\"$QT_PATH/lib:$SDK_PATH/lib:\$LD_LIBRARY_PATH\""
log_info "  export MPF_SDK=\"$SDK_PATH\""
log_info "  $PROJECT_ROOT/host/build/bin/mpf-host"
echo ""
