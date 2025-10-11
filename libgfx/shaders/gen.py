#!/usr/bin/env python3

import sys
import os

def name_is_cxx_keyword(name: str) -> bool:
    keywords = [ "default" ]
    return name in keywords

def gen_entry(file, name: str, path: str):
    if name_is_cxx_keyword(name):
        name += "_"

    file.write(f"""\
inline constexpr char {name}[] = {{
#embed "../shaders/{path}" suffix(,)
'\\0'
}};\n\n\
""");

def gen_namespace(file, name: str, shaders: list[str]):

    file.write(f"namespace {name} {{\n\n");

    for shader in shaders:
        filename, _ = shader.split(".");
        gen_entry(file, filename, shader);

    file.write(f"}} // namespace {name}\n\n");


def split_shader_types(shaders_filenames: list[str]) -> tuple[list[str], list[str]]:

    vertex_shaders = []
    fragment_shaders = []

    for filename in shaders_filenames:
        _, extension = filename.split(".");
        match extension:
            case "frag":
                fragment_shaders.append(filename)
            case "vert":
                vertex_shaders.append(filename)

    return vertex_shaders, fragment_shaders

def main():
    file = open("shaders.hh", "w")
    file.write("#pragma once\n\n");
    file.write("namespace shaders {\n\n");

    shaders = os.listdir(".")
    vertex, fragment = split_shader_types(shaders);

    gen_namespace(file, "vertex", vertex)
    gen_namespace(file, "fragment", fragment)

    file.write("} // namespace shaders\n");


if __name__ == '__main__':
    sys.exit(main())
