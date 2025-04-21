#!/bin/bash
#
# Example batch run script for OMNeT++ simulations.
# It reads a list of configurations from an array (Configs)，
# then loops over them, runs each simulation, and moves "results/"
# into a subfolder named after the config.

#############################
# 1. user configurators
#############################
SIM_EXE="../../../src/MobiEdgeSim"

NED_PATHS="../../../src:../..:../../../../inet/examples:../../../../inet/showcases:../../../../inet/src:../../../../inet/tests/validation:../../../../inet/tests/networks:../../../../inet/tutorials:../../../../Simu5G/emulation:../../../../Simu5G/simulations:../../../../Simu5G/src:../../../../veins_inet/src/veins_inet:../../../../veins_inet/examples/veins_inet:../../../../veins/examples/veins:../../../../veins/src/veins"

X_OPTION="inet.common.selfdoc;inet.emulation;inet.showcases.visualizer.osg;inet.examples.emulation;inet.showcases.emulation;inet.applications.voipstream;inet.visualizer.osg;inet.examples.voipstream;simu5g.simulations.LTE.cars;simu5g.simulations.NR.cars;simu5g.nodes.cars"

IMAGE_PATHS="../../../../inet/images:../../../../Simu5G/images:../../../../veins_inet/images:../../../../veins/images"

LIBRARIES="-l ../../../../inet/src/INET -l ../../../../Simu5G/src/simu5g -l ../../../../veins_inet/src/veins_inet -l ../../../../veins/src/veins"

INI_FILE="omnetpp.ini"

SIMULATION_DIR="/home/tianhao/Documents/workspace/MobiEdgeSim/MobiEdgeSim/simulations/SimpleNet/Four_Km_GridMap"

Configs=(
"fifty_static_zero_mobile"
    #"hundred_static_zero_mobile"
    #"zero_static_half_mobile"
    "fifty_static_half_mobile"
    #"hundred_static_half_mobile"
    #"zero_static_total_mobile"
    "fifty_static_total_mobile"
    #"hundred_static_total_mobile"
    
    "fifty_static_zero_mobile_low_resource"
    #"hundred_static_zero_mobile_low_resource"
    #"zero_static_half_mobile_low_resource"
    "fifty_static_half_mobile_low_resource"
    #"hundred_static_half_mobile_low_resource"
    #"zero_static_total_mobile_low_resource"
    "fifty_static_total_mobile_low_resource"
    #"hundred_static_total_mobile_low_resource"
    
)

XML_LOG="simulation_run_log.xml"


#############################
# 2. logic
#############################

cd "$SIMULATION_DIR" || { echo "Error: Could not cd to $SIMULATION_DIR"; exit 1; }

echo '<?xml version="1.0" encoding="UTF-8"?>' > "$XML_LOG"
echo '<Simulations>' >> "$XML_LOG"

for cfg in "${Configs[@]}"; do
    echo "===================================================================="
    echo "Running simulation with configuration: $cfg"

    start_time=$(date +"%Y-%m-%d %H:%M:%S")
    SECONDS=0 

    "$SIM_EXE" \
        -r 0 -m -u Cmdenv \
        -c "$cfg" \
        -n "$NED_PATHS" \
        -x "$X_OPTION" \
        --image-path="$IMAGE_PATHS" \
        $LIBRARIES \
        "$INI_FILE"

    stop_time=$(date +"%Y-%m-%d %H:%M:%S")
    elapsed_time=$SECONDS

    output_dir="$cfg"
    mkdir -p "$output_dir"

    if [ -d "results" ]; then
        mv -v results/* "$output_dir/" 2>/dev/null
    else
        echo "Warning: results/ directory not found or empty."
    fi

    echo "Results for $cfg have been moved into: $output_dir"
    
    echo "  <Simulation>" >> "$XML_LOG"
    echo "    <ConfigName>$cfg</ConfigName>" >> "$XML_LOG"
    echo "    <StartTime>$start_time</StartTime>" >> "$XML_LOG"
    echo "    <StopTime>$stop_time</StopTime>" >> "$XML_LOG"
    echo "    <ElapsedTime>$elapsed_time</ElapsedTime>" >> "$XML_LOG"
    echo "  </Simulation>" >> "$XML_LOG"

    echo "Simulation with configuration '$cfg' completed."
    echo
done

echo '</Simulations>' >> "$XML_LOG"

echo "All simulations finished. Summary written to $XML_LOG."

