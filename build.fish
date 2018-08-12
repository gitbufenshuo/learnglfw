#!/usr/local/bin/fish
echo "will build" (whoami).c
gcc -o a.out (whoami).c utils/*.c hello/*.c -lglfw -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
