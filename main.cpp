#include <emscripten/emscripten.h>
#include <cmath>
#include <cstdlib>

// img_size is the size of the img to display it
#define img_size  400

// size is the gird size for testing
#define size      10

// this code is for the look up tables
#define scale     100000
#define precision 10000

// for the rays
#define rays      300
#define ray_steps 100

// the look up tables size
#define angle_range 1000

// the gird to display
int arr[size][size][size];

// this is to give js to display to code
unsigned char image[img_size * img_size * 4];

// the int look up tables
int sin_cos[angle_range][angle_range];
int cos_cos[angle_range][angle_range];
int sine[angle_range];

int x = 4 * scale, y = 3 * scale, z = 3 * scale;
int alpha = 0, beta = 0;
int i, j, k;

int rx, ry, rz, r_alpha, r_beta;

extern "C" {
EMSCRIPTEN_KEEPALIVE
void draw() {

    for (i = 0; i < rays; i++) {
        for (j = 0; j < rays; j++) {
            //is there a error here?
            r_alpha = (alpha + i) % angle_range;
            r_beta  = (beta +  j) % angle_range;

            rx = x;
            ry = y;
            rz = z;

            while(true){

                //is there a error here?
                rx += sin_cos[r_alpha][r_beta];
                ry += sine[r_beta];
                rz += cos_cos[r_alpha][r_beta];

                
                if (arr[ry / scale][rx / scale][rz / scale] != 0){
                    //draws color so i can test the code
                    image[( (i + 10) * img_size + (j + 10) ) * 4 + 3] = arr[ry / scale][rx / scale][rz / scale] * 10;
                    break;
                }
            }
        }
    }
}

EMSCRIPTEN_KEEPALIVE
void init() {
    // this is for testing
    std::srand(576);
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            for (k = 0; k < size; k++) {
                arr[i][j][k] = (i == 0 || i == size - 1 || j == 0 || j == size - 1 || k == 0 || k == size - 1) ? std::rand() % 10 + 1: 0;
            }
        }
    }
    arr[1][1][1] = 20;

    //makes the sin and cos tables
    for (i = 0; i < angle_range; i++) {
        double angleX = (2.0 * M_PI * i) / angle_range;
        sine[i] = static_cast<int>(sin(angleX) * precision);
        for (j = 0; j < angle_range; j++) {
            sin_cos[i][j] = static_cast<int>((sin(angleX) * cos( (2.0 * M_PI * j) / angle_range)) * precision);
            cos_cos[i][j] = static_cast<int>((cos(angleX) * cos( (2.0 * M_PI * j) / angle_range)) * precision);
        }
    }

    for (i = 0; i < img_size; i++) {
        for (j = 0; j < img_size; j++){
            image[(i * img_size + j) * 4] = 255;
            image[(i * img_size + j) * 4 + 1] = 255;
            image[(i * img_size + j) * 4 + 2] = 255;
            image[(i * img_size + j) * 4 + 3] = 0;
        }
    }
}

//this code prints x y and z and the angles
EMSCRIPTEN_KEEPALIVE int print_x() { return x; }
EMSCRIPTEN_KEEPALIVE int print_y() { return y; }
EMSCRIPTEN_KEEPALIVE int print_z() { return z; }
EMSCRIPTEN_KEEPALIVE int print_alpha() { return alpha; }
EMSCRIPTEN_KEEPALIVE int print_beta() { return beta; }
EMSCRIPTEN_KEEPALIVE void forward() {}
EMSCRIPTEN_KEEPALIVE void backward() {}

//this code is so i can look
EMSCRIPTEN_KEEPALIVE void up() {
    alpha -= 10;
    if (alpha < 0){
        alpha = angle_range-1;
    }}
EMSCRIPTEN_KEEPALIVE void down() {
    alpha += 10;
    if (alpha > angle_range){
        alpha = 0;
    }}
EMSCRIPTEN_KEEPALIVE void left() {
    beta -= 10;
    if (beta < 0){
        beta = angle_range-1;
    }}
EMSCRIPTEN_KEEPALIVE void right() {
    beta += 10;
    if (beta > angle_range){
        beta = 0;
    }}

EMSCRIPTEN_KEEPALIVE unsigned char *get_image_data(){
    return image;}
}
