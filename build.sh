#Build and run code

rm -rf /build
mkdir build & cd build
# Build
cmake ..
make

# wait 2 seconds
sleep 2

# Run code
./setup_protocole