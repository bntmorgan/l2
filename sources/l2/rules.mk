sp              := $(sp).x
dirstack_$(sp)  := $(d)
d               := $(dir)

TARGET					:= $(call SRC_2_BIN, $(d)/l2)
TARGETS 				+= $(TARGET)
OBJS_$(d)				:= $(call SRC_2_OBJ, $(d)/main.o $(d)/cube.o $(d)/scene.o \
	$(d)/textures.o $(d)/texture.o \
	$(d)/common/math_3d.o	$(d)/common/pipeline.o $(d)/common/camera.o \
	$(d)/gamepad.o $(d)/player.o $(d)/aabbtree.o $(d)/rect.o $(d)/shader.o)

OBJECTS 				+= $(OBJS_$(d))

$(OBJS_$(d))		:  CC_FLAGS_TARGET	:= -Wno-unused-function \
		-Wno-unused-private-field -I$(d) \
		$(shell Magick++-config --cxxflags --cppflags)

$(TARGET)				:  LD_FLAGS_TARGET	:= -lGL -lglut -lGLEW -lm \
	$(shell Magick++-config --ldflags --libs)
$(TARGET)				:  LD_OBJECTS	:= $(OBJS_$(d))
$(TARGET)				:  $(OBJS_$(d))

d               := $(dirstack_$(sp))
sp              := $(basename $(sp))
