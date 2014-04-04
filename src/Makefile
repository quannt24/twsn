#
# OMNeT++/OMNEST Makefile for TWSN
#
# This file was generated with the command:
#  opp_makemake -f --deep
#

# Name of target to be created (-o option)
TARGET = TWSN$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I. \
    -Ibase \
    -Ibase/channelmgr \
    -Ibase/entities \
    -Ibase/entities/nodes \
    -Ibase/messages \
    -Ibase/messages/control \
    -Ibase/messages/data \
    -Ibase/modules \
    -Ibase/modules/applayer \
    -Ibase/modules/energy \
    -Ibase/modules/linklayer \
    -Ibase/modules/mobility \
    -Ibase/modules/netlayer \
    -Ibase/modules/phylayer \
    -Ibase/modules/queue \
    -Ibase/networks \
    -Ientities \
    -Ientities/nodes \
    -Ientities/other \
    -Ihelpers \
    -Imodules \
    -Imodules/applayer \
    -Imodules/applayer/tracking \
    -Imodules/energy \
    -Imodules/linklayer \
    -Imodules/mobility \
    -Imodules/netlayer \
    -Imodules/other \
    -Inetworks \
    -Iutil

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS =

# Output directory
PROJECT_OUTPUT_DIR = ../out
PROJECTRELATIVE_PATH = src
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = \
    $O/base/channelmgr/phyentry.o \
    $O/base/channelmgr/channelmgr.o \
    $O/base/modules/baselayer.o \
    $O/base/modules/basesimple.o \
    $O/base/modules/applayer/baseapp.o \
    $O/base/modules/energy/baseenergy.o \
    $O/base/modules/linklayer/baselink.o \
    $O/base/modules/mobility/basemobility.o \
    $O/base/modules/netlayer/basenet.o \
    $O/base/modules/phylayer/basephy.o \
    $O/base/modules/phylayer/basewirelessphy.o \
    $O/base/modules/queue/downqueue.o \
    $O/helpers/stathelper.o \
    $O/helpers/decohelper.o \
    $O/helpers/worldhelper.o \
    $O/modules/applayer/tracking/apptrackingsensor.o \
    $O/modules/linklayer/linkunslottedcsmaca.o \
    $O/modules/mobility/mobilitypath.o \
    $O/modules/netlayer/netemrp.o \
    $O/modules/other/acousticgenerator.o \
    $O/modules/other/acousticsensor.o \
    $O/util/matrix.o \
    $O/util/coord.o \
    $O/util/measurement.o \
    $O/util/gaussiannoise.o \
    $O/base/messages/control/command_m.o \
    $O/base/messages/data/airframe_m.o \
    $O/base/messages/data/apppkt_m.o \
    $O/base/messages/data/netpkt_m.o \
    $O/base/messages/data/macpkt_m.o \
    $O/modules/applayer/tracking/apptrackingpkt_m.o \
    $O/modules/netlayer/netemrppkt_m.o \
    $O/modules/other/sensemsg_m.o

# Message files
MSGFILES = \
    base/messages/control/command.msg \
    base/messages/data/airframe.msg \
    base/messages/data/apppkt.msg \
    base/messages/data/netpkt.msg \
    base/messages/data/macpkt.msg \
    modules/applayer/tracking/apptrackingpkt.msg \
    modules/netlayer/netemrppkt.msg \
    modules/other/sensemsg.msg

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" -loppmain$D $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ($(MAKECMDGOALS),depend)
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $O/$(TARGET)
	$(Q)$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	@echo Creating executable: $@
	$(Q)$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY: all clean cleanall depend msgheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE)
	@$(MKPATH) $(dir $@)
	$(qecho) "$<"
	$(Q)$(CXX) -c $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(qecho) MSGC: $<
	$(Q)$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	$(qecho) Cleaning...
	$(Q)-rm -rf $O
	$(Q)-rm -f TWSN TWSN.exe libTWSN.so libTWSN.a libTWSN.dll libTWSN.dylib
	$(Q)-rm -f ./*_m.cc ./*_m.h
	$(Q)-rm -f base/*_m.cc base/*_m.h
	$(Q)-rm -f base/channelmgr/*_m.cc base/channelmgr/*_m.h
	$(Q)-rm -f base/entities/*_m.cc base/entities/*_m.h
	$(Q)-rm -f base/entities/nodes/*_m.cc base/entities/nodes/*_m.h
	$(Q)-rm -f base/messages/*_m.cc base/messages/*_m.h
	$(Q)-rm -f base/messages/control/*_m.cc base/messages/control/*_m.h
	$(Q)-rm -f base/messages/data/*_m.cc base/messages/data/*_m.h
	$(Q)-rm -f base/modules/*_m.cc base/modules/*_m.h
	$(Q)-rm -f base/modules/applayer/*_m.cc base/modules/applayer/*_m.h
	$(Q)-rm -f base/modules/energy/*_m.cc base/modules/energy/*_m.h
	$(Q)-rm -f base/modules/linklayer/*_m.cc base/modules/linklayer/*_m.h
	$(Q)-rm -f base/modules/mobility/*_m.cc base/modules/mobility/*_m.h
	$(Q)-rm -f base/modules/netlayer/*_m.cc base/modules/netlayer/*_m.h
	$(Q)-rm -f base/modules/phylayer/*_m.cc base/modules/phylayer/*_m.h
	$(Q)-rm -f base/modules/queue/*_m.cc base/modules/queue/*_m.h
	$(Q)-rm -f base/networks/*_m.cc base/networks/*_m.h
	$(Q)-rm -f entities/*_m.cc entities/*_m.h
	$(Q)-rm -f entities/nodes/*_m.cc entities/nodes/*_m.h
	$(Q)-rm -f entities/other/*_m.cc entities/other/*_m.h
	$(Q)-rm -f helpers/*_m.cc helpers/*_m.h
	$(Q)-rm -f modules/*_m.cc modules/*_m.h
	$(Q)-rm -f modules/applayer/*_m.cc modules/applayer/*_m.h
	$(Q)-rm -f modules/applayer/tracking/*_m.cc modules/applayer/tracking/*_m.h
	$(Q)-rm -f modules/energy/*_m.cc modules/energy/*_m.h
	$(Q)-rm -f modules/linklayer/*_m.cc modules/linklayer/*_m.h
	$(Q)-rm -f modules/mobility/*_m.cc modules/mobility/*_m.h
	$(Q)-rm -f modules/netlayer/*_m.cc modules/netlayer/*_m.h
	$(Q)-rm -f modules/other/*_m.cc modules/other/*_m.h
	$(Q)-rm -f networks/*_m.cc networks/*_m.h
	$(Q)-rm -f util/*_m.cc util/*_m.h

cleanall: clean
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(qecho) Creating dependencies...
	$(Q)$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc base/*.cc base/channelmgr/*.cc base/entities/*.cc base/entities/nodes/*.cc base/messages/*.cc base/messages/control/*.cc base/messages/data/*.cc base/modules/*.cc base/modules/applayer/*.cc base/modules/energy/*.cc base/modules/linklayer/*.cc base/modules/mobility/*.cc base/modules/netlayer/*.cc base/modules/phylayer/*.cc base/modules/queue/*.cc base/networks/*.cc entities/*.cc entities/nodes/*.cc entities/other/*.cc helpers/*.cc modules/*.cc modules/applayer/*.cc modules/applayer/tracking/*.cc modules/energy/*.cc modules/linklayer/*.cc modules/mobility/*.cc modules/netlayer/*.cc modules/other/*.cc networks/*.cc util/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/base/channelmgr/channelmgr.o: base/channelmgr/channelmgr.cc \
	base/channelmgr/channelmgr.h \
	base/channelmgr/phyentry.h \
	base/messages/data/airframe_m.h \
	base/modules/basesimple.h \
	util/coord.h \
	util/twsndef.h
$O/base/channelmgr/phyentry.o: base/channelmgr/phyentry.cc \
	base/channelmgr/phyentry.h \
	util/coord.h \
	util/twsndef.h
$O/base/messages/control/command_m.o: base/messages/control/command_m.cc \
	base/messages/control/command_m.h
$O/base/messages/data/airframe_m.o: base/messages/data/airframe_m.cc \
	base/messages/data/airframe_m.h \
	util/twsndef.h
$O/base/messages/data/apppkt_m.o: base/messages/data/apppkt_m.cc \
	base/messages/data/apppkt_m.h \
	util/twsndef.h
$O/base/messages/data/macpkt_m.o: base/messages/data/macpkt_m.cc \
	base/messages/data/macpkt_m.h \
	util/twsndef.h
$O/base/messages/data/netpkt_m.o: base/messages/data/netpkt_m.cc \
	base/messages/data/netpkt_m.h \
	util/twsndef.h
$O/base/modules/baselayer.o: base/modules/baselayer.cc \
	base/messages/control/command_m.h \
	base/modules/baselayer.h \
	base/modules/basesimple.h \
	util/twsndef.h
$O/base/modules/basesimple.o: base/modules/basesimple.cc \
	base/modules/basesimple.h \
	util/twsndef.h
$O/base/modules/applayer/baseapp.o: base/modules/applayer/baseapp.cc \
	base/messages/control/command_m.h \
	base/modules/applayer/baseapp.h \
	base/modules/baselayer.h \
	base/modules/basesimple.h \
	util/twsndef.h
$O/base/modules/energy/baseenergy.o: base/modules/energy/baseenergy.cc \
	base/modules/basesimple.h \
	base/modules/energy/baseenergy.h \
	util/twsndef.h
$O/base/modules/linklayer/baselink.o: base/modules/linklayer/baselink.cc \
	base/messages/control/command_m.h \
	base/modules/baselayer.h \
	base/modules/basesimple.h \
	base/modules/linklayer/baselink.h \
	util/twsndef.h
$O/base/modules/mobility/basemobility.o: base/modules/mobility/basemobility.cc \
	base/modules/basesimple.h \
	base/modules/mobility/basemobility.h \
	util/coord.h \
	util/twsndef.h
$O/base/modules/netlayer/basenet.o: base/modules/netlayer/basenet.cc \
	base/messages/control/command_m.h \
	base/modules/baselayer.h \
	base/modules/basesimple.h \
	base/modules/mobility/basemobility.h \
	base/modules/netlayer/basenet.h \
	util/coord.h \
	util/twsndef.h
$O/base/modules/phylayer/basephy.o: base/modules/phylayer/basephy.cc \
	base/messages/data/macpkt_m.h \
	base/modules/basesimple.h \
	base/modules/phylayer/basephy.h \
	util/twsndef.h
$O/base/modules/phylayer/basewirelessphy.o: base/modules/phylayer/basewirelessphy.cc \
	base/channelmgr/channelmgr.h \
	base/channelmgr/phyentry.h \
	base/messages/control/command_m.h \
	base/messages/data/airframe_m.h \
	base/messages/data/macpkt_m.h \
	base/modules/basesimple.h \
	base/modules/energy/baseenergy.h \
	base/modules/mobility/basemobility.h \
	base/modules/phylayer/basephy.h \
	base/modules/phylayer/basewirelessphy.h \
	util/coord.h \
	util/twsndef.h
$O/base/modules/queue/downqueue.o: base/modules/queue/downqueue.cc \
	base/messages/control/command_m.h \
	base/modules/baselayer.h \
	base/modules/basesimple.h \
	base/modules/queue/downqueue.h \
	util/twsndef.h
$O/helpers/decohelper.o: helpers/decohelper.cc \
	base/modules/basesimple.h \
	helpers/decohelper.h \
	util/twsndef.h
$O/helpers/stathelper.o: helpers/stathelper.cc \
	base/modules/basesimple.h \
	helpers/stathelper.h \
	util/twsndef.h
$O/helpers/worldhelper.o: helpers/worldhelper.cc \
	base/messages/control/command_m.h \
	base/modules/baselayer.h \
	base/modules/basesimple.h \
	base/modules/mobility/basemobility.h \
	base/modules/netlayer/basenet.h \
	helpers/worldhelper.h \
	util/coord.h \
	util/twsndef.h
$O/modules/applayer/tracking/apptrackingpkt_m.o: modules/applayer/tracking/apptrackingpkt_m.cc \
	base/messages/data/apppkt_m.h \
	modules/applayer/tracking/apptrackingpkt_m.h \
	util/coord.h \
	util/measurement.h \
	util/targetpos.h \
	util/twsndef.h
$O/modules/applayer/tracking/apptrackingsensor.o: modules/applayer/tracking/apptrackingsensor.cc \
	base/messages/control/command_m.h \
	base/messages/data/apppkt_m.h \
	base/modules/applayer/baseapp.h \
	base/modules/baselayer.h \
	base/modules/basesimple.h \
	base/modules/energy/baseenergy.h \
	base/modules/mobility/basemobility.h \
	helpers/stathelper.h \
	modules/applayer/tracking/apptrackingpkt_m.h \
	modules/applayer/tracking/apptrackingsensor.h \
	modules/other/sensemsg_m.h \
	util/coord.h \
	util/matrix.h \
	util/measurement.h \
	util/targetpos.h \
	util/twsndef.h
$O/modules/linklayer/linkunslottedcsmaca.o: modules/linklayer/linkunslottedcsmaca.cc \
	base/messages/control/command_m.h \
	base/messages/data/macpkt_m.h \
	base/messages/data/netpkt_m.h \
	base/modules/baselayer.h \
	base/modules/basesimple.h \
	base/modules/linklayer/baselink.h \
	modules/linklayer/linkunslottedcsmaca.h \
	util/twsndef.h
$O/modules/mobility/mobilitypath.o: modules/mobility/mobilitypath.cc \
	base/modules/basesimple.h \
	base/modules/mobility/basemobility.h \
	modules/mobility/mobilitypath.h \
	util/coord.h \
	util/twsndef.h
$O/modules/netlayer/netemrp.o: modules/netlayer/netemrp.cc \
	base/messages/control/command_m.h \
	base/messages/data/apppkt_m.h \
	base/messages/data/netpkt_m.h \
	base/modules/baselayer.h \
	base/modules/basesimple.h \
	base/modules/energy/baseenergy.h \
	base/modules/mobility/basemobility.h \
	base/modules/netlayer/basenet.h \
	helpers/decohelper.h \
	modules/netlayer/netemrp.h \
	modules/netlayer/netemrppkt_m.h \
	util/coord.h \
	util/twsndef.h
$O/modules/netlayer/netemrppkt_m.o: modules/netlayer/netemrppkt_m.cc \
	base/messages/data/netpkt_m.h \
	modules/netlayer/netemrppkt_m.h \
	util/twsndef.h
$O/modules/other/acousticgenerator.o: modules/other/acousticgenerator.cc \
	base/modules/basesimple.h \
	base/modules/mobility/basemobility.h \
	modules/other/acousticgenerator.h \
	modules/other/sensemsg_m.h \
	util/coord.h \
	util/measurement.h \
	util/twsndef.h
$O/modules/other/acousticsensor.o: modules/other/acousticsensor.cc \
	base/modules/basesimple.h \
	base/modules/mobility/basemobility.h \
	modules/other/acousticsensor.h \
	modules/other/sensemsg_m.h \
	util/coord.h \
	util/gaussiannoise.h \
	util/measurement.h \
	util/twsndef.h
$O/modules/other/sensemsg_m.o: modules/other/sensemsg_m.cc \
	modules/other/sensemsg_m.h \
	util/coord.h \
	util/measurement.h \
	util/twsndef.h
$O/util/coord.o: util/coord.cc \
	util/coord.h \
	util/twsndef.h
$O/util/gaussiannoise.o: util/gaussiannoise.cc \
	util/gaussiannoise.h
$O/util/matrix.o: util/matrix.cc \
	util/matrix.h
$O/util/measurement.o: util/measurement.cc \
	util/coord.h \
	util/measurement.h \
	util/twsndef.h

