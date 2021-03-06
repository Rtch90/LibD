CONFIG -= qt
LIBS += -L$$PWD/../Libs
LIBS += -lGL \
    -lSDL \
    -lSDL_image \
    -ltinyxml \
    -lGLU \
    -lz \
    -ltinyxml \
    -lSDL_mixer \
    -lSDL_ttf \
    -lminizip

win32: {
    LIBS -= -lGL \
        -lGLU
    LIBS += -lkernel32 \
        -luser32 \
        -lgdi32 \
        -lopengl32 \
        -lglu32 \
        -lSDL_mixer \
        -lminizip
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
    ../src/Level/AreaMap.h \
    ../src/Main/Game.h \
    ../src/Math/Timer.h \
    ../src/Math/Rect.h \
    ../src/Math/MathBox.h \
    ../src/Math/FPS.h \
    ../src/Math/Vec2.h \
    ../src/Sprite/Sprite.h \
    ../src/System/Debug.h \
    ../src/System/ResourceManager.h \
    ../src/System/Convert/str2int.h \
    ../src/System/Convert/ConvertType.h \
    ../src/Texture/Texture.h \
    ../src/Sound/Music.h \
    ../src/TMXParser/base64.h \
    ../src/Collision/TileCollision.h \
    ../src/Actor/NPC.h \
    ../src/Sound/SoundEffect.h \
    ../src/Actor/Actor.h \
    ../src/Animation/AnimimationSequence.h \
    ../src/Animation/AnimationSequence.h \
    ../src/System/String.h \
    ../src/Font/Font.h \
    ../src/Animation/AnimatingSprite.h \
    ../src/UI/Menu.h \
    ../src/UI/Button.h \
    ../src/Main/TitleScreen.h \
    ../src/Level/Warp.h \
    ../src/Math/Rect.h \
    ../src/BattleSys/Slot.h \
    ../src/TMXParser/TmxTile.h \
    ../src/TMXParser/TmxPropertySet.h \
    ../src/TMXParser/TmxPolyline.h \
    ../src/TMXParser/TmxPolygon.h \
    ../src/TMXParser/TmxPoint.h \
    ../src/TMXParser/TmxObjectGroup.h \
    ../src/TMXParser/TmxObject.h \
    ../src/TMXParser/TmxMapTile.h \
    ../src/TMXParser/TmxMap.h \
    ../src/TMXParser/TmxLayer.h \
    ../src/TMXParser/TmxImage.h \
    ../src/TMXParser/Tmx.h \
    ../src/TMXParser/base64/base64.h \
    ../src/TMXParser/TmxUtil.h \
    ../src/System/Filesystem/SCCopyFile.h

SOURCES += ../src/Actor/Player.cpp \
    ../src/Collision/AABB.cpp \
    ../src/Global/Globals.cpp \
    ../src/Global/Constants.cpp \
    ../src/IO/Input.cpp \
    ../src/Level/Tileset.cpp \
    ../src/Level/Level.cpp \
    ../src/Level/Layer.cpp \
    ../src/Level/AreaMap.cpp \
    ../src/Main/Game.cpp \
    ../src/Main/main.cpp \
    ../src/Math/Vec2.cpp \
    ../src/Math/Timer.cpp \
    ../src/Math/FPS.cpp \
    ../src/Sprite/Sprite.cpp \
    ../src/System/Debug.cpp \
    ../src/System/Convert/str2int.cpp \
    ../src/Texture/Texture.cpp \
    ../src/Sound/Music.cpp \
    ../src/Actor/NPC.cpp \
    ../src/Sound/SoundEffect.cpp \
    ../src/Actor/Actor.cpp \
    ../src/Animation/AnimationSequence.cpp \
    ../src/System/String.cpp \
    ../src/Font/Font.cpp \
    ../src/Animation/AnimatingSprite.cpp \
    ../src/UI/Menu.cpp \
    ../src/UI/Button.cpp \
    ../src/Main/TitleScreen.cpp \
    ../src/Level/Warp.cpp \
    ../src/BattleSys/Slot.cpp \
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
    ../src/TMXParser/base64/base64.cpp \
    ../src/TMXParser/TmxUtil.cpp

QMAKE_CLEAN += LibD Debug.log
