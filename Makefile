CC						:= gcc
CXX						:= clang++
LD						:= clang++
USB						:= /dev/sdb1

INCLUDES			:= -Isources/include

CC_FLAGS_ALL		:= $(INCLUDES) -Wall -Werror -O2 -g

LD_FLAGS_ALL		:=

define SRC_2_OBJ
  $(foreach src,$(1),$(patsubst sources/%,build/%,$(src)))
endef

define SRC_2_BIN
  $(foreach src,$(1),$(patsubst sources/%,binary/%,$(src)))
endef

all: targets
# make -C test

# Overriden in rules.mk
TARGETS :=
OBJECTS :=

dir	:= sources
include	$(dir)/rules.mk

build/%.o: sources/%.s
	@echo "  [CC]    $< -> $@"
	@mkdir -p $(dir $@)
	@$(CC) $(CC_FLAGS_ALL) $(CC_FLAGS_TARGET) -o $@ -c $<

build/%.o: sources/%.c
	@echo "  [CC]    $< -> $@"
	@mkdir -p $(dir $@)
	@$(CC) $(CC_FLAGS_ALL) $(CC_FLAGS_TARGET) -o $@ -c $<

build/%.o: sources/%.cpp
	@echo "  [CC]    $< -> $@"
	@mkdir -p $(dir $@)
	@$(CXX) $(CC_FLAGS_ALL) $(CC_FLAGS_TARGET) -o $@ -c $<

binary/%:
	@echo "  [LD]    $@"
	@mkdir -p $(dir $@)
	@$(LD) $(LD_FLAGS_ALL) $(LD_FLAGS_TARGET) $(LD_OBJECTS) -o $@

targets: $(patsubst sources/%, binary/%, $(TARGETS))

clean:
	@rm -f $(TARGETS) $(OBJECTS)

run: all
	./binary/l2/l2

info:
	@echo Targets [$(TARGETS)]
	@echo Objects [$(OBJECTS)]
