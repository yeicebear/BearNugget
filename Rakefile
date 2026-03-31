[kernel_cxx]
inputs: src/cpp/*.cpp 
1) for f in src/cpp/*.cpp; do g++ -std=gnu++23 -ffreestanding -fno-exceptions -fno-rtti -fno-stack-protector -mno-red-zone -mno-mmx -mno-sse -m64 -march=x86-64 -Wall -Wextra -O2 -Isrc -fno-pie -fno-pic -c "$f" -o "${f%.cpp}.o"; done
