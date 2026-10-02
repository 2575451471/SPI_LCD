# CH32V103C 墨水屏驱动项目

基于 **CH32V103C8T6** 的 1.54 英寸黑白墨水屏驱动项目。

项目由原 SPI LCD Demo 修改而来，目前已经移除原 LCD、SPI Flash 等无关驱动，并重新实现了适用于墨水屏的 SPI 底层驱动、SSD1608 控制、Framebuffer 绘图以及中英文字符显示功能。

当前使用屏幕分辨率为 **200 × 200**，控制器为 **SSD1608**。

---

## 一、硬件信息

### MCU

- MCU：CH32V103C8T6
- 核心板：艾尔赛 CH32V103C-MINI
- 架构：RISC-V
- 开发环境：MounRiver Studio

### 墨水屏

- 型号：HINK-E0154A05
- 尺寸：1.54 英寸
- 分辨率：200 × 200
- 颜色：黑白
- 控制器：SSD1608
- 通信方式：SPI

---

## 二、硬件接线

当前硬件已经按照以下方式焊接：

| 墨水屏引脚 | CH32V103 | 功能 |
|---|---|---|
| RST | PA1 | 墨水屏复位 |
| DC | PA2 | 数据 / 命令选择 |
| BUSY | PA3 | 忙状态输入 |
| CS1 | PA4 | SPI 片选 |
| SCK | PA5 | SPI1 时钟 |
| SDA / DIN | PA7 | SPI1 MOSI |

PA6 / SPI1 MISO 未使用。

墨水屏使用软件控制 PA4 作为 CS。

---

## 三、屏幕坐标定义

当前程序使用的屏幕方向：

```text
屏幕排线 / 接口位于下方

(0,0) ─────────────→ X
  │
  │
  │
  │
  ↓
  Y

右下角：(199,199)
```

Framebuffer 像素定义：

```text
1 = 白色
0 = 黑色
```

每个字节的最高位对应左侧像素：

```c
mask = 0x80 >> (x % 8);
```

---

## 四、当前已实现功能

目前已经实现：

- SPI1 墨水屏通信
- SSD1608 初始化
- SSD1608 BUSY 状态检测
- SSD1608 全屏刷新
- 200 × 200 Framebuffer
- 全屏清白 / 清黑
- 单像素绘制
- 水平线
- 垂直线
- 实心矩形
- 8 × 16 ASCII 字符显示
- 16 × 16 中文字符显示
- Unicode 中文字模查询
- UTF-8 字符解析
- 中英文混合显示

当前 Framebuffer 大小：

```text
200 × 200 / 8
= 5000 Byte
```

---

## 五、项目结构

当前主要代码位于 `LCD` 目录。

```text
LCD/
├── epd_port.c
├── epd_port.h
├── epd.c
├── epd.h
├── epd_paint.c
├── epd_paint.h
├── font.c
├── font.h
├── font_cn.c
├── font_cn.h
├── utf8.c
└── utf8.h
```

### epd_port

```text
epd_port.c
epd_port.h
```

负责 MCU 与墨水屏之间的底层硬件接口：

- GPIO 初始化
- SPI1 初始化
- SPI 字节发送
- RST
- DC
- CS
- BUSY

### epd

```text
epd.c
epd.h
```

负责 SSD1608 控制器：

- 硬件复位
- 命令发送
- 数据发送
- BUSY 等待
- LUT 配置
- 显存窗口设置
- 显存地址设置
- 全屏刷新
- Framebuffer 显示
- 墨水屏休眠

### epd_paint

```text
epd_paint.c
epd_paint.h
```

负责 Framebuffer 绘图：

- 清屏
- 像素
- 直线
- 矩形
- ASCII 字符
- 中文字符
- UTF-8 字符串显示

### font

```text
font.c
font.h
```

ASCII 字体模块。

当前主要用于 8 × 16 英文、数字和符号显示。

### font_cn

```text
font_cn.c
font_cn.h
```

16 × 16 中文点阵字模模块。

中文字模使用 Unicode 编码进行索引。

例如：

```c
0x58A8    /* 墨 */
0x6C34    /* 水 */
0x5C4F    /* 屏 */
```

### utf8

```text
utf8.c
utf8.h
```

负责将 UTF-8 字节序列解析为 Unicode 编码。

数据流程：

```text
UTF-8
  ↓
UTF8_Decode()
  ↓
Unicode
  ↓
FontCN16_Get()
  ↓
16 × 16 字模
  ↓
Framebuffer
```

---

## 六、字符显示

### ASCII

例如：

```c
Paint_DrawString8x16(
    12,
    15,
    "CH32V103",
    EPD_BLACK
);
```

也可以通过 UTF-8 混合显示接口：

```c
Paint_DrawUTF8String16x16(
    12,
    15,
    "SSD1608 200X200",
    EPD_BLACK
);
```

---

## 七、中文显示

底层中文接口支持 Unicode：

```c
Paint_DrawChinese16x16(
    x,
    y,
    0x58A8,
    EPD_BLACK
);
```

也支持 Unicode 数组：

```c
static const uint16_t text[] =
{
    0x58A8,    /* 墨 */
    0x6C34,    /* 水 */
    0x5C4F,    /* 屏 */
    0x0000
};
```

然后：

```c
Paint_DrawChineseString16x16(
    10,
    10,
    text,
    EPD_BLACK
);
```

---

## 八、源码编码说明

当前原始 MounRiver 工程使用的源码编码为 **GBK / GB18030 系列编码**。

因此目前不直接在 C 字符串中使用 UTF-8 中文：

```c
"墨水屏"
```

否则字符串实际生成的字节可能是 GBK，而不是 UTF-8。

目前采用显式 UTF-8 字节方式：

```c
static const char Text_EPD_UTF8[] =
    "\xE5\xA2\xA8"   /* 墨 */
    "\xE6\xB0\xB4"   /* 水 */
    "\xE5\xB1\x8F";  /* 屏 */
```

然后：

```c
Paint_DrawUTF8String16x16(
    12,
    60,
    Text_EPD_UTF8,
    EPD_BLACK
);
```

这样可以保证：

```text
GBK C 源文件
      ↓
显式 UTF-8 字节
      ↓
UTF8_Decode()
      ↓
Unicode
      ↓
中文字模
```

不依赖源文件自身编码。

> README.md 本身建议保存为 UTF-8，它不参与 MCU 编译。

---

## 九、当前中文字体

目前 `font_cn.c` 仅保存项目实际使用的中文字模，并不是完整中文字库。

当前已经验证的字符包括：

```text
墨
水
屏
状
态
正
常
```

每个 16 × 16 汉字占用：

```text
16 × 16 bit
= 256 bit
= 32 Byte
```

后续计划增加 PC 端字模生成工具，根据实际需要自动生成中文字模代码，避免手工录入点阵数据。

---

## 十、编译与下载

### 编译

使用：

```text
MounRiver Studio
```

进行 Clean / Build。

建议在增加或删除源文件后执行一次：

```text
Project
→ Clean
→ Build
```

避免旧的 `.o` 文件影响链接结果。

### 下载

目前使用核心板 Type-C 接口，通过 **USB ISP** 下载程序。

进入 ISP 模式：

```text
1. 按住 BOOT0
2. 按下并松开 RST
3. 等待约 1 秒
4. 松开 BOOT0
5. WCHISPStudio 搜索设备
6. Download
```

WCHISPStudio 中应关闭：

```text
启用读保护
```

否则每次下载程序后都会重新开启芯片读保护。

当前暂不使用 WCH-Link / WCH-LinkE。

---

## 十一、当前测试显示

目前已经成功测试：

```text
CH32V103

墨水屏

状态正常

EPD 墨水屏 OK

SSD1608 200X200
```

中英文字符均可正确显示。

---

## 十二、后续计划

计划继续实现：

- 中文字模自动生成工具
- 更完整的中文字库管理
- 图片转 Framebuffer 工具
- BMP / 图像显示
- SSD1608 局部刷新
- 局部刷新残影优化
- 墨水屏休眠
- CH32V103 低功耗
- UI 布局与实际应用界面

后期项目稳定后，再考虑将当前：

```text
LCD/
```

目录重命名为：

```text
EPD/
```

并统一整理 MounRiver 工程的 Include Path 和目录结构。

---

## 十三、项目状态

当前阶段：

```text
SPI 通信               √
SSD1608 初始化         √
黑白全屏刷新           √
Framebuffer            √
基础绘图               √
ASCII 显示             √
16×16 中文显示         √
UTF-8 解码             √
中英文混合显示         √

中文字模自动生成        待开发
图片显示                待开发
局部刷新                待开发
低功耗                  待开发
```

项目仍处于开发阶段。