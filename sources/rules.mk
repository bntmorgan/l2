sp 		:= $(sp).x
dirstack_$(sp)	:= $(d)
d		:= $(dir)

dir	:= $(d)/l2
include	$(dir)/rules.mk

d		:= $(dirstack_$(sp))
sp		:= $(basename $(sp))
