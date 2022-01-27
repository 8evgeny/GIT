#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# @file       ps/configurator/conan.py 
# @brief      The Conan class provides
# @author     S.Panin <dix75@mail.ru>
# @copyright  S.Panin, 2009 - 2020
# @version    v.1.1
# @created    July      (the) 13(th), 2020, 15:49 MSK
# @updated    July      (the) 13(th), 2020, 15:49 MSK
# @TODO       
#

import os
import shutil
import argparse
import sys

qt_msvc_path = "C:\\Qt5\\5.15.2\\msvc2019_64"
qt_msvc_bin_path = "C:\\Qt5\\5.15.2\\msvc2019_64\\bin"


def rmTree():
    if os.path.exists("build"):
        shutil.rmtree("build")
    os.system("mkdir build")


def rmProfile():
    if os.path.exists("build.profile"):
        os.remove("build.profile")


def ProfileMSVC_d():
    file = open(r"./build.profile", "x")
    file.write("[settings]\n     os=Windows\n     os_build=Windows\n     arch=x86_64\n     arch_build=x86_64\n     compiler=Visual Studio\n     compiler.version=16\n     compiler.runtime=MDd\n     build_type=Debug\n[options]\n[build_requires]\n[env]")
    file.close()


def ProfileClang():
    file = open(r"./build.profile", "x")
    file.write("[settings]\n    arch=x86_64\n    arch_build=x86_64\n    build_type=Release\n    compiler=clang\n    compiler.version=10\n    os=Linux\n    os_build=Linux\n    compiler.libcxx=libstdc++11\n[options]\n[build_requires]\n[env]")
    file.close()


def ProfileMSVC():
    file = open(r"./build.profile", "x")
    file.write("[settings]\n     os=Windows\n     os_build=Windows\n     arch=x86_64\n     arch_build=x86_64\n     compiler=Visual Studio\n     compiler.version=16\n     compiler.runtime=MD\n     build_type=Release\n[options]\n[build_requires]\n[env]")
    file.close()

def ProfileGCC():
    file = open(r"./build.profile", "x")
    file.write("[settings]\n    arch=x86_64\n    arch_build=x86_64\n    build_type=Release\n    compiler=gcc\n    compiler.version=9\n    os=Linux\n    os_build=Linux\n    compiler.libcxx=libstdc++11\n[options]\n[build_requires]\n[env]")
    file.close()

def ProfileGCC_d():
    file = open(r"./build.profile", "x")
    file.write("[settings]\n    arch=x86_64\n    arch_build=x86_64\n    build_type=Debug\n    compiler=gcc\n    compiler.version=9\n    os=Linux\n    os_build=Linux\n    compiler.libcxx=libstdc++11\n[options]\n[build_requires]\n[env]")
    file.close()


def run(compiler):
    if(compiler == "msvc"):
        os.environ["PATH"] = os.environ["PATH"] + f";{qt_msvc_path};{qt_msvc_bin_path}"
        os.chdir("runtests")
        rmTree()
        rmProfile()
        ProfileMSVC_d()
        os.chdir("build")
        os.system("conan install .. --profile=../build.profile")
        os.system("cmake .. -G\"Visual Studio 16 2019\"")
        os.system("cmake --build . --target RunTests --config Debug  ")
        os.chdir("debug")
        x = os.system("RunTests.exe")
        if(x != 0):
            sys.exit(1)
    elif(compiler == "gcc"):
        os.chdir("runtests")
        rmTree()
        rmProfile()
        ProfileGCC()
        os.chdir("build")
        os.system("conan install .. --profile=../build.profile")
        rmProfile()
        os.system("cmake ..")
        os.system("cmake --build . -j 16 --config Debug ")
        x = os.system("xvfb-run -a -s \"-screen 0 1024x768x24\" ./RunTests")
        if(x != 0):
            sys.exit(1)
def release(compiler):
    if(compiler == "msvc"):
        rmTree()
        rmProfile()
        ProfileMSVC()
        os.environ["PATH"] = os.environ["PATH"] + f";{qt_msvc_path};{qt_msvc_bin_path}"
        os.system("cd build && conan install .. --profile=../build.profile")
        rmProfile()
    elif(compiler == "gcc"):
        rmTree()
        rmProfile()
        ProfileGCC()
        os.system("cd build && conan install .. --profile=../build.profile")
        rmProfile()
    else:
        print(f"Compiler {compiler} is not supported!!!\nOnly gcc or msvc supported!!!")

def debug(compiler):
    if(compiler == "msvc"):
        rmTree()
        rmProfile()
        ProfileMSVC_d()
        os.environ["PATH"] = os.environ["PATH"] + f";{qt_msvc_path};{qt_msvc_bin_path}"
        os.system("cd build && conan install .. --profile=../build.profile")
        rmProfile()
    elif(compiler == "gcc"):
        rmProfile()
        rmTree()
        ProfileGCC_d()
        os.system("cd build && conan install .. --profile=../build.profile")
        rmProfile()
    else:
        print(f"Compiler {compiler} is not supported!!!\nOnly msvc supported!!!")


def build(compiler):
    if(compiler == "msvc"):
        rmTree()
        rmProfile()
        ProfileMSVC()
        os.environ["PATH"] = os.environ["PATH"] + f";{qt_msvc_path};{qt_msvc_bin_path}"
        os.system("cd build && conan install .. --profile=../build.profile")
        rmProfile()
        os.system(f"cd build && cmake .. -DCMAKE_BUILD_TYPE=Release -G\"Visual Studio 16 2019\"")
        os.system(f"cd build && cmake --build . --target ips-configurator --config Release")
        x = os.system("cd bin\ && windeployqt.exe --serialport --multimedia --plugindir plugins ips-configurator.exe")
        if(x != 0):
            sys.exit(1)
    if(compiler == "msvcd"):
        rmTree()
        rmProfile()
        ProfileMSVC_d()
        os.environ["PATH"] = os.environ["PATH"] + f";{qt_msvc_path};{qt_msvc_bin_path}"
        os.system("cd build && conan install .. --profile=../build.profile")
        rmProfile()
        os.system(f"cd build && cmake .. -DCMAKE_BUILD_TYPE=Debug -G\"Visual Studio 16 2019\"")
        os.system(f"cd build && cmake --build . --target ips-configurator --config Debug")
        x = os.system("cd bin\ && windeployqt.exe --serialport --multimedia --plugindir plugins ips-configurator.exe")
        if(x != 0):
            sys.exit(1)
    elif(compiler == "gcc"):
        rmTree()
        rmProfile()
        ProfileGCC()
        os.system("cd build && conan install .. --profile=../build.profile")
        rmProfile()
        os.system(f"cd build && cmake .. -DCMAKE_BUILD_TYPE=Release -G\"Unix Makefiles\"")
        os.system(f"cd build && cmake --build . --parallel --config Release")
        x = os.system("cd build && make package")
        if(x != 0):
            sys.exit(1)
    elif(compiler == "gccd"):
        rmTree()
        rmProfile()
        ProfileGCC_d()
        os.system("cd build && conan install .. --profile=../build.profile")
        rmProfile()
        os.system(f"cd build && cmake .. -DCMAKE_BUILD_TYPE=Debug -G\"Unix Makefiles\"")
        os.system(f"cd build && cmake --build . --parallel --config Debug")
        x = os.system("cd build && make package")
        if(x != 0):
            sys.exit(1)
    elif(compiler == "clang"):
        rmTree()
        rmProfile()
        ProfileClang()
        os.system("cd build && conan install .. --profile=../build.profile")
        rmProfile()
        os.system("cd build && cmake .. -DCMAKE_CXX_COMPILER:STRING=\"/usr/bin/clang++\" -DCMAKE_BUILD_TYPE=Release")
        x = os.system('cd build && cmake --build . -j 16 --target ips-configurator --config Release')
        if(x != 0):
            sys.exit(1)


    else:
        print(f"Compiler {compiler} is not supported!!!\nOnly msvc supported!!!")


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-r', '--release', nargs='?', const='msvc', metavar="compiler (msvc, gcc)", help=f"Install conan dependences")
    parser.add_argument('-d', '--debug', nargs='?', const='msvc', metavar="compiler (msvc)", help=f"Install conan dependences")
    parser.add_argument('-b', '--build', nargs='?', const='msvc', metavar="compiler (msvc, gcc, clang, msvcd, gccd)", help=f"Build the project")
    parser.add_argument('-t', '--test', nargs='?', const='msvc', metavar="compiler (msvc, gcc)", help=f"Run tests")
    args = parser.parse_args()

    if args.release is not None:
        release(args.release)
    elif args.debug is not None:
        debug(args.debug)
    elif args.build is not None:
        build(args.build)
    elif args.test is not None:
        run(args.test)
    else:
        parser.print_help()
