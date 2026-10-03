from PIL import Image
import sys
import os


WIDTH = 200
HEIGHT = 200

THRESHOLD = 128


def convert_image(input_file, output_file):
    img = Image.open(input_file)

    # 转换成灰度图
    img = img.convert("L")

    # 强制缩放到 200x200
    img = img.resize(
        (WIDTH, HEIGHT),
        Image.Resampling.LANCZOS
    )

    data = []

    # 每行 200 pixel = 25 bytes
    for y in range(HEIGHT):

        for byte_x in range(WIDTH // 8):

            value = 0x00

            for bit in range(8):

                x = byte_x * 8 + bit

                pixel = img.getpixel((x, y))

                # 当前墨水屏格式：
                #
                # 1 = 白
                # 0 = 黑
                #
                if pixel >= THRESHOLD:
                    value |= (0x80 >> bit)

            data.append(value)


    with open(output_file, "w", encoding="utf-8") as f:

        f.write('#include "image.h"\n\n')

        f.write(
            "const uint8_t Image_Test[IMAGE_SIZE] =\n"
        )

        f.write("{\n")

        for i in range(0, len(data), 16):

            line = data[i:i + 16]

            f.write("    ")

            f.write(
                ", ".join(
                    f"0x{value:02X}"
                    for value in line
                )
            )

            f.write(",\n")

        f.write("};\n")


    print("转换完成")
    print("输入:", input_file)
    print("输出:", output_file)
    print("尺寸:", WIDTH, "x", HEIGHT)
    print("数据:", len(data), "bytes")


if __name__ == "__main__":

    if len(sys.argv) != 3:
        print(
            "用法: python image_to_c.py input.png image.c"
        )

        sys.exit(1)

    convert_image(
        sys.argv[1],
        sys.argv[2]
    )