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
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Release_x86
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
LDLIBSOPTIONS=-L../../../CrossPlatformLibrary/dist/Debug_x86/GNU-Linux-x86 -L../../../ForecourtCommonLibrary/dist/Debug_x86/GNU-Linux-x86 -L../../PumpLibrary/CEMInit/dist/Debug_x86/GNU-Linux-x86 -lrt -lcrossplatformlibrary -lforecourtcommonlibrary -lceminit

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Release_x86.mk dist/Release_x86/GNU-Linux-x86/udp_tester

dist/Release_x86/GNU-Linux-x86/udp_tester: ${OBJECTFILES}
	${MKDIR} -p dist/Release_x86/GNU-Linux-x86
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/udp_tester ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/main.o: nbproject/Makefile-${CND_CONF}.mk main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -DNDEBUG -I../../../ForecourtCommonLibrary -I../../../CrossPlatformLibrary -I../../PumpLibrary/CEMInit -I../../.. -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Release_x86
	${RM} dist/Release_x86/GNU-Linux-x86/udp_tester

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
