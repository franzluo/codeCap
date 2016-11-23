CROSS := arm-linux-
TARGET_STRIP := $(CROSS)strip

all:
	@echo "Usage: make -f build.mk <sub_mk>"
	@echo "  mk"
	@echo "  InstallToRootfs"

mk:
	@make;	\
	ProcessExitCode=$$?;	\
	if [ $${ProcessExitCode} -ne 0 ]; then	\
		exit 1;	\
	fi;	\
	
InstallToRootfs:
	@ROOTFS_DIR="$${NFS_DBG_DIR}";	\
	if [ -z "$${ROOTFS_DIR}" ]; then	\
		echo "The env var NFS_DBG_DIR is empty";	\
		exit 1;	\
	fi;	\
	echo "ROOTFS_DIR=$${ROOTFS_DIR}";	\
	# Create $(ROOTFS_DIR)/MP7XX if needed;	\
	if [ ! -d "$${ROOTFS_DIR}/MP7XX" ]; then	\
		echo "Create the directory $${ROOTFS_DIR}/MP7XX";	\
		mkdir -p "$${ROOTFS_DIR}/MP7XX";	\
		ProcessExitCode=$$?;	\
		if [ $${ProcessExitCode} -ne 0 ]; then	\
			exit 1;	\
		fi;	\
	fi;	\
	# Out/libMp7xxCommon.so;	\
	SRC_FILE="Out/libMp7xxCommon.so";	\
	DST_FILE="$${ROOTFS_DIR}/lib/libMp7xxCommon.so";	\
	mkdir -p "$${ROOTFS_DIR}/lib";	\
	if [ "$${SRC_FILE}" -nt "$${DST_FILE}" -o "$${SRC_FILE}" -ot "$${DST_FILE}" ]; then	\
		echo "$${SRC_FILE} => $${DST_FILE}";	\
		cp -fa "$${SRC_FILE}" $${DST_FILE};	\
		ProcessExitCode=$$?;	\
		if [ $${ProcessExitCode} -ne 0 ]; then	\
			exit 1;	\
		fi;	\
		echo "Target strip: $${DST_FILE}";	\
		$(TARGET_STRIP) --strip-debug $${DST_FILE};	\
		ProcessExitCode=$$?;	\
		if [ $${ProcessExitCode} -ne 0 ]; then	\
			exit 1;	\
		fi;	\
	fi;	\
