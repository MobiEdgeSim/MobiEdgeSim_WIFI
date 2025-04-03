# ==============================
# config.sh
# ==============================

# 1) 需要跑哪些配置
ConfigNames=( "combination1_HHO"  "combination2_HHO"  "combination3_HHO"  "combination4_HHO" )

# 2) 仿真所在目录
simulation_dir="/home/tianhao/Documents/workspace/MobiEdgeSim/simulations/DublinNet"

# 3) 可执行文件（相对路径：相对于 simulation_dir 的上一级目录）
simulation_executable="../../src/MobiEdgeSim"

# 4) 输出的 XML 文件（记录仿真运行信息）
xml_file="simulation_run_log.xml"

# 5) NED 路径(-n 参数)，需要把你 IDE 命令里的那串照搬过来
ned_paths="../../src:..:../../../inet/examples:../../../inet/showcases:../../../inet/src:../../../inet/tests/validation:../../../inet/tests/networks:../../../inet/tutorials:../../../Simu5G/emulation:../../../Simu5G/simulations:../../../Simu5G/src:../../../veins_inet/src/veins_inet:../../../veins_inet/examples/veins_inet:../../../veins/examples/veins:../../../veins/src/veins"

# 6) 要排除的模块(-x 参数)，从你 IDE 命令里拷贝过来即可
exclude_modules="inet.common.selfdoc;inet.emulation;inet.showcases.visualizer.osg;inet.examples.emulation;inet.showcases.emulation;inet.applications.voipstream;inet.visualizer.osg;inet.examples.voipstream;simu5g.simulations.LTE.cars;simu5g.simulations.NR.cars;simu5g.nodes.cars"

# 7) 图像路径(--image-path 参数)，同 IDE 命令
image_path="../../../inet/images:../../../Simu5G/images:../../../veins_inet/images:../../../veins/images"

# 8) 需要链接的库(-l 参数)，与 IDE 命令相同顺序即可
library_paths="-l ../../../inet/src/INET -l ../../../Simu5G/src/simu5g -l ../../../veins_inet/src/veins_inet -l ../../../veins/src/veins"

# 9) 你的 omnetpp.ini
config_file="omnetpp.ini"

