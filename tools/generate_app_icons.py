#!/usr/bin/env python3
from pathlib import Path
from PIL import Image, ImageDraw
import math

out = Path("app/src/main/res/drawable-nodpi")
out.mkdir(parents=True, exist_ok=True)
items = {
    "ios_settings": ((142,142,147), "gear"),
    "ios_photos": ((255,70,112), "flower"),
    "ios_camera": ((75,82,95), "camera"),
    "ios_messages": ((52,199,89), "chat"),
    "ios_safari": ((45,155,245), "compass"),
    "ios_music": ((255,45,75), "music"),
    "ios_phone": ((48,195,91), "phone"),
    "ios_calendar": ((245,245,248), "calendar"),
    "ios_weather": ((65,175,245), "cloud"),
    "ios_notes": ((255,205,55), "note"),
    "ios_clock": ((38,42,54), "clock"),
    "ios_files": ((60,145,250), "folder"),
}
for name, (bg, kind) in items.items():
    im = Image.new("RGBA", (192,192), (*bg,255))
    d = ImageDraw.Draw(im)
    white = (255,255,255,245)
    if kind == "gear":
        d.ellipse((58,58,134,134), outline=white, width=12)
        d.ellipse((82,82,110,110), fill=white)
        for a in range(0,360,45):
            x=96+int(math.cos(math.radians(a))*48); y=96+int(math.sin(math.radians(a))*48)
            d.ellipse((x-10,y-10,x+10,y+10), fill=white)
    elif kind == "flower":
        for a in range(0,360,45):
            x=96+int(math.cos(math.radians(a))*28); y=96+int(math.sin(math.radians(a))*28)
            d.ellipse((x-19,y-30,x+19,y+30), fill=white)
        d.ellipse((82,82,110,110), fill=(255,205,45,255))
    elif kind == "camera":
        d.rounded_rectangle((34,58,158,142), radius=18, fill=white)
        d.rounded_rectangle((60,45,103,66), radius=8, fill=white)
        d.ellipse((70,75,122,127), fill=bg)
        d.ellipse((81,86,111,116), fill=white)
    elif kind == "chat":
        d.rounded_rectangle((34,42,158,132), radius=35, fill=white)
        d.polygon([(58,120),(52,151),(91,126)], fill=white)
    elif kind == "compass":
        d.ellipse((37,37,155,155), outline=white, width=10)
        d.polygon([(111,55),(92,103),(66,137),(87,88)], fill=white)
    elif kind == "music":
        d.line((112,48,112,127), fill=white, width=13); d.line((112,48,151,39), fill=white, width=13)
        d.ellipse((55,112,103,157), fill=white); d.ellipse((93,105,141,150), fill=white)
    elif kind == "phone":
        d.arc((43,35,150,156), 35, 145, fill=white, width=24)
        d.rounded_rectangle((39,39,77,82), radius=12, fill=white)
        d.rounded_rectangle((116,112,155,153), radius=12, fill=white)
    elif kind == "calendar":
        d.rounded_rectangle((36,40,156,154), radius=15, fill=(255,255,255,255))
        d.rectangle((36,64,156,83), fill=(255,65,75,255))
        d.text((60,91),"27", fill=(35,35,42,255))
    elif kind == "cloud":
        d.ellipse((48,75,112,137), fill=white); d.ellipse((78,53,143,127), fill=white)
        d.rounded_rectangle((54,94,145,137), radius=22, fill=white)
    elif kind == "note":
        d.rounded_rectangle((46,35,146,158), radius=10, fill=white)
        for y in (75,96,117): d.line((63,y,129,y), fill=(210,170,55,255), width=5)
    elif kind == "clock":
        d.ellipse((36,36,156,156), outline=white, width=9)
        d.line((96,96,96,58), fill=white, width=8); d.line((96,96,128,111), fill=white, width=8)
    elif kind == "folder":
        d.rounded_rectangle((31,56,161,145), radius=15, fill=white)
        d.rounded_rectangle((39,42,91,72), radius=10, fill=white)
    im.save(out / f"{name}.png")
print("Generated launcher icon PNGs")
