echo starting > bin/out1.txt
echo starting > bin/out2.txt
EXE=${1:-main}
C="$(1)correct"
make $EXE
make $C
while diff -i bin/out1.txt bin/out2.txt; do
    python3 gen.py > bin/in.txt
    ./$C < bin/in.txt > bin/out1.txt
    ./$EXE < bin/in.txt > bin/out2.txt
    echo "OK"
done
echo "FAIL"
echo 

echo "input:"
cat bin/in.txt
echo

echo "correct:"
cat bin/out1.txt
echo 

echo "main:"
cat bin/out2.txt
echo 