#!/usr/bin/env python3

import os
import json

e = {}
with open("./entity_flags.json", "r") as f:
    e = json.load(f)

with open("./include/game/entity_flags.h", "w") as f:
    flags_h  = "#ifndef __ENTITY_FLAGS_H__\n"
    flags_h += "#define __ENTITY_FLAGS_H__\n\n"
    flags_h += "enum entity_flag {\n"
    flags_h += "  NO_FLAGS = 0,\n"
    for i, flag in enumerate(e["flags"]):
        flags_h += "  " + flag + (" = 1 << %d,\n" % i)
    flags_h += "};\n\n"
    flags_h += "#endif/*__ENTITY_FLAGS_H__*/\n"
    f.write(flags_h)

src = ""
with open("./src/game/systems.c", "r") as f:
    src = f.read()


lexer_tokens = []
lexer_at     = 0

def lexer_add(token):
    global lexer_tokens
    lexer_tokens.append(token)

def lexer_has_tokens():
    global lexer_at
    global lexer_tokens
    return lexer_at < len(lexer_tokens)

def lexer_chop(err, expected=[]):
    global lexer_at
    global lexer_tokens
    if not lexer_has_tokens():
        print(err)
        exit(1)
    token = lexer_tokens[lexer_at]
    if len(expected) > 0:
        found = False
        for e in expected:
            if token == e:
                found = True
                break
        if not found:
            print(err)
            exit(1)
    lexer_at += 1
    return token

is_comment = False
for line in src.splitlines():
    chars = list(line)
    i = 0
    while i < len(chars):
        if is_comment:
            if i+1 < len(chars) and chars[i] == '*' and chars[i+1] == '/':
                i += 2
                is_comment = False
            else:
                i += 1
                continue
        token = ""
        while i < len(chars) and chars[i].isspace():
            i += 1
        if i == len(chars):
            continue
        if i+1 < len(chars) and chars[i] == '/':
            if chars[i+1] == '/':
                break
            elif chars[i+1] == '*':
                is_comment = True
                break;
        if chars[i].isalpha() or chars[i] == '_':
            while i < len(chars) and (chars[i].isalnum() or chars[i] == '_'):
                token+=chars[i]
                i += 1
        else:
            token=chars[i]
            if token == '#':
                break
            i += 1
        if token != "":
            lexer_add(token)

on_update_systems = []
on_render_systems = []

def add_system(systems_list, name):
    system = {
        "name": "",
        "flags": []
    }
    token = lexer_chop("some '"+name+"' is missing a '('", ['('])
    token = lexer_chop("some '"+name+"' is missing the system name")
    if not token.isidentifier():
        print("invalid system name '"+token+"'")
        exit(1)
    system["name"] = token
    token = lexer_chop("missing a ',' after systems name on system '"+system["name"]+"'", [','])
    while True:
        token = lexer_chop("system '"+system["name"]+"' missing ')'")
        if token == ')':
            break
        if not token.isidentifier():
            print("invalid flag name '"+token+"'")
            exit(1)
        system["flags"].append(token)
        token = lexer_chop("system '"+system["name"]+"' missing ')'", [')',','])
        if token == ')':
            break
    systems_list.append(system)

while lexer_has_tokens():
    token = lexer_chop("unreachable")
    if token == "ON_UPDATE_SYSTEM":
        add_system(on_update_systems, "ON_UPDATE_SYSTEM")
    elif token == "ON_RENDER_SYSTEM":
        add_system(on_render_systems, "ON_RENDER_SYSTEM")

src = "void\nentity_manager_update(float dt) {\n"
if len(on_update_systems) == 0:
    src += "  (void)dt;\n"
else:
    src += "  for (uint32_t i = 0; i < g_entities.cached_amount; i++) {\n"
    src += "    auto e = g_entities.cached[i];\n"
    src += "    e->flags = e->next_flags;\n"
    src += "  }\n"
    for system in on_update_systems:
        src += "  for (uint32_t i = 0; i < g_entities.cached_amount; i++) {\n"
        src += "    auto e = g_entities.cached[i];\n"
        src += "    if (entity_get_flags(e, "
        for idx, flag in enumerate(system["flags"]):
            if idx > 0:
                src += "|"
            src += flag
        src += ")) " + system["name"] + "(e, dt);\n  }\n"
for system in on_render_systems:
    src += "  for (uint32_t i = 0; i < g_entities.cached_amount; i++) {\n"
    src += "    auto e = g_entities.cached[i];\n"
    src += "    if (entity_get_flags(e, "
    for idx, flag in enumerate(system["flags"]):
        if idx > 0:
            src += "|"
        src += flag
    src += ")) " + system["name"] + "(e);\n  }\n"
src += "}\n"

with open("./include/game/entities_update_and_render_impl.h", "w") as f:
    f.write(src)

src  = "#ifndef __SYSTEMS_H__\n"
src += "#define __SYSTEMS_H__\n\n"
src += "#include \"game/entity.h\"\n\n"
for system in on_update_systems:
    src += "void " + system["name"] + "(struct entity *self, float dt);\n"
for system in on_render_systems:
    src += "void " + system["name"] + "(struct entity *self);\n"
src += "\n#endif/*__SYSTEMS_H__*/\n"

with open("./include/game/systems.h", "w") as f:
    f.write(src)

print("updated systems")
