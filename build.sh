clang -o main \
    -Wall -Wextra -pedantic \
    -lSDL2 -lGL -lm -lGLU -ldl -lGLEW \
    main.c widgets.c

