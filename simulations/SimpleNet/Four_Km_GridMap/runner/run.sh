#!/bin/bash
#
# Example batch run script for OMNeT++ simulations.
# It reads a list of configurations from an array (Configs)，
# then loops over them, runs each simulation, and moves "results/"
# into a subfolder named after the config.

#############################
# 1. 用户可在此处自定义配置信息
#############################

# 你的 OMNeT++ 可执行文件完整路径
# 假设你在 /home/tianhao/Documents/workspace/MobiEdgeSim/src/MobiEdgeSim 编译生成了 MobiEdgeSim
SIM_EXE="../../../src/MobiEdgeSim"

# 你的 NED 搜索路径（与示例中大体一致，按需调整）
NED_PATHS="../../../src:../..:../../../../inet/examples:../../../../inet/showcases:../../../../inet/src:../../../../inet/tests/validation:../../../../inet/tests/networks:../../../../inet/tutorials:../../../../Simu5G/emulation:../../../../Simu5G/simulations:../../../../Simu5G/src:../../../../veins_inet/src/veins_inet:../../../../veins_inet/examples/veins_inet:../../../../veins/examples/veins:../../../../veins/src/veins"

# 额外要排除或包含的名字空间
X_OPTION="inet.common.selfdoc;inet.emulation;inet.showcases.visualizer.osg;inet.examples.emulation;inet.showcases.emulation;inet.applications.voipstream;inet.visualizer.osg;inet.examples.voipstream;simu5g.simulations.LTE.cars;simu5g.simulations.NR.cars;simu5g.nodes.cars"

# 镜像资源路径（image-path）
IMAGE_PATHS="../../../../inet/images:../../../../Simu5G/images:../../../../veins_inet/images:../../../../veins/images"

# 需要链接的库
LIBRARIES="-l ../../../../inet/src/INET -l ../../../../Simu5G/src/simu5g -l ../../../../veins_inet/src/veins_inet -l ../../../../veins/src/veins"

# INI 文件 (omnetpp.ini) 的相对或绝对路径
INI_FILE="omnetpp.ini"

# 仿真所在目录
SIMULATION_DIR="/home/tianhao/Documents/workspace/MobiEdgeSim/simulations/SimpleNet/Four_Km_GridMap"

# 准备一个要运行的配置列表 (即 [Config <name>] )
Configs=(
"fifty_static_zero_mobile"
    "hundred_static_zero_mobile"
    "zero_static_half_mobile"
    "fifty_static_half_mobile"
    "hundred_static_half_mobile"
    "zero_static_total_mobile"
    "fifty_static_total_mobile"
    "hundred_static_total_mobile"
    
    "fifty_static_zero_mobile_low_resource"
    "hundred_static_zero_mobile_low_resource"
    "zero_static_half_mobile_low_resource"
    "fifty_static_half_mobile_low_resource"
    "hundred_static_half_mobile_low_resource"
    "zero_static_total_mobile_low_resource"
    "fifty_static_total_mobile_low_resource"
    "hundred_static_total_mobile_low_resource"
    
)

# 输出一个简单的 XML 日志文件名称
XML_LOG="simulation_run_log.xml"


#############################
# 2. 脚本逻辑部分
#############################

# 进入仿真所在目录
cd "$SIMULATION_DIR" || { echo "Error: Could not cd to $SIMULATION_DIR"; exit 1; }

# 初始化 XML 日志
echo '<?xml version="1.0" encoding="UTF-8"?>' > "$XML_LOG"
echo '<Simulations>' >> "$XML_LOG"

# 遍历配置数组
for cfg in "${Configs[@]}"; do
    echo "===================================================================="
    echo "Running simulation with configuration: $cfg"

    # 记录开始时间
    start_time=$(date +"%Y-%m-%d %H:%M:%S")
    SECONDS=0  # bash 内建变量，用来记录经过的秒数

    # 运行仿真
    "$SIM_EXE" \
        -r 0 -m -u Cmdenv \
        -c "$cfg" \
        -n "$NED_PATHS" \
        -x "$X_OPTION" \
        --image-path="$IMAGE_PATHS" \
        $LIBRARIES \
        "$INI_FILE"

    # 记录结束时间
    stop_time=$(date +"%Y-%m-%d %H:%M:%S")
    elapsed_time=$SECONDS

    # 创建一个以配置命名的输出目录
    output_dir="$cfg"
    mkdir -p "$output_dir"

    # 将 results/ 下的所有输出文件移动到该目录
    if [ -d "results" ]; then
        # 这里 2>/dev/null 仅仅是为了隐藏没有文件时的错误
        mv -v results/* "$output_dir/" 2>/dev/null
    else
        echo "Warning: results/ directory not found or empty."
    fi

    echo "Results for $cfg have been moved into: $output_dir"

    # 写入 XML 日志
    echo "  <Simulation>" >> "$XML_LOG"
    echo "    <ConfigName>$cfg</ConfigName>" >> "$XML_LOG"
    echo "    <StartTime>$start_time</StartTime>" >> "$XML_LOG"
    echo "    <StopTime>$stop_time</StopTime>" >> "$XML_LOG"
    echo "    <ElapsedTime>$elapsed_time</ElapsedTime>" >> "$XML_LOG"
    echo "  </Simulation>" >> "$XML_LOG"

    echo "Simulation with configuration '$cfg' completed."
    echo
done

# 结束 XML
echo '</Simulations>' >> "$XML_LOG"

echo "All simulations finished. Summary written to $XML_LOG."

