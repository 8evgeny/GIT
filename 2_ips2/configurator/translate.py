#!/usr/bin/env python3
# -*-coding: UTF-8 -*-
#
# @file      translate.py
# @brief     The main file creates translate files
# @author    S.Panin <dix75@mail.ru>
# @copyright S.Panin, 2015 - 2020
# @version   v.1.1
# @created   July (the) 14(th), 2015, 17:43 MSK
# @updated   July (the) 16(th), 2020, 16:37 MSK
# @TODO
#
import os
import re
import sys
import fileinput

# lupdate = 'lupdate'
lupdate = 'c:\\Qt\\5.14.2\\mingw73_64\\bin\\lupdate.exe'
dir="*"
ru = './resources/langs/ru.ts'
en = './resources/langs/en.ts'


def removeVanishedAttr(file):
    prog = re.compile(r"(^.*)( type=\"vanished\")(.*$)")
    for line in fileinput.input(file, inplace=1):
        m = prog.match(line)
        if(m is not None):
            line = "{0}{1}\n".format(m.group(1), m.group(3))
        sys.stdout.write(line)


def addTranslationAsCopySource(file):
    prog1 = re.compile(r"(^.*<source>)(.*)(</source>.*$)")
    prog2 = re.compile(r"(^.*)(<translation type=\"unfinished\")(.*$)")
    t = ""
    for line in fileinput.input(file, inplace=1):
        m1 = prog1.match(line)
        if(m1 is not None):
            t = m1.group(2)
        m = prog2.match(line)
        if(m is not None):
            line = "{0}<translation>{1}</translation>\n".format(m.group(1), t)
        sys.stdout.write(line)


def containsLangFiles(file):
    for line in open(file):
        if(None != re.search(r"\bTRANSLATIONS\b", line)):
            return True
    return False


def find(path):
    result = []
    for root, dirs, files in os.walk(os.path.abspath(path)):
        for name in files:
            if(None != re.search(r".*\.pro$", name)):
                file = os.path.join(root, name)
                if(containsLangFiles(file) is True):
                    result.append(file)
    return result


def main():
    files = find(path)

    for file in files:
        os.system("{0} -verbose {1}".format(lupdate, file))
        addTranslationAsCopySource(ru)
        addTranslationAsCopySource(en)
    removeVanishedAttr(ru)
    removeVanishedAttr(en)

if __name__ == "__main__":
    print(f"{lupdate} {dir} -verbose -ts {ru}")
    os.system(f"{lupdate} {dir} -verbose -ts {ru}")

