CONFIG -= qt
LIBS += -lGL \
    -lSDL \
    -lSDL_image \
    -ltinyxml \
    -lGLU \
    -lz \
    -ltinyxml \
    -lSDL_mixer
win32: { 
    LIBS -= -lGL \
        -lGLU
    LIBS += -lkernel32 \
        -luser32 \
        -lgdi32 \
        -lopengl32 \
        -lglu32 \
        -lSDL_mixer
}
HEADERS += ../src/Actor/Player.h \
    ../src/Collision/AABB.h \
    ../src/Global/Constants.h \
    ../src/Global/Globals.h \
    ../src/IO/Input.h \
    ../src/Level/MapTile.h \
    ../src/Level/Level.h \
    ../src/Level/Layer.h \
    ../src/Level/Tileset.h \
    ../src/Main/Game.h \
    ../src/Math/Timer.h \
    ../src/Math/Rect.h \
    ../src/Math/MathBox.h \
    ../src/Math/FPS.h \
    ../src/Math/Vec2.h \
    ../src/Sprite/Sprite.h \
    ../src/System/Debug.h \
    ../src/System/ResourceManager.h \
    ../src/Texture/Texture.h \
    ../src/Sound/Music.h \
    ../src/TMXParser/TmxUtil.h \
    ../src/TMXParser/TmxTileset.h \
    ../src/TMXParser/TmxTile.h \
    ../src/TMXParser/TmxPropertySet.h \
    ../src/TMXParser/TmxPolyline.h \
    ../src/TMXParser/TmxPolygon.h \
    ../src/TMXParser/TmxObjectGroup.h \
    ../src/TMXParser/TmxObject.h \
    ../src/TMXParser/TmxMapTile.h \
    ../src/TMXParser/TmxImage.h \
    ../src/TMXParser/TmxPoint.h \
    ../src/TMXParser/TmxMap.h \
    ../src/TMXParser/TmxLayer.h \
    ../src/TMXParser/Tmx.h \
    ../src/TMXParser/base64.h \
    ../src/Collision/TileCollision.h \
    ../src/Actor/NPC.h
SOURCES += ../src/Actor/Player.cpp \
    ../src/Collision/AABB.cpp \
    ../src/Global/Globals.cpp \
    ../src/Global/Constants.cpp \
    ../src/IO/Input.cpp \
    ../src/Level/Tileset.cpp \
    ../src/Level/Level.cpp \
    ../src/Level/Layer.cpp \
    ../src/Main/Game.cpp \
    ../src/Main/main.cpp \
    ../src/Math/Vec2.cpp \
    ../src/Math/Timer.cpp \
    ../src/Math/FPS.cpp \
    ../src/Sprite/Sprite.cpp \
    ../src/System/Debug.cpp \
    ../src/Texture/Texture.cpp \
    ../src/Sound/Music.cpp \
    ../src/TMXParser/TmxUtil.cpp \
    ../src/TMXParser/TmxTileset.cpp \
    ../src/TMXParser/TmxTile.cpp \
    ../src/TMXParser/TmxPropertySet.cpp \
    ../src/TMXParser/TmxPolyline.cpp \
    ../src/TMXParser/TmxPolygon.cpp \
    ../src/TMXParser/TmxObjectGroup.cpp \
    ../src/TMXParser/TmxObject.cpp \
    ../src/TMXParser/TmxMap.cpp \
    ../src/TMXParser/TmxLayer.cpp \
    ../src/TMXParser/TmxImage.cpp \
    ../src/TMXParser/base64.cpp \
    ../src/Actor/NPC.cpp
OTHER_FILES += 
