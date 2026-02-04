# Cmark

Camera Log / Camera Mark —— 一款为照片自动叠加拍摄信息（水印）的桌面工具。

### 功能简介

- **EXIF 水印叠加**：读取图片 EXIF 信息，将常用参数（如相机型号、镜头、焦距、光圈、快门、ISO 等）以文字形式叠加到图片上。
- **相机品牌 Logo 显示**：根据图片 EXIF 中的机型信息，在画面中自动显示对应品牌的 Logo（尼康、佳能、索尼、松下等）。
- **多图预览与管理**：
  - 支持选择文件夹，左侧以列表形式展示该目录下所有支持的图片文件。
  - 点击列表中的任意文件，右侧区域会预览显示对应图片。
- **场景布局与排版**：
  - 使用基于 `QGraphicsScene` 的场景系统，将图片、Logo、水印文字等元素组合在同一画面。
  - 支持在不同位置（左上、左下、右上、右下）显示多条 EXIF 文本。
- **多线程图片加载与缓存**：通过 `PictureManager` 等组件，对图片进行异步加载与缓存管理，提升预览性能。

### 支持的相机品牌

当前代码中已内置多种相机品牌 Logo 资源，包括但不限于：

- 尼康（Nikon）
- 佳能（Canon）
- 索尼（Sony）
- 松下（Panasonic）
- 富士（Fujifilm）
- 徕卡（Leica）
- 哈苏（Hasselblad）

具体支持程度受到 EXIF 解析结果与 Logo 资源的影响。

### 技术栈与项目结构概览

- **UI 框架**：Qt（基于 `QApplication`、`QMainWindow`、`QGraphicsScene` 等组件）。
- **构建系统**：CMake，支持多平台构建。
- **核心模块示例**：
  - `src/Cmark/Window/MainWindow.*`：主窗口与菜单栏、工具栏等 UI 入口。
  - `src/Cmark/Scene/CScene.*`：预览场景，负责图片、Logo 以及 EXIF 文字的排版与布局。
  - `src/Cmark/File/PictureManager.*`：图片加载与缓存管理。
  - `src/Cmark/File/Resolver/*`：EXIF 信息解析。
  - `src/Cmark/Log/CLog.*`：日志系统。

### 构建说明

#### Windows

1. **准备环境**
   - 已安装 Qt（建议 Qt5，且包含对应的 MSVC/MinGW 工具链）。
   - 已安装 CMake（建议 3.x 及以上）。
2. **使用 IDE（推荐）**
   - 使用 Visual Studio 或其他支持 CMake 的 IDE 打开项目根目录。
   - 选择对应的 CMake 生成器与 Qt 工具链。
   - 配置并生成后，编译 `Cmark` 目标并运行可执行程序。
3. **命令行构建（示例）**
   ```bash
   cmake -S . -B build
   cmake --build build --config Release
   ```
   > 具体生成器/编译器参数，请根据本机 Qt 和编译器环境调整。

#### Linux

1. **安装依赖**
   - `Qt5`
   - `CMake`
   - `Clang` 或其他支持的编译器
   - `ninja-build`（可选，如使用 Ninja 生成器）
2. **使用脚本构建**
   - 在项目根目录执行：
     ```bash
     chmod +x build.sh
     ./build.sh
     ```
   - 或参考 `build.sh` 中的命令，手动执行 CMake 配置与编译。

### 运行与使用

启动程序后，主窗口标题为 **Camera Mark**：

- **打开图片/文件夹**
  - 通过菜单栏中的“文件 → 打开”，选择单个图片文件或某个文件夹。
  - 若选择文件夹，左侧文件树/列表会显示该目录下所有支持的图片文件。
- **预览与水印查看**
  - 在左侧点击某个图片文件，右侧预览区域会显示该图片。
  - 程序会解析该图片的 EXIF 信息，并在场景中按照预设布局显示文字与相机 Logo。
- **布局与显示**
  - EXIF 信息可在画面四角以多行文本形式展示，Logo 通常位于右侧区域，并与文字之间有分割线。
  - 当切换图片或调整布局时，场景会自动重新排版。

### 开发者提示

- 入口函数位于 `src/Cmark/main.cpp`，其中创建 `QApplication` 和主窗口，并初始化日志与 Logo 资源。
- 若需要扩展支持的 EXIF 字段或自定义布局，可从以下位置入手：
  - EXIF 解析与键名定义：`Base/ExifInfo.h` 及 `File/Resolver` 模块。
  - 布局与场景：`Scene/CScene.*` 及 `SceneLayoutSettings*` 相关文件。
- 图标和 Logo 资源位于 `sources/icons` 与 `sources/logos` 等目录中，可根据需要替换或新增。

# Cmark
Camera Log

- 为照片添加水印(拍摄信息)

- 目前支持 尼康，佳能, 索尼，松下

### Build

#### Build it in Windows
- Open folder in VS or Other IDE(must support CMake project)
- build it & run


#### Build it in Linux
- need `Qt5 , CMake , Clang Complier , ninja-build`(please install them)
- Run build.sh


[nikon_first]: https://raw.githubusercontent.com/Ranbun/images/main/Cmark/previewimage.png "first"

[nikon_secone]: https://raw.githubusercontent.com/Ranbun/images/main/Cmark/previewimage1.png "second"

[previewFile]: https://raw.githubusercontent.com/Ranbun/images/main/Cmark/previewFile.png
















