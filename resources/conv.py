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

import sys
import os
from PIL import Image
from RLE24 import encodeRLE24, decodeRLE24

img = Image.open(sys.argv[1])

bgcolor = (0x00, 0x00, 0x00, 0x00)

if img.mode == "RGB" or img.mode == "P" or img.mode == "L":
    background = Image.new("RGB", img.size, bgcolor)
    img.load()
    background.paste(img)
elif img.mode == "RGBA":
    background = Image.new("RGB", img.size, bgcolor)
    img.load()
    background.paste(img, mask=img.split()[3])  # alpha channel
else:
    print("sorry, can't support this format")
    sys.exit()

#r, g, b = background.split()
#data = Image.merge("RGB", (r, g, b)).tobytes()

data = encodeRLE24(background)

f = open(sys.argv[2], "wb")
f.write(data)
f.close()
