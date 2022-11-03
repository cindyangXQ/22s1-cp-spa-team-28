# Python script to run all Autotester tests, modified for CI run.
# To add new Autotester tests, add a source code file {name}_source.txt to Tests28/source
# and a query file {name}_queries.txt to Tests28/queries

import sys
import os

def autotest(source_path):
    cmd = "Team28\\Code28\\build\\src\\autotester\\Release\\autotester.exe Team28\\Tests28\\source\\{}_source.txt Team28\\Tests28\\queries\\{}_queries.txt Team28\\Tests28\\out_{}.xml".format(
        source_path, source_path, source_path
    )
    os.system(cmd)

def remove_suffix(input_string, suffix):
    if suffix and input_string.endswith(suffix):
        return input_string[:-len(suffix)]
    return input_string

if __name__ == "__main__":
    pathArr = os.listdir("Team28\\Tests28\\source")

    for i in range(len(pathArr)):
        pathArr[i] = remove_suffix(pathArr[i], "_source.txt")

    try:
        autotest("Affects_Code6")
    except Exception as e:
        print(path)
        print(str(e))
