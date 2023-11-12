short colCon(long hexValue, char color) {
    if (color == 'R')        
        return ((hexValue >> 16) & 0xFF);    
    if (color == 'G')
        return ((hexValue >> 8) & 0xFF);
    if (color == 'B')
        return (hexValue & 0xFF);

    return 0;
}

long getMixCol(long col1, long col2, float br) {  
    return long((col1 >> 16 & 0xff) + (int(col2 >> 16 & 0xFF) - int(col1 >> 16 & 0xFF)) * br) << 16 |
           long((col1 >> 8 & 0xff) + (int(col2 >> 8 & 0xFF) - int(col1 >> 8 & 0xFF)) * br) << 8 |
           long((col1 & 0xff) + (int(col2 & 0xFF) - int(col1 & 0xFF)) * br);
}

long getBrCol(long col, float br) {
    return (long((((col >> 16) & 0xFF) * br)) << 16) |
           (long((((col >> 8) & 0xFF) * br)) << 8) |
           (long((col & 0xFF) * br));
}

float fract(float x) {
    return x - int(x);
}

float mix(float a, float b, float t) {
    return a + (b - a) * t;
}

float step(float e, float x) {
    return x < e ? 0.0 : 1.0;
}

// RGB to HSV conversion
float* RGB2HSV(byte* params, float* hsv) {
    float r = params[0] / (float) 255;
    float g = params[1] / (float) 255;
    float b = params[2] / (float) 255;

    float s = step(b, g);
    float px = mix(b, g, s);
    float py = mix(g, b, s);
    float pz = mix(-1.0, 0.0, s);
    float pw = mix(0.6666666, -0.3333333, s);
    s = step(px, r);
    float qx = mix(px, r, s);
    float qz = mix(pw, pz, s);
    float qw = mix(r, px, s);
    float d = qx - min(qw, py);
    hsv[0] = abs(qz + (qw - py) / (6.0 * d + 1e-10));
    hsv[1] = d / (qx + 1e-10);
    hsv[2] = qx;
    return hsv;
}

// HSV to RGB conversion
void HSV2RGB(float h, float s, float v, byte* RGB) {
    int i;
    float m, n, f;

    s /= 100;
    v /= 100;

    if (s == 0) {
        RGB[0] = RGB[1] = RGB[2] = round(v * 255);
        return;
    }

    h /= 60;
    i = floor(h);
    f = h - i;

    if (!(i & 1)) {
        f = 1 - f;
    }

    m = v * (1 - s);
    n = v * (1 - s * f);

    switch (i) {
        case 0: case 6:
            RGB[0] = round(v * 255);
            RGB[1] = round(n * 255);
            RGB[2] = round(m * 255);
            break;

        case 1:
            RGB[0] = round(n * 255);
            RGB[1] = round(v * 255);
            RGB[2] = round(m * 255);
            break;

        case 2:
            RGB[0] = round(m * 255);
            RGB[1] = round(v * 255);
            RGB[2] = round(n * 255);
            break;

        case 3:
            RGB[0] = round(m * 255);
            RGB[1] = round(n * 255);
            RGB[2] = round(v * 255);
            break;

        case 4:
            RGB[0] = round(n * 255);
            RGB[1] = round(m * 255);
            RGB[2] = round(v * 255);
            break;

        case 5:
            RGB[0] = round(v * 255);
            RGB[1] = round(m * 255);
            RGB[2] = round(n * 255);
            break;
    }
}
