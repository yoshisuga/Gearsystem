#-------------------------------------------------
#
# Project created by QtCreator 2016-04-18T18:02:25
#
#-------------------------------------------------

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += opengl

TARGET = Gearsystem
TEMPLATE = app

INCLUDEPATH += /usr/local/include
DEPENDPATH += /usr/local/lib

LIBS += -L/usr/local/lib -lSDL2 \
-lGLEW -lGLU -lGL

SOURCES += \
    ../../../src/KoreanMemoryRule.cpp \
    ../../../src/SG1000MemoryRule.cpp \
    ../../../src/audio/Blip_Buffer.cpp \
    ../../../src/audio/Effects_Buffer.cpp \
    ../../../src/audio/Multi_Buffer.cpp \
    ../../../src/audio/Sms_Apu.cpp \
    ../../audio-shared/Sound_Queue.cpp \
    ../../../src/Audio.cpp \
    ../../../src/Cartridge.cpp \
    ../../../src/CodemastersMemoryRule.cpp \
    ../../../src/GameGearIOPorts.cpp \
    ../../../src/GearsystemCore.cpp \
    ../../../src/Input.cpp \
    ../../../src/Memory.cpp \
    ../../../src/MemoryRule.cpp \
    ../../../src/opcodes.cpp \
    ../../../src/opcodes_cb.cpp \
    ../../../src/opcodes_ed.cpp \
    ../../../src/Processor.cpp \
    ../../../src/RomOnlyMemoryRule.cpp \
    ../../../src/SegaMemoryRule.cpp \
    ../../../src/SmsIOPorts.cpp \
    ../../../src/Video.cpp \
    ../../../src/miniz/miniz.c \
    ../../qt-shared/About.cpp \
    ../../qt-shared/Emulator.cpp \
    ../../qt-shared/GLFrame.cpp \
    ../../qt-shared/InputSettings.cpp \
    ../../qt-shared/main.cpp \
    ../../qt-shared/MainWindow.cpp \
    ../../qt-shared/RenderThread.cpp \
    ../../qt-shared/SoundSettings.cpp \
    ../../qt-shared/VideoSettings.cpp

HEADERS  += \
    ../../../src/KoreanMemoryRule.h \
    ../../../src/SG1000MemoryRule.h \
    ../../../src/audio/blargg_common.h \
    ../../../src/audio/blargg_config.h \
    ../../../src/audio/blargg_source.h \
    ../../../src/audio/Blip_Buffer.h \
    ../../../src/audio/Blip_Synth.h \
    ../../../src/audio/Effects_Buffer.h \
    ../../../src/audio/Multi_Buffer.h \
    ../../../src/audio/Sms_Apu.h \
    ../../../src/audio/Sms_Oscs.h \
    ../../audio-shared/Sound_Queue.h \
    ../../../src/Audio.h \
    ../../../src/Cartridge.h \
    ../../../src/CodemastersMemoryRule.h \
    ../../../src/definitions.h \
    ../../../src/EightBitRegister.h \
    ../../../src/game_db.h \
    ../../../src/GameGearIOPorts.h \
    ../../../src/gearsystem.h \
    ../../../src/GearsystemCore.h \
    ../../../src/Input.h \
    ../../../src/IOPorts.h \
    ../../../src/Memory.h \
    ../../../src/Memory_inline.h \
    ../../../src/MemoryRule.h \
    ../../../src/opcode_daa.h \
    ../../../src/opcode_names.h \
    ../../../src/opcode_timing.h \
    ../../../src/opcodecb_names.h \
    ../../../src/opcodedd_names.h \
    ../../../src/opcodeddcb_names.h \
    ../../../src/opcodeed_names.h \
    ../../../src/opcodefd_names.h \
    ../../../src/opcodefdcb_names.h \
    ../../../src/opcodexx_names.h \
    ../../../src/Processor.h \
    ../../../src/Processor_inline.h \
    ../../../src/RomOnlyMemoryRule.h \
    ../../../src/SegaMemoryRule.h \
    ../../../src/SixteenBitRegister.h \
    ../../../src/SmsIOPorts.h \
    ../../../src/Video.h \
    ../../qt-shared/About.h \
    ../../qt-shared/Emulator.h \
    ../../qt-shared/GLFrame.h \
    ../../qt-shared/InputSettings.h \
    ../../qt-shared/MainWindow.h \
    ../../qt-shared/RenderThread.h \
    ../../qt-shared/SoundSettings.h \
    ../../qt-shared/VideoSettings.h

FORMS += \
    ../../qt-shared/VideoSettings.ui \
    ../../qt-shared/SoundSettings.ui \
    ../../qt-shared/MainWindow.ui \
    ../../qt-shared/InputSettings.ui \
    ../../qt-shared/About.ui
