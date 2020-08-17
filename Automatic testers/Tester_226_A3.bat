echo off
echo:
javac -cp ..\algs4.jar PrimVsKruskal.java
for %%x in (0 1 2 3 4) do (
    echo Running Test %%x in Test Files...
    java -cp .;..\algs4.jar PrimVsKruskal Test_files\ewg_10vertices_%%x.txt > output.txt
    tail -2 Test_files\ewg_10vertices_%%x_output.txt > a.txt
    tail -1 output.txt > b.txt
    fc /w a.txt b.txt > nul &&Echo ewg_10vertices_%%x_.txt test passed || Echo ewg_10vertices_%%x_.txt test failed
)
for %%x in (0 1 2 3 4) do (
    echo Running Test %%x in Test Files...
    java -cp .;..\algs4.jar PrimVsKruskal Test_files\ewg_50vertices_%%x.txt > output.txt
    tail -0 Test_files\ewg_50vertices_%%x_output.txt > a.txt
    tail -1 output.txt > b.txt
    fc /w a.txt b.txt > nul &&Echo ewg_50vertices_%%x_.txt test passed || Echo ewg_50vertices_%%x_.txt test failed
)
for %%x in (0 1 2 3 4) do (
    echo Running Test %%x in Test Files...
    java -cp .;..\algs4.jar PrimVsKruskal Test_files\ewg_100vertices_%%x.txt > output.txt
    tail -0 Test_files\ewg_100vertices_%%x_output.txt > a.txt
    tail -1 output.txt > b.txt
    fc /w a.txt b.txt > nul &&Echo ewg_100vertices_%%x_.txt test passed || Echo ewg_100vertices_%%x_.txt test failed
)
echo:
echo:
for %%x in (0 1 2 3 4) do (
    echo Running Test %%x in Sparce Test Files...
    java -cp .;..\algs4.jar PrimVsKruskal Test_files\Test_files_sparse\ewg_10vertices_%%x.txt > output.txt
    tail -0 Test_files\Test_files_sparse\ewg_10vertices_%%x_output.txt > a.txt
    tail -1 output.txt > b.txt
    fc /w a.txt b.txt > nul &&Echo ewg_10vertices_%%x_.txt test passed || Echo ewg_10vertices_%%x_.txt test failed
)
for %%x in (0 1 2 3 4) do (
    echo Running Test %%x in Sparce Test Files...
    java -cp .;..\algs4.jar PrimVsKruskal Test_files\Test_files_sparse\ewg_50vertices_%%x.txt > output.txt
    tail -0 Test_files\Test_files_sparse\ewg_50vertices_%%x_output.txt > a.txt
    tail -1 output.txt > b.txt
    fc /w a.txt b.txt > nul &&Echo ewg_50vertices_%%x_.txt test passed || Echo ewg_50vertices_%%x_.txt test failed
)
for %%x in (0 1 2 3 4) do (
    echo Running Test %%x in Sparce Test Files...
    java -cp .;..\algs4.jar PrimVsKruskal Test_files\Test_files_sparse\ewg_100vertices_%%x.txt > output.txt
    tail -0 Test_files\Test_files_sparse\ewg_100vertices_%%x_output.txt > a.txt
    tail -1 output.txt > b.txt
    fc /w a.txt b.txt > nul &&Echo ewg_100vertices_%%x_.txt test passed || Echo ewg_100vertices_%%x_.txt test failed
)
echo: