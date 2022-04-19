#
# Copyright (c) 2021 MechaResearch
#
# This program is free software; you can redistribute it and/or modify it
# under the terms and conditions of the GNU General Public License,
# version 2, as published by the Free Software Foundation.
#
# This program is distributed in the hope it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
# more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

from struct import pack, unpack
from PIL import Image


def encode(line):
    count = 0
    lst = []
    repeat = -1
    run = []
    total = len(line) - 1
    for index, current in enumerate(line[:-1]):
        if current != line[index + 1]:
            run.append(current)
            count += 1
            if repeat == 1:
                lst.append((count + 128, run))
                count = 0
                run = []
                repeat = -1
                if index == total - 1:
                    run = [line[index + 1]]
                    lst.append((1, run))
            else:
                repeat = 0

                if count == 128:
                    lst.append((128, run))
                    count = 0
                    run = []
                    repeat = -1
                if index == total - 1:
                    run.append(line[index + 1])
                    lst.append((count + 1, run))
        else:
            if repeat == 0:
                lst.append((count, run))
                count = 0
                run = []
                repeat = -1
                if index == total - 1:
                    run.append(line[index + 1])
                    run.append(line[index + 1])
                    lst.append((2 + 128, run))
                    break
            run.append(current)
            repeat = 1
            count += 1
            if count == 128:
                lst.append((256, run))
                count = 0
                run = []
                repeat = -1
            if index == total - 1:
                if count == 0:
                    run = [line[index + 1]]
                    lst.append((1, run))
                else:
                    run.append(current)
                    lst.append((count + 1 + 128, run))
    return lst


def encodeRLE24(img):
    width, height = img.size
    output = ''

    for h in range(height):
        line = []
        result = []
        for w in range(width):
            r, g, b = img.getpixel((w, h))
            line.append((r << 16) + (g << 8) + b)
        result = encode(line)
        for count, pixel in result:
            output += pack("B", count - 1)
            if count > 128:
                output += pack("B", (pixel[0]) & 0xFF)
                output += pack("B", ((pixel[0]) >> 8) & 0xFF)
                output += pack("B", ((pixel[0]) >> 16) & 0xFF)
            else:
                for item in pixel:
                    output += pack("B", (item) & 0xFF)
                    output += pack("B", (item >> 8) & 0xFF)
                    output += pack("B", (item >> 16) & 0xFF)
    return output


def decodeRLE24(rle, size):
    bgcolor = (0x00, 0x00, 0x00)
    width, height = size
    img = Image.new("RGB", size, bgcolor)
    pixels = img.load()
    pos = 0
    x = 0
    y = 0

    hw = 0
    while hw < height * width:
        count = unpack("B", rle[pos: pos + 1])[0]
        pos += 1
        repeat_run = count & 0x80
        count = (count & 0x7f) + 1

        for i in range(count):
            b, g, r = unpack("BBB", rle[pos: pos + 3])
            pixels[x, y] = r, g, b
            hw += 1
            x += 1

            if not repeat_run:
                pos += 3

        if repeat_run:
            pos += 3

        if x == width + 0:
            y += 1
            x = 0

    return img
