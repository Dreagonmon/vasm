'''framebuf pure python implement
'''
MVLSB = 0
MONO_VLSB = 0
MONO_VMSB = 7
RGB565 = 1
GS2_HMSB = 5
GS4_HMSB = 2
GS8 = 6
MONO_HLSB = 3 # in fact, it is MSB
MONO_HMSB = 4 # in fact, it is LSB

MAX = lambda x, y: x if x > y else y
MIN = lambda x, y: x if x < y else y

# source at @micropython: /ports/stm32/font_petme128_8x8.h
font_petme128_8x8 = b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00OO\x00\x00\x00\x00\x07\x07\x00\x00\x07\x07\x00\x14\x7f\x7f\x14\x14\x7f\x7f\x14\x00$.kk:\x12\x00\x00c3\x18\x0cfc\x00\x002\x7fMMwrP\x00\x00\x00\x04\x06\x03\x01\x00\x00\x00\x1c>cA\x00\x00\x00\x00Ac>\x1c\x00\x00\x08*>\x1c\x1c>*\x08\x00\x08\x08>>\x08\x08\x00\x00\x00\x80\xe0`\x00\x00\x00\x00\x08\x08\x08\x08\x08\x08\x00\x00\x00\x00``\x00\x00\x00\x00@`0\x18\x0c\x06\x02\x00>\x7fIE\x7f>\x00\x00@D\x7f\x7f@@\x00\x00bsQIOF\x00\x00"cII\x7f6\x00\x00\x18\x18\x14\x16\x7f\x7f\x10\x00\'gEE}9\x00\x00>\x7fII{2\x00\x00\x03\x03y}\x07\x03\x00\x006\x7fII\x7f6\x00\x00&oII\x7f>\x00\x00\x00\x00$$\x00\x00\x00\x00\x00\x80\xe4d\x00\x00\x00\x00\x08\x1c6cAA\x00\x00\x14\x14\x14\x14\x14\x14\x00\x00AAc6\x1c\x08\x00\x00\x02\x03QY\x0f\x06\x00\x00>\x7fAMO.\x00\x00|~\x0b\x0b~|\x00\x00\x7f\x7fII\x7f6\x00\x00>\x7fAAc"\x00\x00\x7f\x7fAc>\x1c\x00\x00\x7f\x7fIIAA\x00\x00\x7f\x7f\t\t\x01\x01\x00\x00>\x7fAI{:\x00\x00\x7f\x7f\x08\x08\x7f\x7f\x00\x00\x00A\x7f\x7fA\x00\x00\x00 `A\x7f?\x01\x00\x00\x7f\x7f\x1c6cA\x00\x00\x7f\x7f@@@@\x00\x00\x7f\x7f\x06\x0c\x06\x7f\x7f\x00\x7f\x7f\x0e\x1c\x7f\x7f\x00\x00>\x7fAA\x7f>\x00\x00\x7f\x7f\t\t\x0f\x06\x00\x00\x1e?!a\x7f^\x00\x00\x7f\x7f\x199oF\x00\x00&oII{2\x00\x00\x01\x01\x7f\x7f\x01\x01\x00\x00?\x7f@@\x7f?\x00\x00\x1f?``?\x1f\x00\x00\x7f\x7f0\x180\x7f\x7f\x00cw\x1c\x1cwc\x00\x00\x07\x0fxx\x0f\x07\x00\x00aqYMGC\x00\x00\x00\x7f\x7fAA\x00\x00\x00\x02\x06\x0c\x180`@\x00\x00AA\x7f\x7f\x00\x00\x00\x08\x0c\x06\x06\x0c\x08\x00\xc0\xc0\xc0\xc0\xc0\xc0\xc0\xc0\x00\x00\x01\x03\x06\x04\x00\x00\x00 tTT|x\x00\x00\x7f\x7fDD|8\x00\x008|DDl(\x00\x008|DD\x7f\x7f\x00\x008|TT\\X\x00\x00\x08~\x7f\t\x03\x02\x00\x00\x98\xbc\xa4\xa4\xfc|\x00\x00\x7f\x7f\x04\x04|x\x00\x00\x00\x00}}\x00\x00\x00\x00@\xc0\x80\x80\xfd}\x00\x00\x7f\x7f08lD\x00\x00\x00A\x7f\x7f@\x00\x00\x00||\x180\x18||\x00||\x04\x04|x\x00\x008|DD|8\x00\x00\xfc\xfc$$<\x18\x00\x00\x18<$$\xfc\xfc\x00\x00||\x04\x04\x0c\x08\x00\x00H\\TTt \x00\x04\x04?\x7fDd \x00\x00<|@@|<\x00\x00\x1c<``<\x1c\x00\x00\x1c|0\x180|\x1c\x00Dl88lD\x00\x00\x9c\xbc\xa0\xa0\xfc|\x00\x00Ddt\\LD\x00\x00\x08\x08>wAA\x00\x00\x00\x00\xff\xff\x00\x00\x00\x00AAw>\x08\x08\x00\x00\x02\x03\x01\x03\x02\x03\x01\xaaU\xaaU\xaaU\xaaU'

class AbstractFormat(object):
    def __init__(self, buffer, width, height, format, stride = None):
        self.buffer = buffer
        self.width = width
        self.height = height
        self.format = format
        self.stride = stride if stride else width
        pass
    def _set_pixel(self, x, y, c): pass
    def _get_pixel(self, x, y): pass
    def _fill_rect(self, x, y, w, h, c): pass
    def __repr__(self):
        txt = '<framebuffer.FrameBuffer object Width: {:}, Height: {:}>\n'.format(self.width, self.height)
        for y in range(self.height):
            for x in range(self.width):
                txt += '██' if self._get_pixel(x, y) != 0 else '  '
            txt += '\n'
        return txt[:-1]

class FormatMonoVertical(AbstractFormat):
    def _set_pixel(self, x, y, c):
        index = (y >> 3) * self.stride + x
        offset = y & 0x07 if self.format == MONO_VLSB else 7 - (y & 0x07)
        self.buffer[index] = (self.buffer[index] & ~(0x01 << offset)) | ((1 if c != 0 else 0) << offset)
    def _get_pixel(self, x, y):
        index = (y >> 3) * self.stride + x
        offset = y & 0x07 if self.format == MONO_VLSB else 7 - (y & 0x07)
        return (self.buffer[index] >> offset) & 0x01
    def _fill_rect(self, x, y, w, h, c):
        reverse = self.format == MONO_VLSB
        while h > 0:
            h -= 1
            bp = (y >> 3) * self.stride + x
            offset = y & 0x07 if reverse else 7 - (y & 0x07)
            for _ in range(w, 0, -1):
                self.buffer[bp] = (self.buffer[bp] & ~(0x01 << offset)) | ((1 if c != 0 else 0) << offset)
                bp += 1
            y += 1
class FormatMonoHorizontal(AbstractFormat):
    def _set_pixel(self, x, y, c):
        index = (x + y * self.stride) >> 3
        offset = x & 0x07 if self.format == MONO_HMSB else 7 - (x & 0x07)
        self.buffer[index] = (self.buffer[index] & ~(0x01 << offset)) | ((1 if c != 0 else 0) << offset)
    def _get_pixel(self, x, y):
        index = (x + y * self.stride) >> 3
        offset = x & 0x07 if self.format == MONO_HMSB else 7 - (x & 0x07)
        return (self.buffer[index] >> offset) & 0x01
    def _fill_rect(self, x, y, w, h, c):
        reverse = self.format == MONO_HMSB
        advance = self.stride >> 3
        while w > 0:
            w -= 1
            bp = (x >> 3) + y * advance
            offset = x & 7 if reverse else 7 - (x & 7)
            for _ in range(h, 0, -1):
                self.buffer[bp] = (self.buffer[bp] & ~(0x01 << offset)) | ((1 if c != 0 else 0) << offset)
                bp += advance
            x += 1
class FormatRGB565(AbstractFormat):
    def _set_pixel(self, x, y, c):
        offset = (x + y*self.stride) * 2 # 16bit
        self.buffer[offset:offset+2] = c.to_bytes(2, 'little')
    def _get_pixel(self, x, y):
        offset = (x + y*self.stride) * 2
        return int.from_bytes(self.buffer[offset:offset+2], 'little')
    def _fill_rect(self, x, y, w, h, c):
        bp = (x + y*self.stride) * 2
        while h > 0:
            h -= 1
            for _ in range(w, 0, -1):
                self.buffer[bp:bp+2] = c.to_bytes(2, 'little')
                bp += 2
            bp += (self.stride - w) * 2

class FrameBuffer(object):
    def __init__(self, buffer, width, height, format, stride = None):
        self.buffer = buffer
        self.width = width
        self.height = height
        self.format = format
        self.stride = stride if stride else width
        if self.format == MONO_HMSB or self.format == MONO_HLSB:
            self.stride = (self.stride + 7) & ~7
            self.__format = FormatMonoHorizontal(self.buffer, self.width, self.height, self.format, self.stride)
        elif self.format == MONO_VMSB or self.format == MONO_VLSB:
            self.__format = FormatMonoVertical(self.buffer, self.width, self.height, self.format, self.stride)
        elif self.format == RGB565:
            self.__format = FormatRGB565(self.buffer, self.width, self.height, self.format, self.stride)
        else:
            self.__format = FormatRGB565(self.buffer, self.width, self.height, self.format, self.stride)
            print('Warning: this framebuf fotmat is not implement yet, use RGB565 instead.')
    def __repr__(self):
        return self.__format.__repr__()
    def fill(self, c):
        self.__format._fill_rect(0, 0, self.width, self.height, c)
    def pixel(self, x, y, c=None):
        if 0 <= x and x < self.width and 0 <= y and y < self.height:
            if c == None:
                return self.__format._get_pixel(x, y)
            else:
                self.__format._set_pixel(x, y, c)
        return None
    def hline(self, x, y, w, c):
        self.fill_rect(x, y, w, 1, c)
    def vline(self, x, y, h, c):
        self.fill_rect(x, y, 1, h, c)
    def line(self, x1, y1, x2, y2, c):
        dx = x2 - x1
        if dx > 0:
            sx = 1
        else:
            dx = -dx
            sx = -1
        dy = y2 - y1
        if dy > 0:
            sy = 1
        else:
            dy = -dy
            sy = -1
        if dy > dx:
            temp = x1
            x1 = y1
            y1 = temp
            temp = dx
            dx = dy
            dy = temp
            temp = sx
            sx = sy
            sy = temp
            steep = True
        else:
            steep = False
        e = 2 * dy - dx
        for i in range(dx):# (mp_int_t i = 0; i < dx; ++i) {
            if steep:
                if 0 <= y1 and y1 < self.width and 0 <= x1 and x1 < self.height:
                    self.__format._set_pixel(y1, x1, c)
            else:
                if 0 <= x1 and x1 < self.width and 0 <= y1 and y1 < self.height:
                    self.__format._set_pixel(x1, y1, c)
            while e >= 0:
                y1 += sy
                e -= 2 * dx
            x1 += sx
            e += 2 * dy
        if 0 <= x2 and x2 < self.width and 0 <= y2 and y2 < self.height:
            self.__format._set_pixel(x2, y2, c)
    def rect(self, x, y, w, h, c):
        self.fill_rect(x, y, w, 1, c)
        self.fill_rect(x, y + h - 1, w, 1, c)
        self.fill_rect(x, y, 1, h, c)
        self.fill_rect(x + w - 1, y, 1, h, c)
    def fill_rect(self, x, y, w, h, c):
        xend = MIN(self.width, x + w)
        yend = MIN(self.height, y + h)
        x = MAX(x, 0)
        y = MAX(y, 0)
        self.__format._fill_rect(x, y, xend - x, yend - y, c)
    def blit(self, fbuf, x, y, key=None):
        if (x >= self.width) or (y >= self.height) or (-x >= fbuf.width) or (-y >= fbuf.height):
            # Out of bounds, no-op.
            return
        # Clip.
        x0 = MAX(0, x)
        y0 = MAX(0, y)
        x1 = MAX(0, -x)
        y1 = MAX(0, -y)
        x0end = MIN(self.width, x + fbuf.width)
        y0end = MIN(self.height, y + fbuf.height)
        # for (; y0 < y0end; ++y0)
        while y0 < y0end:
            cx1 = x1
            for cx0 in range(x0, x0end, 1):
                col = fbuf.__format._get_pixel(cx1, y1)
                if col != key:
                    self.__format._set_pixel(cx0, y0, col)
                cx1 += 1
            y0 += 1
            y1 += 1
    
    # not require function below
    def scroll(self, xstep, ystep):
        if xstep < 0:
            sx = 0
            xend = self.width + xstep
            dx = 1
        else:
            sx = self.width - 1
            xend = xstep - 1
            dx = -1
        if ystep < 0:
            y = 0
            yend = self.height + ystep
            dy = 1
        else:
            y = self.height - 1
            yend = ystep - 1
            dy = -1
        while y != yend:
            for x in range(sx, xend, dx):
                self.__format._set_pixel(x, y, self.__format._get_pixel(x - xstep, y - ystep))
            y += dy
    def text(self, t:str, x, y, c=1):
        # loop over chars
        for chr in t.encode():
            # get char and make sure its in range of font
            if chr < 32 or chr > 127:
                chr = 127
            # get char data
            chr_data_offset = (chr - 32) * 8
            # chr_data = font_petme128_8x8[(chr - 32) * 8: (chr - 32) * 8 + 8]
            # loop over char data
            for j in range(8):
                if 0 <= x and x < self.width: # clip x
                    vline_data = font_petme128_8x8[chr_data_offset+j]; # each byte is a column of 8 pixels, LSB at top
                    m_y = y
                    while vline_data > 0:
                        if vline_data & 1: # only draw if pixel set
                            if 0 <= m_y and m_y < self.height: # clip y
                                self.__format._set_pixel(x, m_y, c)
                        m_y += 1
                        vline_data >>= 1
                x += 1
