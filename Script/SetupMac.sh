#!/bin/sh
#git submodule update

cd ../Source/External/GitSubmodules


GREEN='\033[0;32m'
NC='\033[0m' # No Color

# test command

echo "${GREEN} $PWD ${NC}"

mkdir -p ../Include/assimp
cp -R "$PWD"/assimp/include/assimp/ ../Include/assimp/

mkdir -p ../Include/vulkan
cp -R "$PWD"/Vulkan-Headers/include/vulkan/ ../Include/vulkan/

mkdir -p ../Include/stb
cp "$PWD"/stb/stb_image.h ../Include/stb/
cp "$PWD"/stb/stb_image_write.h ../Include/stb/

mkdir -p ../../Engine/ThirdParty/ImGui/
cp "$PWD"/imgui/*.h ../../Engine/ThirdParty/ImGui/
cp "$PWD"/imgui/*.cpp ../../Engine/ThirdParty/ImGui/

cp "$PWD"/imgui/examples/imgui_impl_opengl3.h ../../Engine/ThirdParty/ImGui/
cp "$PWD"/imgui/examples/imgui_impl_opengl3.cpp ../../Engine/ThirdParty/ImGui/

echo "#define IMGUI_IMPL_OPENGL_LOADER_GLAD" >temp.h.new
cat ../../Engine/ThirdParty/ImGui/imgui_impl_opengl3.h >>temp.h.new
mv -f temp.h.new ../../Engine/ThirdParty/ImGui/imgui_impl_opengl3.h

mkdir -p ../Include/json/
cp "$PWD"/json/single_include/nlohmann/json.hpp ../Include/json/

mkdir -p ../Include/PhysX/
cp -R "$PWD"/PhysX/physx/include/ ../Include/PhysX/
cp -R "$PWD"/PhysX/pxshared/include/ ../Include/PhysX/

mkdir -p ../../editor/InnoEditor/qdarkstyle/
mkdir -p ../../editor/InnoEditor/qdarkstyle/rc/
cp "$PWD"/QDarkStyleSheet/qdarkstyle/rc/* ../../Editor/InnoEditor/qdarkstyle/rc/
cp "$PWD"/QDarkStyleSheet/qdarkstyle/style.qss ../../Editor/InnoEditor/qdarkstyle/
cp "$PWD"/QDarkStyleSheet/qdarkstyle/style.qrc ../../Editor/InnoEditor/qdarkstyle/

cd ../

mkdir -p DLL/Mac/Debug
mkdir -p Lib/Mac/Debug
mkdir -p DLL/Mac/Release
mkdir -p Lib/Mac/Release

echo "${GREEN} passed mkdir Lib Mac ${NC}"
