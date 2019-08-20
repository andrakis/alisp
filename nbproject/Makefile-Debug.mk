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
FC=gfortran
AS=as

# Macros
CND_PLATFORM=Cygwin_x86_64-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/ALisp.o \
	${OBJECTDIR}/atoms.o \
	${OBJECTDIR}/cell.o \
	${OBJECTDIR}/core.o \
	${OBJECTDIR}/eval_simple.o \
	${OBJECTDIR}/parser.o \
	${OBJECTDIR}/repl.o \
	${OBJECTDIR}/stdlib.o \
	${OBJECTDIR}/stdlib/conio.o \
	${OBJECTDIR}/stdlib/debug.o \
	${OBJECTDIR}/stdlib/file.o \
	${OBJECTDIR}/stdlib/libcore.o \
	${OBJECTDIR}/stdlib/operators.o \
	${OBJECTDIR}/tests.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-shared-libgcc
CXXFLAGS=-shared-libgcc

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/alisp.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/alisp.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/alisp ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/ALisp.o: ALisp.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ALisp.o ALisp.cpp

${OBJECTDIR}/atoms.o: atoms.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/atoms.o atoms.cpp

${OBJECTDIR}/cell.o: cell.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cell.o cell.cpp

${OBJECTDIR}/core.o: core.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/core.o core.cpp

${OBJECTDIR}/eval_simple.o: eval_simple.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/eval_simple.o eval_simple.cpp

${OBJECTDIR}/parser.o: parser.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/parser.o parser.cpp

${OBJECTDIR}/repl.o: repl.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/repl.o repl.cpp

${OBJECTDIR}/stdlib.o: stdlib.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/stdlib.o stdlib.cpp

${OBJECTDIR}/stdlib/conio.o: stdlib/conio.cpp
	${MKDIR} -p ${OBJECTDIR}/stdlib
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/stdlib/conio.o stdlib/conio.cpp

${OBJECTDIR}/stdlib/debug.o: stdlib/debug.cpp
	${MKDIR} -p ${OBJECTDIR}/stdlib
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/stdlib/debug.o stdlib/debug.cpp

${OBJECTDIR}/stdlib/file.o: stdlib/file.cpp
	${MKDIR} -p ${OBJECTDIR}/stdlib
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/stdlib/file.o stdlib/file.cpp

${OBJECTDIR}/stdlib/libcore.o: stdlib/libcore.cpp
	${MKDIR} -p ${OBJECTDIR}/stdlib
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/stdlib/libcore.o stdlib/libcore.cpp

${OBJECTDIR}/stdlib/operators.o: stdlib/operators.cpp
	${MKDIR} -p ${OBJECTDIR}/stdlib
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/stdlib/operators.o stdlib/operators.cpp

${OBJECTDIR}/tests.o: tests.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/tests.o tests.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
