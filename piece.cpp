#include "piece.h"

piece init_piece(int16_t type, int length) {
    Image image;
    switch (type) {
        case 1:
            image = LoadImage("./pieces/white_pawn.png");
            if (!IsImageReady(image)) return none();

            ImageResize(&image, length, length); 
            return pawn(true, 1, image);
        case 2:
            image = LoadImage("./pieces/white_bishop.png");
            if (!IsImageReady(image)) return none();

            ImageResize(&image, length, length); 
            return bishop(true, 1, image);
        case 3:
            image = LoadImage("./pieces/white_knight.png");
            if (!IsImageReady(image)) return none();

            ImageResize(&image, length, length); 
            return knight(true, 1, image);
        case 4:
            image = LoadImage("./pieces/white_rook.png");
            if (!IsImageReady(image)) return none();

            ImageResize(&image, length, length); 
            return rook(true, 1, image);
        case 5:
            image = LoadImage("./pieces/white_queen.png");
            if (!IsImageReady(image)) return none();

            ImageResize(&image, length, length); 
            return queen(true, 1, image);
        case 6:
            image = LoadImage("./pieces/white_king.png");
            if (!IsImageReady(image)) return none();

            ImageResize(&image, length, length); 
            return king(true, 1, image);

        case -1:
            image = LoadImage("./pieces/black_pawn.png");
            if (!IsImageReady(image)) return none();

            ImageResize(&image, length, length); 
            return pawn(true, -1, image);
        case -2:
            image = LoadImage("./pieces/black_bishop.png");
            if (!IsImageReady(image)) return none();

            ImageResize(&image, length, length);     
            return bishop(true, -1, image);
        case -3:
            image = LoadImage("./pieces/black_knight.png");
            if (!IsImageReady(image)) return none();

            ImageResize(&image, length, length); 
            return knight(true, -1, image);
        case -4:
            image = LoadImage("./pieces/black_rook.png");
            if (!IsImageReady(image)) return none();

            ImageResize(&image, length, length); 
            return rook(true, -1, image);
        case -5:
            image = LoadImage("./pieces/black_queen.png");
            if (!IsImageReady(image)) return none();
            
            ImageResize(&image, length, length); 
            return queen(true, -1, image);
        case -6:
            image = LoadImage("./pieces/black_king.png");
            if (!IsImageReady(image)) return none();

            ImageResize(&image, length, length); 
            return king(true, -1, image);
            
        default:
            return none();
    }
}
