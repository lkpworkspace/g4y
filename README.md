# G4Y

## 安装依赖

### Ubuntu 18.04
- sudo apt-get install libassimp-dev
- sudo apt-get install libglew-dev
- sudo apt-get install libglfw3-dev
- sudo apt-get install libglm-dev
- sudo apt-get install libbullet-dev
- git submodule init
- git submodule update

### Windows
- Windows平台下需要自行从github上下载依赖库，编译，并配置CMake。

## 构建

```sh
mkdir build
cd build
cmake ../
make 
```

## 贪吃蛇示例
![snake](https://github.com/lkpworkspace/demo/blob/master/pics/snake.gif)



