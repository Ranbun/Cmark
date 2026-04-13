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

### 功能特性

- **EXIF 水印叠加**：智能读取图片 EXIF 信息，将常用参数（相机型号、镜头、焦距、光圈、快门、ISO 等）优雅地叠加到图片上。
- **相机品牌 Logo**：依据 EXIF 自动识别并显示对应品牌 Logo，支持 **尼康、佳能、索尼、松下、富士、徕卡、哈苏** 等。
- **批量处理**：支持对整个目录下所有图片进行一键批量处理与水印添加。
- **预览管理**：
  - **实时预览**：点击左侧列表即可实时查看水印叠加后的效果。
  - **预览切换**：支持在“纯净图片”与“带水印预览”模式间快速切换。
  - **保存预览**：支持将预览场景直接导出为图片文件。
- **多线程性能**：采用异步加载与缓存机制，保证大图预览的流畅性。

### 支持的相机品牌

当前已内置以下品牌的 Logo 与识别支持：

- **Nikon** (尼康)
- **Canon** (佳能)
- **Sony** (索尼)
- **Fujifilm** (富士)
- **Panasonic** (松下)
- **Leica** (徕卡)
- **Hasselblad** (哈苏)
- **Apple** (部分支持)

### 技术栈与项目结构

- **UI 框架**：Qt 5 (C++)
- **核心组件**：`QGraphicsScene` 场景排版系统、`QtConcurrent` 异步任务。
- **构建系统**：CMake
- **核心模块**：
  - `src/Cmark/Scene`：核心绘图与布局逻辑。
  - `src/Cmark/File/Resolver`：EXIF 解析引擎。
  - `src/Cmark/File/LogoManager`：Logo 资源管理与匹配。
  - `src/Cmark/Window`：主界面与交互组件。
  - `resources/`：存放所有的图标、底片 Logo 及 Qt 资源文件。

### 构建说明

#### Windows
1. **环境准备**：安装 Qt5 (MSVC/MinGW) 及 CMake 3.10+。
2. **IDE 构建**：推荐使用 Visual Studio 或 CLion 打开项目根目录。
3. **命令行构建**：
   ```bash
   cmake -S . -B build
   cmake --build build --config Release
   ```

#### Linux
1. **依赖安装**：需要 `qt5-default`, `cmake`, `clang`, `ninja-build`。
2. **快速构建**：
   ```bash
   chmod +x build.sh
   ./build.sh
   ```

### 运行与使用

1. **打开文件/文件夹**：通过 `File -> Open Directory` (Ctrl+P) 加载图片库。
2. **查看详情**：选中图片后，右侧面板将显示详细的 EXIF 原始参数。
3. **调整布局**：水印将根据预设模板自动排版。
4. **批量处理**：点击 `File -> Process All` (Ctrl+Shift+A) 自动化处理。
5. **保存结果**：点击工具栏的 `Save preview` 图标即可保存当前效果。

---

### 开发者提示

- 扩展字段：在 `Base/ExifInfo.h` 中定义新键值，并在 `Resolver` 中补充逻辑。
- 自定义样式：详见 `Scene/CScene.cpp` 中的排版比例与字体设置。
- 资源路径：默认从 Qt 资源系统（`resources.qrc`）中加载图标与字体文件。

[preview_demo]: https://raw.githubusercontent.com/Ranbun/images/main/Cmark/previewimage.png
[preview_list]: https://raw.githubusercontent.com/Ranbun/images/main/Cmark/previewFile.png
















