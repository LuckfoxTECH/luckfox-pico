PLUGIN_UUID = f07bfc66-958c-4a15-99c0-260e4e7375dd

OPTEE_CLIENT_EXPORT ?= ../../client_export/

PLUGIN			= $(PLUGIN_UUID).plugin
PLUGIN_SRS		= $(wildcard ./*.c)
PLUGIN_OBJ		= $(patsubst %.c, $(O)/supp_plugin/%.o, $(PLUGIN_SRS))
PLUGIN_INCLUDES_DIR	= $(CURDIR)/include $(OPTEE_CLIENT_EXPORT)/public

PLUGIN_INCLUDES		= $(addprefix -I, $(PLUGIN_INCLUDES_DIR))
PLUGIN_CCFLAGS		= $(CFLAGS) -Wall -fPIC
PLUGIN_LDFLAGS		= $(LDFLAGS) -shared

CC			?= $(CROSS_COMPILE)gcc

$(O)/supp_plugin/$(PLUGIN): $(PLUGIN_OBJ)
	$(q)$(CC) $(PLUGIN_LDFLAGS) $(PLUGIN_OBJ) -o $@

$(O)/supp_plugin/%.o: $(CURDIR)/%.c
	$(q)mkdir -p $(O)/supp_plugin
	@echo '  CC      $<'
	$(q)$(CC) $(PLUGIN_INCLUDES) $(PLUGIN_CCFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(q)$(RM) -rf $(O)/supp_plugin/
