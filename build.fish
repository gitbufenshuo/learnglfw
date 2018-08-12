#!/usr/local/bin/fish
echo "will build" (whoami).c
gcc -o a.out (whoami).c glad/src/glad.c utils/*.c hello/*.c -Iglad/include -lglfw -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
# gcc -o a.out (whoami).c utils/*.c hello/*.c -Iglad/include -lglfw -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
