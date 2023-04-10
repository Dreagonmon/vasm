from sys import argv
from pbm import read_image
import framebuf

if __name__ == "__main__":
    source = argv[1]
    with open(source, "rb") as f:
        iw, ih, _fmt, data, _comment = read_image(f)
    img = framebuf.FrameBuffer(data, iw, ih, framebuf.MONO_HLSB)
    print(img)
    # convert tile one by one
    tile_buffer = bytearray(8*(8//8))
    tile = framebuf.FrameBuffer(tile_buffer, 8, 8, framebuf.MONO_VLSB)
    for row in range(ih // 8):
        for col in range(iw // 8):
            offx = col * 8
            offy = row * 8
            index = row * (iw // 8) + col
            tile.blit(img, -offx, -offy)
            print("TILE", end=" ")
            print(index, end=" ")
            for byt in tile_buffer:
                print(f"0x{byt:02X}", end=" ")
            print("; ")
