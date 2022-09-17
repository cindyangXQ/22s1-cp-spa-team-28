import subprocess
import sys
import os
import platform

import xml.etree.cElementTree as et

def autotest(source_path):
    cmd = "Team28//Code28//out//build//x64-Debug//src//autotester//autotester.exe Team28//Tests28//source//{}.txt Team28//Tests28//queries//{}_queries.txt Team28//Tests28//out_{}.xml".format(
        source_path, source_path, source_path
    )
    os.system(cmd)

def remove_suffix(input_string, suffix):
    if suffix and input_string.endswith(suffix):
        return input_string[:-len(suffix)]
    return input_string

if __name__ == "__main__":
    pathArr = os.listdir("Team28//Tests28//source")

    for i in range(len(pathArr)):
        pathArr[i] = remove_suffix(pathArr[i], ".txt")

    for path in pathArr:
        autotest(path)
