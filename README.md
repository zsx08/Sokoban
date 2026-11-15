# 推箱子框架

框架已提供关卡初始化、即时按键输入、地图与图例显示、重开和退出。移动、推箱子和通关判定需要补全，因此首次运行时人物不会移动，步数保持为 0。

## 编译运行

先解压整个压缩包，保留源码与项目文件的相对位置。

### Windows：Visual Studio

1. 安装 Visual Studio，并勾选“使用 C++ 的桌面开发”工作负载。
2. 打开 `Sokoban.sln`，选择 `Debug / x64`。
3. 按 **Ctrl + F5** 编译运行；修改源码后再次按下即可重新编译。

### macOS：

若尚未安装编译工具，先执行 `xcode-select --install`。安装完成后，在终端进入包含 `Makefile` 的目录，例如：

```bash
cd ~/Downloads/大作业/src
make
./sokoban
```

也可以用 `make run` 编译并运行。`make clean` 会删除生成的可执行程序。

### Windows：MinGW-w64（可选）

已配置好 `g++` 时，可在源码目录中执行：

```powershell
g++ -std=c++11 -Wall -Wextra main.cpp game.cpp input.cpp render.cpp console_color.cpp -o sokoban.exe
.\sokoban.exe
```

## 操作与地图

按键不区分大小写，无需回车：**W/A/S/D** 上/左/下/右，**R** 重开，**Q** 退出。

| 字符 | 含义 |
| --- | --- |
| `#` | 墙壁 |
| 空格 | 地板 |
| `.` | 目标点 |
| `@` | 玩家 |
| `$` | 箱子 |
| `*` | 目标点上的箱子 |
| `+` | 目标点上的玩家 |

关卡保存在 `game.cpp` 的 `DEFAULT_LEVEL` 数组中，修改后需要重新编译。

## 开发入口

建议按 `game.h` → `main.cpp` → `game.cpp` 的顺序阅读。

| 文件 | 职责 |
| --- | --- |
| `game.h` | 数据结构和函数声明 |
| `main.cpp` | 主循环、重开、退出与通关状态维护 |
| `game.cpp` | 内置关卡、初始化、位置查询和游戏规则 |
| `input.cpp` | Windows/macOS 单键输入 |
| `render.cpp` | 地图、右侧图例、步数与提示显示 |
| `console_color.h/.cpp` | 颜色名称和着色输出 |
| `Sokoban.sln`、`Sokoban.vcxproj`、`Makefile` | 编译配置 |

基础功能需要在 `game.cpp` 中完成：

- `updateGame()`：处理移动、边界、墙壁与箱子碰撞，更新玩家、箱子和步数。
- `checkWin()`：所有箱子都在目标点上时返回 `1`，否则返回 `0`，不修改游戏状态。

实现时注意：

- 坐标从 0 开始，`row` 向下、`col` 向右增加。访问地形数组前用 `inBounds()` 检查边界。
- `findBox()` 返回箱子下标或 `-1`，应使用 `>= 0` 判断是否找到。
- 成功移动计一步，推箱子也计一步；受阻时位置和步数不变。
- 地形与玩家、箱子分开存储，离开目标点后应保留目标点。`running`、`won` 由主循环维护。

可以按自己的设计调整数据结构、接口和文件划分。新增 `.cpp` 文件后，记得将其加入 Visual Studio 项目，并更新 Makefile 的 `SOURCES` 或手动编译命令。

显示颜色时可调用 `printColoredChar('$', COLOR_YELLOW)` 等函数，ANSI 控制序列已封装在 `console_color.cpp` 中。

补全后，建议检查四向移动与推箱子、撞墙与越界、连续箱子阻挡、目标点保留、步数、通关、重开与退出。

## 提交整理

压缩包已提供 `doc/`、`src/`、`bin/` 三个目录，提交时按以下结构整理：

```text
学号_姓名/
├─ doc/    设计文档等提交材料
├─ src/    源码、头文件、项目配置、Makefile、README
└─ bin/    最终可执行程序和运行所需的资源
```

编译成功后，从以下位置找到最新程序，复制到 `bin/`：

| 编译方式 | 程序位置（相对于源码目录） |
| --- | --- |
| Visual Studio，Release / x64 | 通常为 `x64/Release/Sokoban.exe` |
| Visual Studio，Debug / x64 | 通常为 `x64/Debug/Sokoban.exe` |
| Windows，前文 g++ 命令 | `sokoban.exe` |
| macOS，make | `sokoban` |


更改输入读取时要同时更改input.cpp和main.cpp