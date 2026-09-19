EXE=$1
make $EXE
python3 gen.py > bin/in.txt
while ./$EXE < bin/in.txt; do
    echo "OK"
    python3 gen.py > bin/in.txt
done
