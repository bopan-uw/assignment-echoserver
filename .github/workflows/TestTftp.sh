#!/bin/bash

set -e

echo "starting server..."
./tftp-server &

sleep 2

echo "starting client..."
./tftp-client < input.txt

kill "$(jobs -p)"

if [ ! -e output_client.txt ]
then
  echo "output_client does not exist."
  exit 1
fi

if [ ! -e output_server.txt ]
then
  echo "output_server does not exist."
  exist 1
fi

diff input.txt output_client.txt
diff_result1=$?
diff input.txt output_server.txt
diff_result2=$?
if [ $diff_result1 -eq 1 ]
then
  echo "expected output from client:"
  echo "$(<input.txt)"
  echo "actual output from client:"
  echo "$(<output_client.txt)"
  exit 1
elif [ $diff_result2 -eq 1 ]
then
  echo "expected output from server:"
  echo "$(<input.txt)"
  echo "actual output from server:"
  echo "$(<output_server.txt)"
  exit 1
else
  echo "test passed"
fi
