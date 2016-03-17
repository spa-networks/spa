#! /bin/bash
# Author: Jean-Gabriel Young
# Date: Nov. 10, 2014.
#
# Determine the number of element on a line

while read p; do
    TMP_NAME=$(echo $(date)" "$RANDOM  | sha256sum | cut --delimiter=" " -f 1)
    echo $p > $TMP_NAME
    sed -i 's/ /\n/' $TMP_NAME
    echo $(wc -l $TMP_NAME | cut -f1 -d " ")
    rm $TMP_NAME
done <$1
