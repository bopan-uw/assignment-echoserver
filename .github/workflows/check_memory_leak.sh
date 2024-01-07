#!/bin/bash

sudo apt install valgrind

valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out-server.txt \
         ./tftp-server &

valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out-client.txt \
         ./tftp-client < input.txt

kill "$(jobs -p)"

RED='\033[0;31m'
NC='\033[0m' # No Color
GREEN='\033[0;32m'

echo "======================================"
echo "= Checking memory leak for server... ="
echo "======================================"
cat valgrind-out-server.txt

result1=$(grep -c "definitely lost: 0 bytes in 0 blocks" valgrind-out-server.txt)
result2=$(grep -c "indirectly lost: 0 bytes in 0 blocks" valgrind-out-server.txt)
if [[ $(grep -c "All heap blocks were freed -- no leaks are possible" valgrind-out-server.txt) -eq 1 ]]
then
  echo -e "${GREEN}Memory check passed. All heap blocks were freed"
elif [[ $result1 -eq 1 && $result2 -eq 1 ]]
then
  echo -e "${GREEN}Memory check passed."
else
  echo -e "${RED}Detected memory leak!"
  exit 1
fi

echo "======================================"
echo "= Checking memory leak for client... ="
echo "======================================"
cat valgrind-out-client.txt

result1=$(grep -c "definitely lost: 0 bytes in 0 blocks" valgrind-out-client.txt)
result2=$(grep -c "indirectly lost: 0 bytes in 0 blocks" valgrind-out-client.txt)
if [[ $(grep -c "All heap blocks were freed -- no leaks are possible" valgrind-out-client.txt) -eq 1 ]]
then
  echo -e "${GREEN}Memory check passed. All heap blocks were freed"
elif [[ $result1 -eq 1 && $result2 -eq 1 ]]
then
  echo -e "${GREEN}Memory check passed."
else
  echo -e "${RED}Detected memory leak!"
  exit 1
fi
