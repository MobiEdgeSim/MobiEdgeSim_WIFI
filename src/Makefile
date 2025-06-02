#
# OMNeT++/OMNEST Makefile for MobiEdgeSim_WIFI
#
# This file was generated with the command:
#  opp_makemake -f --deep -KAIRMOBISIM_LIBVEINS_PROJ=../../AirMobiSim_libveins -KVEINS_PROJ=../../veins -KVEINS_INET_PROJ=../../veins/subprojects/veins_inet -KINET_PROJ=../../inet -KSIMU5G_PROJ=../../Simu5G -DVEINS_LIBAIRMOBISIM_IMPORT -DINET_IMPORT -DVEINS_INET_IMPORT -I/usr/lib/jvm/java-11-openjdk-amd64/include -I/usr/lib/jvm/java-11-openjdk-amd64/include/linux -I/usr/lib/jvm/java-11-openjdk-amd64/lib -I/usr/lib/jvm/java-11-openjdk-amd64/lib/server -I$$\(AIRMOBISIM_LIBVEINS_PROJ\)/src -I$$\(VEINS_PROJ\)/src -I. -I$$\(VEINS_PROJ\)/subprojects/veins_inet/src -I$$\(INET_PROJ\)/src -I$$\(SIMU5G_PROJ\)/src -L/usr/lib/jvm/java-11-openjdk-amd64/lib/server -L$$\(AIRMOBISIM_LIBVEINS_PROJ\)/src -L$$\(INET_PROJ\)/src -L$$\(SIMU5G_PROJ\)/src -L$$\(VEINS_PROJ\)/subprojects/veins_inet/src -lveins_libairmobisim$$\(D\) -lINET$$\(D\) -lsimu5g$$\(D\) -lveins_inet$$\(D\)
#

# Name of target to be created (-o option)
TARGET_DIR = .
TARGET_NAME = MobiEdgeSim_WIFI$(D)
TARGET = $(TARGET_NAME)$(EXE_SUFFIX)
TARGET_IMPLIB = $(TARGET_NAME)$(IMPLIB_SUFFIX)
TARGET_IMPDEF = $(TARGET_NAME)$(IMPDEF_SUFFIX)
TARGET_FILES = $(TARGET_DIR)/$(TARGET)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(QTENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(QTENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I/usr/lib/jvm/java-11-openjdk-amd64/include \
    -I/usr/lib/jvm/java-11-openjdk-amd64/include/linux \
    -I/usr/lib/jvm/java-11-openjdk-amd64/lib \
    -I/usr/lib/jvm/java-11-openjdk-amd64/lib/server \
    -I$(AIRMOBISIM_LIBVEINS_PROJ)/src \
    -I$(VEINS_PROJ)/src \
    -I. \
    -I$(VEINS_PROJ)/subprojects/veins_inet/src \
    -I$(INET_PROJ)/src \
    -I$(SIMU5G_PROJ)/src

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = $(LDFLAG_LIBPATH)/usr/lib/jvm/java-11-openjdk-amd64/lib/server $(LDFLAG_LIBPATH)$(AIRMOBISIM_LIBVEINS_PROJ)/src $(LDFLAG_LIBPATH)$(INET_PROJ)/src $(LDFLAG_LIBPATH)$(SIMU5G_PROJ)/src $(LDFLAG_LIBPATH)$(VEINS_PROJ)/subprojects/veins_inet/src  -lveins_libairmobisim$(D) -lINET$(D) -lsimu5g$(D) -lveins_inet$(D)

# Output directory
PROJECT_OUTPUT_DIR = ../out
PROJECTRELATIVE_PATH = src
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc, .msg and .sm files
OBJS = \
    $O/apps/UdpHostSink.o \
    $O/apps/UdpUeApp.o \
    $O/nodes/mecHost/mecHost.o \
    $O/nodes/mobileNodes/ue/ue.o \
    $O/nodes/orchestrator/orchestrator.o \
    $O/scheduler/SchedulerPolicy.o \
    $O/scheduler/JVM/JVMManager.o \
    $O/scheduler/JVM/MetaheuristicScheduler.o \
    $O/scheduler/ResultLogger/ResultLogger.o \
    $O/scheduler/placementPolicy/ABC/ABC_Scheduler.o \
    $O/scheduler/placementPolicy/Heuristics/BestHost_Scheduler.o \
    $O/scheduler/placementPolicy/Heuristics/ClosestFit_Scheduler.o \
    $O/apps/TimestampedPacket_m.o

# Message files
MSGFILES = \
    apps/TimestampedPacket.msg

# SM files
SMFILES =

# Other makefile variables (-K)
AIRMOBISIM_LIBVEINS_PROJ=../../AirMobiSim_libveins
VEINS_PROJ=../../veins
VEINS_INET_PROJ=../../veins/subprojects/veins_inet
INET_PROJ=../../inet
SIMU5G_PROJ=../../Simu5G

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
CONFIGFILE = $(shell opp_configfilepath)
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIBS = $(OPPMAIN_LIB) $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)
ifneq ($(PLATFORM),win32)
LIBS += -Wl,-rpath,$(abspath /usr/lib/jvm/java-11-openjdk-amd64/lib/server) -Wl,-rpath,$(abspath $(AIRMOBISIM_LIBVEINS_PROJ)/src) -Wl,-rpath,$(abspath $(INET_PROJ)/src) -Wl,-rpath,$(abspath $(SIMU5G_PROJ)/src) -Wl,-rpath,$(abspath $(VEINS_PROJ)/subprojects/veins_inet/src)
endif

COPTS = $(CFLAGS) $(IMPORT_DEFINES) -DVEINS_LIBAIRMOBISIM_IMPORT -DINET_IMPORT -DVEINS_INET_IMPORT $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)
SMCOPTS =

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE (if COPTS has changed since last build)
# and make the object files depend on it
COPTS_FILE = $O/.last-copts
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
  $(shell $(MKPATH) "$O")
  $(file >$(COPTS_FILE),$(COPTS))
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
-include makefrag

#------------------------------------------------------------------------------

# Main target
all: $(TARGET_FILES)

$(TARGET_DIR)/% :: $O/%
	@mkdir -p $(TARGET_DIR)
	$(Q)$(LN) $< $@
ifeq ($(TOOLCHAIN_NAME),clang-msabi)
	-$(Q)-$(LN) $(<:%.dll=%.lib) $(@:%.dll=%.lib) 2>/dev/null

$O/$(TARGET_NAME).pdb: $O/$(TARGET)
endif

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile $(CONFIGFILE)
	@$(MKPATH) $O
	@echo Creating executable: $@
	$(Q)$(CXX) $(LDFLAGS) -o $O/$(TARGET) $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY: all clean cleanall depend msgheaders smheaders

# disabling all implicit rules
.SUFFIXES :
.PRECIOUS : %_m.h %_m.cc

$O/%.o: %.cc $(COPTS_FILE) | msgheaders smheaders
	@$(MKPATH) $(dir $@)
	$(qecho) "$<"
	$(Q)$(CXX) -c $(CXXFLAGS) $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(qecho) MSGC: $<
	$(Q)$(MSGC) -s _m.cc -MD -MP -MF $O/$(basename $<)_m.h.d $(MSGCOPTS) $?

%_sm.cc %_sm.h: %.sm
	$(qecho) SMC: $<
	$(Q)$(SMC) -c++ -suffix cc $(SMCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

smheaders: $(SMFILES:.sm=_sm.h)

clean:
	$(qecho) Cleaning $(TARGET)
	$(Q)-rm -rf $O
	$(Q)-rm -f $(TARGET_FILES)
	$(Q)-rm -f $(call opp_rwildcard, . , *_m.cc *_m.h *_sm.cc *_sm.h)

cleanall:
	$(Q)$(CLEANALL_COMMAND)
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

help:
	@echo "$$HELP_SYNOPSYS"
	@echo "$$HELP_TARGETS"
	@echo "$$HELP_VARIABLES"
	@echo "$$HELP_EXAMPLES"

# include all dependencies
-include $(OBJS:%=%.d) $(MSGFILES:%.msg=$O/%_m.h.d)
