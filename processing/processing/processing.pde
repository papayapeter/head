int FrameCount;
int FPS = 10;          // ÄNDERN

int[][] AnimationData;
String[] lines;

Table Left_Eyebow, Right_Eyebrow, Jaw;

void setup()
{
  // Tabellen Laden
  Left_Eyebow = loadTable("Left_Eyebrow.txt", "header, tsv");
  Right_Eyebrow = loadTable("Right_Eyebrow.txt", "header, tsv");
  Jaw = loadTable("Jaw.txt", "header, tsv");
  
  FrameCount = Jaw.getRowCount();
  
  AnimationData = new int[FrameCount][3];
  lines = new String[FrameCount + 4];
  
  // Tabellen in Arrays schreiben un Minimum und Maximum werte feststellen
  int[] Max = {0, 0, 0};
  int[] Min = {1080, 1080, 1080};
  for (int i = 0; i < FrameCount; i++)
  {
    int l = Left_Eyebow.getInt(i, "Y pixels");
    int r = Right_Eyebrow.getInt(i, "Y pixels");
    int j = Jaw.getInt(i, "Y pixels");
    
    if (l < Min[0]) Min[0] = l;
    if (r < Min[1]) Min[1] = r;
    if (j < Min[2]) Min[2] = j;
    
    if (l > Max[0]) Max[0] = l;
    if (r > Max[1]) Max[1] = r;
    if (j > Max[2]) Max[2] = j;
    
    AnimationData[i][0] = l;
    AnimationData[i][1] = r;
    AnimationData[i][2] = j;
  }
  
  // Die neuen Werte von 0 bis 255 in die Arrays schreiben und dabei invertieren, sodass Bildunten 0 ist und Bildoben 255
  for (int i = 0; i < FrameCount; i++)
  {
    AnimationData[i][0] = int(map(AnimationData[i][0], Max[0], Min[0], 0, 255));
    print(AnimationData[i][0] + "\t");
    AnimationData[i][1] = int(map(AnimationData[i][1], Max[1], Min[1], 0, 255));
    print(AnimationData[i][1] + "\t");
    AnimationData[i][2] = int(map(AnimationData[i][2], Max[2], Min[2], 0, 255));
    print(AnimationData[i][2] + "\n");
  }
  
  // Die Werte in einen String-Array schreiben
  lines[0] = "const byte FPS = " + str(FPS) + ";";
  lines[1] = "const int Animation_Length = " + str(FrameCount) + ";";
  lines[2] = "const byte Animation_Sequence[Animation_Length][3] = {";
  for (int i = 0; i < FrameCount; i++)
  {
    lines[i + 3] = "{" + str(AnimationData[i][0]) + ", " + str(AnimationData[i][1]) + ", " + str(AnimationData[i][2]) + "}, ";
  }
  lines[FrameCount + 3] = "};";
  
  // Den String-Array in eine Datei schreiben;
  saveStrings("Animation_Data.h", lines);
  
  exit();
}
