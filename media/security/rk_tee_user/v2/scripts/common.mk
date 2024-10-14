# Makefile script used in various OP-TEE test components

# Since 'clean' make rules rely on 'rmdir', removed directories shall
# not end with 'current directory' (./ or .) subpath information.
# This macro remove trailing './' info from input path.
define strip-trailing-slashes-and-dots
$(eval _o := $(patsubst %/,%,$(patsubst %/.,%,$(1))))$(if \
	$(filter-out $(1),$(_o)),$(call strip-trailing-slashes-and-dots,$(_o)),$(_o))
endef
