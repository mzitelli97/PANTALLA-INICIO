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
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/548587b/BurgleBrosBoard.o \
	${OBJECTDIR}/_ext/548587b/BurgleBrosCard.o \
	${OBJECTDIR}/_ext/548587b/BurgleBrosController.o \
	${OBJECTDIR}/_ext/548587b/BurgleBrosDices.o \
	${OBJECTDIR}/_ext/548587b/BurgleBrosFloor.o \
	${OBJECTDIR}/_ext/548587b/BurgleBrosGuard.o \
	${OBJECTDIR}/_ext/548587b/BurgleBrosLoots.o \
	${OBJECTDIR}/_ext/548587b/BurgleBrosModel.o \
	${OBJECTDIR}/_ext/548587b/BurgleBrosPlayer.o \
	${OBJECTDIR}/_ext/548587b/BurgleBrosSound.o \
	${OBJECTDIR}/_ext/548587b/BurgleBrosTokens.o \
	${OBJECTDIR}/_ext/548587b/BurgleBrosView.o \
	${OBJECTDIR}/_ext/548587b/CView.o \
	${OBJECTDIR}/_ext/548587b/Controller.o \
	${OBJECTDIR}/_ext/548587b/EventData.o \
	${OBJECTDIR}/_ext/548587b/GUI.o \
	${OBJECTDIR}/_ext/548587b/GraphicButton.o \
	${OBJECTDIR}/_ext/548587b/GraphicEDices.o \
	${OBJECTDIR}/_ext/548587b/GraphicGDie.o \
	${OBJECTDIR}/_ext/548587b/GraphicGuard.o \
	${OBJECTDIR}/_ext/548587b/GraphicGuardCards.o \
	${OBJECTDIR}/_ext/548587b/GraphicItem.o \
	${OBJECTDIR}/_ext/548587b/GraphicLoot.o \
	${OBJECTDIR}/_ext/548587b/GraphicMenuItem.o \
	${OBJECTDIR}/_ext/548587b/GraphicPlayer.o \
	${OBJECTDIR}/_ext/548587b/GraphicPlayerCard.o \
	${OBJECTDIR}/_ext/548587b/GraphicTile.o \
	${OBJECTDIR}/_ext/548587b/GraphicToken.o \
	${OBJECTDIR}/_ext/548587b/GraphicWall.o \
	${OBJECTDIR}/_ext/548587b/ImageLoader.o \
	${OBJECTDIR}/_ext/548587b/KeyboardED.o \
	${OBJECTDIR}/_ext/548587b/LibsInit.o \
	${OBJECTDIR}/_ext/548587b/Model.o \
	${OBJECTDIR}/_ext/548587b/MouseED.o \
	${OBJECTDIR}/_ext/548587b/NetworkED.o \
	${OBJECTDIR}/_ext/548587b/NetworkInterface.o \
	${OBJECTDIR}/_ext/548587b/Networking.o \
	${OBJECTDIR}/_ext/548587b/ProtocolFunctions.o \
	${OBJECTDIR}/_ext/548587b/SoundManager.o \
	${OBJECTDIR}/_ext/548587b/TextBox.o \
	${OBJECTDIR}/_ext/548587b/View.o \
	${OBJECTDIR}/_ext/548587b/cController.o \
	${OBJECTDIR}/_ext/548587b/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/pantalla-inicio

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/pantalla-inicio: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/pantalla-inicio ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/548587b/BurgleBrosBoard.o: ../../PANTALLA-INICIO/BurgleBrosBoard.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/BurgleBrosBoard.o ../../PANTALLA-INICIO/BurgleBrosBoard.cpp

${OBJECTDIR}/_ext/548587b/BurgleBrosCard.o: ../../PANTALLA-INICIO/BurgleBrosCard.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/BurgleBrosCard.o ../../PANTALLA-INICIO/BurgleBrosCard.cpp

${OBJECTDIR}/_ext/548587b/BurgleBrosController.o: ../../PANTALLA-INICIO/BurgleBrosController.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/BurgleBrosController.o ../../PANTALLA-INICIO/BurgleBrosController.cpp

${OBJECTDIR}/_ext/548587b/BurgleBrosDices.o: ../../PANTALLA-INICIO/BurgleBrosDices.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/BurgleBrosDices.o ../../PANTALLA-INICIO/BurgleBrosDices.cpp

${OBJECTDIR}/_ext/548587b/BurgleBrosFloor.o: ../../PANTALLA-INICIO/BurgleBrosFloor.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/BurgleBrosFloor.o ../../PANTALLA-INICIO/BurgleBrosFloor.cpp

${OBJECTDIR}/_ext/548587b/BurgleBrosGuard.o: ../../PANTALLA-INICIO/BurgleBrosGuard.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/BurgleBrosGuard.o ../../PANTALLA-INICIO/BurgleBrosGuard.cpp

${OBJECTDIR}/_ext/548587b/BurgleBrosLoots.o: ../../PANTALLA-INICIO/BurgleBrosLoots.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/BurgleBrosLoots.o ../../PANTALLA-INICIO/BurgleBrosLoots.cpp

${OBJECTDIR}/_ext/548587b/BurgleBrosModel.o: ../../PANTALLA-INICIO/BurgleBrosModel.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/BurgleBrosModel.o ../../PANTALLA-INICIO/BurgleBrosModel.cpp

${OBJECTDIR}/_ext/548587b/BurgleBrosPlayer.o: ../../PANTALLA-INICIO/BurgleBrosPlayer.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/BurgleBrosPlayer.o ../../PANTALLA-INICIO/BurgleBrosPlayer.cpp

${OBJECTDIR}/_ext/548587b/BurgleBrosSound.o: ../../PANTALLA-INICIO/BurgleBrosSound.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/BurgleBrosSound.o ../../PANTALLA-INICIO/BurgleBrosSound.cpp

${OBJECTDIR}/_ext/548587b/BurgleBrosTokens.o: ../../PANTALLA-INICIO/BurgleBrosTokens.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/BurgleBrosTokens.o ../../PANTALLA-INICIO/BurgleBrosTokens.cpp

${OBJECTDIR}/_ext/548587b/BurgleBrosView.o: ../../PANTALLA-INICIO/BurgleBrosView.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/BurgleBrosView.o ../../PANTALLA-INICIO/BurgleBrosView.cpp

${OBJECTDIR}/_ext/548587b/CView.o: ../../PANTALLA-INICIO/CView.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/CView.o ../../PANTALLA-INICIO/CView.cpp

${OBJECTDIR}/_ext/548587b/Controller.o: ../../PANTALLA-INICIO/Controller.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/Controller.o ../../PANTALLA-INICIO/Controller.cpp

${OBJECTDIR}/_ext/548587b/EventData.o: ../../PANTALLA-INICIO/EventData.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/EventData.o ../../PANTALLA-INICIO/EventData.cpp

${OBJECTDIR}/_ext/548587b/GUI.o: ../../PANTALLA-INICIO/GUI.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/GUI.o ../../PANTALLA-INICIO/GUI.cpp

${OBJECTDIR}/_ext/548587b/GraphicButton.o: ../../PANTALLA-INICIO/GraphicButton.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/GraphicButton.o ../../PANTALLA-INICIO/GraphicButton.cpp

${OBJECTDIR}/_ext/548587b/GraphicEDices.o: ../../PANTALLA-INICIO/GraphicEDices.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/GraphicEDices.o ../../PANTALLA-INICIO/GraphicEDices.cpp

${OBJECTDIR}/_ext/548587b/GraphicGDie.o: ../../PANTALLA-INICIO/GraphicGDie.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/GraphicGDie.o ../../PANTALLA-INICIO/GraphicGDie.cpp

${OBJECTDIR}/_ext/548587b/GraphicGuard.o: ../../PANTALLA-INICIO/GraphicGuard.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/GraphicGuard.o ../../PANTALLA-INICIO/GraphicGuard.cpp

${OBJECTDIR}/_ext/548587b/GraphicGuardCards.o: ../../PANTALLA-INICIO/GraphicGuardCards.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/GraphicGuardCards.o ../../PANTALLA-INICIO/GraphicGuardCards.cpp

${OBJECTDIR}/_ext/548587b/GraphicItem.o: ../../PANTALLA-INICIO/GraphicItem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/GraphicItem.o ../../PANTALLA-INICIO/GraphicItem.cpp

${OBJECTDIR}/_ext/548587b/GraphicLoot.o: ../../PANTALLA-INICIO/GraphicLoot.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/GraphicLoot.o ../../PANTALLA-INICIO/GraphicLoot.cpp

${OBJECTDIR}/_ext/548587b/GraphicMenuItem.o: ../../PANTALLA-INICIO/GraphicMenuItem.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/GraphicMenuItem.o ../../PANTALLA-INICIO/GraphicMenuItem.cpp

${OBJECTDIR}/_ext/548587b/GraphicPlayer.o: ../../PANTALLA-INICIO/GraphicPlayer.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/GraphicPlayer.o ../../PANTALLA-INICIO/GraphicPlayer.cpp

${OBJECTDIR}/_ext/548587b/GraphicPlayerCard.o: ../../PANTALLA-INICIO/GraphicPlayerCard.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/GraphicPlayerCard.o ../../PANTALLA-INICIO/GraphicPlayerCard.cpp

${OBJECTDIR}/_ext/548587b/GraphicTile.o: ../../PANTALLA-INICIO/GraphicTile.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/GraphicTile.o ../../PANTALLA-INICIO/GraphicTile.cpp

${OBJECTDIR}/_ext/548587b/GraphicToken.o: ../../PANTALLA-INICIO/GraphicToken.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/GraphicToken.o ../../PANTALLA-INICIO/GraphicToken.cpp

${OBJECTDIR}/_ext/548587b/GraphicWall.o: ../../PANTALLA-INICIO/GraphicWall.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/GraphicWall.o ../../PANTALLA-INICIO/GraphicWall.cpp

${OBJECTDIR}/_ext/548587b/ImageLoader.o: ../../PANTALLA-INICIO/ImageLoader.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/ImageLoader.o ../../PANTALLA-INICIO/ImageLoader.cpp

${OBJECTDIR}/_ext/548587b/KeyboardED.o: ../../PANTALLA-INICIO/KeyboardED.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/KeyboardED.o ../../PANTALLA-INICIO/KeyboardED.cpp

${OBJECTDIR}/_ext/548587b/LibsInit.o: ../../PANTALLA-INICIO/LibsInit.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/LibsInit.o ../../PANTALLA-INICIO/LibsInit.cpp

${OBJECTDIR}/_ext/548587b/Model.o: ../../PANTALLA-INICIO/Model.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/Model.o ../../PANTALLA-INICIO/Model.cpp

${OBJECTDIR}/_ext/548587b/MouseED.o: ../../PANTALLA-INICIO/MouseED.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/MouseED.o ../../PANTALLA-INICIO/MouseED.cpp

${OBJECTDIR}/_ext/548587b/NetworkED.o: ../../PANTALLA-INICIO/NetworkED.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/NetworkED.o ../../PANTALLA-INICIO/NetworkED.cpp

${OBJECTDIR}/_ext/548587b/NetworkInterface.o: ../../PANTALLA-INICIO/NetworkInterface.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/NetworkInterface.o ../../PANTALLA-INICIO/NetworkInterface.cpp

${OBJECTDIR}/_ext/548587b/Networking.o: ../../PANTALLA-INICIO/Networking.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/Networking.o ../../PANTALLA-INICIO/Networking.cpp

${OBJECTDIR}/_ext/548587b/ProtocolFunctions.o: ../../PANTALLA-INICIO/ProtocolFunctions.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/ProtocolFunctions.o ../../PANTALLA-INICIO/ProtocolFunctions.cpp

${OBJECTDIR}/_ext/548587b/SoundManager.o: ../../PANTALLA-INICIO/SoundManager.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/SoundManager.o ../../PANTALLA-INICIO/SoundManager.cpp

${OBJECTDIR}/_ext/548587b/TextBox.o: ../../PANTALLA-INICIO/TextBox.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/TextBox.o ../../PANTALLA-INICIO/TextBox.cpp

${OBJECTDIR}/_ext/548587b/View.o: ../../PANTALLA-INICIO/View.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/View.o ../../PANTALLA-INICIO/View.cpp

${OBJECTDIR}/_ext/548587b/cController.o: ../../PANTALLA-INICIO/cController.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/cController.o ../../PANTALLA-INICIO/cController.cpp

${OBJECTDIR}/_ext/548587b/main.o: ../../PANTALLA-INICIO/main.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/548587b
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/548587b/main.o ../../PANTALLA-INICIO/main.cpp

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
