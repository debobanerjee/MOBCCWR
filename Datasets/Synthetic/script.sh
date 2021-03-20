g++ generatetest.cpp

./a.out

g++ preprocess_nxt.cpp

./a.out < test-2000.txt > syn2000.txt
rm "test-2000.txt"

./a.out < test-4000.txt > syn4000.txt
rm "test-2000.txt"

./a.out < test-8000.txt > syn8000.txt
rm "test-2000.txt"

./a.out < test-16000.txt > syn16000.txt
rm "test-2000.txt"

./a.out < test-32000.txt > syn32000.txt
rm "test-2000.txt"

./a.out < test-64000.txt > syn64000.txt
rm "test-2000.txt"

./a.out < test-128000.txt > syn128000.txt
rm "test-2000.txt"

./a.out < test-256000.txt > syn256000.txt
rm "test-2000.txt"

./a.out < test-512000.txt > syn512000.txt
rm "test-2000.txt"

./a.out < test-1000000.txt > syn1000000.txt
rm "test-2000.txt"

rm "a.out"