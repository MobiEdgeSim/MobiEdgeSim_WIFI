#!/bin/bash

source ./config.sh

# 1) 先切换到 simulation_dir
cd "$simulation_dir" || exit 1

# 2) 新建 XML 文件头
echo '<?xml version="1.0" encoding="UTF-8"?>' > "$xml_file"
echo '<Simulations>' >> "$xml_file"

# 3) 新建 outputs 目录
output_root_dir="${simulation_dir}/outputs"
mkdir -p "$output_root_dir"

# 4) 针对 config.sh 里列出的每个配置（ConfigName），循环仿真
for ConfigName in "${ConfigNames[@]}"; do
    echo "Starting simulation with configuration: $ConfigName"
    start_time=$(date +"%Y-%m-%d %H:%M:%S")
    SECONDS=0

    # 打印当前使用的配置名称
    echo "Using configuration: $ConfigName"

    # 5) **核心：执行仿真可执行文件**, 参数与在 IDE 中那条命令对应
    $simulation_executable \
        -r 0 \
        -m \
        -u Cmdenv \
        -c "$ConfigName" \
        -n "$ned_paths" \
        -x "$exclude_modules" \
        --image-path="$image_path" \
        $library_paths \
        "$config_file"

    # 6) 记录时间
    stop_time=$(date +"%Y-%m-%d %H:%M:%S")
    elapsed_time=$SECONDS

    # 7) 把生成的 results 文件夹搬到单独目录
    output_dir="${output_root_dir}/${ConfigName}"
    mkdir -p "$output_dir"
    find "$simulation_dir/results" -type f -exec mv {} "$output_dir/" \;

    # 8) 生成 XML 记录
    echo "  <Simulation>" >> "$xml_file"
    echo "    <ConfigNames>${ConfigName}</ConfigNames>" >> "$xml_file"
    echo "    <StartTime>${start_time}</StartTime>" >> "$xml_file"
    echo "    <StopTime>${stop_time}</StopTime>" >> "$xml_file"
    echo "    <ElapsedTime>${elapsed_time}</ElapsedTime>" >> "$xml_file"
    echo "  </Simulation>" >> "$xml_file"

    echo "Simulation with configuration: $ConfigName completed."
done

# 9) 收尾
echo '</Simulations>' >> "$xml_file"
echo "All simulations completed. Results saved in $xml_file."


