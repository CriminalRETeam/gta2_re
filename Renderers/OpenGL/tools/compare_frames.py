#!/usr/bin/env python3
"""Compares GL captures with reference captures, frame by frame (GTA2GL_CAPTURE output, f_NNNNN.png or .bmp).

compare_frames.py <ref_dir> <gl_dir> [k] [--rect x0,y0,x1,y1]

Both frames are reduced to RGB565 first (both renderers draw into 16-bit targets). Per frame: mean absolute
error, PSNR, bad% (largest channel difference > 24), exact%, and the error against reference frames N-k..N+k
(a timing offset shows up as a smaller error at a non zero shift).
"""
import glob
import os
import re
import sys

import numpy as np
from PIL import Image


def q565(a):
    a = a.copy()
    for c, bits in ((0, 5), (1, 6), (2, 5)):
        v = a[..., c] >> (8 - bits)
        a[..., c] = (v << (8 - bits)) | (v >> (2 * bits - 8))
    return a


def frames(d):
    out = {}
    for f in glob.glob(os.path.join(d, '*')):
        m = re.search(r'(\d+)\.(png|bmp)$', f)
        if m:
            out[int(m.group(1))] = f
    return out


def main():
    args = sys.argv[1:]
    rect = None
    if '--rect' in args:
        i = args.index('--rect')
        rect = [int(v) for v in args[i + 1].split(',')]
        del args[i:i + 2]
    if len(args) < 2:
        sys.exit(__doc__)
    refs, gls = frames(args[0]), frames(args[1])
    k = int(args[2]) if len(args) > 2 else 0

    def load(p):
        a = q565(np.asarray(Image.open(p).convert('RGB'), dtype=np.int32))
        return a[rect[1]:rect[3], rect[0]:rect[2]] if rect else a

    rows = []
    for n in sorted(gls):
        if n not in refs:
            continue
        g = load(gls[n])
        d = np.abs(load(refs[n]) - g)
        mse = (d * d).mean()
        row = (d.mean(), 99.0 if mse == 0 else 10 * np.log10(65025 / mse), (d.max(2) > 24).mean() * 100,
               (d.max(2) == 0).mean() * 100)
        shifts = ' '.join('%+d:%.2f' % (s, np.abs(load(refs[n + s]) - g).mean()) for s in range(-k, k + 1) if s and n + s in refs)
        print('%05d MAE %.2f PSNR %.2f bad%% %.2f exact%% %.1f %s' % ((n,) + row + (shifts,)))
        rows.append(row)
    if rows:
        t = np.array(rows)
        print('mean MAE %.2f PSNR %.2f bad%% %.2f exact%% %.1f over %d frames' % (tuple(t.mean(0)) + (len(t),)))


if __name__ == '__main__':
    main()
