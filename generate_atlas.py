#!/usr/bin/env python3

import os
import json

from PIL import Image

imgs_dir  = "./assets/imgs/"
png_names  = [f for f in os.listdir(imgs_dir) if os.path.isfile(os.path.join(imgs_dir, f)) and f.endswith("png") ]
file_names = [n.removesuffix(".png") for n in png_names]
names      = [n.upper() for n in file_names]
json_names = [(f,n) for f,n in [(f+".json",n) for f,n in zip(file_names,names)] if os.path.isfile(os.path.join(imgs_dir, f)) ]

img_paths  = [os.path.join(imgs_dir, n) for n in png_names]
meta_paths = [(os.path.join(imgs_dir, f),n) for f,n in json_names]

imgs = [(name, Image.open(file).convert("RGBA")) for file,name in zip(img_paths, names)]

metas = []
for file,n in meta_paths:
    with open(file) as f:
        metas.append((json.load(f),n))

extrude_pixels = 2

animations        = []
in_animation_info = {}
glyphs            = []
font_name         = "";
repeated_glyph    = set()
ascii             = [ " ", "!", "\"", "#", "$", "%", "&", "'", "(", ")", "*", "+", ",", "-", ".", "/", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ":", ";", "<", "=", ">", "?", "@", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "[", "\\", "]", "^", "_", "`", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "{", "|", "}", "~" ]
for meta, name in metas:
    if not "frameTags" in meta["meta"]:
        continue
    if "data" in meta["meta"]["frameTags"][0] and meta["meta"]["frameTags"][0]["data"] == "is_font":
        if font_name != "":
            print("can't have more than two fonts, but '", name,"' is a font while already having one");
            exit(1)
        font_name = name
        if not "slices" in meta["meta"]:
            print("font must have slices, but '", name, "' doesn't", sep="");
            exit(1)
        for char in ascii:
            for slice in meta["meta"]["slices"]:
                if slice["name"] != char:
                    continue
                if slice["name"] in repeated_glyph:
                    print("font '", name, "' has two '", slice["name"], "' slices", sep="");
                    exit(1)
                repeated_glyph.add(slice["name"])
                glyphs.append({
                    "char": char,
                    "offset": slice["keys"][0]["bounds"]["x"],
                    "width": slice["keys"][0]["bounds"]["w"],
                })
    else:
        for tag in meta["meta"]["frameTags"]:
            animations.append({
                "name": name+"_"+tag["name"].upper(),
                "sprite": "SPR_"+name,
                "durations": [f["duration"]/1000 for f in meta["frames"][tag["from"]:(tag["to"]+1)]],
                "frame-width": meta["frames"][0]["frame"]["w"],
                "first-frame": tag["from"]
            })
        in_animation_info[name] = (len(meta["frames"]), meta["frames"][0]["frame"]["w"])
        print(name, in_animation_info[name])
#print(animations)

def next_pow2(x):
    return 1 << (x - 1).bit_length()

atlas_height = 0

x, y = extrude_pixels, extrude_pixels
row_height = 0

sprite_imgs  = []
sprite_infos = []

atlas_width = 0
exp = 0
for name, img in imgs:
    w, _ = img.size
    if name in in_animation_info:
        frames_amount, _ = in_animation_info[name]
        w += frames_amount*extrude_pixels*2
    while w > atlas_width:
        exp += 1
        atlas_width = 1<<exp

for name, img in imgs:
    w, h = img.size
    if x + w > atlas_width:
        x = extrude_pixels
        y += row_height+extrude_pixels
        row_height = 0
    spr_x = x + extrude_pixels
    spr_y = y + extrude_pixels
    sprite_imgs.append((img, name, x, y, w, h))
    sprite_infos.append({ "name": name, "x": spr_x, "y": spr_y, "w": w, "h": h });
    x += w+extrude_pixels
    if name in in_animation_info:
        frames_amount, _ = in_animation_info[name]
        x += frames_amount*extrude_pixels*2
    else:
        x += extrude_pixels
    row_height = max(row_height, h+extrude_pixels*2)
    atlas_height = max(atlas_height, y + h+extrude_pixels*2)

atlas_height = next_pow2(atlas_height)

atlas_img = Image.new("RGBA", (atlas_width, atlas_height), (0, 0, 0, 0))
def extrude_image(img, x, y, w, h):
    top          = img.crop((0, 0, w, 1)).resize((w, extrude_pixels));
    left         = img.crop((0, 0, 1, h)).resize((extrude_pixels, h));
    bottom       = img.crop((0, h-1, w, h)).resize((w, extrude_pixels));
    right        = img.crop((w-1, 0, w, h)).resize((extrude_pixels, h));
    top_left     = Image.new("RGBA", (extrude_pixels, extrude_pixels), img.getpixel((0, 0)));
    top_right    = Image.new("RGBA", (extrude_pixels, extrude_pixels), img.getpixel((w-1, 0)));
    bottom_left  = Image.new("RGBA", (extrude_pixels, extrude_pixels), img.getpixel((0, h-1)));
    bottom_right = Image.new("RGBA", (extrude_pixels, extrude_pixels), img.getpixel((w-1, h-1)));
    atlas_img.paste(img,          (x+extrude_pixels, y+extrude_pixels))
    atlas_img.paste(top,          (x+extrude_pixels, y))
    atlas_img.paste(left,         (x, y+extrude_pixels))
    atlas_img.paste(bottom,       (x+extrude_pixels, y+extrude_pixels+h))
    atlas_img.paste(right,        (x+w+extrude_pixels, y+extrude_pixels))
    atlas_img.paste(top_left,     (x, y))
    atlas_img.paste(top_right,    (x+w+extrude_pixels, y))
    atlas_img.paste(bottom_left,  (x, y+h+extrude_pixels))
    atlas_img.paste(bottom_right, (x+w+extrude_pixels, y+h+extrude_pixels))

for img, name, x, y, w, h in sprite_imgs:
    if name in in_animation_info:
        frames_amount, frame_width = in_animation_info[name]
        frame_x = x
        for i in range(0, frames_amount):
            frame_x += extrude_pixels
            extrude_image(img.crop((i*frame_width, 0, (i + 1) * frame_width, h)), frame_x, y, frame_width, h)
            frame_x += frame_width + extrude_pixels
    else:
        extrude_image(img, x, y, w, h)

atlas_data = atlas_img.tobytes()

sprites_h  = "#ifndef __SPRITES_H__\n"
sprites_h += "#define __SPRITES_H__\n\n"
sprites_h += "enum sprite {\n"
for i, info in enumerate(sprite_infos):
    sprites_h += "    SPR_%s" % info["name"]
    if i == 0:
        sprites_h += " = 0"
    sprites_h += ",\n"
sprites_h += "    SPRITES_AMOUNT\n};\n\n"
sprites_h += "enum animation {\n"
for i, anim in enumerate(animations):
    sprites_h += "    ANIM_%s" % anim["name"]
    if i == 0:
        sprites_h += " = 0"
    sprites_h += ",\n"
sprites_h += "    ANIMATIONS_AMOUNT\n};\n\n"
sprites_h += "struct animation_data {\n"
sprites_h += "    const float *durations;\n"
sprites_h += "    float frame_width;\n"
sprites_h += "    uint32_t frames_amount;\n"
sprites_h += "    uint32_t first_frame;\n"
sprites_h += "    enum sprite sprite;\n"
sprites_h += "};\n\n"
sprites_h += "#endif/*__SPRITES_H__*/\n"

atlas_h  = "#ifndef __ATLAS_H__\n"
atlas_h += "#define __ATLAS_H__\n\n"
atlas_h += "#include \"engine/math.h\"\n"
atlas_h += "#include \"engine/sprites.h\"\n\n"
atlas_h += "#define ATLAS_WIDTH %d\n" % atlas_width
atlas_h += "#define ATLAS_HEIGHT %d\n" % atlas_height
atlas_h += "#define ATLAS_PIXEL_W (1.0f/%d)\n" % atlas_width
atlas_h += "#define ATLAS_PIXEL_H (1.0f/%d)\n\n" % atlas_height
atlas_h += "#define ATLAS_EXTRUDE_W (%d.0f * ATLAS_PIXEL_W)\n\n" % extrude_pixels
atlas_h += "#define ATLAS_EXTRUDE_H (%d.0f * ATLAS_PIXEL_H)\n\n" % extrude_pixels
atlas_h += "static const uint8_t g_atlas_data[ATLAS_WIDTH*ATLAS_HEIGHT*4] = {\n"
#for i in range(0, len(atlas_data), 4):
#    atlas_h += "0x%02x%02x%02x%02x," % (atlas_data[i + 3], atlas_data[i + 2], atlas_data[i + 1], atlas_data[i + 0])
atlas_h += "#embed \"imgs/atlas.bin\"\n"
atlas_h += "};\n\n"
atlas_h += "static const struct v2 g_atlas_sprite_positions[SPRITES_AMOUNT] = {\n"
for info in sprite_infos:
    atlas_h += "    { %d.0f * ATLAS_PIXEL_W, %d.0f * ATLAS_PIXEL_H },\n" % (info["x"], info["y"])
atlas_h += "};\n\n"
atlas_h += "static const struct v2 g_atlas_sprite_sizes[SPRITES_AMOUNT] = {\n"
for info in sprite_infos:
    atlas_h += "    { %d.0f * ATLAS_PIXEL_W, %d.0f * ATLAS_PIXEL_H },\n" % (info["w"], info["h"])
atlas_h += "};\n\n"
atlas_h += "static const struct v2 g_atlas_sprite_half_sizes[SPRITES_AMOUNT] = {\n"
for info in sprite_infos:
    w = "%g" % (info["w"] * 0.5)
    if "." not in w:
        w += ".0"
    h = "%g" % (info["h"] * 0.5)
    if "." not in h:
        h += ".0"
    atlas_h += "    { %sf * UNIT_ONE_PIXEL, %sf * UNIT_ONE_PIXEL },\n" % (w, h)
atlas_h += "};\n\n"
for anim in animations:
    atlas_h += "static const float g_atlas_animation_durations_" + anim["name"].lower() + ("[%u]" % len(anim["durations"])) + " = {\n"
    atlas_h += "".join("    %g,\n" % d for d in anim["durations"])
    atlas_h += "};\n\n"
atlas_h += "static const struct animation_data g_atlas_animations[ANIMATIONS_AMOUNT] = {\n"
for anim in animations:
    atlas_h += "    {\n"
    atlas_h += "        .durations = g_atlas_animation_durations_" + anim["name"].lower() + ",\n"
    atlas_h += "        .frame_width = %d.0f * ATLAS_PIXEL_W,\n" % anim["frame-width"]
    atlas_h += "        .frames_amount = %d,\n" % len(anim["durations"])
    atlas_h += "        .first_frame = %d,\n" % anim["first-frame"]
    atlas_h += "        .sprite = %s\n" % anim["sprite"]
    atlas_h += "    },\n"
atlas_h += "};\n\n"
if font_name != "":
    atlas_h += "static const struct {\n"
    atlas_h += "    float offset;\n"
    atlas_h += "    float atlas_width;\n"
    atlas_h += "    float game_width;\n"
    atlas_h += "} g_glyphs['~'-' '+1] = {\n"
    for glyph in glyphs:
        atlas_h += "    { .offset = %d.0f * ATLAS_PIXEL_W, .atlas_width  = %d.0f * ATLAS_PIXEL_W, .game_width  = %d.0f * UNIT_ONE_PIXEL }, /* %s */\n" % (glyph["offset"], glyph["width"], glyph["width"], glyph["char"])
    atlas_h += "};\n\n"
atlas_h += "#endif/*__ATLAS_H__*/\n"

#print(atlas_h)
#print(sprites_h)

atlas_img.save("./atlas_debug.png");

path = "./include/engine/atlas.h"
try:
    with open(path, "w") as f:
        f.write(atlas_h)
        pass
except:
    print("couldn't open '", path, "'", sep="")

path = "./include/engine/sprites.h"
try:
    with open(path, "w") as f:
        f.write(sprites_h)
        pass
except:
    print("couldn't open '", path, "'", sep="")

path = imgs_dir + "atlas.bin"
try:
    with open(path, "wb") as f:
        f.write(atlas_data)
        pass
except:
    print("couldn't open '", path, "'", sep="")

print("atlas updated")
