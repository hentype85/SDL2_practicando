# SDL2 / Simple DirectMedia Layer
https://www.libsdl.org/  
https://github.com/libsdl-org/SDL/releases/tag/release-2.30.1  
descargar: SDL2-devel-2.30.1-mingw.zip  
usar:  
    \SDL2-devel-2.30.1-mingw.zip\SDL2-2.30.1\x86_64-w64-mingw32  
    - \include  
    - \lib  
    \SDL2-devel-2.30.1-mingw.zip\SDL2-2.30.1\x86_64-w64-mingw32\bin  
    - SDL2.dll  

# SDL_image
https://www.libsdl.org/projects/SDL_image/release  
descargar: SDL2_image-devel-2.8.2-mingw.zip  
copiar:  
    \include desde - \SDL2_image-devel-2.8.2-mingw\SDL2_image-2.8.2\x86_64-w64-mingw32\include  
    \lib desde - \SDL2_image-devel-2.8.2-mingw\SDL2_image-2.8.2\x86_64-w64-mingw32\lib  
    SDL2_image.dll desde - \SDL2_image-devel-2.8.2-mingw\SDL2_image-2.8.2\x86_64-w64-mingw32\bin  
pegar:  
    \include en - \SDL2_practicando\SDL2\include  
    \lib en - \SDL2_practicando\SDL2\lib  
    SDL2_image.dll junto con SDL2.dll  
    (ejemplo: 06_SDL2)  
    

# usar g++ con MinGW
https://code.visualstudio.com/docs/cpp/config-mingw#_prerequisites  
https://www.youtube.com/watch?v=oC69vlWofJQ  

comando para instalar MinGW Toolkit:
```
pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain
```