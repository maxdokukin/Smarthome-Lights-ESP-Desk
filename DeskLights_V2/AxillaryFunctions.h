short colCon(long hexValue, char color){

    if(color == 'R')        
      return ((hexValue >> 16) & 0xFF);    
    if(color == 'G')
        return ((hexValue >> 8) & 0xFF);
    if(color == 'B')
      return ((hexValue) & 0xFF);
}


long getMixCol(long col1, long col2, float br){  

  return long((col1 >> 16 & 0xff) + (int(col2 >> 16 & 0xFF) - int(col1 >> 16 & 0xFF)) * br) << 16 | long((col1 >> 8 & 0xff) + (int(col2 >> 8 & 0xFF) - int(col1 >> 8 & 0xFF)) * br) << 8 | long((col1 & 0xff) + (int(col2 & 0xFF) - int(col1 & 0xFF)) * br);
}

long getBrCol(long col, float br){
  
  return (long((((col >> 16) & 0xFF) * br)) << 16) | (long((((col >> 8) & 0xFF) * br)) << 8) | (long((col & 0xFF) * br));
}

float fract(float x) { return x - int(x); }

float mix(float a, float b, float t) { return a + (b - a) * t; }

float step(float e, float x) { return x < e ? 0.0 : 1.0; }

//RGB (0-255), HSV(0-255)
long HSVtoRGB(byte h, byte s, byte v){

  h = map(h, 0, 255, 0, 1000) / (float) 1000;
  s = map(s, 0, 255, 0, 1000) / (float) 1000;
  v = map(v, 0, 255, 0, 1000) / (float) 1000;

  byte r = v * mix(1.0, constrain(abs(fract(h + 1.0) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s);
  byte g = v * mix(1.0, constrain(abs(fract(h + 0.6666666) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s);
  byte b = v * mix(1.0, constrain(abs(fract(h + 0.3333333) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s);

  return (long(r & 0x0000ff) << 16) | (long(g & 0x0000ff) << 8) | long(b & 0x0000ff);
}

//RGB (0-255), HSV(0.0-1.0)
byte* rgb2hsv(byte r, byte g, byte b, byte* hsv) {

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
  hsv[0] = map((abs(qz + (qw - py) / (6.0 * d + 1e-10))) * 1000, 0, 1000, 0, 255);
  hsv[1] = map((d / (qx + 1e-10)) * 1000, 0, 1000, 0, 255);
  hsv[2] = map(qx * 1000, 0, 1000, 0, 255);
  return hsv;
}


long getLong(byte a, byte b, byte c){

  return (long(a) << 16) | (long(b) << 8) | long(c);
}

 //returns 0xff0000 or 0x00ff00 or 0x0000ff
  byte getByteFromLong(long a, byte shift){

    return (a >> 8 * shift) & 0x0000ff;
  }