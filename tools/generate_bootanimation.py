#!/usr/bin/env python3
from pathlib import Path
from PIL import Image, ImageDraw, ImageFilter
import math
import zipfile

W, H = 720, 1280
FRAMES = 180
OUT = Path("build")
PART = OUT / "bootanim" / "part0"
PART.mkdir(parents=True, exist_ok=True)

def apple_mark(draw, cx, cy, scale, fill):
    # Clean, original apple-inspired silhouette assembled from smooth ellipses.
    s = scale
    draw.ellipse((cx-94*s, cy-70*s, cx-8*s, cy+65*s), fill=fill)
    draw.ellipse((cx-4*s, cy-70*s, cx+82*s, cy+65*s), fill=fill)
    draw.ellipse((cx-65*s, cy-4*s, cx+61*s, cy+100*s), fill=fill)
    draw.polygon([(cx-8*s,cy-70*s),(cx+8*s,cy-111*s),(cx+53*s,cy-125*s),(cx+37*s,cy-80*s)], fill=fill)
    # notch and leaf separation
    draw.ellipse((cx-18*s, cy-93*s, cx+19*s, cy-42*s), fill=(7,10,18,255))

for i in range(FRAMES):
    im = Image.new("RGBA", (W, H), (7, 10, 18, 255))
    glow = Image.new("RGBA", (W, H), (0,0,0,0))
    gd = ImageDraw.Draw(glow)
    pulse = 0.5 + 0.5 * math.sin(i / FRAMES * math.tau)
    radius = int(170 + pulse * 26)
    gd.ellipse((W//2-radius, H//2-radius, W//2+radius, H//2+radius), fill=(115,165,255,28))
    glow = glow.filter(ImageFilter.GaussianBlur(70))
    im = Image.alpha_composite(im, glow)
    d = ImageDraw.Draw(im)
    alpha = int(220 + pulse * 35)
    apple_mark(d, W//2, H//2 + 8, 1.0, (248,249,255,alpha))
    # Subtle light line under the mark.
    d.rounded_rectangle((W//2-38, H//2+155, W//2+38, H//2+158), radius=2, fill=(190,210,255,95))
    im.convert("RGB").save(PART / f"{i:03d}.png", optimize=True)

(OUT / "bootanim").mkdir(parents=True, exist_ok=True)
(OUT / "bootanim" / "desc.txt").write_text("720 1280 30\np 1 0 part0\n", encoding="utf-8")
with zipfile.ZipFile(OUT / "bootanimation.zip", "w", compression=zipfile.ZIP_STORED) as z:
    z.write(OUT / "bootanim" / "desc.txt", "desc.txt")
    for frame in sorted(PART.glob("*.png")):
        z.write(frame, f"part0/{frame.name}")
print("Created build/bootanimation.zip")
