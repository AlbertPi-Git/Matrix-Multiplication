g++ -O3 -std=c++11 -pthread tester.cpp -o tester
./tester > test_output.txt
python3 verifier.py