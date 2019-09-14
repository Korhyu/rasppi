#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=
AS=as

# Macros
CND_PLATFORM=GNU_ARM-Linux-x86
CND_CONF=Debug_ARM
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=-fsigned-char

# CC Compiler Flags
CCFLAGS=-fsigned-char
CXXFLAGS=-fsigned-char

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L../../../ForecourtCommonLibrary/dist/Release_ARM/GNU_ARM-Linux-x86 -L../../../CrossPlatformLibrary/dist/Release_ARM/GNU_ARM-Linux-x86 -L../../PumpLibrary/CEMInit/dist/Release_ARM/GNU_ARM-Linux-x86 -L../../../Extra/PDK/PumpService/rootfs/usr/lib -Wl,-rpath /opt/cem44/libraries -lrt -lforecourtcommonlibrary -lcrossplatformlibrary -lceminit

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Debug_ARM.mk dist/Debug_ARM/GNU_ARM-Linux-x86/udp_tester

dist/Debug_ARM/GNU_ARM-Linux-x86/udp_tester: ${OBJECTFILES}
	${MKDIR} -p dist/Debug_ARM/GNU_ARM-Linux-x86
	${LINK.cc} -Wl,-rpath-link=../../../Extra/PDK/PumpService/rootfs/lib -Wl,-rpath-link=../../../Extra/PDK/PumpService/rootfs/usr/lib -Wl,-rpath-link=../../PumpLibrary/CEMInit/dist/Release_ARM/GNU_ARM-Linux-x86 -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/udp_tester ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/main.o: nbproject/Makefile-${CND_CONF}.mk main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -D_DEBUG -I../../../ForecourtCommonLibrary -I../../../CrossPlatformLibrary -I../../PumpLibrary/CEMInit -I../../../Extra/PDK/PumpService/rootfs-devel/usr/include/glib-2.0 -I../../../Extra/PDK/PumpService/rootfs-devel/usr/lib/glib-2.0/include -I../../../Extra/PDK/PumpService/rootfs/usr/lib/glibmm-2.4/include -I../../../Extra/PDK/PumpService/rootfs-devel/usr/include/sigc++-2.0 -I../../../Extra/PDK/PumpService/rootfs-devel/usr/lib/sigc++-2.0/include -I../../../Extra/PDK/PumpService/rootfs-devel/usr/include/glibmm-2.4 -I../../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug_ARM
	${RM} dist/Debug_ARM/GNU_ARM-Linux-x86/udp_tester

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
