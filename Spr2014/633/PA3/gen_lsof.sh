python ps.py acme2013-d257a779.vmem | awk '{print $2}' > test.txt
while read i; do
  python lsof.py acme2013-d257a779.vmem $i
done < test.txt
