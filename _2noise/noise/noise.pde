int currentX, currentY;
int lastX, lastY;

float v = 0.0;
float inc = 0.08;
float stepRange = 3;

void setup() {
  size (800, 600);
  smooth();
  frameRate(10);
  background(100);
}

void draw() {
  clear();
  noStroke();
  fill(0, 25);
  rect(0,0, width,height);

  lastX = currentX;
  lastY = currentY;

  currentX += int(random(-stepRange, stepRange));
  currentY += int(random(-stepRange, stepRange));

  currentX = constrain(currentX, 0, width);
  currentY = constrain(currentY, 0, height);

  stroke(255);
  rect(currentX, currentY, random(currentX), random(currentY));

  for(int i = 0; i < width; i = i+4) {
    float n = noise(v) * 400.0;
    rect(i, 10 + n, 3, 20);
    v = v +inc;
  }
}