make $1
for i in ~/Desktop/samps/$1*/sample/*.in; do
    ./$1 < $i > bin/out.txt
    echo $i ": "
    diff -Z ${i%.in}.ans bin/out.txt
done