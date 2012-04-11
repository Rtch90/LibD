CONFIG -= qt
LIBS += -lGL \
    -lSDL \
    -lSDL_ttf \
    -lSDL_image \
    -lSDL_gfx \
    -ltinyxml \
    -lGLU
HEADERS += ../src/Actor/Player.h \
    ../src/Global/Globals.h \
    ../src/IO/Input.h \
    ../src/Main/Game.h \
    ../src/Main/LGLXWindow.h \
    ../src/Math/Timer.h \
    ../src/Math/MathBox.h \
    ../src/Math/FPS.h \
    ../src/Math/Vec2.h \
    ../src/Sprite/Sprite.h \
    ../src/System/Debug.h \
    ../src/Texture/Texture.h \
    ../src/Global/Constants.h \
    ../src/Collision/AABB.h
SOURCES += ../src/Actor/Player.cpp \
    ../src/Global/Globals.cpp \
    ../src/IO/Input.cpp \
    ../src/Main/main.cpp \
    ../src/Main/LGLXWindow.cpp \
    ../src/Main/Game.cpp \
    ../src/Math/Vec2.cpp \
    ../src/Math/Timer.cpp \
    ../src/Math/FPS.cpp \
    ../src/Sprite/Sprite.cpp \
    ../src/System/Debug.cpp \
    ../src/Texture/Texture.cpp \
    ../src/Collision/AABB.cpp
OTHER_FILES += 
