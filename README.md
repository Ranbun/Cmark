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


### Run 

#### 操作介绍

- 选择文件菜单中的打开选项
    - 选择文件夹打开，左侧视图将会预览当前文件夹下所有支持的图片文件
    - ![][previewFile]

- 点相应的文件 右侧窗口将会预览显示选择的文件
    - ![][nikon_secone]

- 存储预览结果 - check it
    ![][savePreviewResult]

- 按原始尺寸存储
    ![][saveOriginResult]


#### 软件预览显示效果：

##### Nikon
![][nikon_first]

![][nikon_secone]

##### Sony
![][sony_first]

#### output

##### 保持原尺寸的输出

![][output_O_1]


![][output_O_2]

##### 预览输出

![][output_P_1]


![][output_P_2]


[nikon_first]: https://raw.githubusercontent.com/Ranbun/images/main/Cmark/previewimage.png "first"

[nikon_secone]: https://raw.githubusercontent.com/Ranbun/images/main/Cmark/previewimage1.png "second"

[sony_first]: https://raw.githubusercontent.com/Ranbun/images/main/Cmark/sony_previewimage.png "first"

[output_O_1]: https://raw.githubusercontent.com/Ranbun/images/main/Cmark/nikon_output_o_1.png
[output_O_2]: https://raw.githubusercontent.com/Ranbun/images/main/Cmark/nikon_output_o_2.png

[output_P_1]: https://raw.githubusercontent.com/Ranbun/images/main/Cmark/nikon_output_p_1.png
[output_P_2]: https://raw.githubusercontent.com/Ranbun/images/main/Cmark/nikon_output_p_2.png


[savePreviewResult]: https://raw.githubusercontent.com/Ranbun/images/main/Cmark/previewSceneSave.png

[saveOriginResult]: https://raw.githubusercontent.com/Ranbun/images/main/Cmark/save.png

[previewFile]: https://raw.githubusercontent.com/Ranbun/images/main/Cmark/previewFile.png
















