CROSS := arm-linux-
CROSS_GNU := arm-linux-gnu-
STRIP := $(CROSS)strip
STRIP := $(shell which "$(STRIP)")
#${warning STRIP=$(STRIP)}
#STRIP :=
ifeq ($(STRIP),)
  STRIP := $(CROSS_GNU)strip
  STRIP := $(shell which "$(STRIP)")
  #${warning STRIP=$(STRIP)}
endif
#${warning STRIP=$(STRIP)}

all:
	@echo "Usage: make -f build.mk <sub_mk>"
	@echo "  mk"
	@echo "  InstallToRootfs"

mk:
	make
	
InstallToRootfs:
	@ROOTFS_DIR="$${NFS_DBG_DIR}";	\
	if [ -z "$${ROOTFS_DIR}" ]; then	\
		echo "The env var NFS_DBG_DIR is empty";	\
		exit 1;	\
	fi;	\
	echo "ROOTFS_DIR=$${ROOTFS_DIR}";	\
	# Out/libLinuxBaseLib.so;	\
	SRC_FILE="Out/libLinuxBaseLib.so";	\
	DST_FILE="$${ROOTFS_DIR}/lib/libLinuxBaseLib.so";	\
	mkdir -p "$${ROOTFS_DIR}/lib"; \
	echo "$${SRC_FILE} => $${DST_FILE}";	\
	cp -fa "$${SRC_FILE}" $${DST_FILE};	\
	ProcessExitCode=$$?;	\
	if [ $${ProcessExitCode} -ne 0 ]; then	\
		exit 1;	\
	fi;	\
	echo "Target strip: $${DST_FILE}";	\
	$(STRIP) --strip-debug $${DST_FILE};	\
	ProcessExitCode=$$?;	\
	if [ $${ProcessExitCode} -ne 0 ]; then	\
		exit 1;	\
	fi;	\
