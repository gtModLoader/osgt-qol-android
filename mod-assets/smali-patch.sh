#!/bin/bash
grep -q "const-string v2, \"osgtqol\"" smali/com/rtsoft/growtopia/Main.smali
if [[ $? -ne 0 ]]; then
    echo "Patching Main.smali to load libosgtqol.so first"
    sed -i 's/sget-object v0, Lcom\/rtsoft\/growtopia\/Main;->dllname:Ljava\/lang\/String;/\n    const-string v2, "osgtqol"\n\n    invoke-static {v2}, Ljava\/lang\/system;->loadLibrary(LJava\/lang\/String;)V\n\n    sget-object v0, Lcom\/rtsoft\/growtopia\/Main;->dllname:Ljava\/lang\/String;/' smali/com/rtsoft/growtopia/Main.smali
else
    echo "Nothing to patch"
fi