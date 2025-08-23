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

imgs = [Image.open(file).convert("RGBA") for file in img_paths]
metas = []
for file,n in meta_paths:
    with open(file) as f:
        metas.append((json.load(f),n))

def next_pow2(x):
    return 1 << (x - 1).bit_length()

atlas_height = 0

x, y = 0, 0
row_height = 0

sprite_imgs  = []
sprite_infos = []

atlas_width = 0
exp = 0
for img in imgs:
    w, _ = img.size
    while w > atlas_width:
        exp += 1
        atlas_width = 1<<exp

for name, img in zip(names, imgs):
    w, h = img.size
    if x + w > atlas_width:
        x = 0
        y += row_height
        row_height = 0
    sprite_imgs.append((img, x, y))
    sprite_infos.append({ "name": name, "x": x, "y": y, "w": w, "h": h, });
    x += w
    row_height = max(row_height, h)
    atlas_height = max(atlas_height, y + h)

animations = []
for meta, name in metas:
    if not "frameTags" in meta["meta"]:
        continue
    for tag in meta["meta"]["frameTags"]:
        animations.append({
            "name": name+"_"+tag["name"].upper(),
            "sprite": "SPR_"+name,
            "durations": [f["duration"]/1000 for f in meta["frames"][tag["from"]:(tag["to"]+1)]],
            "frame-width": meta["frames"][0]["frame"]["w"],
            "first-frame": tag["from"]
        })
#print(animations)

atlas_height = next_pow2(atlas_height)

atlas_img = Image.new("RGBA", (atlas_width, atlas_height), (0, 0, 0, 0))
for img, x, y in sprite_imgs:
    atlas_img.paste(img, (x, y))

atlas_data = atlas_img.tobytes()

sprites_h  = "#ifndef __SPRITES_H__\n"
sprites_h += "#define __SPRITES_H__\n\n"
sprites_h += "enum sprite {\n"
for i, info in enumerate(sprite_infos):
    sprites_h += "  SPR_%s" % info["name"]
    if i == 0:
        sprites_h += " = 0"
    sprites_h += ",\n"
sprites_h += "  SPRITES_AMOUNT\n};\n\n"
sprites_h += "enum animation {\n"
for i, anim in enumerate(animations):
    sprites_h += "  ANIM_%s" % anim["name"]
    if i == 0:
        sprites_h += " = 0"
    sprites_h += ",\n"
sprites_h += "  ANIMATIONS_AMOUNT\n};\n\n"
sprites_h += "struct animation_data {\n"
sprites_h += "  const float *durations;\n"
sprites_h += "  float frame_width;\n"
sprites_h += "  uint32_t frames_amount;\n"
sprites_h += "  uint32_t first_frame;\n"
sprites_h += "  enum sprite sprite;\n"
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
atlas_h += "static const uint8_t g_atlas_data[ATLAS_WIDTH*ATLAS_HEIGHT*4] = {\n"
#for i in range(0, len(atlas_data), 4):
#    atlas_h += "0x%02x%02x%02x%02x," % (atlas_data[i + 3], atlas_data[i + 2], atlas_data[i + 1], atlas_data[i + 0])
atlas_h += "#embed \"imgs/atlas.bin\"\n"
atlas_h += "};\n\n"
atlas_h += "static const struct v2 g_atlas_sprite_positions[SPRITES_AMOUNT] = {\n"
for info in sprite_infos:
    atlas_h += "  { %d.0f * ATLAS_PIXEL_W, %d.0f * ATLAS_PIXEL_H },\n" % (info["x"], info["y"])
atlas_h += "};\n\n"
atlas_h += "static const struct v2 g_atlas_sprite_sizes[SPRITES_AMOUNT] = {\n"
for info in sprite_infos:
    atlas_h += "  { %d.0f * ATLAS_PIXEL_W, %d.0f * ATLAS_PIXEL_H },\n" % (info["w"], info["h"])
atlas_h += "};\n\n"
atlas_h += "static const struct v2 g_atlas_sprite_half_sizes[SPRITES_AMOUNT] = {\n"
for info in sprite_infos:
    w = "%g" % (info["w"] * 0.5)
    if "." not in w:
        w += ".0"
    h = "%g" % (info["h"] * 0.5)
    if "." not in h:
        h += ".0"
    atlas_h += "  { %sf * UNIT_ONE_PIXEL, %sf * UNIT_ONE_PIXEL },\n" % (w, h)
atlas_h += "};\n\n"
for anim in animations:
    atlas_h += "static const float g_atlas_animation_durations_" + anim["name"].lower() + ("[%u]" % len(anim["durations"])) + " = {\n"
    atlas_h += "".join("  %g,\n" % d for d in anim["durations"])
    atlas_h += "};\n\n"
atlas_h += "static const struct animation_data g_atlas_animations[ANIMATIONS_AMOUNT] = {\n"
for anim in animations:
    atlas_h += "  {\n"
    atlas_h += "    .durations = g_atlas_animation_durations_" + anim["name"].lower() + ",\n"
    atlas_h += "    .frame_width = %d.0f * ATLAS_PIXEL_W,\n" % anim["frame-width"]
    atlas_h += "    .frames_amount = %d,\n" % len(anim["durations"])
    atlas_h += "    .first_frame = %d,\n" % anim["first-frame"]
    atlas_h += "    .sprite = %s\n" % anim["sprite"]
    atlas_h += "  },\n"
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
