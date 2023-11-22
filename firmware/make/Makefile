###############################################################################
# Makefile for building the self-awareness module.
###############################################################################

# Created by: David Arnaiz

# Usage:
#	Build test for the power agent:
#				$ make test TEST=POWER
#	Build and run test:
#				$ make test RUN=true TEST=POWER
#	Clean, build and run test:
#				$ make clean && make test RUN=true TEST=POWER
#   List available tests:
#               $ make list_test

###############################################################################
# OPTIONS
###############################################################################
TEST 		?=
RUN			?= false
###############################################################################
# DEFINITIONS
###############################################################################
PROJECTNAME = self-awareness module

###############################################################################
# FLAGS
###############################################################################
CC			:= gcc
CFLAGS		:= -Wall
OBJ_DIR 	:= build
BIN_DIR		:= build

TEST_FLAGS 	:= -DTEST_$(TEST)

###############################################################################
# SOURCES
###############################################################################
INCLUDE_DIRS := \
-I..

# Platform
H_PLATFORM := \
../platform/sa_types.h \
../platform/sa_utils.h
C_PLATFORM := \
../platform/sa_utils.c
O_PLATFORM := $(basename $(C_PLATFORM))

# Main agent
H_AGENT := \
../include/agents_main.h
C_AGENT := \
../agents/agents_main.c
O_AGENT := $(basename $(C_AGENT))

# Power Agent
H_POWER_AGENT :=  \
../configs/battery_cfg.h
C_POWER_AGENT := \
../agents/power_agent/power_agent.c
O_POWER_AGENT := $(basename $(C_POWER_AGENT))

# Radio Agent
H_RADIO_AGENT :=  \
../include/radio_agent.h \
../configs/radio_cfg.h
C_RADIO_AGENT := \
../configs/radio_cfg.c \
../agents/radio_agent/radio_agent.c
O_RADIO_AGENT := $(basename $(C_RADIO_AGENT))

# Application Agent
H_APP_AGENT :=  \
../include/sensor_agent.h \
../include/trigger_agent.h \
../include/app_agent.h \
../configs/sensor_cfg.h \
../configs/trigger_cfg.h \
../configs/app_cfg.h
C_APP_AGENT := \
../agents/sensor_agent/sensor_agent.c \
../configs/sensor_cfg.c \
../agents/trigger_agent/trigger_agent.c \
../configs/trigger_cfg.c \
../agents/app_agent/app_agent.c
O_APP_AGENT := $(basename $(C_APP_AGENT))

# Decision Engine
H_DECISION_ENG := \
../configs/config.h \
../configs/mote_cfg.h \
../include/decision_engine.h
C_DECISION_ENG := \
../configs/mote_cfg.c \
../decision_engine/decision_engine.c
O_DECISION_ENG := $(basename $(C_DECISION_ENG))

# Test
H_TEST := \
../test/power_agent_test.h \
../test/radio_agent_test.h \
../test/app_agent_test.h \
../test/decision_engine_test.h
C_TEST := \
../test/main.c\
../test/power_agent_test.c \
../test/radio_agent_test.c \
../test/app_agent_test.c \
../test/decision_engine_test.c
O_TEST := $(basename $(C_TEST))

OBJECT_LIST := $(O_POWER_AGENT) $(O_RADIO_AGENT) $(O_APP_AGENT) \
               $(O_TEST) $(O_PLATFORM) $(O_AGENT) $(O_DECISION_ENG)

ifeq ($(RUN), true)
COMMAND := $(BIN_DIR)/test.exe
else
COMMAND := echo "Nothing to run"
endif

dir_guard=@mkdir -p $(OBJ_DIR) $(BIN_DIR)

###############################################################################
# RULES
###############################################################################
test: $(OBJECT_LIST)
	@echo
	@echo "Building all"
	$(CC) $(addprefix $(OBJ_DIR)/,$(addsuffix .o,$(notdir $(OBJECT_LIST)))) -o $(BIN_DIR)/$@
	@echo
	@echo "Test build successfully!"
	@echo
	@echo
	$(COMMAND)


$(O_POWER_AGENT): $(C_POWER_AGENT) $(H_POWER_AGENT) $(O_PLATFORM) $(O_AGENT)
	@echo
	@echo "Building Power agent: $@.c"
	$(dir_guard)
	$(CC) $(INCLUDE_DIRS) $(CFLAGS) -c $@.c -o $(OBJ_DIR)/$(notdir $@).o

$(O_RADIO_AGENT): $(C_RADIO_AGENT) $(H_RADIO_AGENT) $(O_PLATFORM) $(O_AGENT)
	@echo
	@echo "Building Radio agent: $@.c"
	$(dir_guard)
	$(CC) $(INCLUDE_DIRS) $(CFLAGS) -c $@.c -o $(OBJ_DIR)/$(notdir $@).o

$(O_APP_AGENT): $(C_APP_AGENT) $(H_APP_AGENT) $(O_PLATFORM) $(O_AGENT)
	@echo
	@echo "Building application agent: $@.c"
	$(dir_guard)
	$(CC) $(INCLUDE_DIRS) $(CFLAGS) -c $@.c -o $(OBJ_DIR)/$(notdir $@).o

$(O_DECISION_ENG): $(C_DECISION_ENG) $(H_DECISION_ENG) $(O_PLATFORM)
	@echo
	@echo "Building decision engine: $@.c"
	$(dir_guard)
	$(CC) $(INCLUDE_DIRS) $(CFLAGS) -c $@.c -o $(OBJ_DIR)/$(notdir $@).o

$(O_TEST): $(C_TEST) $(H_TEST) $(O_PLATFORM)
	@echo
	@echo "Building test: $@.c"
	$(dir_guard)
	$(CC) $(INCLUDE_DIRS) $(CFLAGS) $(TEST_FLAGS) -c $@.c -o $(OBJ_DIR)/$(notdir $@).o

$(O_AGENT): $(C_AGENT) $(H_AGENT) $(O_PLATFORM)
	@echo
	@echo "Building main agent: $@.c"
	$(CC) $(INCLUDE_DIRS) $(CFLAGS) $(TEST_FLAGS) -c $@.c -o $(OBJ_DIR)/$(notdir $@).o

$(O_PLATFORM): $(C_PLATFORM) $(H_PLATFORM)
	@echo
	@echo "Building platform: $@.c"
	$(dir_guard)
	$(CC) $(INCLUDE_DIRS) $(CFLAGS) $(TEST_FLAGS) -c $@.c -o $(OBJ_DIR)/$(notdir $@).o

clean:
	rm -f $(wildcard $(OBJ_DIR)/*.o) $(wildcard $(BIN_DIR)/*.exe)

###############################################################################
# HELP
###############################################################################
help:
	@echo "Help information"
	@echo "Usage:"
	@echo "    Build test for the power agent:"
	@echo "        make test TEST=POWER"
	@echo ""
	@echo "    Build and run test:"
	@echo "        make test RUN=true TEST=POWER"
	@echo ""
	@echo "    Clean, build and run test:"
	@echo "        make clean && make test RUN=true TEST=POWER"
	@echo ""
	@echo "    List available tests:"
	@echo "         make list_test"
	@echo ""

list_test:
	@echo "listing tests:"
	@echo "  Power Agent: 		TEST=POWER"
	@echo "  Radio Agent: 		TEST=RADIO"
	@echo "  App Agent:   		TEST=APP"
	@echo "  Decision engine: 	TEST=DECISION"
