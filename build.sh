#Build and run code

rm -rf build/*
sleep 1
cd build
# Build
cmake ..
make

# wait 1 seconds & copy
sleep 1

cd ..
cp device.xml build/
cd build

sleep 1

# Run code
./setup_protocole

