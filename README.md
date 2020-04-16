# G4Y

## 安装依赖

### Ubuntu 18.04
- sudo apt-get install libassimp-dev
- sudo apt-get install libglew-dev
- sudo apt-get install libglfw3-dev
- sudo apt-get install libglm-dev
- sudo apt-get install libbullet-dev

### Windows
- git clone https://github.com/microsoft/vcpkg.git
- ./bootstrap-vcpkg.bat
- vcpkg install glm:x64-windows
- vcpkg install assimp:x64-windows
- vcpkg install glew:x64-windows
- vcpkg install glfw3:x64-windows
- vcpkg install bullet3:x64-windows
- vcpkg install python3:x64-windows
- vcpkg install protobuf:x64-windows
- vcpkg install boost:x64-windows
- vcpkg install boost-asio:x64-windows
- vcpkg install boost-python:x64-windows

## 构建

```sh
git clone https://github.com/lkpworkspace/g4y.git
git submodule init
git submodule update
mkdir build
cd build
cmake ../
make 
```

## 可执行程序
![g4y-bin](https://gitee.com/lkpworkspace/g4y-bin.git)

## 贪吃蛇示例
![snake](https://github.com/lkpworkspace/demo/blob/master/pics/snake.gif)

## 编辑器示例
![snake](https://github.com/lkpworkspace/demo/blob/master/pics/editor.gif)


