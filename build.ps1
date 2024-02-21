
if ( -Not (Test-Path "out")) {
	New-Item "out" -ItemType Directory
}

g++ -std=c++20 -Wall -Wextra -static ./examples/colors.cpp       -o ./out/colors
g++ -std=c++20 -Wall -Wextra -static ./examples/hello_world.cpp  -o ./out/hello_world
g++ -std=c++20 -Wall -Wextra -static ./examples/keys.cpp         -o ./out/keys
g++ -std=c++20 -Wall -Wextra -static ./examples/raw_escapes.cpp  -o ./out/raw_escapes
